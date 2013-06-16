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
	// Metody zajmuj�ce si� wy�wietlaniem.
	//-------------------------------------

	void Show(void);  // Wy�wietla wszytskie przeszkody na ekranie.
	void Show(short iType);  // Wy�wietla przeszkody podanego typu.

	//-------------------
	// Metody dost�powe.
	//-------------------

	// Zwraca wska�nik do kontenera z przeszkodami.
	std::vector<CObstacle*>* GetObstaclesContainer(void) { return &m_ObstaclesContainer; }

	//----------------------
	// Tworzenie przeszk�d.
	//----------------------

	void CreatePalisadesAndStones(int iNumPalisades, int iNumStones);  // Tworzy zadan� ilo�� palisad i kamieni.
	void CreateObstacles(int iNumObstacles, short iType);  // Tworzy zadan� ilo�� podanego rodzaju przeszk�d.
private:
	//----------
	// Zmienne.
	//----------

	std::vector<CObstacle*> m_ObstaclesContainer;  // Kontener z przeszkodami.
};
