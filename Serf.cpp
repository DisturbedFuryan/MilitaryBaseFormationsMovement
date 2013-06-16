#include "Serf.h"
#include "singletons.h"

CSerf::CSerf(short iId, CVector* vPos, short iColour) : m_pNearestCommander(NULL),
		m_fDistToNearestCommander(g_fInfinity) {
	//--------------------------------
	// Przypisanie wartoœci zmiennym.
	//--------------------------------

	m_iId = iId;  // Przypisanie wartoœci identyfikatorowi rycerza.
	m_vPos = *vPos;  // Przypisanie podanej pozycji rycerzowi.
	ShiftCollisionCircle();  // Synchronizacja ko³a kolizji z pozycj¹.

	// Nadanie wektorowi prêdkoœci losowych wartoœci.
	m_vVel.x = (Rand() * 100.0f);
	m_vVel.y = (Rand() * 100.0f);

	// Ewentualne odwrócenie sk³adowych wektora prêdkoœci dla zwiêkszenia wra¿enia losowoœci.
	if(Rand() > 0.5f) m_vVel.x *= -1;
	if(Rand() > 0.5f) m_vVel.y *= -1;

	m_fSpeed = m_vVel.Length();  // Obliczenie ³¹cznej prêdkoœci, równej d³ugoœci wektora prêdkoœci.
	m_fAng = m_vVel.Yaw();  // Obliczenie pocz¹tkowej orientacji rycerza.

	//-----------------------------------------
	// Utworzenie odpowiedniego podœwietlenia.
	//-----------------------------------------

	switch(iColour) {
		case IBacklight::yellow: m_pBacklight = new CSerfBacklight(IBacklight::yellow); break;
		case IBacklight::blue: m_pBacklight = new CSerfBacklight(IBacklight::blue); break;
		case IBacklight::green: m_pBacklight = new CSerfBacklight(IBacklight::green);
	}
}

