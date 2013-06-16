#include "Serf.h"
#include "singletons.h"

CSerf::CSerf(short iId, CVector* vPos, short iColour) : m_pNearestCommander(NULL),
		m_fDistToNearestCommander(g_fInfinity) {
	//--------------------------------
	// Przypisanie warto�ci zmiennym.
	//--------------------------------

	m_iId = iId;  // Przypisanie warto�ci identyfikatorowi rycerza.
	m_vPos = *vPos;  // Przypisanie podanej pozycji rycerzowi.
	ShiftCollisionCircle();  // Synchronizacja ko�a kolizji z pozycj�.

	// Nadanie wektorowi pr�dko�ci losowych warto�ci.
	m_vVel.x = (Rand() * 100.0f);
	m_vVel.y = (Rand() * 100.0f);

	// Ewentualne odwr�cenie sk�adowych wektora pr�dko�ci dla zwi�kszenia wra�enia losowo�ci.
	if(Rand() > 0.5f) m_vVel.x *= -1;
	if(Rand() > 0.5f) m_vVel.y *= -1;

	m_fSpeed = m_vVel.Length();  // Obliczenie ��cznej pr�dko�ci, r�wnej d�ugo�ci wektora pr�dko�ci.
	m_fAng = m_vVel.Yaw();  // Obliczenie pocz�tkowej orientacji rycerza.

	//-----------------------------------------
	// Utworzenie odpowiedniego pod�wietlenia.
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
	
	m_vOldPos = m_vPos;  // Zapami�tanie ostatniej pozycji rycerza.

	// Uaktualnienie wektora pozycji wzgl�dem wektora pr�dko�ci, obliczonego ostatnim razem.
	m_vPos += (m_vVel * (iDeltaTicks / 1000.0f));

	ShiftCollisionCircle();  // Synchronizacja ko�a kolizji z now� pozycj� rycerza.

	//-------------------------------
	// Obliczenie wektora pr�dko�ci.
	//-------------------------------

	SeeComrades(pComradesContainer);  // Sprawdzenie, czy rycerz widzi kt�regokolwiek ze swoich kompan�w. 
	CVector vAcc;  // Stworzenie wektora reprezentuj�cego zmian�, jaka powinna zaj�� na wektorze pr�dko�ci.

	// Je�eli w polu widzenia rycerza nie znajduj� si� �adni kompani, nie b�dzie
	// mia� on sposobno�ci zastosowania zasad okre�laj�cych zachowanie w armii.
	if(m_VisibleComradesContainer.size()) {
		//------------------------------
		// ZASADA 1. Separacja rycerzy.
		//------------------------------

		// Usi�owanie utrzymania po��danego dystansu mi�dzy rycerzem, a jego najbli�szym kompanem.
		AccumulateChanges(vAcc, KeepDistance());

		//-----------------------
		// ZASADA 2. Wyr�wnanie.
		//-----------------------

		// Je�eli dow�dca jest widoczny i wska�nik na najbli�szego dow�dc� nie b�dzie wskazywa� na ten sam objekt,
		// co wska�nik na najbli�szego kompana, rycerz b�dzie usi�owa� dopasowa� sw�j kierunek do kierunku dow�dcy i kompana.
		// W przeciwnym wypadku rycerz b�dzie usi�owa� i�� zgodnie z kierunkiem nabli�szego widocznego kompana.
		if(m_pNearestCommander && (m_pNearestCommander != m_pNearestComrade))
			AccumulateChanges(vAcc, CommanderMatchHeading());
		else 
			AccumulateChanges(vAcc, MatchHeading());

		//---------------------
		// ZASADA 3. Sp�jno��.
		//---------------------

		// Je�eli dow�dca jest widoczny, rycerz b�dzie usi�owa� i�� w jego kierunku.
		// W przeciwnym wypadku rycerz b�dzie kierowa� si� do "�rodka armii".
		if(m_pNearestCommander)
			AccumulateChanges(vAcc, SteerToCommander());
		else 
			AccumulateChanges(vAcc, SteerToCenter());
	}

	// Reakcja na wrogie jednostki.
	SeeEnemies(iArmyId);  // Zlokalizowanie najbli�szej wrogiej jednostki.
	AccumulateChanges(vAcc, FleeEnemies());  // Usi�owanie oddalenia si� od wroga b�d�cego najbli�ej.

	// Reakcja na przeszkody.
	SeeObstacles();  // Zlokalizowanie najbli�szej przeszkody.
	AccumulateChanges(vAcc, FleeObstacles());  // Usi�owanie oddalenia si� od przeszkody b�d�cej najbli�ej.

	// Ruch niezale�ny. Nie zwa�aj�c na przynale�no�� do armii, okre�lenie charakteru poruszania si� rycerza.
	AccumulateChanges(vAcc, Cruising());

	if(vAcc.Length() > g_DefaultsMgr.GetMaxChange())  // Redukcja przy�pieszenia.
		vAcc.SetMagnitude(g_DefaultsMgr.GetMaxChange());

	// Zastosowanie obliczonego przy�pieszenia. 
	m_vOldVel = m_vVel;  // Zapami�tanie poprzedniej pr�dko�ci.
	m_vVel += vAcc;  // Dodanie do pr�dko�ci obliczonego przy�pieszenia.

	// Redukcja pr�dko�ci. Je�eli d�ugo�� wektora pr�dko�ci jest wi�ksza od maksymalnej pr�dko�ci.
	if((m_fSpeed = m_vVel.Length()) > g_DefaultsMgr.GetMaxSpeed()) {
		// Normalizacja i przeskalowanie wektora pr�dko�ci do maksymalnej warto�ci.
		m_vVel.SetMagnitude(g_DefaultsMgr.GetMaxSpeed());

		m_fSpeed = g_DefaultsMgr.GetMaxSpeed();  // ��czna pr�dko�� rycerza wynosi teraz maksymaln� dopuszczaln� warto��.
	}

	//----------------------------------------
	// Ewentualna reakcja na bezruch dow�dcy.
	//----------------------------------------

	CommanderStop();  // Metoda mo�e mie� drastyczny wp�yw na wektor pr�dko�ci!

	//------------------------
	// Obliczenie orientacji.
	//------------------------

	if(!m_vVel.IsZero())  // Znalezienie orientacji rycerza, je�li wektor pr�dko�ci jest niezerowy.
		ComputeYaw(iDeltaTicks);
	// W przeciwnym wypadku, je�li istnieje najbli�szy widoczny dow�dca, rycerz dopasuje orientacj� do orientacji dow�dcy.
	else if(m_pNearestCommander) 
		FetchToAng(m_pNearestCommander->GetAng(), iDeltaTicks);
	// W�a�ciwie sprawdzenie, czy najbli�szy widoczny dow�dca istnieje, nie jest potrzebne.
	// Gdyby nie istnia�, wektor pr�dko�ci nie m�g�by sta� si� zerowym. To nadgorliwe zabezpieczenie.

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
}

