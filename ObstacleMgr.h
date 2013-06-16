#pragma once
#include "Obstacle.h"
#include "Singleton.h"
#include <vector>

class CObstacleMgr : public Singleton<CObstacleMgr> {
public:
	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	~CObstacleMgr(void);

	//-------------------------------------
	// Metody zajmuj¹ce siê wyœwietlaniem.
	//-------------------------------------

	void Show(void);  // Wyœwietla wszytskie przeszkody na ekranie.
	void Show(short iType);  // Wyœwietla przeszkody podanego typu.

	//-------------------
	// Metody dostêpowe.
	//-------------------

	// Zwraca wskaŸnik do kontenera z przeszkodami.
	std::vector<CObstacle*>* GetObstaclesContainer(void) { return &m_ObstaclesContainer; }

	//----------------------
	// Tworzenie przeszkód.
	//----------------------

	void CreatePalisadesAndStones(int iNumPalisades, int iNumStones);  // Tworzy zadan¹ iloœæ palisad i kamieni.
	void CreateObstacles(int iNumObstacles, short iType);  // Tworzy zadan¹ iloœæ podanego rodzaju przeszkód.
private:
	//----------
	// Zmienne.
	//----------

	std::vector<CObstacle*> m_ObstaclesContainer;  // Kontener z przeszkodami.
};
