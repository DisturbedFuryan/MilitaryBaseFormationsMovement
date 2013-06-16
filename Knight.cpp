#include "Knight.h"
#include "singletons.h"

bool IKnight::m_bVisibilityBacklight = true;  // Inicjalizacja zmiennej statycznej.

IKnight::IKnight(void) : m_iStatus(stand), m_fFrame(0.0f), m_bIsShoesprint(false) {
	m_CollisionCircle.r = g_DefaultsMgr.GetKnightRadius();  // Przypisanie warto�ci promieniowi ko�a kolizji.
}

IKnight::~IKnight(void) {
	//-------------------------------------
	// Usuni�cie �lad�w po butach rycerza.
	//-------------------------------------

	std::vector<CShoesprint*>::iterator It;
	for(It = m_ShoesprintsContainer.begin(); It != m_ShoesprintsContainer.end(); ) {
			delete *It;
			It = m_ShoesprintsContainer.erase(It);
			// Poniewa� elementy b�d�ce za elementem usuni�tym zostan� przesuni�te odpowiednio na jego miejsce,
			// nie ma potrzeby inkrementowania iteratora.
	}

	//----------------------------------
	// Usuni�cie pod�wietlenia rycerza.
	//----------------------------------

	delete m_pBacklight;
}

void IKnight::ShowShoesprints(void) {
	std::vector<CShoesprint*>::iterator It;
	for(It = m_ShoesprintsContainer.begin(); It != m_ShoesprintsContainer.end(); ++It)
		(*It)->Show();
}

void IKnight::PrintData(void) {
	g_DebugWindow.Print("    Knight ", m_iId, ":");
	g_DebugWindow.Print("      Position: (", m_vPos.x, "; ", m_vPos.y, ")");
	g_DebugWindow.Print("      Velocity: (", m_vVel.x, "; ", m_vVel.y, ")");
	g_DebugWindow.Print("      Angle: ", m_fAng);
	g_DebugWindow.Print("      Speed: ", m_fSpeed);
}

void IKnight::CollisionWithObstacles(void) {
	CVector vSafePos;  // Wektor "prawdopodobnie bezpiecznej pozycji".
	int ColCount = 0;  // Licznik przeszk�d, z kt�rymi rycerz koliduje.

	// Zsumowanie wszystkich pozycji, na kt�rych rycerz powinien stan�� po kolizji z przeszkodami.
	std::vector<CObstacle*>::iterator It;
	for(It = g_ObstacleMgr.GetObstaclesContainer()->begin(); It != g_ObstacleMgr.GetObstaclesContainer()->end(); ++It)
		if(g_CollisionController.CheckCollision(&m_CollisionCircle, (*It)->GetCollisionCircle())) {
			// Obliczenie wektora, kt�ry poprowadzony od pozycji koliduj�cej przeszkody wska�e pozycj� niekolizyjn�.
			CVector vTemp = (m_vPos - *(*It)->GetPos());
			vTemp.SetMagnitude((*It)->GetCollisionCircle()->r + g_DefaultsMgr.GetKnightRadius());
			vSafePos += (*(*It)->GetPos() + vTemp);
			ColCount++;  // Inkrementacja licznika kolizji.
		}

	if(ColCount) {  // Je�eli zosta�a zlokalizowana przynajmniej jedna koliduj�ca z rycerzem przeszkoda.
		vSafePos /= static_cast<float>(ColCount);  // U�rednienie wektora prawdopodobnie bezpiecznej pozycji.
		m_vPos = vSafePos;  // Pozycja rycerza zostaje ustawiona na t� prawdopodobnie bezpieczn�.
		ShiftCollisionCircle();  // Synchronizacja ko�a kolizji z rycerzem.
	}
}

