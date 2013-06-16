#include "Knight.h"
#include "singletons.h"

bool IKnight::m_bVisibilityBacklight = true;  // Inicjalizacja zmiennej statycznej.

IKnight::IKnight(void) : m_iStatus(stand), m_fFrame(0.0f), m_bIsShoesprint(false) {
	m_CollisionCircle.r = g_DefaultsMgr.GetKnightRadius();  // Przypisanie wartoœci promieniowi ko³a kolizji.
}

IKnight::~IKnight(void) {
	//-------------------------------------
	// Usuniêcie œladów po butach rycerza.
	//-------------------------------------

	std::vector<CShoesprint*>::iterator It;
	for(It = m_ShoesprintsContainer.begin(); It != m_ShoesprintsContainer.end(); ) {
			delete *It;
			It = m_ShoesprintsContainer.erase(It);
			// Poniewa¿ elementy bêd¹ce za elementem usuniêtym zostan¹ przesuniête odpowiednio na jego miejsce,
			// nie ma potrzeby inkrementowania iteratora.
	}

	//----------------------------------
	// Usuniêcie podœwietlenia rycerza.
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
	int ColCount = 0;  // Licznik przeszkód, z którymi rycerz koliduje.

	// Zsumowanie wszystkich pozycji, na których rycerz powinien stan¹æ po kolizji z przeszkodami.
	std::vector<CObstacle*>::iterator It;
	for(It = g_ObstacleMgr.GetObstaclesContainer()->begin(); It != g_ObstacleMgr.GetObstaclesContainer()->end(); ++It)
		if(g_CollisionController.CheckCollision(&m_CollisionCircle, (*It)->GetCollisionCircle())) {
			// Obliczenie wektora, który poprowadzony od pozycji koliduj¹cej przeszkody wska¿e pozycjê niekolizyjn¹.
			CVector vTemp = (m_vPos - *(*It)->GetPos());
			vTemp.SetMagnitude((*It)->GetCollisionCircle()->r + g_DefaultsMgr.GetKnightRadius());
			vSafePos += (*(*It)->GetPos() + vTemp);
			ColCount++;  // Inkrementacja licznika kolizji.
		}

	if(ColCount) {  // Je¿eli zosta³a zlokalizowana przynajmniej jedna koliduj¹ca z rycerzem przeszkoda.
		vSafePos /= static_cast<float>(ColCount);  // Uœrednienie wektora prawdopodobnie bezpiecznej pozycji.
		m_vPos = vSafePos;  // Pozycja rycerza zostaje ustawiona na t¹ prawdopodobnie bezpieczn¹.
		ShiftCollisionCircle();  // Synchronizacja ko³a kolizji z rycerzem.
	}
}

void IKnight::CollisionWithOthers(void) {
	CVector vSafePos;  // Wektor "prawdopodobnie bezpiecznej pozycji".
	int ColCount = 0;  // Licznik rycerzy, z którymi nast¹pi³a kolizja.

	// Zsumowanie wszystkich pozycji, na których rycerz powinien stan¹æ po kolizji z pozosta³ymi rycerzami.
	std::vector<IKnight*>::iterator It;
	for(It = g_KnightMgr.GetKnightsContainer()->begin(); It != g_KnightMgr.GetKnightsContainer()->end(); ++It) {
		if(*It == this)  // Nie bierzemy pod uwagê kolzji z samym sob¹...
			continue;
		if(g_CollisionController.CheckCollision(&m_CollisionCircle, (*It)->GetCollisionCircle())) {
			CVector vTemp = (m_vPos - *(*It)->GetPos());
			vTemp.SetMagnitude(g_DefaultsMgr.GetKnightRadius() * 2.0f);
			vSafePos += (*(*It)->GetPos() + vTemp);
			ColCount++;  // Inkrementacja licznika kolizji.
		}
	}

	if(ColCount) {  // Je¿eli zosta³ zlokalizowany przynajmniej jeden koliduj¹cy rycerz.
		vSafePos /= static_cast<float>(ColCount);  // Uœrednienie wektora prawdopodobnie bezpiecznej pozycji.
		m_vPos = vSafePos;  // Pozycja rycerza zostaje ustawiona na t¹ prawdopodobnie bezpieczn¹.
		ShiftCollisionCircle();  // Synchronizacja ko³a kolizji z rycerzem.
	}
}

