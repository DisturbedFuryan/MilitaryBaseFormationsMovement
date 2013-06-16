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
	// Kolizje mi�dzy podstawowymi figurami.
	//---------------------------------------

	bool CheckCollision(SDL_Rect* pRectA, SDL_Rect* pRectB);
	bool CheckCollision(CCircle* pCircleA, CCircle* pCircleB) {
		// Je�eli odleg�o�� od �rodk�w dw�ch k�ek jest mniejsza od sumy ich promieni, to...
		if(pCircleA->pos.GetDist(pCircleA->pos, pCircleB->pos) < (pCircleA->r + pCircleB->r))
			return true;  // ...nast�pi�a kolizja.
		return false;  // Brak kolizji.
	}
	bool CheckCollision(CVector* pVector, CCircle* pCircle)
		{ return (pVector->GetDist(*pVector, pCircle->pos) < pCircle->r); }

	//---------------------------------------------------
	// Kolizje mi�dzy podstawowymi figurami a obiektami.
	//---------------------------------------------------

	// Sprawdza, czy wyst�pi�a kolizja pomi�dzy podanym ko�em a ko�em kolizji kt�rego� z rycerzy.
	bool CheckCollisionWithKnights(CCircle* pCircle);

	// Sprawdza, czy wyst�pi�a kolizja pomi�dzy podanym ko�em a ko�em kolizji kt�rej� z przeszk�d.
	bool CheckCollisionWithObstacles(CCircle* pCircle);

	// Sprawdza, czy wyst�pi�a kolizja pomi�dzy podanym ko�em a ko�ami kolizji rycerzy i przeszk�d.
	bool CheckCollisionWithObjects(CCircle* pCircle) {
		if(CheckCollisionWithKnights(pCircle) || CheckCollisionWithObstacles(pCircle))
			return true;
		return false;
	}

	//--------------------------------------
	// Kolizje zwi�zane z granicami �wiata.
	//--------------------------------------

	// Sprawdza, czy ko�o znajduje si� poza granicami �wiata.
	bool IsOutWorld(CCircle* pCircle) {
		if(((pCircle->pos.x + pCircle->r) < 0.0f) 
			 || ((pCircle->pos.x - pCircle->r) > static_cast<float>(g_DefaultsMgr.GetAreaWidth()))
			 || ((pCircle->pos.y + pCircle->r) < 0.0f) 
			 || ((pCircle->pos.y - pCircle->r) > static_cast<float>(g_DefaultsMgr.GetAreaHeight())))
			return true;  // Ko�o znajduje si� poza �wiatem.
		return false;  // Ko�o znajduje si� w obszarze �wiata.
	}
};
