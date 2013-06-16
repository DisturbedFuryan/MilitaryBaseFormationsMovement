#include "Leader.h"
#include "singletons.h"

CLeader::CLeader(short iId, CVector* vPos, short iColour) {
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
		case IBacklight::yellow: m_pBacklight = new CCommanderBacklight(IBacklight::yellow); break;
		case IBacklight::blue: m_pBacklight = new CCommanderBacklight(IBacklight::blue); break;
		case IBacklight::green: m_pBacklight = new CCommanderBacklight(IBacklight::green);
	}
}

void CLeader::ArmyIt(int iArmyId, std::vector<IKnight*>* pComradesContainer, const Uint32 iDeltaTicks) {
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

		// Brak. Powinno�ci� poddanego jest pod��a� zgodnie z kierunkiem dow�dcy. Lider nie musi si� tym przejmowa�.

		//---------------------
		// ZASADA 3. Sp�jno��.
		//---------------------

		// Usi�owanie poruszania si� w kierunku "�rodka armii". Dzi�ki temu lider b�dzie skupia� jednostki wok� siebie.
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
}