void IKnight::CollisionWithOthers(void) {
	CVector vSafePos;  // Wektor "prawdopodobnie bezpiecznej pozycji".
	int ColCount = 0;  // Licznik rycerzy, z kt�rymi nast�pi�a kolizja.

	// Zsumowanie wszystkich pozycji, na kt�rych rycerz powinien stan�� po kolizji z pozosta�ymi rycerzami.
	std::vector<IKnight*>::iterator It;
	for(It = g_KnightMgr.GetKnightsContainer()->begin(); It != g_KnightMgr.GetKnightsContainer()->end(); ++It) {
		if(*It == this)  // Nie bierzemy pod uwag� kolzji z samym sob�...
			continue;
		if(g_CollisionController.CheckCollision(&m_CollisionCircle, (*It)->GetCollisionCircle())) {
			CVector vTemp = (m_vPos - *(*It)->GetPos());
			vTemp.SetMagnitude(g_DefaultsMgr.GetKnightRadius() * 2.0f);
			vSafePos += (*(*It)->GetPos() + vTemp);
			ColCount++;  // Inkrementacja licznika kolizji.
		}
	}

	if(ColCount) {  // Je�eli zosta� zlokalizowany przynajmniej jeden koliduj�cy rycerz.
		vSafePos /= static_cast<float>(ColCount);  // U�rednienie wektora prawdopodobnie bezpiecznej pozycji.
		m_vPos = vSafePos;  // Pozycja rycerza zostaje ustawiona na t� prawdopodobnie bezpieczn�.
		ShiftCollisionCircle();  // Synchronizacja ko�a kolizji z rycerzem.
	}
}

void IKnight::UpdateFrameAndStatus(const Uint32 iDeltaTicks) {
	m_iPreviousFrame = static_cast<short>(m_fFrame);  // Zapami�tanie poprzedniej klatki animacji.
	if(!m_vVel.IsZero()) {  // Je�eli wektor pr�dko�ci rycerza nie jest zerowy.
		m_iStatus = walk;  // Aktualizacja statusu. Rycerz si� porusza.
		m_fFrame += ((m_fSpeed / 8.0f) * (iDeltaTicks / 1000.0f));  // Aktualizacja klatki animacji.
		if(m_fFrame >= 8.0f) m_fFrame = 0.0f;  // Zap�tlenie animacji.
		m_fOldSpeed = m_fSpeed;  // Zapami�tanie ostatniej pr�dko�ci, z jak� porusza� si� rycerz.
	}
	// W przeciwnym wypadku nast�puje sprawdzenie, czy rycerz nie posiada
	// jeszcze statusu "stand", mimo i� jego wektor pr�dko�ci jest zerowy.
	else if(m_iStatus != stand) {
		// Aktualizacja klatki na podstawie ostatniej pr�dko�ci, z jak� porusza� si� rycerz.
		m_fFrame += ((m_fOldSpeed / 8.0f) * (iDeltaTicks / 1000.0f));

		// W przypadku zmiany klatki animacji, status zostaje zaktualizowany, a animacja zostaje zrestartowana.
		if(IsFrameChanged()) {
			m_iStatus = stand;  // Aktualizacja statusu. Rycerz stoi w bezruchu, animacja klatki dobieg�a ko�ca.
			m_fFrame = 0.0f;  // Restart animacji.
		}
	}
}

void IKnight::ComputeYaw(const Uint32 iDeltaTicks) {
	//----------------------------------------------------------
	// Obliczenie k�ta, pod jakim powinien porusza� si� rycerz.
	//----------------------------------------------------------

	float fSuggestedAng = m_vVel.Yaw();  // Sugerowany k�t jest odchyleniem wektora pr�dko�ci.

	//--------------------------------------------------------------------------------
	// Obliczenie k�ta w�a�ciwego, pod jakim rzeczywi�cie b�dzie porusza� si� rycerz.
	//--------------------------------------------------------------------------------

	// Dlaczego? Czy sugerowanego k�ta nie mo�na po prostu przypisa� k�towi w�a�ciwemu?
	// Nie, nale�y jeszcze uwzgl�dni� czas, rycerz nie mo�e si� przecie� obraca� z "pr�dko�ci� �wiat�a".
	// Zapobiegnie to r�wnie� przesadnemu "drganiu" rycerzy obieraj�cych co rusz to nowy kierunek.
	// Jako pr�dko�� obrotu zostanie przyj�ta warto�� k�ta pomi�dzy k�tem w�a�ciwym a k�tem sugerowanym,
	// pomno�ona przez trzy dla uzyskania optymalnego efektu.

	FetchToAng(fSuggestedAng, iDeltaTicks);
}

