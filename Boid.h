#pragma once
#include "DefaultsMgr.h"
#include "Knight.h"
#include "Obstacle.h"
#include "utilities.h"
#include "Vector.h"
#include <cmath>
#include <vector>

#define g_DefaultsMgr CDefaultsMgr::GetSingleton()

class IBoid : public IKnight {
public:
	//--------------------
	// Zmienne statyczne.
	//--------------------

	// Kontener widzianych kompanów, u¿ywany przez ka¿dego rycerza, poruszaj¹cego siê zgodnie z algorytmem armii.
	// Kontener przed aktualizacj¹ jest zawsze czyszczony. Dlatego rycerze moga korzystaæ ze wspólnego kontenera.
	static std::vector<IKnight*> m_VisibleComradesContainer;

	//-----------------------------
	// Konstruktory i destruktory. 
	//-----------------------------

	IBoid(void);

	//-------------------------------------
	// Metody zwi¹zane z algorytmem armii.
	//-------------------------------------

	// Aktualizacja, metoda wykorzystywana jest w ka¿dej klatce. Metoda czysto wirtualna.
	virtual void ArmyIt(int iArmyId, std::vector<IKnight*>* pComradesContainer, const Uint32 iDeltaTicks) = 0;

	//-------------------
	// Pozosta³e metody.
	//-------------------

	// Dodanie rycerza do kontenera widocznoœci.
	void AddToVisibleContainer(IKnight* pKnight) {
		// Sprawdzenie, czy rycerz nie widzi zbyt wielu kompanów.
		if(m_VisibleComradesContainer.size() < static_cast<unsigned>(g_DefaultsMgr.GetMaxComradesVisible()))
			// Rycerz nie widzi zbyt wielu kompanów, wiêc mo¿na dodaæ kolejnego do kontenera.
			m_VisibleComradesContainer.push_back(pKnight);
	}

	// Wyczyszczenie kontenera widocznoœci.
	virtual void ClearVisibleContainer(void) {
		m_VisibleComradesContainer.clear();  // Wyczyszczenie kontenera widocznoœci.

		// Wyczyszczenie pozosta³ych informacji zwi¹zanych z kontenerem widocznoœci.
		m_pNearestComrade = NULL;
		m_fDistToNearestComrade = g_fInfinity;
	}
protected:
	//----------
	// Zmienne.
	//----------

	float m_fPerceptionRange;  // Zasiêg widzenia rycerza.
	short m_iNumEnemiesSeen;  // Iloœæ widzianych wrogów.
	short m_iNumObstaclesSeen;  // Iloœæ widzianych przeszkód.
	IKnight* m_pNearestComrade;  // WskaŸnik na najbli¿szego kompana, jeœli istnieje.
	IKnight* m_pNearestEnemy;  // WskaŸnik na najbli¿szego wroga, jeœli istnieje.
	CObstacle* m_pNearestObstacle;  // WskaŸnik na najbli¿sz¹ przeszkodê, jeœli istnieje.
	float m_fDistToNearestComrade;  // Odleg³oœæ do najbli¿szego kompana, jeœli istnieje.
	float m_fDistToNearestEnemy;  // Odleg³oœæ do najbli¿szego wroga, jeœli istnieje.
	float m_fDistToNearestObstacle;  // Odleg³oœæ do najbli¿szej przeszkody, jeœli istnieje.

	//-------------------------------------
	// Metody zwi¹zane z algorytmem armii.
	//-------------------------------------

	// Generuje wektor okreœlaj¹cy jak powininen poruszaæ siê rycerz niezale¿ny.
	CVector Cruising(void);

	// Generuje wektor zapobiegaj¹cy kontaktowi z najbli¿szym wrogiem (cz³onkiem innej armii).
	CVector FleeEnemies(void) {
		CVector vChange;  // Wektor zmiany.

		// Je¿eli rycerz jest zbyt blisko wroga.
		if(m_fDistToNearestEnemy < (g_DefaultsMgr.GetEnemyKeepAwayDist() + (g_DefaultsMgr.GetKnightRadius() * 2.0f)))
			vChange = (m_vPos - *m_pNearestEnemy->GetPos());  // Obliczenie wektora oddalenia siê od wroga.

		return vChange;  // Zwrócenie wektora zmiany.
	}

	// Generuje wektor zapobiegaj¹cy kontaktowi z najbli¿sz¹ przeszkod¹.
	CVector FleeObstacles(void) {
		CVector vChange;  // Wektor zmiany.

		// Je¿eli rycerz jest zbyt blisko przeszkody.
		if(m_pNearestObstacle &&  // Najbli¿sza przeszkoda jest potrzebna do odczytania promienia.
			 (m_fDistToNearestObstacle < (g_DefaultsMgr.GetObstacleKeepAwayDist() + g_DefaultsMgr.GetKnightRadius() 
			 + m_pNearestObstacle->GetCollisionCircle()->r)))
			vChange = (m_vPos - *m_pNearestObstacle->GetPos());  // Obliczenie wektora oddalenia siê od przeszkody.

		return vChange;  // Zwrócenie wektora zmiany.
	}

	CVector KeepDistance(void);  // Generuje wektor utrzymuj¹cy odpowiedni¹ odleg³oœæ wzglêdem najbli¿szego kompana.

	// Generuje wektor podobny do kierunku najbli¿szego kompana.
	CVector MatchHeading(void) {
		CVector vChange = *m_pNearestComrade->GetVel();  // Skopiowanie wektora prêdkoœci od najbli¿szego kompana.

		// Znormalizowanie i przeskalowanie wektora zmiany o "trochê".
		vChange.SetMagnitude(g_DefaultsMgr.GetMinUrgency() * 100.0f);

		return vChange;  // Zwrócenie wektora zmiany.
	}

	int SeeEnemies(short iArmyId);  // Okreœla, którzy wrogowie s¹ w zasiêgu pola widzenia rycerza.
	int SeeObstacles(void);  // Okreœla, które przeszkody s¹ w zasiêgu pola widzenia rycerza.
	virtual int SeeComrades(std::vector<IKnight*>* pComradesContainer);  // Okreœla, których kompanów widzi rycerz.
	CVector SteerToCenter(void);  // Generuje wektor zapobiegaj¹cy oddaleniu siê rycerza od "œrodka armii".

	// Implementacja granic œwiata. Rycerze opuszczaj¹cy œwiat z lewej strony zostan¹ przeniesieni na jego praw¹ stronê.
	// Natomiast rycerze opuszczaj¹cy œwiat z pozosta³ych trzech stron zostan¹ przeniesieni w sposób analogiczny.
	void WorldBound(void);

	//-------------------
	// Pozosta³e metody.
	//-------------------

	// Dodaje do siebie policzone wektory, zwraca wielkoœæ wektora akumulacji.
	float AccumulateChanges(CVector& vAccumulator, CVector vChanges) {
		vAccumulator += vChanges;  // Dodanie do akumulatora wektora zmiany.
		return vAccumulator.Length();  // Zwrócenie d³ugoœci akumulatora.
	}

	// Okreœla, czy przepytywany rycerz widzi tego podanego jako parametr, zwraca odleg³oœæ do niego.
	float CanISee(IKnight* pKnight);

	// Okreœla, czy przepytywany rycerz widzi podan¹ przeszkodê, zwraca odleg³oœæ do niej.
	float CanISee(CObstacle* pObstacle);
};
