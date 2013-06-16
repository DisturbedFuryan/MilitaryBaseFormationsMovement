#pragma once
#include "utilities.h"
#include <cmath>

class CVector {
public:
	//-----------------------
	// Kwintesencja wektora. 
	//-----------------------

	float x, y;  // Sk�adowe: x i y wektora. S� to zmienne o dost�pie publicznym, aby korzystanie z wektor�w by�o wygodne.

	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	CVector(void);
	CVector(float fX, float fY);
	CVector(const CVector& v);  // "const" zabezpiecza przed modyfikacj� wektora wej�ciowego.

	//------------
	// Operatory.
	//------------

	CVector& operator=(const CVector& v);  // Przypisanie podanego wektora.
	CVector& operator+=(const CVector& v);  // Dodanie podanego wektora.
	CVector& operator-=(const CVector& v);  // Odj�cie podanego wektora.
	CVector& operator*=(float fVal);  // Przemno�enie przez podan� warto��.
	CVector& operator/=(float fVal);  // Podzielenie przez podan� warto��.
	friend bool operator==(const CVector& vA, const CVector& vB);  // Czy oba wektory s� identyczne.
	friend bool operator!=(const CVector& vA, const CVector& vB);  // Czy oba wektory s� r�ne.
	friend CVector operator+(const CVector& vA, const CVector& vB);  // Zwraca sum� wektor�w.
	friend CVector operator-(const CVector& vA, const CVector& vB);  // Zwraca r�nic� wektor�w.
	friend CVector operator-(const CVector& v);  // Zwraca wektor ze zmienionymi znakami.
	friend CVector operator*(const CVector& v, float fVal);  // Zwraca wektor przemno�ony przez warto��.
	friend CVector operator*(float fVal, const CVector& v);
	friend float operator*(const CVector& vA, const CVector& vB);  // Zwraca iloczyn skalarny dw�ch wektor�w.
	friend CVector operator/(const CVector& v, float fVal);  // Zwraca wektor podzielony przez warto��.

	//---------
	// Metody. 
	//---------

	void Set(float fX, float fY) { x = fX; y = fY; }  // Ustawia obydwie zmienne wektora.

	// Zwraca d�ugo�� wektora. "const" pilnuje, aby przy obliczeniach wektor pozosta� bez zmian.
	float Length(void) const { return static_cast<float>(sqrt((x * x) + (y * y))); }

	bool IsZero(void) const { return ((x == 0.0f) && (y == 0.0f)); }  // Czy wektor jest zerowy.
	CVector& Normalize(void);  // Normalizuje wektor, staje si� on wektorem jednostkowym.
	void SetMagnitude(float fVel) { this->Normalize(); x *= fVel; y *= fVel; }  // Ustawia wielko�� wektora.

	// Zwraca dystans mi�dzy wektorami.
	float GetDist(CVector vPos1, CVector vPos2) { CVector vTemp = (vPos1 - vPos2); return vTemp.Length(); }

	float Yaw(void);  // Zwraca odchylenie wektora w stopniach.
};
