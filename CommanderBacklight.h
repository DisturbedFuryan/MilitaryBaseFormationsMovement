#pragma once
#include "Backlight.h"
#include "SDL.h"

class CCommanderBacklight : public IBacklight {
public:
	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	CCommanderBacklight(short iColour);
	
	//---------
	// Metody.
	//---------

	void Update(const Uint32 iDeltaTicks);  // Aktualizuje k�t pod�wietlenia.
	void Show(const CVector* pvPos);  // Wy�wietlenie pod�wietlenia w podanej pozycji.
private:
	//----------
	// Zmienne.
	//----------

	SDL_Surface* m_aBacklightSurfaces[360];  // Tablica wska�nik�w do poobracanych powierzchni z pod�wietleniem.
	SDL_Rect m_aBacklightSurfacesRects[360];  // Prostok�ty opisuj�ce powierzchnie z pod�wietleniem.
	float m_fAng; // K�t, pod jakim powinno by� wy�wietlone pod�wietlenie.
	float m_fAnimationVel;  // Szybko�� animacji/pr�dko�� obrotu.
};