void CSerf::ArmyIt(int iArmyId, std::vector<IKnight*>* pComradesContainer, const Uint32 iDeltaTicks) {
	//-----------------------
	// Aktualizacja pozycji.
	//-----------------------
	
	m_vOldPos = m_vPos;  // Zapamiêtanie ostatniej pozycji rycerza.

	// Uaktualnienie wektora pozycji wzglêdem wektora prêdkoœci, obliczonego ostatnim razem.
	m_vPos += (m_vVel * (iDeltaTicks / 1000.0f));

	ShiftCollisionCircle();  // Synchronizacja ko³a kolizji z now¹ pozycj¹ rycerza.

	//-------------------------------
	// Obliczenie wektora prêdkoœci.
	//-------------------------------

	SeeComrades(pComradesContainer);  // Sprawdzenie, czy rycerz widzi któregokolwiek ze swoich kompanów. 
	CVector vAcc;  // Stworzenie wektora reprezentuj¹cego zmianê, jaka powinna zajœæ na wektorze prêdkoœci.

	// Je¿eli w polu widzenia rycerza nie znajduj¹ siê ¿adni kompani, nie bêdzie
	// mia³ on sposobnoœci zastosowania zasad okreœlaj¹cych zachowanie w armii.
	if(m_VisibleComradesContainer.size()) {
		//------------------------------
		// ZASADA 1. Separacja rycerzy.
		//------------------------------

		// Usi³owanie utrzymania po¿¹danego dystansu miêdzy rycerzem, a jego najbli¿szym kompanem.
		AccumulateChanges(vAcc, KeepDistance());

		//-----------------------
		// ZASADA 2. Wyrównanie.
		//-----------------------

		// Je¿eli dowódca jest widoczny i wskaŸnik na najbli¿szego dowódcê nie bêdzie wskazywa³ na ten sam objekt,
		// co wskaŸnik na najbli¿szego kompana, rycerz bêdzie usi³owa³ dopasowaæ swój kierunek do kierunku dowódcy i kompana.
		// W przeciwnym wypadku rycerz bêdzie usi³owa³ iœæ zgodnie z kierunkiem nabli¿szego widocznego kompana.
		if(m_pNearestCommander && (m_pNearestCommander != m_pNearestComrade))
			AccumulateChanges(vAcc, CommanderMatchHeading());
		else 
			AccumulateChanges(vAcc, MatchHeading());

		//---------------------
		// ZASADA 3. Spójnoœæ.
		//---------------------

		// Je¿eli dowódca jest widoczny, rycerz bêdzie usi³owa³ iœæ w jego kierunku.
		// W przeciwnym wypadku rycerz bêdzie kierowa³ siê do "œrodka armii".
		if(m_pNearestCommander)
			AccumulateChanges(vAcc, SteerToCommander());
		else 
			AccumulateChanges(vAcc, SteerToCenter());
	}

	// Reakcja na wrogie jednostki.
	SeeEnemies(iArmyId);  // Zlokalizowanie najbli¿szej wrogiej jednostki.
	AccumulateChanges(vAcc, FleeEnemies());  // Usi³owanie oddalenia siê od wroga bêd¹cego najbli¿ej.

	// Reakcja na przeszkody.
	SeeObstacles();  // Zlokalizowanie najbli¿szej przeszkody.
	AccumulateChanges(vAcc, FleeObstacles());  // Usi³owanie oddalenia siê od przeszkody bêd¹cej najbli¿ej.

	// Ruch niezale¿ny. Nie zwa¿aj¹c na przynale¿noœæ do armii, okreœlenie charakteru poruszania siê rycerza.
	AccumulateChanges(vAcc, Cruising());

	if(vAcc.Length() > g_DefaultsMgr.GetMaxChange())  // Redukcja przyœpieszenia.
		vAcc.SetMagnitude(g_DefaultsMgr.GetMaxChange());

	// Zastosowanie obliczonego przyœpieszenia. 
	m_vOldVel = m_vVel;  // Zapamiêtanie poprzedniej prêdkoœci.
	m_vVel += vAcc;  // Dodanie do prêdkoœci obliczonego przyœpieszenia.

	// Redukcja prêdkoœci. Je¿eli d³ugoœæ wektora prêdkoœci jest wiêksza od maksymalnej prêdkoœci.
	if((m_fSpeed = m_vVel.Length()) > g_DefaultsMgr.GetMaxSpeed()) {
		// Normalizacja i przeskalowanie wektora prêdkoœci do maksymalnej wartoœci.
		m_vVel.SetMagnitude(g_DefaultsMgr.GetMaxSpeed());

		m_fSpeed = g_DefaultsMgr.GetMaxSpeed();  // £¹czna prêdkoœæ rycerza wynosi teraz maksymaln¹ dopuszczaln¹ wartoœæ.
	}

	//----------------------------------------
	// Ewentualna reakcja na bezruch dowódcy.
	//----------------------------------------

	CommanderStop();  // Metoda mo¿e mieæ drastyczny wp³yw na wektor prêdkoœci!

	//------------------------
	// Obliczenie orientacji.
	//------------------------

	if(!m_vVel.IsZero())  // Znalezienie orientacji rycerza, jeœli wektor prêdkoœci jest niezerowy.
		ComputeYaw(iDeltaTicks);
	// W przeciwnym wypadku, jeœli istnieje najbli¿szy widoczny dowódca, rycerz dopasuje orientacjê do orientacji dowódcy.
	else if(m_pNearestCommander) 
		FetchToAng(m_pNearestCommander->GetAng(), iDeltaTicks);
	// W³aœciwie sprawdzenie, czy najbli¿szy widoczny dowódca istnieje, nie jest potrzebne.
	// Gdyby nie istnia³, wektor prêdkoœci nie móg³by staæ siê zerowym. To nadgorliwe zabezpieczenie.

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
}

