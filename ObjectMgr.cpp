#include "ObjectMgr.h"
#include "singletons.h"

void CObjectMgr::CreateObjects(void) {
	//------------------------------------------
	// Stworzenie otoczenia: palisad i kamieni.
	//------------------------------------------

	ObstacleMgr.CreatePalisadesAndStones(g_DefaultsMgr.GetNumPalisades(), g_DefaultsMgr.GetNumStones());

	//---------------------------
	// Stworzenie armii rycerzy.
	//---------------------------

	// Stworzenie armii z użytkownikiem.
	KnightMgr.AddArmy(1, (g_DefaultsMgr.GetNumLeadersArmy() - 1), g_DefaultsMgr.GetNumSerfsArmy());

	// Stworzenie pozostałych armii.
	for(int i = 1; i < g_DefaultsMgr.GetNumArmies(); i++)
		KnightMgr.AddArmy(0, g_DefaultsMgr.GetNumLeadersArmy(), g_DefaultsMgr.GetNumSerfsArmy());
}
