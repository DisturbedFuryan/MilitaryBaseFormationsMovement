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

	// Stworzenie armii z u¿ytkownikiem.
	KnightMgr.AddArmy(1, (g_DefaultsMgr.GetNumLeadersArmy() - 1), g_DefaultsMgr.GetNumSerfsArmy());

	// Stworzenie pozosta³ych armii.
	for(int i = 1; i < g_DefaultsMgr.GetNumArmies(); i++)
		KnightMgr.AddArmy(0, g_DefaultsMgr.GetNumLeadersArmy(), g_DefaultsMgr.GetNumSerfsArmy());
}