void IKnight::UpdateFrameAndStatus(const Uint32 iDeltaTicks) {
	m_iPreviousFrame = static_cast<short>(m_fFrame);  // Zapamiêtanie poprzedniej klatki animacji.
	if(!m_vVel.IsZero()) {  // Je¿eli wektor prêdkoœci rycerza nie jest zerowy.
		m_iStatus = walk;  // Aktualizacja statusu. Rycerz siê porusza.
		m_fFrame += ((m_fSpeed / 8.0f) * (iDeltaTicks / 1000.0f));  // Aktualizacja klatki animacji.
		if(m_fFrame >= 8.0f) m_fFrame = 0.0f;  // Zapêtlenie animacji.
		m_fOldSpeed = m_fSpeed;  // Zapamiêtanie ostatniej prêdkoœci, z jak¹ porusza³ siê rycerz.
	}
	// W przeciwnym wypadku nastêpuje sprawdzenie, czy rycerz nie posiada
	// jeszcze statusu "stand", mimo i¿ jego wektor prêdkoœci jest zerowy.
	else if(m_iStatus != stand) {
		// Aktualizacja klatki na podstawie ostatniej prêdkoœci, z jak¹ porusza³ siê rycerz.
		m_fFrame += ((m_fOldSpeed / 8.0f) * (iDeltaTicks / 1000.0f));

		// W przypadku zmiany klatki animacji, status zostaje zaktualizowany, a animacja zostaje zrestartowana.
		if(IsFrameChanged()) {
			m_iStatus = stand;  // Aktualizacja statusu. Rycerz stoi w bezruchu, animacja klatki dobieg³a koñca.
			m_fFrame = 0.0f;  // Restart animacji.
		}
	}
}

void IKnight::ComputeYaw(const Uint32 iDeltaTicks) {
	//----------------------------------------------------------
	// Obliczenie k¹ta, pod jakim powinien poruszaæ siê rycerz.
	//----------------------------------------------------------

	float fSuggestedAng = m_vVel.Yaw();  // Sugerowany k¹t jest odchyleniem wektora prêdkoœci.

	//--------------------------------------------------------------------------------
	// Obliczenie k¹ta w³aœciwego, pod jakim rzeczywiœcie bêdzie porusza³ siê rycerz.
	//--------------------------------------------------------------------------------

	// Dlaczego? Czy sugerowanego k¹ta nie mo¿na po prostu przypisaæ k¹towi w³aœciwemu?
	// Nie, nale¿y jeszcze uwzglêdniæ czas, rycerz nie mo¿e siê przecie¿ obracaæ z "prêdkoœci¹ œwiat³a".
	// Zapobiegnie to równie¿ przesadnemu "drganiu" rycerzy obieraj¹cych co rusz to nowy kierunek.
	// Jako prêdkoœæ obrotu zostanie przyjêta wartoœæ k¹ta pomiêdzy k¹tem w³aœciwym a k¹tem sugerowanym,
	// pomno¿ona przez trzy dla uzyskania optymalnego efektu.

	FetchToAng(fSuggestedAng, iDeltaTicks);
}

