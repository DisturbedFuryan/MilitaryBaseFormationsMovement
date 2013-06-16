#include "User.h"
#include "singletons.h"

CUser::CUser(short iId, CVector* vPos, short iColour) : m_bIsTargetVisible(false) {
	//--------------------------------
	// Przypisanie warto�ci zmiennym.
	//--------------------------------

	m_iId = iId;  // Przypisanie warto�ci identyfikatorowi rycerza.
	m_vPos = *vPos;  // Przypisanie podanej pozycji rycerzowi.
	ShiftCollisionCircle();  // Synchronizacja ko�a kolizji z pozycj�.
	m_vTarget = m_vPos;  // Przypisanie wektorowi celu pozycji u�ytkownika.

	// Przypisanie warto�ci promieniowi ko�a kontroluj�cego ruchu.
	m_MoveControlCircle.r = (g_DefaultsMgr.GetKnightRadius() / 3.0f);

	ShiftMoveControlCircle();  // Zsynchronizowanie ko�a kontroluj�cego ruch z pozycj� rycerza.
	m_fAng = 0.0f;  // Przypisanie pocz�tkowej warto�ci orientacji rycerza.

	// Przypisanie warto�ci prostok�tom opisuj�cym ko�o kontroluj�ce ruch i cel u�ytkownika.

	MoveControlCircRect.w = (static_cast<int>(m_MoveControlCircle.r) * 2);
	MoveControlCircRect.h = (static_cast<int>(m_MoveControlCircle.r) * 2);

	TargetRect.w = 4;
	TargetRect.h = 4;

	//-----------------------------------------
	// Utworzenie odpowiedniego pod�wietlenia.
	//-----------------------------------------

	switch(iColour) {
		case IBacklight::yellow: m_pBacklight = new CCommanderBacklight(IBacklight::yellow); break;
		case IBacklight::blue: m_pBacklight = new CCommanderBacklight(IBacklight::blue); break;
		case IBacklight::green: m_pBacklight = new CCommanderBacklight(IBacklight::green);
	}
}

void CUser::ArmyIt(int iArmyId, std::vector<IKnight*>* pComradesContainer, const Uint32 iDeltaTicks) {
	//-----------------------
	// Aktualizacja pozycji.
	//-----------------------

	m_vOldPos = m_vPos;  // Zapami�tanie ostatniej pozycji.
	m_vPos += (m_vVel * (iDeltaTicks / 1000.0f));  // Uaktualnienie pozycji wzgl�dem wektora pr�dko�ci.
	ShiftCollisionCircle();  // Zsynchronizowanie ko�a kolizji z pozycj�.

	//-------------------------------
	// Obliczenie wektora pr�dko�ci.
	//-------------------------------

	m_vOldVel = m_vVel;  // Zapami�tanie ostatniej pr�dko�ci.
	LocalizeTarget();  // Zlokalizowanie celu.
	ComputeVel();  // Obliczenie wektora pr�dko�ci na podstawie celu.
	m_fSpeed = m_vVel.Length();  // Obliczenie ca�kowitej pr�dko�ci u�ytkownika.

	//---------------------
	// Implementacja celu.
	//---------------------

	Target();  // Metoda mo�e mie� drastyczny wp�yw na wektor pr�dko�ci!

	//------------------------
	// Obliczenie orientacji.
	//------------------------

	if(!m_vVel.IsZero())  // Wektor pr�dko�ci nie mo�e by� zerowy! Jest wykorzystywany przy obliczaniu orientacji.
		ComputeYaw(iDeltaTicks);

	//-----------------------------
	// Kolizja z innymi rycerzami.
	//-----------------------------

	CollisionWithOthers();

	//-------------------------
	// Kolizja z przeszkodami.
	//-------------------------

	CollisionWithObstacles();

	//------------------------------
	// Implementacja granic �wiata.
	//------------------------------

	WorldBound();

	//-----------------------------------------
	// Aktualizacja klatki i statusu animacji.
	//-----------------------------------------

	UpdateFrameAndStatus(iDeltaTicks);

	//------------------------------------------
	// Aktualizacja kontenera �lad�w po butach.
	//------------------------------------------

	UpdateShoesprintsContainer(iDeltaTicks);

	//-----------------------------
	// Aktualizacja pod�wietlenia.
	//-----------------------------

	m_pBacklight->Update(iDeltaTicks);

	//-------------------------------------------------
	// Ustawienie kamery centralnie nad u�ytkownikiem.
	//-------------------------------------------------

	SetCamera();
}

