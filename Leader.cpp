#include "Leader.h"
#include "singletons.h"

CLeader::CLeader(short iId, CVector* vPos, short iColour) {
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
		case IBacklight::yellow: m_pBacklight = new CCommanderBacklight(IBacklight::yellow); break;
		case IBacklight::blue: m_pBacklight = new CCommanderBacklight(IBacklight::blue); break;
		case IBacklight::green: m_pBacklight = new CCommanderBacklight(IBacklight::green);
	}
}

void CLeader::ArmyIt(int iArmyId, std::vector<IKnight*>* pComradesContainer, const Uint32 iDeltaTicks) {
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

		// Brak. Powinnoœci¹ poddanego jest pod¹¿aæ zgodnie z kierunkiem dowódcy. Lider nie musi siê tym przejmowaæ.

		//---------------------
		// ZASADA 3. Spójnoœæ.
		//---------------------

		// Usi³owanie poruszania siê w kierunku "œrodka armii". Dziêki temu lider bêdzie skupia³ jednostki wokó³ siebie.
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
}