void IKnight::FetchToAng(float fSuggestedAng, const Uint32 iDeltaTicks) {
	// W celu znalezienia najkrótszego obrotu rycerza trzeba rozró¿niæ dwa przypadki:
	
	// 1. przypadek: rycerz podczas obrotu powinien przekroczyæ k¹t 0 stopni.
	if(static_cast<float>(fabs(fSuggestedAng - m_fAng)) > 180.0f) {
		if(m_fAng < fSuggestedAng) {  // Jeœli k¹t w³aœciwy jest mniejszy od k¹ta sugerowanego.
			// Zmniejszenie k¹ta w³aœciwego z uwzglêdnieniem czasu, rycerz powinien obracaæ siê prawoskrêtnie.
			m_fAng -= (((m_fAng + (360.0f - fSuggestedAng)) * 3.0f) * (iDeltaTicks / 1000.0f));

			if(m_fAng < 0.0f) {  // W przypadku, gdy k¹t w³aœciwy wyjdzie poza skalê i bêdzie mniejszy od zera.
				// Skorygowanie k¹ta w³aœciwego - "przerzucenie" go do pierwszej æwiartki uk³adu wspó³rzêdnych.
				m_fAng = (360.0f + m_fAng);

				// Jeœli k¹t w³aœciwy zosta³ "przerzucony" do æwiartki, w której aktualnie znajduje siê k¹t sugerowany,
				// nale¿y sprawdziæ, czy czasem nie przekroczy³ on owego k¹ta.
				if(m_fAng < fSuggestedAng) m_fAng = fSuggestedAng;  // Skorygowanie k¹ta w³aœciwego.
			}
		}
		else if(m_fAng > fSuggestedAng) {  // Jeœli k¹t w³aœciwy jest wiêkszy od k¹ta sugerowanego.
			// Powiêkszenie k¹ta w³aœciwego z uwzglêdnieniem czasu, rycerz powinien obracaæ siê lewoskrêtnie.
			m_fAng += ((((360.0f - m_fAng) + fSuggestedAng) * 3.0f) * (iDeltaTicks / 1000.0f));

			if(m_fAng >= 360.0f) {  // W przypadku, gdy k¹t w³aœciwy wyjdzie poza skalê i bêdzie wiêkszy, równy 360 stopni.
				// Skorygowanie k¹ta w³aœciwego - "przerzucenie" go do drugiej æwiartki uk³adu wspó³rzêdnych.
				m_fAng = (m_fAng - 360.0f);

				// Jeœli k¹t w³aœciwy zosta³ "przerzucony" do æwiartki, w której aktualnie znajduje siê k¹t sugerowany,
				// nale¿y sprawdziæ, czy czasem nie przekroczy³ on owego k¹ta.
				if(m_fAng > fSuggestedAng) m_fAng = fSuggestedAng;  // Skorygowanie k¹ta w³aœciwego.
			}
		}
	}
	// 2. przypadek: rycerz podczas obrotu nie powinien przekroczyæ k¹ta 0 stopni.
	else {
		// Jeœli k¹t w³aœciwy jest mniejszy od k¹ta sugerowanego, to...
		if(m_fAng < fSuggestedAng) {
			// ... nale¿y go powiêkszyæ, uwzglêdniaj¹c przy tym czas.
			m_fAng += (((fSuggestedAng - m_fAng) * 3.0f) * (iDeltaTicks / 1000.0f));

			if(m_fAng > fSuggestedAng) m_fAng = fSuggestedAng;  // Zabezpieczenie przed przekroczeniem k¹ta sugerowanego.
		}
		// Jeœli k¹t w³aœciwy jest wiêkszy od k¹ta sugerowanego, to...
		else if(m_fAng > fSuggestedAng) {
			// ... nale¿y go zmniejszyæ, uwzglêdniaj¹c przy tym czas.
			m_fAng -= (((m_fAng - fSuggestedAng) * 3.0f) * (iDeltaTicks / 1000.0f));

			if(m_fAng < fSuggestedAng) m_fAng = fSuggestedAng;  // Zabezpieczenie przed przekroczeniem k¹ta sugerowanego.
		}
	}
}