void CSerf::ClearVisibleContainer(void) {
	m_VisibleComradesContainer.clear();  // Wyczyszczenie kontenera widocznoœci.

	// Wyczyszczenie pozosta³ych informacji zwi¹zanych z kontenerem widocznoœci.
	m_pNearestComrade = NULL;
	m_fDistToNearestComrade = g_fInfinity;
	m_pNearestCommander = NULL;
	m_fDistToNearestCommander = g_fInfinity;
}

int CSerf::SeeComrades(std::vector<IKnight*>* pComradesContainer) {
	float fDist;  // Dystans dziel¹cy rycerzy.
	ClearVisibleContainer();  // Wyczyszczenie poprzedniego kontenera widocznoœci.
	
	// Zlokalizowanie widocznych kompanów.
	std::vector<IKnight*>::iterator It;
	for(It = pComradesContainer->begin(); It != pComradesContainer->end(); ++It)
		if((fDist = CanISee(*It)) != g_fInfinity) {
			AddToVisibleContainer(*It);  // Dodanie rycerza do kontenera widocznoœci.

			// Jeœli dystans jest mniejszy od dotychczasowego najmniejszego dystansu,
			// widziany rycerz staje siê kompanem bêd¹cym najbli¿ej.
			if(fDist < m_fDistToNearestComrade) {
				m_fDistToNearestComrade = fDist;
				m_pNearestComrade = *It;
			}

			// Je¿eli rycerz jest dowódc¹ oraz dystans jest mniejszy od dotychczasowego najmniejszego dystansu,
			// widziany rycerz staje siê dowódc¹ bêd¹cym najbli¿ej.
			if(IsCommander(*It) && (fDist < m_fDistToNearestCommander)) { 
				m_fDistToNearestCommander = fDist;
				m_pNearestCommander = *It;
			}
		}

	return m_VisibleComradesContainer.size();  // Zwrócenie iloœci widocznych kompanów.
}

CVector CSerf::SteerToCommander(void) {
	// Stosunek odleg³oœci od najbli¿szego dowódcy, do zasiêgu widzenia rycerza.
	float fRatio = (m_fDistToNearestCommander / g_DefaultsMgr.GetDefaultPerceptionRange());

	CVector vCloseToCommander;  // Pozycja blisko dowódcy, oddalona od jego pozycji o ustalony dystans pomiêdzy rycerzami.

	// Obliczenie wektora pomocniczego, który biegn¹c od pozycji dowódcy wska¿e pozycjê, do której powinien d¹¿yæ rycerz.
	// Wektor ten jest równoleg³y do odcinka ³¹cz¹cego pozycjê dowódcy z pozycj¹ rycerza.
	CVector vHelper = (m_vPos - *m_pNearestCommander->GetPos());
	vHelper.SetMagnitude(g_DefaultsMgr.GetSeparationDist());

	// Obliczenie pozycji bêd¹cej blisko dowódcy przy wykorzystaniu powy¿ej obliczonego wektora pomocniczego.
	vCloseToCommander = (*m_pNearestCommander->GetPos() + vHelper);

	CVector vChange = (vCloseToCommander - m_vPos);  // Obliczenie wektora zmiany.

	// Ograniczenie obliczonego wczeœniej stosunku.
	if(fRatio < g_DefaultsMgr.GetMinUrgency()) fRatio = g_DefaultsMgr.GetMinUrgency();
	if(fRatio > g_DefaultsMgr.GetMaxUrgency()) fRatio = g_DefaultsMgr.GetMaxUrgency();

	// Je¿eli dowódca stoi w miejscu, priorytetem rycerza staje siê dojœcie do niego.
	if(m_pNearestCommander->GetVel()->IsZero())
		vChange.SetMagnitude(g_DefaultsMgr.GetMaxUrgency() * 100.0f);  // Ustawienie maksymalnej wielkoœci wektora zmiany.
	else
		vChange.SetMagnitude(fRatio * 100.0f);  // Ustawienie odpowiedniej wielkoœci wektora zmiany.

	return vChange;  // Zwrócenie wektora zmiany.
}