void CSerf::ClearVisibleContainer(void) {
	m_VisibleComradesContainer.clear();  // Wyczyszczenie kontenera widoczno�ci.

	// Wyczyszczenie pozosta�ych informacji zwi�zanych z kontenerem widoczno�ci.
	m_pNearestComrade = NULL;
	m_fDistToNearestComrade = g_fInfinity;
	m_pNearestCommander = NULL;
	m_fDistToNearestCommander = g_fInfinity;
}

int CSerf::SeeComrades(std::vector<IKnight*>* pComradesContainer) {
	float fDist;  // Dystans dziel�cy rycerzy.
	ClearVisibleContainer();  // Wyczyszczenie poprzedniego kontenera widoczno�ci.
	
	// Zlokalizowanie widocznych kompan�w.
	std::vector<IKnight*>::iterator It;
	for(It = pComradesContainer->begin(); It != pComradesContainer->end(); ++It)
		if((fDist = CanISee(*It)) != g_fInfinity) {
			AddToVisibleContainer(*It);  // Dodanie rycerza do kontenera widoczno�ci.

			// Je�li dystans jest mniejszy od dotychczasowego najmniejszego dystansu,
			// widziany rycerz staje si� kompanem b�d�cym najbli�ej.
			if(fDist < m_fDistToNearestComrade) {
				m_fDistToNearestComrade = fDist;
				m_pNearestComrade = *It;
			}

			// Je�eli rycerz jest dow�dc� oraz dystans jest mniejszy od dotychczasowego najmniejszego dystansu,
			// widziany rycerz staje si� dow�dc� b�d�cym najbli�ej.
			if(IsCommander(*It) && (fDist < m_fDistToNearestCommander)) { 
				m_fDistToNearestCommander = fDist;
				m_pNearestCommander = *It;
			}
		}

	return m_VisibleComradesContainer.size();  // Zwr�cenie ilo�ci widocznych kompan�w.
}

CVector CSerf::SteerToCommander(void) {
	// Stosunek odleg�o�ci od najbli�szego dow�dcy, do zasi�gu widzenia rycerza.
	float fRatio = (m_fDistToNearestCommander / g_DefaultsMgr.GetDefaultPerceptionRange());

	CVector vCloseToCommander;  // Pozycja blisko dow�dcy, oddalona od jego pozycji o ustalony dystans pomi�dzy rycerzami.

	// Obliczenie wektora pomocniczego, kt�ry biegn�c od pozycji dow�dcy wska�e pozycj�, do kt�rej powinien d��y� rycerz.
	// Wektor ten jest r�wnoleg�y do odcinka ��cz�cego pozycj� dow�dcy z pozycj� rycerza.
	CVector vHelper = (m_vPos - *m_pNearestCommander->GetPos());
	vHelper.SetMagnitude(g_DefaultsMgr.GetSeparationDist());

	// Obliczenie pozycji b�d�cej blisko dow�dcy przy wykorzystaniu powy�ej obliczonego wektora pomocniczego.
	vCloseToCommander = (*m_pNearestCommander->GetPos() + vHelper);

	CVector vChange = (vCloseToCommander - m_vPos);  // Obliczenie wektora zmiany.

	// Ograniczenie obliczonego wcze�niej stosunku.
	if(fRatio < g_DefaultsMgr.GetMinUrgency()) fRatio = g_DefaultsMgr.GetMinUrgency();
	if(fRatio > g_DefaultsMgr.GetMaxUrgency()) fRatio = g_DefaultsMgr.GetMaxUrgency();

	// Je�eli dow�dca stoi w miejscu, priorytetem rycerza staje si� doj�cie do niego.
	if(m_pNearestCommander->GetVel()->IsZero())
		vChange.SetMagnitude(g_DefaultsMgr.GetMaxUrgency() * 100.0f);  // Ustawienie maksymalnej wielko�ci wektora zmiany.
	else
		vChange.SetMagnitude(fRatio * 100.0f);  // Ustawienie odpowiedniej wielko�ci wektora zmiany.

	return vChange;  // Zwr�cenie wektora zmiany.
}
