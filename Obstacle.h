#pragma once
#include "Circle.h"
#include "SDL.h"
#include "Vector.h"

class CObstacle {
public:
	//--------------------
	// Typy wyliczeniowe.
	//--------------------

	enum Type { palisade, stone };  // Typ przeszkody: palisada, kamieñ.

	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	CObstacle(int iId, CVector* vPos, short iType);

	//-------------------------------------
	// Metody zajmuj¹ce siê wyœwietlaniem.
	//-------------------------------------

	void Show(void);  // Wyœwietla przeszkodê.

	//-------------------
	// Metody dostêpowe.
	//-------------------

	short GetId(void) const { return m_iId; }
	short GetType(void) const { return m_iType; }
	CVector* GetPos(void) { return &m_vPos; }
	CCircle* GetCollisionCircle(void) { return &m_CollisionCircle; }
protected:
	//----------
	// Zmienne.
	//----------

	short m_iId;  // Identyfikator przeszkody.
	short m_iType;  // Typ przeszkody.
	SDL_Surface* m_pObstacleSurface;  // Powierzchnia z przeszkod¹.
	SDL_Rect m_SurfaceRect;  // Prostok¹t opisuj¹cy powierzchniê.
	CVector m_vPos;  // Po³o¿enie przeszkody.
	CCircle m_CollisionCircle;  // Ko³o kolizji.

	//---------------------------
	// Metody dotycz¹ce kolizji.
	//---------------------------

	// Zsynchronizowanie pozycji ko³a kolizji z pozycj¹ przeszkody.
	void ShiftCollisionCircle(void) { m_CollisionCircle.pos = m_vPos; }
};
