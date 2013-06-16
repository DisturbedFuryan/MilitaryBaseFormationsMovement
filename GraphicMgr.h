#pragma once
#include "AppWindow.h"
#include "Image.h"
#include "ImageMgr.h"
#include "SDL.h"
#include "SDL_rotozoom.h"
#include <cassert>

class CGraphicMgr : public Singleton<CGraphicMgr> {
public:
	//-------------
	// Singletony.
	//-------------

	CAppWindow AppWindow;
	CImageMgr ImageMgr;

	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	CGraphicMgr(void);

	//---------
	// Metody.
	//---------

	// Blittinguje grafik�. Pobiera koordynaty, w jakie miejsce chcemy zblittingowa� grafik�, powierzchni�,
	// kt�ra b�dzie blittingowana oraz powierzchni�, na kt�r� b�dziemy t� pierwsz� blittingowa�.
	// Ostatni argument reprezentuje prostok�tny obszar, jaki b�dzie blittingowany z powierzchni pSource.
	// Je�li nie zostanie podany, blittingowana b�dzie ca�a powierzchnia pSource.
	void ApplySurface(int iX, int iY, SDL_Surface* pSourceSurf, SDL_Surface* pDestinationSurf, SDL_Rect* pClipRect = NULL);

	// Obraca grafik� o podany k�t (fAngle). Zwraca wska�nik do utworzonej obr�conej grafiki.
	SDL_Surface* RotateSurface(SDL_Surface* pSourceSurf, float fAngle, float fZoom = 1.0f, int iSmooth = 1);

	void Show(void);  // Renderuje ca�� scen�: rycerzy, palisady, traw�...
private:
	void ShowIntro(void);  // Wy�wietla intro - logo na czarnym tle.
};