void CUser::HandleInput(void) {
	//Je�eli zostanie naci�ni�ta spacja, pod�wietlenie rycerzy zniknie/pojawi si�.
	if((g_EventMgr.GetEvent()->type == SDL_KEYDOWN) && (g_EventMgr.GetEvent()->key.keysym.sym == SDLK_SPACE))
		m_bVisibilityBacklight = !m_bVisibilityBacklight;  // Aktualizacja flagi.

	//Je�eli zostanie naci�ni�ta litera "t", cel rycerza zniknie/pojawi si�.
	if((g_EventMgr.GetEvent()->type == SDL_KEYDOWN) && (g_EventMgr.GetEvent()->key.keysym.sym == SDLK_t))
		m_bIsTargetVisible = !m_bIsTargetVisible;
}

void CUser::ShowTarget(void) {
	if(m_bIsTargetVisible) {  // Je�eli cel i ko�o kontroluj�ce ruch s� widoczne.
		MoveControlCircRect.x = (static_cast<int>(m_MoveControlCircle.pos.x) - (MoveControlCircRect.w / 2));
		MoveControlCircRect.y = (static_cast<int>(m_MoveControlCircle.pos.y) - (MoveControlCircRect.h / 2));
		if(g_CollisionController.CheckCollision(&MoveControlCircRect, g_Camera.GetRect())) {
			MoveControlCircRect.x -= g_Camera.GetX();
			MoveControlCircRect.y -= g_Camera.GetY();

			// Zapami�tanie szeroko�ci i wysoko�ci prostok�ta, poniewa� SDL_FillRect() mo�e go zmodyfikowa�.
			// Dzieje si� to wtedy, kiedy wype�niany kolorem prostok�t wyjdzie poza ekran.
			int iW = MoveControlCircRect.w;
			int iH = MoveControlCircRect.h;

			SDL_FillRect(g_AppWindow.GetScreenSurf(), &MoveControlCircRect, 
									 SDL_MapRGB(g_AppWindow.GetScreenSurf()->format, 0x00, 0x00, 0x00));

			// Renowacja warto�ci prostok�ta.
			MoveControlCircRect.w = iW;
			MoveControlCircRect.h = iH;
		}
		TargetRect.x = (static_cast<int>(m_vTarget.x) - (TargetRect.w / 2));
		TargetRect.y = (static_cast<int>(m_vTarget.y) - (TargetRect.h / 2));
		if(g_CollisionController.CheckCollision(&TargetRect, g_Camera.GetRect())) {
			TargetRect.x -= g_Camera.GetX();
			TargetRect.y -= g_Camera.GetY();
			int iW = TargetRect.w;
			int iH = TargetRect.h;
			SDL_FillRect(g_AppWindow.GetScreenSurf(), &TargetRect, 
									 SDL_MapRGB(g_AppWindow.GetScreenSurf()->format, 0xFF, 0x00, 0x00));
			TargetRect.w = iW;
			TargetRect.h = iH;
		}
	}
}

