#pragma once
#include <cstdlib>

//--------
// Sta³e.
//--------

const float g_fPi = 3.14159f;  // Liczba Pi.
const float g_fInfinity = 999999.0f;  // "Nieskoñczonoœæ".

//--------------------------
// Ró¿ne przydatne funkcje.
//--------------------------

// Generator liczb losowych.
inline float Rand(void) {
	return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
}

// Funkcja zwracaj¹ca -1, jeœli zadana wartoœæ jest mniejsza od zera, lub 1 w przeciwnym wypadku.
inline int Sign(float fX) {
	return ((fX < 0.0f) ? (-1) : (1)); 
}
