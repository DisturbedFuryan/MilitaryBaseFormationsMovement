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

	// Blittinguje grafikê. Pobiera koordynaty, w jakie miejsce chcemy zblittingowaæ grafikê, powierzchniê,
	// która bêdzie blittingowana oraz powierzchniê, na któr¹ bêdziemy t¹ pierwsz¹ blittingowaæ.
	// Ostatni argument reprezentuje prostok¹tny obszar, jaki bêdzie blittingowany z powierzchni pSource.
	// Jeœli nie zostanie podany, blittingowana bêdzie ca³a powierzchnia pSource.
	void ApplySurface(int iX, int iY, SDL_Surface* pSourceSurf, SDL_Surface* pDestinationSurf, SDL_Rect* pClipRect = NULL);

	// Obraca grafikê o podany k¹t (fAngle). Zwraca wskaŸnik do utworzonej obróconej grafiki.
	SDL_Surface* RotateSurface(SDL_Surface* pSourceSurf, float fAngle, float fZoom = 1.0f, int iSmooth = 1);

	void Show(void);  // Renderuje ca³¹ scenê: rycerzy, palisady, trawê...
private:
	void ShowIntro(void);  // Wyœwietla intro - logo na czarnym tle.
};
