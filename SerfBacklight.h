#pragma once
#include "Backlight.h"
#include "SDL.h"

class CSerfBacklight : public IBacklight {
public:
	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	CSerfBacklight(short iColour);

	//---------
	// Metody.
	//---------

	void Update(const Uint32 iDeltaTicks) {}  // Pod�wielenie poddanego nie wymaga aktualizacji.
	void Show(const CVector* pvPos);  // Wy�wietlenie pod�wietlenia w podanej pozycji.
private:
	//----------
	// Zmienne.
	//----------

	SDL_Surface* m_pBacklightSurface;  // Wska�nik do powierzchni z pod�wietleniem.
	SDL_Rect m_BacklightSurfaceRect;  // Prostok�t opisuj�cy powierzchni� z pod�wietleniem.
};
