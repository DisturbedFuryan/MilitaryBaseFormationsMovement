#pragma once
#include "utilities.h"
#include "Vector.h"

class CCircle {
public:
	//----------
	// Zmienne.
	//----------

	CVector pos;  // Pozycja.
	float r;  // Promieñ.

	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	CCircle(void);
	CCircle(float fX, float fY, float fR);
	CCircle(const CVector& vPos, float fR);
	CCircle(const CVector* pvPos, float fR);
	CCircle(const CCircle& Circle);
	CCircle(const CCircle* pCircle);

	//------------
	// Operatory.
	//------------

	CCircle& operator=(const CCircle& Circle);  // Przypisanie podanego ko³a.

	//----------------------------------------
	// Metody przypisuj¹ce wartoœci zmiennym.
	//----------------------------------------

	void Set(float fX, float fY, float fR) { pos.x = fX; pos.y = fY; r = fR; }
	void Set(const CVector& vPos, float fR) { pos = vPos; r = fR; }
	void Set(const CVector* pvPos, float fR) { pos = *pvPos; r = fR; }
	void Set(const CCircle& Circle) { *this = Circle; }
	void Set(const CCircle* pCircle) { *this = *pCircle; }

	//-------------------
	// Pozosta³e metody.
	//-------------------

	float Area(void) const { return (g_fPi * r * r); }  // Zwraca pole ko³a.
};
