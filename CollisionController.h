#pragma once
#include "Circle.h"
#include "DefaultsMgr.h"
#include "SDL.h"
#include "Singleton.h"
#include "Vector.h"

#define g_DefaultsMgr CDefaultsMgr::GetSingleton()

class CCollisionController : public Singleton<CCollisionController> {
public:
	//---------------------------------------
	// Kolizje miêdzy podstawowymi figurami.
	//---------------------------------------

	bool CheckCollision(SDL_Rect* pRectA, SDL_Rect* pRectB);
	bool CheckCollision(CCircle* pCircleA, CCircle* pCircleB) {
		// Je¿eli odleg³oœæ od œrodków dwóch kó³ek jest mniejsza od sumy ich promieni, to...
		if(pCircleA->pos.GetDist(pCircleA->pos, pCircleB->pos) < (pCircleA->r + pCircleB->r))
			return true;  // ...nast¹pi³a kolizja.
		return false;  // Brak kolizji.
	}
	bool CheckCollision(CVector* pVector, CCircle* pCircle)
		{ return (pVector->GetDist(*pVector, pCircle->pos) < pCircle->r); }

	//---------------------------------------------------
	// Kolizje miêdzy podstawowymi figurami a obiektami.
	//---------------------------------------------------

	// Sprawdza, czy wyst¹pi³a kolizja pomiêdzy podanym ko³em a ko³em kolizji któregoœ z rycerzy.
	bool CheckCollisionWithKnights(CCircle* pCircle);

	// Sprawdza, czy wyst¹pi³a kolizja pomiêdzy podanym ko³em a ko³em kolizji którejœ z przeszkód.
	bool CheckCollisionWithObstacles(CCircle* pCircle);

	// Sprawdza, czy wyst¹pi³a kolizja pomiêdzy podanym ko³em a ko³ami kolizji rycerzy i przeszkód.
	bool CheckCollisionWithObjects(CCircle* pCircle) {
		if(CheckCollisionWithKnights(pCircle) || CheckCollisionWithObstacles(pCircle))
			return true;
		return false;
	}

	//--------------------------------------
	// Kolizje zwi¹zane z granicami œwiata.
	//--------------------------------------

	// Sprawdza, czy ko³o znajduje siê poza granicami œwiata.
	bool IsOutWorld(CCircle* pCircle) {
		if(((pCircle->pos.x + pCircle->r) < 0.0f) 
			 || ((pCircle->pos.x - pCircle->r) > static_cast<float>(g_DefaultsMgr.GetAreaWidth()))
			 || ((pCircle->pos.y + pCircle->r) < 0.0f) 
			 || ((pCircle->pos.y - pCircle->r) > static_cast<float>(g_DefaultsMgr.GetAreaHeight())))
			return true;  // Ko³o znajduje siê poza œwiatem.
		return false;  // Ko³o znajduje siê w obszarze œwiata.
	}
};
