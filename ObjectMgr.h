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
	void HandleInput(void) { KnightMgr.HandleInput(); }  // Obs³uga zdarzeñ dla wszystkich obiektów.
	void UpdateObjects(const Uint32 iDeltaTicks) { KnightMgr.UpdateArmies(iDeltaTicks); }  // Uaktualnia wszystkie obiekty.

	// Wyœwietla wszystkie obiekty.
	void ShowObjects(void) {
		// Wyœwietlenie obiektów w odpowiedniej kolejnoœci, te wyœwietlone jako pierwsze znajd¹ siê na spodzie.
		KnightMgr.ShowShoesprints();  // Œlady zostawiane przez rycerzy.
		KnightMgr.ShowBacklights();  // Podœwietlenie rycerzy.
		KnightMgr.ShowShoes();  // Buty rycerzy.
		ObstacleMgr.Show(CObstacle::stone);  // Kamienie.
		KnightMgr.ShowSwords();  // Miecze rycerzy.
		KnightMgr.ShowBodies();  // Cia³a rycerzy.
		ObstacleMgr.Show(CObstacle::palisade);  // Palisady.
	}
};
