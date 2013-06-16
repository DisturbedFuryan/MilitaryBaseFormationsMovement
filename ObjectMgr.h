#pragma once
#include "CollisionController.h"
#include "KnightMgr.h"
#include "ObstacleMgr.h"
#include "Singleton.h"

class CObjectMgr : public Singleton<CObjectMgr> {
public:
	//-------------
	// Singletony.
	//-------------

	CKnightMgr KnightMgr;
	CObstacleMgr ObstacleMgr;
	CCollisionController CollisionController;

	//---------
	// Metody.
	//---------

	void CreateObjects(void);  // Tworzy wszystkie obiekty, rycerzy i przeszkody.
	void HandleInput(void) { KnightMgr.HandleInput(); }  // Obs�uga zdarze� dla wszystkich obiekt�w.
	void UpdateObjects(const Uint32 iDeltaTicks) { KnightMgr.UpdateArmies(iDeltaTicks); }  // Uaktualnia wszystkie obiekty.

	// Wy�wietla wszystkie obiekty.
	void ShowObjects(void) {
		// Wy�wietlenie obiekt�w w odpowiedniej kolejno�ci, te wy�wietlone jako pierwsze znajd� si� na spodzie.
		KnightMgr.ShowShoesprints();  // �lady zostawiane przez rycerzy.
		KnightMgr.ShowBacklights();  // Pod�wietlenie rycerzy.
		KnightMgr.ShowShoes();  // Buty rycerzy.
		ObstacleMgr.Show(CObstacle::stone);  // Kamienie.
		KnightMgr.ShowSwords();  // Miecze rycerzy.
		KnightMgr.ShowBodies();  // Cia�a rycerzy.
		ObstacleMgr.Show(CObstacle::palisade);  // Palisady.
	}
};
