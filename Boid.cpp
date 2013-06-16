#include "Boid.h"
#include "singletons.h"

std::vector<IKnight*> IBoid::m_VisibleComradesContainer;  // Definicja statycznego kontenera.

IBoid::IBoid(void) : m_fPerceptionRange(g_DefaultsMgr.GetDefaultPerceptionRange()), m_iNumEnemiesSeen(0),
										 m_iNumObstaclesSeen(0), m_pNearestComrade(NULL), m_pNearestEnemy(NULL), m_pNearestObstacle(NULL), 
										 m_fDistToNearestComrade(g_fInfinity), m_fDistToNearestEnemy(g_fInfinity), 
										 m_fDistToNearestObstacle(g_fInfinity) {}

CVector IBoid::Cruising(void) {
	CVector vChange = m_vVel;

	// Je�li wynik b�dzie wi�kszy od zera, to rycerz powinien zwolni�. Je�li za� b�dzie mniejszy - przy�pieszy�.
	float fDiff = ((m_fSpeed - g_DefaultsMgr.GetDesiredSpeed()) / g_DefaultsMgr.GetMaxSpeed());

	// Okre�lenie "wagi", jest ni� warto�� bezwzgl�dna wyniku z powy�szego wzoru.
	float fUrgency = static_cast<float>(fabs(fDiff));

	// Ograniczenie poziomu "wagi".
	if(fUrgency < g_DefaultsMgr.GetMinUrgency()) 
		fUrgency = g_DefaultsMgr.GetMinUrgency();
	if(fUrgency > g_DefaultsMgr.GetMaxUrgency()) 
		fUrgency = g_DefaultsMgr.GetMaxUrgency();

	// Przydzielenie rycerzowi nieco losowo�ci.
	float fJitter = Rand();
	if(fJitter < 0.5f) 
		vChange.x += ((g_DefaultsMgr.GetMinUrgency() * 100.0f) * static_cast<float>(Sign(fDiff)));
	else 
		vChange.y += ((g_DefaultsMgr.GetMinUrgency() * 100.0f) * static_cast<float>(Sign(fDiff)));

	// Obliczenie zmiany pr�dko�ci koniecznej do uzyskania po��danej pr�dko�ci marszu.
	vChange.SetMagnitude((fUrgency * 100.0f) * ((fDiff > 0.0f) ? (-1.0f) : (1.0f)));

	return vChange;  // Zwr�cenie wektora zmiany.
}

CVector IBoid::KeepDistance(void) {
	// Stosunek odleg�o�ci do najbli�szego kompana, do po��danej odleg�o�ci mi�dzy rycerzami.
	float fRatio = (m_fDistToNearestComrade / g_DefaultsMgr.GetSeparationDist());

	CVector vChange = (*m_pNearestComrade->GetPos() - m_vPos);  // Obliczenie wektora biegn�cego do najbli�szego kompana.
	
	// Ograniczenie obliczonego stosunku.
	if(fRatio < g_DefaultsMgr.GetMinUrgency()) fRatio = g_DefaultsMgr.GetMinUrgency();
	if(fRatio > g_DefaultsMgr.GetMaxUrgency()) fRatio = g_DefaultsMgr.GetMaxUrgency();

	// Sprawdzenie, jaki dystans dzieli rycerza od najbli�szego kompana.
	if(m_fDistToNearestComrade < g_DefaultsMgr.GetSeparationDist())
		vChange.SetMagnitude(-fRatio * 100.0f);  // Za blisko, rycerz powinien si� oddali� od najbli�szego kompana.
	else if(m_fDistToNearestComrade > g_DefaultsMgr.GetSeparationDist())
		// Za daleko, rycerz powinien zmniejszy� dystans, dziel�cy go od najbli�szego kompana.
		vChange.SetMagnitude(fRatio * 100.0f);
	else
		// Dystans mi�dzy rycerzami jest idealny, nie ma potrzeby wprowadzania jakichkolwiek zmian.
		vChange.SetMagnitude(0.0f);

	return vChange;  // Zwr�cenie wektora zmiany.
}

int IBoid::SeeEnemies(short iArmyId) {
	float fDist;  // Dystans dziel�cy rycerza z wrogim obiektem.

	// Przypisanie warto�ci zmiennym odnosz�cym si� do wrogich jednostek.
	m_iNumEnemiesSeen = 0;
	m_pNearestEnemy = NULL;
	m_fDistToNearestEnemy = g_fInfinity;

	// Prze�ledzenie ka�dej armii w poszukiwaniu najbli�szej wrogiej jednostki.
	std::vector<CArmy*>::iterator ItArmies = g_KnightMgr.GetArmiesContainer()->begin();
	for(int i = 0; ItArmies != g_KnightMgr.GetArmiesContainer()->end(); ++ItArmies, i++) {
		if(i == iArmyId) continue;  // Nielogicznym by�oby rozgl�danie si� za wrogami we w�asnej armii.

		// Prze�ledzenie ka�dego rycerza w aktualnie analizowanej armii.
		std::vector<IKnight*>::iterator ItMembers = (*ItArmies)->GetMembersContainer()->begin();
		for(; ItMembers != (*ItArmies)->GetMembersContainer()->end(); ++ItMembers)
			if((fDist = CanISee(*ItMembers)) != g_fInfinity) {  // Je�eli odleg�o�� do wroga jest r�na od "niesko�czono�ci".
				m_iNumEnemiesSeen++;  // Inkrementacja licznika widzianych wrog�w.

				// Je�li dystans do wroga jest mniejszy od dotychczasowego najmniejszego dystansu,
				// widziany rycerz staje si� wrogiem b�d�cym najbli�ej.
				if(fDist < m_fDistToNearestEnemy) {
					m_fDistToNearestEnemy = fDist;
					m_pNearestEnemy = *ItMembers;
				}
			}
	}
	return m_iNumEnemiesSeen;  // Zwr�cenie liczby widzianych wrog�w.
}

