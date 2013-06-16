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

	void Update(const Uint32 iDeltaTicks) {}  // Podœwielenie poddanego nie wymaga aktualizacji.
	void Show(const CVector* pvPos);  // Wyœwietlenie podœwietlenia w podanej pozycji.
private:
	//----------
	// Zmienne.
	//----------

	SDL_Surface* m_pBacklightSurface;  // WskaŸnik do powierzchni z podœwietleniem.
	SDL_Rect m_BacklightSurfaceRect;  // Prostok¹t opisuj¹cy powierzchniê z podœwietleniem.
};
