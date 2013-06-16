#pragma once
#include "Circle.h"
#include "SDL.h"
#include "Vector.h"

class CObstacle {
public:
	//--------------------
	// Typy wyliczeniowe.
	//--------------------

	enum Type { palisade, stone };  // Typ przeszkody: palisada, kamie�.

	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	CObstacle(int iId, CVector* vPos, short iType);

	//-------------------------------------
	// Metody zajmuj�ce si� wy�wietlaniem.
	//-------------------------------------

	void Show(void);  // Wy�wietla przeszkod�.

	//-------------------
	// Metody dost�powe.
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
	SDL_Surface* m_pObstacleSurface;  // Powierzchnia z przeszkod�.
	SDL_Rect m_SurfaceRect;  // Prostok�t opisuj�cy powierzchni�.
	CVector m_vPos;  // Po�o�enie przeszkody.
	CCircle m_CollisionCircle;  // Ko�o kolizji.

	//---------------------------
	// Metody dotycz�ce kolizji.
	//---------------------------

	// Zsynchronizowanie pozycji ko�a kolizji z pozycj� przeszkody.
	void ShiftCollisionCircle(void) { m_CollisionCircle.pos = m_vPos; }
};
