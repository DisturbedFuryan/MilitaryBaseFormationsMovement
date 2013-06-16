#include "CollisionController.h"
#include "singletons.h"

bool CCollisionController::CheckCollision(SDL_Rect* pRectA, SDL_Rect* pRectB) {
	int iLeftA, iLeftB, iRightA, iRightB, iTopA, iTopB, iBottomA, iBottomB;

	// Obliczenie po�o�enia kraw�dzi prostok�t�w.
	iLeftA = pRectA->x;
	iRightA = (pRectA->x + pRectA->w);
	iTopA = pRectA->y;
	iBottomA = (pRectA->y + pRectA->h);
	iLeftB = pRectB->x;
	iRightB = (pRectB->x + pRectB->w);
	iTopB = pRectB->y;
	iBottomB = (pRectB->y + pRectB->h);

	// Je�eli prostok�t A jest powy�ej lub poni�ej prostok�ta B, lub prostok�t A jest po lewej
	// lub prawej stronie prostok�ta B, nie nachodz�c na niego, to kolizji nie ma.
	if(iBottomA <= iTopB) 
		return false;
	if(iTopA >= iBottomB) 
		return false;
	if(iRightA <= iLeftB) 
		return false;
	if(iLeftA >= iRightB) 
		return false;

	return true;  // Wyst�pi�a kolizja.
}

bool CCollisionController::CheckCollisionWithKnights(CCircle* pCircle) {
	// Szukamy kolizji pomi�dzy podanym ko�em, a kt�rym� z istniej�cych rycerzy.
	std::vector<IKnight*>::iterator It = g_KnightMgr.GetKnightsContainer()->begin();
	while(It != g_KnightMgr.GetKnightsContainer()->end()) {
		if(CheckCollision(pCircle, (*It)->GetCollisionCircle()))
			return true;  // Wyst�pi�a kolizja.
		++It;
	}
	return false;  // Nie stwierdzono kolizji.
}

bool CCollisionController::CheckCollisionWithObstacles(CCircle* pCircle) {
	// Szukamy kolizji pomi�dzy podanym ko�em, a kt�r�� z istniej�cych przeszk�d.
	std::vector<CObstacle*>::iterator It = g_ObstacleMgr.GetObstaclesContainer()->begin();
	while(It != g_ObstacleMgr.GetObstaclesContainer()->end()) {
		if(CheckCollision(pCircle, (*It)->GetCollisionCircle()))
			return true;  // Wyst�pi�a kolizja.
		++It;
	}
	return false;  // Nie stwierdzono kolizji.
}