void IKnight::FetchToAng(float fSuggestedAng, const Uint32 iDeltaTicks) {
	// W celu znalezienia najkr�tszego obrotu rycerza trzeba rozr�ni� dwa przypadki:
	
	// 1. przypadek: rycerz podczas obrotu powinien przekroczy� k�t 0 stopni.
	if(static_cast<float>(fabs(fSuggestedAng - m_fAng)) > 180.0f) {
		if(m_fAng < fSuggestedAng) {  // Je�li k�t w�a�ciwy jest mniejszy od k�ta sugerowanego.
			// Zmniejszenie k�ta w�a�ciwego z uwzgl�dnieniem czasu, rycerz powinien obraca� si� prawoskr�tnie.
			m_fAng -= (((m_fAng + (360.0f - fSuggestedAng)) * 3.0f) * (iDeltaTicks / 1000.0f));

			if(m_fAng < 0.0f) {  // W przypadku, gdy k�t w�a�ciwy wyjdzie poza skal� i b�dzie mniejszy od zera.
				// Skorygowanie k�ta w�a�ciwego - "przerzucenie" go do pierwszej �wiartki uk�adu wsp�rz�dnych.
				m_fAng = (360.0f + m_fAng);

				// Je�li k�t w�a�ciwy zosta� "przerzucony" do �wiartki, w kt�rej aktualnie znajduje si� k�t sugerowany,
				// nale�y sprawdzi�, czy czasem nie przekroczy� on owego k�ta.
				if(m_fAng < fSuggestedAng) m_fAng = fSuggestedAng;  // Skorygowanie k�ta w�a�ciwego.
			}
		}
		else if(m_fAng > fSuggestedAng) {  // Je�li k�t w�a�ciwy jest wi�kszy od k�ta sugerowanego.
			// Powi�kszenie k�ta w�a�ciwego z uwzgl�dnieniem czasu, rycerz powinien obraca� si� lewoskr�tnie.
			m_fAng += ((((360.0f - m_fAng) + fSuggestedAng) * 3.0f) * (iDeltaTicks / 1000.0f));

			if(m_fAng >= 360.0f) {  // W przypadku, gdy k�t w�a�ciwy wyjdzie poza skal� i b�dzie wi�kszy, r�wny 360 stopni.
				// Skorygowanie k�ta w�a�ciwego - "przerzucenie" go do drugiej �wiartki uk�adu wsp�rz�dnych.
				m_fAng = (m_fAng - 360.0f);

				// Je�li k�t w�a�ciwy zosta� "przerzucony" do �wiartki, w kt�rej aktualnie znajduje si� k�t sugerowany,
				// nale�y sprawdzi�, czy czasem nie przekroczy� on owego k�ta.
				if(m_fAng > fSuggestedAng) m_fAng = fSuggestedAng;  // Skorygowanie k�ta w�a�ciwego.
			}
		}
	}
	// 2. przypadek: rycerz podczas obrotu nie powinien przekroczy� k�ta 0 stopni.
	else {
		// Je�li k�t w�a�ciwy jest mniejszy od k�ta sugerowanego, to...
		if(m_fAng < fSuggestedAng) {
			// ... nale�y go powi�kszy�, uwzgl�dniaj�c przy tym czas.
			m_fAng += (((fSuggestedAng - m_fAng) * 3.0f) * (iDeltaTicks / 1000.0f));

			if(m_fAng > fSuggestedAng) m_fAng = fSuggestedAng;  // Zabezpieczenie przed przekroczeniem k�ta sugerowanego.
		}
		// Je�li k�t w�a�ciwy jest wi�kszy od k�ta sugerowanego, to...
		else if(m_fAng > fSuggestedAng) {
			// ... nale�y go zmniejszy�, uwzgl�dniaj�c przy tym czas.
			m_fAng -= (((m_fAng - fSuggestedAng) * 3.0f) * (iDeltaTicks / 1000.0f));

			if(m_fAng < fSuggestedAng) m_fAng = fSuggestedAng;  // Zabezpieczenie przed przekroczeniem k�ta sugerowanego.
		}
	}
}

