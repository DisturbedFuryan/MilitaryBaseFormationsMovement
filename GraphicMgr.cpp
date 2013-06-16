#include "GraphicMgr.h"
#include "singletons.h"

CGraphicMgr::CGraphicMgr(void) {
	ShowIntro();  // Wy�wietlenie intra.
}

void CGraphicMgr::ApplySurface(int iX, int iY, SDL_Surface* pSourceSurf, SDL_Surface* pDestinationSurf,
															 SDL_Rect* pClipRect) {
  // Utworzenie prostok�ta o koordynatach podanych jako argumenty funkcji.
	// Jest to konieczne, poniewa� nie jest mo�liwym podanie koordynat�w funkcji SDL_BlitSurface() w inny spos�b. 
	SDL_Rect OffsetRect;
	OffsetRect.x = iX;
	OffsetRect.y = iY;

	SDL_BlitSurface(pSourceSurf, pClipRect, pDestinationSurf, &OffsetRect);  // Zblittingowanie grafiki.
}

SDL_Surface* CGraphicMgr::RotateSurface(SDL_Surface* pSourceSurf, float fAngle, float fZoom, int iSmooth) {
	SDL_Surface* pRotatedSurf = NULL;  // Wska�nik na obr�con� powierzchni�.
	if(pSourceSurf)  // Je�eli zosta�a podana grafika do obr�cenia, to tworzymy now� powierzchni� z obr�con� t� grafik�.
		pRotatedSurf = rotozoomSurface(pSourceSurf, fAngle, fZoom, iSmooth);
	return pRotatedSurf;  // Zwr�cenie wska�nika do utworzonej powierzchni.
}

void CGraphicMgr::Show(void) {
	g_WorldArea.Show();  // Rendering pod�o�a.
	g_ObjectMgr.ShowObjects();  // Rendering obiekt�w: rycerzy i przeszk�d.
	g_User.ShowTarget();  // Wy�wietlenie celu u�ytkownika.
	g_DebugWindow.Show();  // Rendering okna debugera.
}

void CGraphicMgr::ShowIntro(void) {
	// Wype�nienie ekranu kolorem czarnym.
	SDL_FillRect(AppWindow.GetScreenSurf(), &AppWindow.GetScreenSurf()->clip_rect,
							 SDL_MapRGB(AppWindow.GetScreenSurf()->format, 0x00, 0x00, 0x00));

	// Pobranie powierzchni z logiem.
	CImage* pIntroImage = ImageMgr.GetImage("intro");
	if(!pIntroImage)
		assert(!"Failed to get image!");
	SDL_Surface* pIntroSurf = pIntroImage->GetSurface();

	// Zblittingowanie pobranej grafiki na �rodek ekranu.
	ApplySurface(((AppWindow.GetScreenWidth() - pIntroSurf->w) / 2), ((AppWindow.GetScreenHeight() - pIntroSurf->h) / 2),
							 pIntroSurf, AppWindow.GetScreenSurf());

	AppWindow.FlipScreen();  // Od�wie�enie ekranu.
}	
