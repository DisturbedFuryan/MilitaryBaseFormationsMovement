#pragma once
#include "SDL.h"
#include "Vector.h"

class IBacklight {
public:
	//--------------------
	// Typy wyliczeniowe.
	//--------------------

	enum Colour { yellow, blue, green };  // Kolor podœwietlenia.

	//---------
	// Metody.
	//---------

	virtual void Update(const Uint32 iDeltaTicks) = 0;  // Aktualizuje podœwietlenie. Metoda czysto wirtualna.
	virtual void Show(const CVector* pvPos) = 0;  // Wyœwietlenie podœwietlenia w podanej pozycji. Metoda czysto wirtualna.
};