void CUser::LocalizeTarget(void) {
	//-----------------------------------------------------
	// Wyszukanie przeszk�d b�d�cych tu� obok u�ytkownika.
	//-----------------------------------------------------

	SearchCloseObstacles();

	//--------------------------------------------------------
	// Zsynchronizowanie ko�a kontroluj�cego ruch z rycerzem.
	//--------------------------------------------------------

	ShiftMoveControlCircle();

	//----------------------
	// Zlokalizowanie celu.
	//----------------------

	if(g_MouseController.GetIsLeftButDown()) {  // Je�eli lewy przycisk myszki jest wci�ni�ty.
		//------------------------------
		// Znalezienie pozycji kursora.
		//------------------------------

		// Pobranie pozycji kursora i kamery.
		CVector vMouse(static_cast<float>(g_MouseController.GetX()), static_cast<float>(g_MouseController.GetY()));
		CVector vCamera(static_cast<float>(m_UserCamera.GetX()), static_cast<float>(m_UserCamera.GetY()));

		vMouse += vCamera;  // Obliczenie pozycji kursora;

		//--------------------------------------
		// Obliczenie wektora celu u�ytkownika.
		//--------------------------------------

		// W przypadku, je�eli pozycja kursora nie koliduje z ko�em kontroluj�cym ruch,
		// rycerz mo�e bez przeszk�d obra� cel wskazany przez u�ytkownika.
		if(!g_CollisionController.CheckCollision(&vMouse, &m_MoveControlCircle))
			m_vTarget = vMouse;
	}
	
	//-------------------------------
	// Ewentualne przesuni�cie celu.
	//-------------------------------

	// 1. Przesuni�cie celu w przypadku, gdy rycerz jest tu� obok przeszkody
	//    i cel znajduje si� na pozycji b�d�cej w jej obr�bie, powi�kszonym o promie� rycerza. 

	CVector vNewTarget;  // Kandydat na nowy cel.
	int iCorr = 0;  // Licznik poprawek nowego celu.

	// Zliczenie wszystkich poprawnych pozycji celu wobec b�d�cych tu� obok u�ytkownika przeszk�d.
	std::vector<CObstacle*>::iterator It;
	for(It = m_CloseObstaclesContainer.begin(); It != m_CloseObstaclesContainer.end(); ++It) {
		if(g_CollisionController.CheckCollision(&m_vTarget, &CCircle((*It)->GetCollisionCircle()->pos, 
			 ((*It)->GetCollisionCircle()->r + g_DefaultsMgr.GetKnightRadius() + 0.5f)))) {
			CVector vTemp = (m_vTarget - *(*It)->GetPos());
			vTemp.SetMagnitude((*It)->GetCollisionCircle()->r + g_DefaultsMgr.GetKnightRadius() + 0.5f);
			vNewTarget += (*(*It)->GetPos() + vTemp);
			iCorr++;
		}
	}

	// 2. Przesuni�cie celu w przypadku, gdy rycerz jest tu� obok granic �wiata
	//    i cel znajduje si� na pozycji b�d�cej odpowiednio blisko tych granic.

	// Kontrola w poziomie.
	if((m_vTarget.x < (g_DefaultsMgr.GetKnightRadius() + 0.5f)) && (m_vPos.x <= (g_DefaultsMgr.GetKnightRadius() + 1.0f))) {
		CVector vTemp(m_vTarget);
		vTemp.x = (g_DefaultsMgr.GetKnightRadius() + 0.5f);
		vNewTarget += vTemp;
		iCorr++;
	}
	else if(((m_vTarget.x + g_DefaultsMgr.GetKnightRadius() + 0.5f) > static_cast<float>(g_DefaultsMgr.GetAreaWidth()))
			&& ((m_vPos.x + (g_DefaultsMgr.GetKnightRadius() + 1.0f)) >= static_cast<float>(g_DefaultsMgr.GetAreaWidth()))) {
		CVector vTemp(m_vTarget);
		vTemp.x = (static_cast<float>(g_DefaultsMgr.GetAreaWidth()) - g_DefaultsMgr.GetKnightRadius() - 0.5f);
		vNewTarget += vTemp;
		iCorr++;
	}

	// Kontrola w pionie.
	if((m_vTarget.y < (g_DefaultsMgr.GetKnightRadius() + 0.5f)) && (m_vPos.y <= (g_DefaultsMgr.GetKnightRadius() + 1.0f))) {
		CVector vTemp(m_vTarget);
		vTemp.y = (g_DefaultsMgr.GetKnightRadius() + 0.5f);
		vNewTarget += vTemp;
		iCorr++;
	}
	else if(((m_vTarget.y + g_DefaultsMgr.GetKnightRadius() + 0.5f) > static_cast<float>(g_DefaultsMgr.GetAreaHeight()))
			&& ((m_vPos.y + (g_DefaultsMgr.GetKnightRadius() + 1.0f)) >= static_cast<float>(g_DefaultsMgr.GetAreaHeight()))) {
		CVector vTemp(m_vTarget);
		vTemp.y = (static_cast<float>(g_DefaultsMgr.GetAreaHeight()) - g_DefaultsMgr.GetKnightRadius() - 0.5f);
		vNewTarget += vTemp;
		iCorr++;
	}

	// 3. Korekcja wektora celu, o ile licznik poprawek jest wi�kszy od zera.

	if(iCorr) {
		vNewTarget /= static_cast<float>(iCorr);  // Obliczenie �redniej.
		m_vTarget = vNewTarget;
	}
}

