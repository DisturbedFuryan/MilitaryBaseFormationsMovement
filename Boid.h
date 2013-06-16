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

	// Kontener widzianych kompan�w, u�ywany przez ka�dego rycerza, poruszaj�cego si� zgodnie z algorytmem armii.
	// Kontener przed aktualizacj� jest zawsze czyszczony. Dlatego rycerze moga korzysta� ze wsp�lnego kontenera.
	static std::vector<IKnight*> m_VisibleComradesContainer;

	//-----------------------------
	// Konstruktory i destruktory. 
	//-----------------------------

	IBoid(void);

	//-------------------------------------
	// Metody zwi�zane z algorytmem armii.
	//-------------------------------------

	// Aktualizacja, metoda wykorzystywana jest w ka�dej klatce. Metoda czysto wirtualna.
	virtual void ArmyIt(int iArmyId, std::vector<IKnight*>* pComradesContainer, const Uint32 iDeltaTicks) = 0;

	//-------------------
	// Pozosta�e metody.
	//-------------------

	// Dodanie rycerza do kontenera widoczno�ci.
	void AddToVisibleContainer(IKnight* pKnight) {
		// Sprawdzenie, czy rycerz nie widzi zbyt wielu kompan�w.
		if(m_VisibleComradesContainer.size() < static_cast<unsigned>(g_DefaultsMgr.GetMaxComradesVisible()))
			// Rycerz nie widzi zbyt wielu kompan�w, wi�c mo�na doda� kolejnego do kontenera.
			m_VisibleComradesContainer.push_back(pKnight);
	}

	// Wyczyszczenie kontenera widoczno�ci.
	virtual void ClearVisibleContainer(void) {
		m_VisibleComradesContainer.clear();  // Wyczyszczenie kontenera widoczno�ci.

		// Wyczyszczenie pozosta�ych informacji zwi�zanych z kontenerem widoczno�ci.
		m_pNearestComrade = NULL;
		m_fDistToNearestComrade = g_fInfinity;
	}
protected:
	//----------
	// Zmienne.
	//----------

	float m_fPerceptionRange;  // Zasi�g widzenia rycerza.
	short m_iNumEnemiesSeen;  // Ilo�� widzianych wrog�w.
	short m_iNumObstaclesSeen;  // Ilo�� widzianych przeszk�d.
	IKnight* m_pNearestComrade;  // Wska�nik na najbli�szego kompana, je�li istnieje.
	IKnight* m_pNearestEnemy;  // Wska�nik na najbli�szego wroga, je�li istnieje.
	CObstacle* m_pNearestObstacle;  // Wska�nik na najbli�sz� przeszkod�, je�li istnieje.
	float m_fDistToNearestComrade;  // Odleg�o�� do najbli�szego kompana, je�li istnieje.
	float m_fDistToNearestEnemy;  // Odleg�o�� do najbli�szego wroga, je�li istnieje.
	float m_fDistToNearestObstacle;  // Odleg�o�� do najbli�szej przeszkody, je�li istnieje.

	//-------------------------------------
	// Metody zwi�zane z algorytmem armii.
	//-------------------------------------

	// Generuje wektor okre�laj�cy jak powininen porusza� si� rycerz niezale�ny.
	CVector Cruising(void);

	// Generuje wektor zapobiegaj�cy kontaktowi z najbli�szym wrogiem (cz�onkiem innej armii).
	CVector FleeEnemies(void) {
		CVector vChange;  // Wektor zmiany.

		// Je�eli rycerz jest zbyt blisko wroga.
		if(m_fDistToNearestEnemy < (g_DefaultsMgr.GetEnemyKeepAwayDist() + (g_DefaultsMgr.GetKnightRadius() * 2.0f)))
			vChange = (m_vPos - *m_pNearestEnemy->GetPos());  // Obliczenie wektora oddalenia si� od wroga.

		return vChange;  // Zwr�cenie wektora zmiany.
	}

	// Generuje wektor zapobiegaj�cy kontaktowi z najbli�sz� przeszkod�.
	CVector FleeObstacles(void) {
		CVector vChange;  // Wektor zmiany.

		// Je�eli rycerz jest zbyt blisko przeszkody.
		if(m_pNearestObstacle &&  // Najbli�sza przeszkoda jest potrzebna do odczytania promienia.
			 (m_fDistToNearestObstacle < (g_DefaultsMgr.GetObstacleKeepAwayDist() + g_DefaultsMgr.GetKnightRadius() 
			 + m_pNearestObstacle->GetCollisionCircle()->r)))
			vChange = (m_vPos - *m_pNearestObstacle->GetPos());  // Obliczenie wektora oddalenia si� od przeszkody.

		return vChange;  // Zwr�cenie wektora zmiany.
	}

	CVector KeepDistance(void);  // Generuje wektor utrzymuj�cy odpowiedni� odleg�o�� wzgl�dem najbli�szego kompana.

	// Generuje wektor podobny do kierunku najbli�szego kompana.
	CVector MatchHeading(void) {
		CVector vChange = *m_pNearestComrade->GetVel();  // Skopiowanie wektora pr�dko�ci od najbli�szego kompana.

		// Znormalizowanie i przeskalowanie wektora zmiany o "troch�".
		vChange.SetMagnitude(g_DefaultsMgr.GetMinUrgency() * 100.0f);

		return vChange;  // Zwr�cenie wektora zmiany.
	}

	int SeeEnemies(short iArmyId);  // Okre�la, kt�rzy wrogowie s� w zasi�gu pola widzenia rycerza.
	int SeeObstacles(void);  // Okre�la, kt�re przeszkody s� w zasi�gu pola widzenia rycerza.
	virtual int SeeComrades(std::vector<IKnight*>* pComradesContainer);  // Okre�la, kt�rych kompan�w widzi rycerz.
	CVector SteerToCenter(void);  // Generuje wektor zapobiegaj�cy oddaleniu si� rycerza od "�rodka armii".

	// Implementacja granic �wiata. Rycerze opuszczaj�cy �wiat z lewej strony zostan� przeniesieni na jego praw� stron�.
	// Natomiast rycerze opuszczaj�cy �wiat z pozosta�ych trzech stron zostan� przeniesieni w spos�b analogiczny.
	void WorldBound(void);

	//-------------------
	// Pozosta�e metody.
	//-------------------

	// Dodaje do siebie policzone wektory, zwraca wielko�� wektora akumulacji.
	float AccumulateChanges(CVector& vAccumulator, CVector vChanges) {
		vAccumulator += vChanges;  // Dodanie do akumulatora wektora zmiany.
		return vAccumulator.Length();  // Zwr�cenie d�ugo�ci akumulatora.
	}

	// Okre�la, czy przepytywany rycerz widzi tego podanego jako parametr, zwraca odleg�o�� do niego.
	float CanISee(IKnight* pKnight);

	// Okre�la, czy przepytywany rycerz widzi podan� przeszkod�, zwraca odleg�o�� do niej.
	float CanISee(CObstacle* pObstacle);
};
