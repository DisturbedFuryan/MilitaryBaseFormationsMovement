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

	void Update(const Uint32 iDeltaTicks);  // Aktualizuje k¹t podœwietlenia.
	void Show(const CVector* pvPos);  // Wyœwietlenie podœwietlenia w podanej pozycji.
private:
	//----------
	// Zmienne.
	//----------

	SDL_Surface* m_aBacklightSurfaces[360];  // Tablica wskaŸników do poobracanych powierzchni z podœwietleniem.
	SDL_Rect m_aBacklightSurfacesRects[360];  // Prostok¹ty opisuj¹ce powierzchnie z podœwietleniem.
	float m_fAng; // K¹t, pod jakim powinno byæ wyœwietlone podœwietlenie.
	float m_fAnimationVel;  // Szybkoœæ animacji/prêdkoœæ obrotu.
};