void IKnight::UpdateShoesprintsContainer(const Uint32 iDeltaTicks) {
	//----------------------------------------------
	// Aktualizacja ka�dego ze �lad�w w kontenerze.
	//----------------------------------------------

	std::vector<CShoesprint*>::iterator It;
	for(It = m_ShoesprintsContainer.begin(); It != m_ShoesprintsContainer.end(); ++It)
		(*It)->Update(iDeltaTicks);

	//------------------------------
	// Tworzenie i usuwanie �lad�w.
	//------------------------------

	// W przypadku zmiany klatki animacji flaga zostaje przestawiona.
	// Nie powsta� jeszcze �aden �lad po bucie w nowej klatce animacji.
	if(IsFrameChanged()) m_bIsShoesprint = false;

	// Je�li rycerz stoi, zostanie podj�ta pr�ba wyczyszczenia ca�ego kontenera.
	if(m_iStatus == stand) {
		std::vector<CShoesprint*>::iterator It;
		for(It = m_ShoesprintsContainer.begin(); It != m_ShoesprintsContainer.end(); ) {
			if((*It)->IsDead()) {  // Je�eli �lad po bucie mo�e zako�czy� sw�j �ywot...
				// ...to niech go zako�czy.
				delete *It;
				It = m_ShoesprintsContainer.erase(It);
			}
			else
				// W przypadku, gdy element nie zosta� usuni�ty i elementy b�d�ce za nim nie zosta�y przesuni�te
				// odpowiednio na jego miejsce, nale�y przej�� do kolejnego poprzez inkrementacj� iteratora.
				++It;
		}
	}
	// W przeciwnym wypadku, i w wypadku, gdy zajdzie takowa potrzeba, zostanie zniszczony najbli�szy "martwy obiekt"
	// i zast�piony nowym. Ewentualnie nowy obiekt zostanie po prostu przypisany do ko�ca kontenera.
	else {
		// Podczas pierwszej klatki animacji rycerz zostawia prawy �lad po bucie.
		// Nale�y przy tym sprawdzi�, czy �lad ten nie powsta� ju� wcze�niej.
		if((static_cast<short>(m_fFrame) == 1) && !m_bIsShoesprint) {
			// Stworzenie prawego �ladu po bucie.
			CShoesprint* pNewShoesprint = new CShoesprint(&m_vPos, m_fAng, CShoesprint::right);

			// Pr�ba zast�pienia "martwego obiektu" nowym.
			std::vector<CShoesprint*>::iterator It = m_ShoesprintsContainer.begin();
			while(It != m_ShoesprintsContainer.end()) {
				if((*It)->IsDead()) {  // Element posiadaj�cy "ch�� do �mierci" zosta� odnaleziony.
					delete *It;  // Spe�nienie ch�ci elementu poprzez jego usuni�cie.
					*It = pNewShoesprint;  // Wstawienie nowego elementu w miejsce "martwego".
					m_bIsShoesprint = true;  // Ustawienie flagi. Powsta� nowy �lad po bucie...
					break;  // ...wi�c mo�na przerwa� p�tl�.
				}
				++It;  // Inkrementacja iteratora.
			}

			// W przypadku nieznalezienia "martwego obiektu", nowy �lad buta zostanie przypisany do ko�ca kontenera.
			if(!m_bIsShoesprint) {
				m_ShoesprintsContainer.push_back(pNewShoesprint);
				m_bIsShoesprint = true;  // Ustawienie flagi. Powsta� nowy �lad po bucie.
			}
		}
		// Natomiast podczas pi�tej klatki animacji rycerz zostawia lewy �lad po bucie.
		// R�wnie� nale�y sprawdzi� przy tym, czy �lad ten nie powsta� ju� wcze�niej.
		else if((static_cast<short>(m_fFrame) == 5) && !m_bIsShoesprint) {
			// Stworzenie lewego �ladu po bucie.
			CShoesprint* pNewShoesprint = new CShoesprint(&m_vPos, m_fAng, CShoesprint::left);

			// Analogiczne post�powanie, jak przy tworzeniu prawego �ladu po bucie.
			std::vector<CShoesprint*>::iterator It = m_ShoesprintsContainer.begin();
			while(It != m_ShoesprintsContainer.end()) {
				if((*It)->IsDead()) {
					delete *It;
					*It = pNewShoesprint;
					m_bIsShoesprint = true;
					break;
				}
				++It;
			}
			if(!m_bIsShoesprint) {
				m_ShoesprintsContainer.push_back(pNewShoesprint);
				m_bIsShoesprint = true;
			}
		}
	}
}