void CUser::WorldBound(void) {
	//-------------------------------------------------
	// Kontrola kolizji z granicami �wiata w poziomie.
	//-------------------------------------------------

	// Rycerz pr�buje opu�ci� �wiat z lewej strony.
	if(m_CollisionCircle.pos.x < g_DefaultsMgr.GetKnightRadius()) {
		m_vPos.x = m_CollisionCircle.r;
		ShiftCollisionCircle();
	}
	// Rycerz pr�buje opu�ci� �wiat z prawej strony.
	else if((m_CollisionCircle.pos.x + g_DefaultsMgr.GetKnightRadius()) > static_cast<float>(g_DefaultsMgr.GetAreaWidth())) 
	{
		m_vPos.x = (static_cast<float>(g_DefaultsMgr.GetAreaWidth()) - g_DefaultsMgr.GetKnightRadius());
		ShiftCollisionCircle();
	}

	//-----------------------------------------------
	// Kontrola kolizji z granicami �wiata w pionie.
	//-----------------------------------------------

	// Rycerz pr�buje opu�ci� �wiat z g�ry.
	if(m_CollisionCircle.pos.y < g_DefaultsMgr.GetKnightRadius()) {
		m_vPos.y = m_CollisionCircle.r;
		ShiftCollisionCircle();
	}
	// Rycerz pr�buje opu�ci� �wiat z do�u.
	else if((m_CollisionCircle.pos.y + g_DefaultsMgr.GetKnightRadius()) > static_cast<float>(g_DefaultsMgr.GetAreaHeight())) 
	{
		m_vPos.y = (static_cast<float>(g_DefaultsMgr.GetAreaHeight()) - g_DefaultsMgr.GetKnightRadius());
		ShiftCollisionCircle();
	}
}

void CUser::Target(void) {
	// Promie� ko�a, kt�re ustawione centralnie na pozycji celu, b�dzie decydowa�o o tym, czy u�ytkownik dotar� do celu.
	float fTargetRadius = (m_vPos.GetDist(m_vPos, m_vOldPos) / 2.0f);
	// Sk�d ten pomys�? Odleg�o�� przesuni�cia u�ytkownika obliczana jest na podstawie czasu, jaki up�yn�� od przesuni�cia
	// w jednej klatce do przesuni�cia w drugiej. Za��my, �e promie� by�by sta�y i wynosi� np. 1.0f, a pr�dko��
	// wykonywania programu spad�aby znacz�co. W�wczas u�ytkownik przemieszcza�by si� d�u�szymi "krokami". Powiedzmy, �e
	// by�yby to kroki wynosz�ce oko�o 5.0f d�ugo�ci. W tym przypadku nie trudno by�oby o to, aby rycerz stawiaj�c tak
	// d�ugie kroki, nie trafi� w obr�b ko�a celu o �rednicy 2.0f. Dlatego jako promie� ko�a zostaje przyj�ta
	// po�owa przesuni�cia. Zatem �rednica ko�a jest r�wna przesuni�ciu, w przytoczonym przypadku 5.0f, i nie ma mo�liwo�ci,
	// aby rycerz id�c nie natrafi� na to ko�o.
	
	// Dopilnowanie, aby promie� ko�a nie by� zbyt kr�tki.
	if(fTargetRadius < 0.5f)
		fTargetRadius = 0.5f;

	// Je�eli odleg�o�� od pozycji u�ytkownika do jego celu jest mniejsza od obliczonego promienia,
	// to u�ytkownik dotar� do celu.
	if(m_vTarget.GetDist(m_vTarget, m_vPos) <= fTargetRadius) {
		m_vPos = m_vTarget;  // U�ytkownik stoi teraz tam, dok�d d��y�.
		ShiftCollisionCircle();  // Synchronizacja ko�a kolizji rycerza z now� pozycj�, w�a�ciwie pozycj� celu.
		m_vVel.Set(0.0f, 0.0f);  // U�ytkownik b�dzie sta� w miejscu, dop�ki nie zidentyfikuje nowego celu.
		m_fSpeed = 0.0f;  // Ca�kowita pr�dko�� rycerza jest teraz r�wna zeru.
	}
}

