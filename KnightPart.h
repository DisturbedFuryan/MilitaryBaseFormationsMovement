#pragma once
#include "SDL.h"
#include "Vector.h"

class IKnightPart {
public:
	//---------
	// Metody.
	//---------

	void Show(const CVector* pvPos, short iStatus, short iFrame, int iAng);  // Wyœwietlenie czêœci rycerza na ekranie.
protected:
	//----------
	// Zmienne.
	//----------

	// Tablice wskaŸników do obrazków.
	SDL_Surface* m_apStandSurfaces[360];
	SDL_Surface* m_apWalkSurfaces[8][360];

	// Prostok¹ty opisuj¹ce obrazki.
	SDL_Rect m_aStandSurfRects[360];
	SDL_Rect m_aWalkSurfRects[8][360];
};
