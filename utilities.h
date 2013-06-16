#pragma once
#include <cstdlib>

//--------
// Sta�e.
//--------

const float g_fPi = 3.14159f;  // Liczba Pi.
const float g_fInfinity = 999999.0f;  // "Niesko�czono��".

//--------------------------
// R�ne przydatne funkcje.
//--------------------------

// Generator liczb losowych.
inline float Rand(void) {
	return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
}

// Funkcja zwracaj�ca -1, je�li zadana warto�� jest mniejsza od zera, lub 1 w przeciwnym wypadku.
inline int Sign(float fX) {
	return ((fX < 0.0f) ? (-1) : (1)); 
}