void CUser::ShiftMoveControlCircle(void) { 
	//---------------------------------------------------------------
	// Zsynchronizowanie ko�a kontroluj�cego ruch z pozycj� rycerza.
	//---------------------------------------------------------------

	// Pozycja ko�a zwalczaj�cego "nadpobudliwo�� ruchu" zostaje zsynchronizowana z pozycj� u�ytkownika.
	m_MoveControlCircle.pos = m_vPos;

	//------------------------------
	// Znalezienie pozycji kursora.
	//------------------------------

	// Pobranie pozycji kursora i kamery.
	CVector vMouse(static_cast<float>(g_MouseController.GetX()), static_cast<float>(g_MouseController.GetY()));
	CVector vCamera(static_cast<float>(m_UserCamera.GetX()), static_cast<float>(m_UserCamera.GetY()));

	vMouse += vCamera;  // Obliczenie pozycji kursora;

	//-------------------------------------------------------
	// Ewentualna korekcja pozycji ko�a kontroluj�cego ruch.
	//-------------------------------------------------------

	CVector vNewMoveContCircPos;  // Kandydat na now� pozycj� ko�a kontroluj�cego ruch.
	int iMouseColl = 0;	 // Licznik zderze� myszki z poszerzonym obszarem granicznym maj�cym kontakt z rycerzem.

	// 1. Przesuni�cie ko�a kontroluj�cego ruch w przypadku, gdy rycerz jest tu� obok przeszkody
	//    i u�ytkownik wskazuje na pozycj� b�d�c� w jej obr�bie, powi�kszonym o promie� rycerza. 

	// Przejrzenie kontenera z b�d�cymi tu� obok rycerza przeszkodami. Je�eli u�ytkownik wskazuje na
	// poszerzony o promie� rycerza (i "ciut"), obszar graniczny kt�rej� z tych przeszk�d, to kandydant na now� pozycj� ko�a
	// zostanie odpowiednio ustawiony. Uwaga! Je�li wyst�pi� przynajmniej dwa takie przypadki, dla dw�ch r�nych przeszk�d,
	// to p�tla zostanie przerwana. W tym wypadku obliczony kandydat nie znajdzie zastosowania.
	std::vector<CObstacle*>::iterator It = m_CloseObstaclesContainer.begin();
	while(It != m_CloseObstaclesContainer.end()) {
		if(g_CollisionController.CheckCollision(&vMouse,
			 &CCircle((*It)->GetCollisionCircle()->pos, 
			 ((*It)->GetCollisionCircle()->r + g_DefaultsMgr.GetKnightRadius() + 0.5f)))) {
			iMouseColl++;
			if(iMouseColl > 1)
				break;
			float fDistance = vMouse.GetDist(vMouse, m_vPos);
			CVector vTemp = (*(*It)->GetPos() - m_vPos);
			vTemp.SetMagnitude(fDistance);
			vNewMoveContCircPos = (m_vPos + vTemp);
		}
		++It;
	}

	// 2. Przesuni�cie ko�a kontroluj�cego ruch w przypadku, gdy rycerz jest tu� obok granic �wiata
	//    i u�ytkownik wskazuje na pozycj� b�d�c� odpowiednio blisko tych granic.

	// Kontrola w poziomie.
	if(((vMouse.x < (g_DefaultsMgr.GetKnightRadius() + 0.5f)) && (m_vPos.x <= (g_DefaultsMgr.GetKnightRadius() + 1.0f)))
			|| (((vMouse.x + g_DefaultsMgr.GetKnightRadius() + 0.5f) > static_cast<float>(g_DefaultsMgr.GetAreaWidth()))
			&& ((m_vPos.x + (g_DefaultsMgr.GetKnightRadius() + 1.0f)) >= static_cast<float>(g_DefaultsMgr.GetAreaWidth())))) {
		vNewMoveContCircPos = m_MoveControlCircle.pos;
		vNewMoveContCircPos.x = vMouse.x;
		iMouseColl++;
	}

	// Kontrola w pionie.
	if(((vMouse.y < (g_DefaultsMgr.GetKnightRadius() + 0.5f)) && (m_vPos.y <= (g_DefaultsMgr.GetKnightRadius() + 1.0f)))
			|| (((vMouse.y + g_DefaultsMgr.GetKnightRadius() + 0.5f) > static_cast<float>(g_DefaultsMgr.GetAreaHeight()))
			&& ((m_vPos.y + (g_DefaultsMgr.GetKnightRadius() + 1.0f)) >= static_cast<float>(g_DefaultsMgr.GetAreaHeight())))) {
		vNewMoveContCircPos = m_MoveControlCircle.pos;
		vNewMoveContCircPos.y = vMouse.y;
		iMouseColl++;
	}

	// 3. Licznik zderze� musi by� r�wny jeden, aby nast�pi�a korekcja pozycji ko�a kontroluj�cego ruch.
	//    Korekta jest zb�dna, kiedy, przyk�adowo, u�ytkownik chce si� wcisn�� pomi�dzy przeszkody.

	if(iMouseColl == 1)
		m_MoveControlCircle.pos = vNewMoveContCircPos;
}