void IKnight::UpdateShoesprintsContainer(const Uint32 iDeltaTicks) {
	//----------------------------------------------
	// Aktualizacja ka¿dego ze œladów w kontenerze.
	//----------------------------------------------

	std::vector<CShoesprint*>::iterator It;
	for(It = m_ShoesprintsContainer.begin(); It != m_ShoesprintsContainer.end(); ++It)
		(*It)->Update(iDeltaTicks);

	//------------------------------
	// Tworzenie i usuwanie œladów.
	//------------------------------

	// W przypadku zmiany klatki animacji flaga zostaje przestawiona.
	// Nie powsta³ jeszcze ¿aden œlad po bucie w nowej klatce animacji.
	if(IsFrameChanged()) m_bIsShoesprint = false;

	// Jeœli rycerz stoi, zostanie podjêta próba wyczyszczenia ca³ego kontenera.
	if(m_iStatus == stand) {
		std::vector<CShoesprint*>::iterator It;
		for(It = m_ShoesprintsContainer.begin(); It != m_ShoesprintsContainer.end(); ) {
			if((*It)->IsDead()) {  // Je¿eli œlad po bucie mo¿e zakoñczyæ swój ¿ywot...
				// ...to niech go zakoñczy.
				delete *It;
				It = m_ShoesprintsContainer.erase(It);
			}
			else
				// W przypadku, gdy element nie zosta³ usuniêty i elementy bêd¹ce za nim nie zosta³y przesuniête
				// odpowiednio na jego miejsce, nale¿y przejœæ do kolejnego poprzez inkrementacjê iteratora.
				++It;
		}
	}
	// W przeciwnym wypadku, i w wypadku, gdy zajdzie takowa potrzeba, zostanie zniszczony najbli¿szy "martwy obiekt"
	// i zast¹piony nowym. Ewentualnie nowy obiekt zostanie po prostu przypisany do koñca kontenera.
	else {
		// Podczas pierwszej klatki animacji rycerz zostawia prawy œlad po bucie.
		// Nale¿y przy tym sprawdziæ, czy œlad ten nie powsta³ ju¿ wczeœniej.
		if((static_cast<short>(m_fFrame) == 1) && !m_bIsShoesprint) {
			// Stworzenie prawego œladu po bucie.
			CShoesprint* pNewShoesprint = new CShoesprint(&m_vPos, m_fAng, CShoesprint::right);

			// Próba zast¹pienia "martwego obiektu" nowym.
			std::vector<CShoesprint*>::iterator It = m_ShoesprintsContainer.begin();
			while(It != m_ShoesprintsContainer.end()) {
				if((*It)->IsDead()) {  // Element posiadaj¹cy "chêæ do œmierci" zosta³ odnaleziony.
					delete *It;  // Spe³nienie chêci elementu poprzez jego usuniêcie.
					*It = pNewShoesprint;  // Wstawienie nowego elementu w miejsce "martwego".
					m_bIsShoesprint = true;  // Ustawienie flagi. Powsta³ nowy œlad po bucie...
					break;  // ...wiêc mo¿na przerwaæ pêtlê.
				}
				++It;  // Inkrementacja iteratora.
			}

			// W przypadku nieznalezienia "martwego obiektu", nowy œlad buta zostanie przypisany do koñca kontenera.
			if(!m_bIsShoesprint) {
				m_ShoesprintsContainer.push_back(pNewShoesprint);
				m_bIsShoesprint = true;  // Ustawienie flagi. Powsta³ nowy œlad po bucie.
			}
		}
		// Natomiast podczas pi¹tej klatki animacji rycerz zostawia lewy œlad po bucie.
		// Równie¿ nale¿y sprawdziæ przy tym, czy œlad ten nie powsta³ ju¿ wczeœniej.
		else if((static_cast<short>(m_fFrame) == 5) && !m_bIsShoesprint) {
			// Stworzenie lewego œladu po bucie.
			CShoesprint* pNewShoesprint = new CShoesprint(&m_vPos, m_fAng, CShoesprint::left);

			// Analogiczne postêpowanie, jak przy tworzeniu prawego œladu po bucie.
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
