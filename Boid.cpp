#include "Boid.h"
#include "singletons.h"

std::vector<IKnight*> IBoid::m_VisibleComradesContainer;  // Definicja statycznego kontenera.

IBoid::IBoid(void) : m_fPerceptionRange(g_DefaultsMgr.GetDefaultPerceptionRange()), m_iNumEnemiesSeen(0),
										 m_iNumObstaclesSeen(0), m_pNearestComrade(NULL), m_pNearestEnemy(NULL), m_pNearestObstacle(NULL), 
										 m_fDistToNearestComrade(g_fInfinity), m_fDistToNearestEnemy(g_fInfinity), 
										 m_fDistToNearestObstacle(g_fInfinity) {}

CVector IBoid::Cruising(void) {
	CVector vChange = m_vVel;

	// Jeœli wynik bêdzie wiêkszy od zera, to rycerz powinien zwolniæ. Jeœli zaœ bêdzie mniejszy - przyœpieszyæ.
	float fDiff = ((m_fSpeed - g_DefaultsMgr.GetDesiredSpeed()) / g_DefaultsMgr.GetMaxSpeed());

	// Okreœlenie "wagi", jest ni¹ wartoœæ bezwzglêdna wyniku z powy¿szego wzoru.
	float fUrgency = static_cast<float>(fabs(fDiff));

	// Ograniczenie poziomu "wagi".
	if(fUrgency < g_DefaultsMgr.GetMinUrgency()) 
		fUrgency = g_DefaultsMgr.GetMinUrgency();
	if(fUrgency > g_DefaultsMgr.GetMaxUrgency()) 
		fUrgency = g_DefaultsMgr.GetMaxUrgency();

	// Przydzielenie rycerzowi nieco losowoœci.
	float fJitter = Rand();
	if(fJitter < 0.5f) 
		vChange.x += ((g_DefaultsMgr.GetMinUrgency() * 100.0f) * static_cast<float>(Sign(fDiff)));
	else 
		vChange.y += ((g_DefaultsMgr.GetMinUrgency() * 100.0f) * static_cast<float>(Sign(fDiff)));

	// Obliczenie zmiany prêdkoœci koniecznej do uzyskania po¿¹danej prêdkoœci marszu.
	vChange.SetMagnitude((fUrgency * 100.0f) * ((fDiff > 0.0f) ? (-1.0f) : (1.0f)));

	return vChange;  // Zwrócenie wektora zmiany.
}

CVector IBoid::KeepDistance(void) {
	// Stosunek odleg³oœci do najbli¿szego kompana, do po¿¹danej odleg³oœci miêdzy rycerzami.
	float fRatio = (m_fDistToNearestComrade / g_DefaultsMgr.GetSeparationDist());

	CVector vChange = (*m_pNearestComrade->GetPos() - m_vPos);  // Obliczenie wektora biegn¹cego do najbli¿szego kompana.
	
	// Ograniczenie obliczonego stosunku.
	if(fRatio < g_DefaultsMgr.GetMinUrgency()) fRatio = g_DefaultsMgr.GetMinUrgency();
	if(fRatio > g_DefaultsMgr.GetMaxUrgency()) fRatio = g_DefaultsMgr.GetMaxUrgency();

	// Sprawdzenie, jaki dystans dzieli rycerza od najbli¿szego kompana.
	if(m_fDistToNearestComrade < g_DefaultsMgr.GetSeparationDist())
		vChange.SetMagnitude(-fRatio * 100.0f);  // Za blisko, rycerz powinien siê oddaliæ od najbli¿szego kompana.
	else if(m_fDistToNearestComrade > g_DefaultsMgr.GetSeparationDist())
		// Za daleko, rycerz powinien zmniejszyæ dystans, dziel¹cy go od najbli¿szego kompana.
		vChange.SetMagnitude(fRatio * 100.0f);
	else
		// Dystans miêdzy rycerzami jest idealny, nie ma potrzeby wprowadzania jakichkolwiek zmian.
		vChange.SetMagnitude(0.0f);

	return vChange;  // Zwrócenie wektora zmiany.
}

int IBoid::SeeEnemies(short iArmyId) {
	float fDist;  // Dystans dziel¹cy rycerza z wrogim obiektem.

	// Przypisanie wartoœci zmiennym odnosz¹cym siê do wrogich jednostek.
	m_iNumEnemiesSeen = 0;
	m_pNearestEnemy = NULL;
	m_fDistToNearestEnemy = g_fInfinity;

	// Przeœledzenie ka¿dej armii w poszukiwaniu najbli¿szej wrogiej jednostki.
	std::vector<CArmy*>::iterator ItArmies = g_KnightMgr.GetArmiesContainer()->begin();
	for(int i = 0; ItArmies != g_KnightMgr.GetArmiesContainer()->end(); ++ItArmies, i++) {
		if(i == iArmyId) continue;  // Nielogicznym by³oby rozgl¹danie siê za wrogami we w³asnej armii.

		// Przeœledzenie ka¿dego rycerza w aktualnie analizowanej armii.
		std::vector<IKnight*>::iterator ItMembers = (*ItArmies)->GetMembersContainer()->begin();
		for(; ItMembers != (*ItArmies)->GetMembersContainer()->end(); ++ItMembers)
			if((fDist = CanISee(*ItMembers)) != g_fInfinity) {  // Je¿eli odleg³oœæ do wroga jest ró¿na od "nieskoñczonoœci".
				m_iNumEnemiesSeen++;  // Inkrementacja licznika widzianych wrogów.

				// Jeœli dystans do wroga jest mniejszy od dotychczasowego najmniejszego dystansu,
				// widziany rycerz staje siê wrogiem bêd¹cym najbli¿ej.
				if(fDist < m_fDistToNearestEnemy) {
					m_fDistToNearestEnemy = fDist;
					m_pNearestEnemy = *ItMembers;
				}
			}
	}
	return m_iNumEnemiesSeen;  // Zwrócenie liczby widzianych wrogów.
}