int IBoid::SeeObstacles(void) {
	float fDist;  // Dystans dziel�cy rycerza z przeszkod�.

	// Przypisanie warto�ci zmiennym odnosz�cym si� do wrogich jednostek.
	m_iNumObstaclesSeen = 0;
	m_pNearestObstacle = NULL;
	m_fDistToNearestObstacle = g_fInfinity;

	// Poszukiwanie najbli�szej przeszkody.
	std::vector<CObstacle*>::iterator It;
	for(It = g_ObstacleMgr.GetObstaclesContainer()->begin(); It != g_ObstacleMgr.GetObstaclesContainer()->end(); ++It)
		if((fDist = CanISee(*It)) != g_fInfinity) {  // Je�eli odleg�o�� do przeszkody jest r�na od "niesko�czono�ci".
			m_iNumObstaclesSeen++;  // Inkrementacja licznika widzianych przeszk�d.

			// Je�li dystans do przeszkody jest mniejszy od dotychczasowego najmniejszego dystansu,
			// widziana przeszkoda staje si� przeszkod� b�d�c� najbli�ej.
			if(fDist < m_fDistToNearestObstacle) {
				m_fDistToNearestObstacle = fDist;
				m_pNearestObstacle = *It;
			}
		}

	return m_iNumObstaclesSeen;  // Zwr�cenie liczby widzianych przeszk�d.
}

int IBoid::SeeComrades(std::vector<IKnight*>* pComradesContainer) {
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
		}

	return m_VisibleComradesContainer.size();  // Zwr�cenie ilo�ci widocznych kompan�w.
}

CVector IBoid::SteerToCenter(void) {
	CVector vCenter;  // "�rodek armii".

	// Zliczenie pozycji wszystkich widocznych kompan�w.
	std::vector<IKnight*>::iterator It;
	for(It = m_VisibleComradesContainer.begin() ; It != m_VisibleComradesContainer.end(); ++It)
		vCenter += *(*It)->GetPos();

	vCenter /= static_cast<float>(m_VisibleComradesContainer.size());  // Wyliczenie �redniej zsumowanych pozycji.
	CVector vChange = (vCenter - m_vPos);  // Obliczenie wektora biegn�cego do "�rodka armii".
	vChange.SetMagnitude(g_DefaultsMgr.GetMinUrgency() * 100.0f);  // Przeskalowanie wektora zmiany.
	return vChange;  // Zwr�cenie wektora zmiany.
}

void IBoid::WorldBound(void) {
	//-------------------------------------------------
	// Kontrola kolizji z granicami �wiata w poziomie.
	//-------------------------------------------------

	// Rycerz opuszcza �wiat z lewej strony.
	if((m_CollisionCircle.pos.x  + (m_CollisionCircle.r * 2.0f)) < 0.0f) {
		m_vPos.x = (static_cast<float>(g_DefaultsMgr.GetAreaWidth()) + (m_CollisionCircle.r * 2.0f));
		ShiftCollisionCircle();
	}
	// Rycerz opuszcza �wiat z prawej strony.
	else if((m_CollisionCircle.pos.x - (m_CollisionCircle.r * 2.0f)) > static_cast<float>(g_DefaultsMgr.GetAreaWidth())) {
		m_vPos.x = (0.0f - (m_CollisionCircle.r * 2.0f));
		ShiftCollisionCircle();
	}

	//-----------------------------------------------
	// Kontrola kolizji z granicami �wiata w pionie.
	//-----------------------------------------------

	// Rycerz opuszcza �wiat z g�ry.
	if((m_CollisionCircle.pos.y  + (m_CollisionCircle.r * 2.0f)) < 0.0f) {
		m_vPos.y = (static_cast<float>(g_DefaultsMgr.GetAreaHeight()) + (m_CollisionCircle.r * 2.0f));
		ShiftCollisionCircle();
	}
	// Rycerz opuszcza �wiat z do�u.
	else if((m_CollisionCircle.pos.y - (m_CollisionCircle.r * 2.0f)) > static_cast<float>(g_DefaultsMgr.GetAreaHeight())) {
		m_vPos.y = (0.0f - (m_CollisionCircle.r * 2.0f));
		ShiftCollisionCircle();
	}
}

float IBoid::CanISee(IKnight* pKnight) {
	if(this == pKnight)	
		return g_fInfinity;  // Je�li rycerz patrzy na samego siebie...
	float fDist = m_vPos.GetDist(m_vPos, *pKnight->GetPos());  // Obliczenie dystansu dziel�cego rycerzy.

	// Sprawdzenie, czy rycerz jest w stanie dostrzec drugiego z takiej odleg�o�ci.
	if(m_fPerceptionRange > fDist) 
		return fDist;

	return g_fInfinity;  // Powy�szy warunek nie zosta� spe�niony, rycerze nie s� w stanie siebie dostrzec.
}

float IBoid::CanISee(CObstacle* pObstacle) {
	float fDist = m_vPos.GetDist(m_vPos, *pObstacle->GetPos());  // Obliczenie dystansu dziel�cego rycerza z przeszkod�.

	// Sprawdzenie, czy rycerz jest w stanie dostrzec przeszkod� z takiej odleg�o�ci.
	if(m_fPerceptionRange > fDist) 
		return fDist;

	return g_fInfinity;  // Powy�szy warunek nie zosta� spe�niony, rycerz nie jest w stanie dostrzec przeszkody.
}
