#pragma once
#include "SDL.h"
#include "Vector.h"

class IBacklight {
public:
	//--------------------
	// Typy wyliczeniowe.
	//--------------------

	enum Colour { yellow, blue, green };  // Kolor pod�wietlenia.

	//---------
	// Metody.
	//---------

	virtual void Update(const Uint32 iDeltaTicks) = 0;  // Aktualizuje pod�wietlenie. Metoda czysto wirtualna.
	virtual void Show(const CVector* pvPos) = 0;  // Wy�wietlenie pod�wietlenia w podanej pozycji. Metoda czysto wirtualna.
};
