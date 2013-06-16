#pragma once
#include "utilities.h"
#include <cmath>

class CVector {
public:
	//-----------------------
	// Kwintesencja wektora. 
	//-----------------------

	float x, y;  // Sk³adowe: x i y wektora. S¹ to zmienne o dostêpie publicznym, aby korzystanie z wektorów by³o wygodne.

	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	CVector(void);
	CVector(float fX, float fY);
	CVector(const CVector& v);  // "const" zabezpiecza przed modyfikacj¹ wektora wejœciowego.

	//------------
	// Operatory.
	//------------

	CVector& operator=(const CVector& v);  // Przypisanie podanego wektora.
	CVector& operator+=(const CVector& v);  // Dodanie podanego wektora.
	CVector& operator-=(const CVector& v);  // Odjêcie podanego wektora.
	CVector& operator*=(float fVal);  // Przemno¿enie przez podan¹ wartoœæ.
	CVector& operator/=(float fVal);  // Podzielenie przez podan¹ wartoœæ.
	friend bool operator==(const CVector& vA, const CVector& vB);  // Czy oba wektory s¹ identyczne.
	friend bool operator!=(const CVector& vA, const CVector& vB);  // Czy oba wektory s¹ ró¿ne.
	friend CVector operator+(const CVector& vA, const CVector& vB);  // Zwraca sumê wektorów.
	friend CVector operator-(const CVector& vA, const CVector& vB);  // Zwraca ró¿nicê wektorów.
	friend CVector operator-(const CVector& v);  // Zwraca wektor ze zmienionymi znakami.
	friend CVector operator*(const CVector& v, float fVal);  // Zwraca wektor przemno¿ony przez wartoœæ.
	friend CVector operator*(float fVal, const CVector& v);
	friend float operator*(const CVector& vA, const CVector& vB);  // Zwraca iloczyn skalarny dwóch wektorów.
	friend CVector operator/(const CVector& v, float fVal);  // Zwraca wektor podzielony przez wartoœæ.

	//---------
	// Metody. 
	//---------

	void Set(float fX, float fY) { x = fX; y = fY; }  // Ustawia obydwie zmienne wektora.

	// Zwraca d³ugoœæ wektora. "const" pilnuje, aby przy obliczeniach wektor pozosta³ bez zmian.
	float Length(void) const { return static_cast<float>(sqrt((x * x) + (y * y))); }

	bool IsZero(void) const { return ((x == 0.0f) && (y == 0.0f)); }  // Czy wektor jest zerowy.
	CVector& Normalize(void);  // Normalizuje wektor, staje siê on wektorem jednostkowym.
	void SetMagnitude(float fVel) { this->Normalize(); x *= fVel; y *= fVel; }  // Ustawia wielkoœæ wektora.

	// Zwraca dystans miêdzy wektorami.
	float GetDist(CVector vPos1, CVector vPos2) { CVector vTemp = (vPos1 - vPos2); return vTemp.Length(); }

	float Yaw(void);  // Zwraca odchylenie wektora w stopniach.
};
