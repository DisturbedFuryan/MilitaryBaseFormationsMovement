#include "User.h"
#include "singletons.h"

CUser::CUser(short iId, CVector* vPos, short iColour) : m_bIsTargetVisible(false) {
	//--------------------------------
	// Przypisanie wartoœci zmiennym.
	//--------------------------------

	m_iId = iId;  // Przypisanie wartoœci identyfikatorowi rycerza.
	m_vPos = *vPos;  // Przypisanie podanej pozycji rycerzowi.
	ShiftCollisionCircle();  // Synchronizacja ko³a kolizji z pozycj¹.
	m_vTarget = m_vPos;  // Przypisanie wektorowi celu pozycji u¿ytkownika.

	// Przypisanie wartoœci promieniowi ko³a kontroluj¹cego ruchu.
	m_MoveControlCircle.r = (g_DefaultsMgr.GetKnightRadius() / 3.0f);

	ShiftMoveControlCircle();  // Zsynchronizowanie ko³a kontroluj¹cego ruch z pozycj¹ rycerza.
	m_fAng = 0.0f;  // Przypisanie pocz¹tkowej wartoœci orientacji rycerza.

	// Przypisanie wartoœci prostok¹tom opisuj¹cym ko³o kontroluj¹ce ruch i cel u¿ytkownika.

	MoveControlCircRect.w = (static_cast<int>(m_MoveControlCircle.r) * 2);
	MoveControlCircRect.h = (static_cast<int>(m_MoveControlCircle.r) * 2);

	TargetRect.w = 4;
	TargetRect.h = 4;

	//-----------------------------------------
	// Utworzenie odpowiedniego podœwietlenia.
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

	m_vOldPos = m_vPos;  // Zapamiêtanie ostatniej pozycji.
	m_vPos += (m_vVel * (iDeltaTicks / 1000.0f));  // Uaktualnienie pozycji wzglêdem wektora prêdkoœci.
	ShiftCollisionCircle();  // Zsynchronizowanie ko³a kolizji z pozycj¹.

	//-------------------------------
	// Obliczenie wektora prêdkoœci.
	//-------------------------------

	m_vOldVel = m_vVel;  // Zapamiêtanie ostatniej prêdkoœci.
	LocalizeTarget();  // Zlokalizowanie celu.
	ComputeVel();  // Obliczenie wektora prêdkoœci na podstawie celu.
	m_fSpeed = m_vVel.Length();  // Obliczenie ca³kowitej prêdkoœci u¿ytkownika.

	//---------------------
	// Implementacja celu.
	//---------------------

	Target();  // Metoda mo¿e mieæ drastyczny wp³yw na wektor prêdkoœci!

	//------------------------
	// Obliczenie orientacji.
	//------------------------

	if(!m_vVel.IsZero())  // Wektor prêdkoœci nie mo¿e byæ zerowy! Jest wykorzystywany przy obliczaniu orientacji.
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
	// Implementacja granic œwiata.
	//------------------------------

	WorldBound();

	//-----------------------------------------
	// Aktualizacja klatki i statusu animacji.
	//-----------------------------------------

	UpdateFrameAndStatus(iDeltaTicks);

	//------------------------------------------
	// Aktualizacja kontenera œladów po butach.
	//------------------------------------------

	UpdateShoesprintsContainer(iDeltaTicks);

	//-----------------------------
	// Aktualizacja podœwietlenia.
	//-----------------------------

	m_pBacklight->Update(iDeltaTicks);

	//-------------------------------------------------
	// Ustawienie kamery centralnie nad u¿ytkownikiem.
	//-------------------------------------------------

	SetCamera();
}

void CUser::HandleInput(void) {
	//Je¿eli zostanie naciœniêta spacja, podœwietlenie rycerzy zniknie/pojawi siê.
	if((g_EventMgr.GetEvent()->type == SDL_KEYDOWN) && (g_EventMgr.GetEvent()->key.keysym.sym == SDLK_SPACE))
		m_bVisibilityBacklight = !m_bVisibilityBacklight;  // Aktualizacja flagi.

	//Je¿eli zostanie naciœniêta litera "t", cel rycerza zniknie/pojawi siê.
	if((g_EventMgr.GetEvent()->type == SDL_KEYDOWN) && (g_EventMgr.GetEvent()->key.keysym.sym == SDLK_t))
		m_bIsTargetVisible = !m_bIsTargetVisible;
}