void CUser::SearchCloseObstacles(void) {
	m_CloseObstaclesContainer.clear();  // Wyczyszczenie kontenera z przeszkodami b�d�cymi tu� obok;

	// Znalezienie wszystkich b�d�cych tu� obok przeszk�d.
	std::vector<CObstacle*>::iterator It;
	for(It = g_ObstacleMgr.GetObstaclesContainer()->begin(); It != g_ObstacleMgr.GetObstaclesContainer()->end(); ++It)
		if(m_vPos.GetDist(m_vPos, *(*It)->GetPos()) 
			 <= ((*It)->GetCollisionCircle()->r + g_DefaultsMgr.GetKnightRadius() + 1.0f))
			m_CloseObstaclesContainer.push_back(*It);
}

void CUser::SetCamera(void) {
	//-------------------------------------------
	// Ustawienie kamery w odpowiedniej pozycji.
	//-------------------------------------------

	// Obliczenie wsp�rz�dnych x i y dla kamery wzgl�dem aktualnej pozycji u�ytkownika w obszarze �wiata.
	int iX = (static_cast<int>(m_vPos.x) - (m_UserCamera.GetWidth() / 2));
	int iY = (static_cast<int>(m_vPos.y) - (m_UserCamera.GetHeight() / 2));

	m_UserCamera.Set(iX, iY);  // Ustawienie kamery w odpowiedniej pozycji, wykorzystuj�c wy�ej obliczone wsp�rz�dne.

	//--------------------------------------------------------------------------------------
	// Ewentualna korekta pozycji kamery na wypadek wyj�cia jej obszaru poza obszar �wiata.
	//--------------------------------------------------------------------------------------

	// Ewentualna korekta pozycji kamery w poziomie.
	if(iX < 0) 
		m_UserCamera.SetX(0);
	else if(iX > (g_DefaultsMgr.GetAreaWidth() - m_UserCamera.GetWidth())) 
		m_UserCamera.SetX(g_DefaultsMgr.GetAreaWidth() - m_UserCamera.GetWidth());

	// Ewentualna korekta pozycji kamery w pionie.
	if(iY < 0) 
		m_UserCamera.SetY(0);
	else if(iY > (g_DefaultsMgr.GetAreaHeight() - m_UserCamera.GetHeight()))
		m_UserCamera.SetY(g_DefaultsMgr.GetAreaHeight() - m_UserCamera.GetHeight());
}