int IBoid::SeeObstacles(void) {
	float fDist;  // Dystans dziel¹cy rycerza z przeszkod¹.

	// Przypisanie wartoœci zmiennym odnosz¹cym siê do wrogich jednostek.
	m_iNumObstaclesSeen = 0;
	m_pNearestObstacle = NULL;
	m_fDistToNearestObstacle = g_fInfinity;

	// Poszukiwanie najbli¿szej przeszkody.
	std::vector<CObstacle*>::iterator It;
	for(It = g_ObstacleMgr.GetObstaclesContainer()->begin(); It != g_ObstacleMgr.GetObstaclesContainer()->end(); ++It)
		if((fDist = CanISee(*It)) != g_fInfinity) {  // Je¿eli odleg³oœæ do przeszkody jest ró¿na od "nieskoñczonoœci".
			m_iNumObstaclesSeen++;  // Inkrementacja licznika widzianych przeszkód.

			// Jeœli dystans do przeszkody jest mniejszy od dotychczasowego najmniejszego dystansu,
			// widziana przeszkoda staje siê przeszkod¹ bêd¹c¹ najbli¿ej.
			if(fDist < m_fDistToNearestObstacle) {
				m_fDistToNearestObstacle = fDist;
				m_pNearestObstacle = *It;
			}
		}

	return m_iNumObstaclesSeen;  // Zwrócenie liczby widzianych przeszkód.
}

int IBoid::SeeComrades(std::vector<IKnight*>* pComradesContainer) {
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
		}

	return m_VisibleComradesContainer.size();  // Zwrócenie iloœci widocznych kompanów.
}

CVector IBoid::SteerToCenter(void) {
	CVector vCenter;  // "Œrodek armii".

	// Zliczenie pozycji wszystkich widocznych kompanów.
	std::vector<IKnight*>::iterator It;
	for(It = m_VisibleComradesContainer.begin() ; It != m_VisibleComradesContainer.end(); ++It)
		vCenter += *(*It)->GetPos();

	vCenter /= static_cast<float>(m_VisibleComradesContainer.size());  // Wyliczenie œredniej zsumowanych pozycji.
	CVector vChange = (vCenter - m_vPos);  // Obliczenie wektora biegn¹cego do "œrodka armii".
	vChange.SetMagnitude(g_DefaultsMgr.GetMinUrgency() * 100.0f);  // Przeskalowanie wektora zmiany.
	return vChange;  // Zwrócenie wektora zmiany.
}

void IBoid::WorldBound(void) {
	//-------------------------------------------------
	// Kontrola kolizji z granicami œwiata w poziomie.
	//-------------------------------------------------

	// Rycerz opuszcza œwiat z lewej strony.
	if((m_CollisionCircle.pos.x  + (m_CollisionCircle.r * 2.0f)) < 0.0f) {
		m_vPos.x = (static_cast<float>(g_DefaultsMgr.GetAreaWidth()) + (m_CollisionCircle.r * 2.0f));
		ShiftCollisionCircle();
	}
	// Rycerz opuszcza œwiat z prawej strony.
	else if((m_CollisionCircle.pos.x - (m_CollisionCircle.r * 2.0f)) > static_cast<float>(g_DefaultsMgr.GetAreaWidth())) {
		m_vPos.x = (0.0f - (m_CollisionCircle.r * 2.0f));
		ShiftCollisionCircle();
	}

	//-----------------------------------------------
	// Kontrola kolizji z granicami œwiata w pionie.
	//-----------------------------------------------

	// Rycerz opuszcza œwiat z góry.
	if((m_CollisionCircle.pos.y  + (m_CollisionCircle.r * 2.0f)) < 0.0f) {
		m_vPos.y = (static_cast<float>(g_DefaultsMgr.GetAreaHeight()) + (m_CollisionCircle.r * 2.0f));
		ShiftCollisionCircle();
	}
	// Rycerz opuszcza œwiat z do³u.
	else if((m_CollisionCircle.pos.y - (m_CollisionCircle.r * 2.0f)) > static_cast<float>(g_DefaultsMgr.GetAreaHeight())) {
		m_vPos.y = (0.0f - (m_CollisionCircle.r * 2.0f));
		ShiftCollisionCircle();
	}
}

float IBoid::CanISee(IKnight* pKnight) {
	if(this == pKnight)	
		return g_fInfinity;  // Jeœli rycerz patrzy na samego siebie...
	float fDist = m_vPos.GetDist(m_vPos, *pKnight->GetPos());  // Obliczenie dystansu dziel¹cego rycerzy.

	// Sprawdzenie, czy rycerz jest w stanie dostrzec drugiego z takiej odleg³oœci.
	if(m_fPerceptionRange > fDist) 
		return fDist;

	return g_fInfinity;  // Powy¿szy warunek nie zosta³ spe³niony, rycerze nie s¹ w stanie siebie dostrzec.
}

float IBoid::CanISee(CObstacle* pObstacle) {
	float fDist = m_vPos.GetDist(m_vPos, *pObstacle->GetPos());  // Obliczenie dystansu dziel¹cego rycerza z przeszkod¹.

	// Sprawdzenie, czy rycerz jest w stanie dostrzec przeszkodê z takiej odleg³oœci.
	if(m_fPerceptionRange > fDist) 
		return fDist;

	return g_fInfinity;  // Powy¿szy warunek nie zosta³ spe³niony, rycerz nie jest w stanie dostrzec przeszkody.
}