void CUser::ShowTarget(void) {
	if(m_bIsTargetVisible) {  // Je¿eli cel i ko³o kontroluj¹ce ruch s¹ widoczne.
		MoveControlCircRect.x = (static_cast<int>(m_MoveControlCircle.pos.x) - (MoveControlCircRect.w / 2));
		MoveControlCircRect.y = (static_cast<int>(m_MoveControlCircle.pos.y) - (MoveControlCircRect.h / 2));
		if(g_CollisionController.CheckCollision(&MoveControlCircRect, g_Camera.GetRect())) {
			MoveControlCircRect.x -= g_Camera.GetX();
			MoveControlCircRect.y -= g_Camera.GetY();

			// Zapamiêtanie szerokoœci i wysokoœci prostok¹ta, poniewa¿ SDL_FillRect() mo¿e go zmodyfikowaæ.
			// Dzieje siê to wtedy, kiedy wype³niany kolorem prostok¹t wyjdzie poza ekran.
			int iW = MoveControlCircRect.w;
			int iH = MoveControlCircRect.h;

			SDL_FillRect(g_AppWindow.GetScreenSurf(), &MoveControlCircRect, 
									 SDL_MapRGB(g_AppWindow.GetScreenSurf()->format, 0x00, 0x00, 0x00));

			// Renowacja wartoœci prostok¹ta.
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
	// Wyszukanie przeszkód bêd¹cych tu¿ obok u¿ytkownika.
	//-----------------------------------------------------

	SearchCloseObstacles();

	//--------------------------------------------------------
	// Zsynchronizowanie ko³a kontroluj¹cego ruch z rycerzem.
	//--------------------------------------------------------

	ShiftMoveControlCircle();

	//----------------------
	// Zlokalizowanie celu.
	//----------------------

	if(g_MouseController.GetIsLeftButDown()) {  // Je¿eli lewy przycisk myszki jest wciœniêty.
		//------------------------------
		// Znalezienie pozycji kursora.
		//------------------------------

		// Pobranie pozycji kursora i kamery.
		CVector vMouse(static_cast<float>(g_MouseController.GetX()), static_cast<float>(g_MouseController.GetY()));
		CVector vCamera(static_cast<float>(m_UserCamera.GetX()), static_cast<float>(m_UserCamera.GetY()));

		vMouse += vCamera;  // Obliczenie pozycji kursora;

		//--------------------------------------
		// Obliczenie wektora celu u¿ytkownika.
		//--------------------------------------

		// W przypadku, je¿eli pozycja kursora nie koliduje z ko³em kontroluj¹cym ruch,
		// rycerz mo¿e bez przeszkód obraæ cel wskazany przez u¿ytkownika.
		if(!g_CollisionController.CheckCollision(&vMouse, &m_MoveControlCircle))
			m_vTarget = vMouse;
	}
	
	//-------------------------------
	// Ewentualne przesuniêcie celu.
	//-------------------------------

	// 1. Przesuniêcie celu w przypadku, gdy rycerz jest tu¿ obok przeszkody
	//    i cel znajduje siê na pozycji bêd¹cej w jej obrêbie, powiêkszonym o promieñ rycerza. 

	CVector vNewTarget;  // Kandydat na nowy cel.
	int iCorr = 0;  // Licznik poprawek nowego celu.

	// Zliczenie wszystkich poprawnych pozycji celu wobec bêd¹cych tu¿ obok u¿ytkownika przeszkód.
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

	// 2. Przesuniêcie celu w przypadku, gdy rycerz jest tu¿ obok granic œwiata
	//    i cel znajduje siê na pozycji bêd¹cej odpowiednio blisko tych granic.

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

	// 3. Korekcja wektora celu, o ile licznik poprawek jest wiêkszy od zera.

	if(iCorr) {
		vNewTarget /= static_cast<float>(iCorr);  // Obliczenie œredniej.
		m_vTarget = vNewTarget;
	}
}

void CUser::WorldBound(void) {
	//-------------------------------------------------
	// Kontrola kolizji z granicami œwiata w poziomie.
	//-------------------------------------------------

	// Rycerz próbuje opuœciæ œwiat z lewej strony.
	if(m_CollisionCircle.pos.x < g_DefaultsMgr.GetKnightRadius()) {
		m_vPos.x = m_CollisionCircle.r;
		ShiftCollisionCircle();
	}
	// Rycerz próbuje opuœciæ œwiat z prawej strony.
	else if((m_CollisionCircle.pos.x + g_DefaultsMgr.GetKnightRadius()) > static_cast<float>(g_DefaultsMgr.GetAreaWidth())) 
	{
		m_vPos.x = (static_cast<float>(g_DefaultsMgr.GetAreaWidth()) - g_DefaultsMgr.GetKnightRadius());
		ShiftCollisionCircle();
	}

	//-----------------------------------------------
	// Kontrola kolizji z granicami œwiata w pionie.
	//-----------------------------------------------

	// Rycerz próbuje opuœciæ œwiat z góry.
	if(m_CollisionCircle.pos.y < g_DefaultsMgr.GetKnightRadius()) {
		m_vPos.y = m_CollisionCircle.r;
		ShiftCollisionCircle();
	}
	// Rycerz próbuje opuœciæ œwiat z do³u.
	else if((m_CollisionCircle.pos.y + g_DefaultsMgr.GetKnightRadius()) > static_cast<float>(g_DefaultsMgr.GetAreaHeight())) 
	{
		m_vPos.y = (static_cast<float>(g_DefaultsMgr.GetAreaHeight()) - g_DefaultsMgr.GetKnightRadius());
		ShiftCollisionCircle();
	}
}

void CUser::Target(void) {
	// Promieñ ko³a, które ustawione centralnie na pozycji celu, bêdzie decydowa³o o tym, czy u¿ytkownik dotar³ do celu.
	float fTargetRadius = (m_vPos.GetDist(m_vPos, m_vOldPos) / 2.0f);
	// Sk¹d ten pomys³? Odleg³oœæ przesuniêcia u¿ytkownika obliczana jest na podstawie czasu, jaki up³yn¹³ od przesuniêcia
	// w jednej klatce do przesuniêcia w drugiej. Za³ó¿my, ¿e promieñ by³by sta³y i wynosi³ np. 1.0f, a prêdkoœæ
	// wykonywania programu spad³aby znacz¹co. Wówczas u¿ytkownik przemieszcza³by siê d³u¿szymi "krokami". Powiedzmy, ¿e
	// by³yby to kroki wynosz¹ce oko³o 5.0f d³ugoœci. W tym przypadku nie trudno by³oby o to, aby rycerz stawiaj¹c tak
	// d³ugie kroki, nie trafi³ w obrêb ko³a celu o œrednicy 2.0f. Dlatego jako promieñ ko³a zostaje przyjêta
	// po³owa przesuniêcia. Zatem œrednica ko³a jest równa przesuniêciu, w przytoczonym przypadku 5.0f, i nie ma mo¿liwoœci,
	// aby rycerz id¹c nie natrafi³ na to ko³o.
	
	// Dopilnowanie, aby promieñ ko³a nie by³ zbyt krótki.
	if(fTargetRadius < 0.5f)
		fTargetRadius = 0.5f;

	// Je¿eli odleg³oœæ od pozycji u¿ytkownika do jego celu jest mniejsza od obliczonego promienia,
	// to u¿ytkownik dotar³ do celu.
	if(m_vTarget.GetDist(m_vTarget, m_vPos) <= fTargetRadius) {
		m_vPos = m_vTarget;  // U¿ytkownik stoi teraz tam, dok¹d d¹¿y³.
		ShiftCollisionCircle();  // Synchronizacja ko³a kolizji rycerza z now¹ pozycj¹, w³aœciwie pozycj¹ celu.
		m_vVel.Set(0.0f, 0.0f);  // U¿ytkownik bêdzie sta³ w miejscu, dopóki nie zidentyfikuje nowego celu.
		m_fSpeed = 0.0f;  // Ca³kowita prêdkoœæ rycerza jest teraz równa zeru.
	}
}

void CUser::ShiftMoveControlCircle(void) { 
	//---------------------------------------------------------------
	// Zsynchronizowanie ko³a kontroluj¹cego ruch z pozycj¹ rycerza.
	//---------------------------------------------------------------

	// Pozycja ko³a zwalczaj¹cego "nadpobudliwoœæ ruchu" zostaje zsynchronizowana z pozycj¹ u¿ytkownika.
	m_MoveControlCircle.pos = m_vPos;

	//------------------------------
	// Znalezienie pozycji kursora.
	//------------------------------

	// Pobranie pozycji kursora i kamery.
	CVector vMouse(static_cast<float>(g_MouseController.GetX()), static_cast<float>(g_MouseController.GetY()));
	CVector vCamera(static_cast<float>(m_UserCamera.GetX()), static_cast<float>(m_UserCamera.GetY()));

	vMouse += vCamera;  // Obliczenie pozycji kursora;

	//-------------------------------------------------------
	// Ewentualna korekcja pozycji ko³a kontroluj¹cego ruch.
	//-------------------------------------------------------

	CVector vNewMoveContCircPos;  // Kandydat na now¹ pozycjê ko³a kontroluj¹cego ruch.
	int iMouseColl = 0;	 // Licznik zderzeñ myszki z poszerzonym obszarem granicznym maj¹cym kontakt z rycerzem.

	// 1. Przesuniêcie ko³a kontroluj¹cego ruch w przypadku, gdy rycerz jest tu¿ obok przeszkody
	//    i u¿ytkownik wskazuje na pozycjê bêd¹c¹ w jej obrêbie, powiêkszonym o promieñ rycerza. 

	// Przejrzenie kontenera z bêd¹cymi tu¿ obok rycerza przeszkodami. Je¿eli u¿ytkownik wskazuje na
	// poszerzony o promieñ rycerza (i "ciut"), obszar graniczny którejœ z tych przeszkód, to kandydant na now¹ pozycjê ko³a
	// zostanie odpowiednio ustawiony. Uwaga! Jeœli wyst¹pi¹ przynajmniej dwa takie przypadki, dla dwóch ró¿nych przeszkód,
	// to pêtla zostanie przerwana. W tym wypadku obliczony kandydat nie znajdzie zastosowania.
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

	// 2. Przesuniêcie ko³a kontroluj¹cego ruch w przypadku, gdy rycerz jest tu¿ obok granic œwiata
	//    i u¿ytkownik wskazuje na pozycjê bêd¹c¹ odpowiednio blisko tych granic.

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

	// 3. Licznik zderzeñ musi byæ równy jeden, aby nast¹pi³a korekcja pozycji ko³a kontroluj¹cego ruch.
	//    Korekta jest zbêdna, kiedy, przyk³adowo, u¿ytkownik chce siê wcisn¹æ pomiêdzy przeszkody.

	if(iMouseColl == 1)
		m_MoveControlCircle.pos = vNewMoveContCircPos;
}

void CUser::SearchCloseObstacles(void) {
	m_CloseObstaclesContainer.clear();  // Wyczyszczenie kontenera z przeszkodami bêd¹cymi tu¿ obok;

	// Znalezienie wszystkich bêd¹cych tu¿ obok przeszkód.
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

	// Obliczenie wspó³rzêdnych x i y dla kamery wzglêdem aktualnej pozycji u¿ytkownika w obszarze œwiata.
	int iX = (static_cast<int>(m_vPos.x) - (m_UserCamera.GetWidth() / 2));
	int iY = (static_cast<int>(m_vPos.y) - (m_UserCamera.GetHeight() / 2));

	m_UserCamera.Set(iX, iY);  // Ustawienie kamery w odpowiedniej pozycji, wykorzystuj¹c wy¿ej obliczone wspó³rzêdne.

	//--------------------------------------------------------------------------------------
	// Ewentualna korekta pozycji kamery na wypadek wyjœcia jej obszaru poza obszar œwiata.
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
