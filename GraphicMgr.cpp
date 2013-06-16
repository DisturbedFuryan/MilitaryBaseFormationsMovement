#include "GraphicMgr.h"
#include "singletons.h"

CGraphicMgr::CGraphicMgr(void) {
	ShowIntro();  // Wyœwietlenie intra.
}

void CGraphicMgr::ApplySurface(int iX, int iY, SDL_Surface* pSourceSurf, SDL_Surface* pDestinationSurf,
															 SDL_Rect* pClipRect) {
  // Utworzenie prostok¹ta o koordynatach podanych jako argumenty funkcji.
	// Jest to konieczne, poniewa¿ nie jest mo¿liwym podanie koordynatów funkcji SDL_BlitSurface() w inny sposób. 
	SDL_Rect OffsetRect;
	OffsetRect.x = iX;
	OffsetRect.y = iY;

	SDL_BlitSurface(pSourceSurf, pClipRect, pDestinationSurf, &OffsetRect);  // Zblittingowanie grafiki.
}

SDL_Surface* CGraphicMgr::RotateSurface(SDL_Surface* pSourceSurf, float fAngle, float fZoom, int iSmooth) {
	SDL_Surface* pRotatedSurf = NULL;  // WskaŸnik na obrócon¹ powierzchniê.
	if(pSourceSurf)  // Je¿eli zosta³a podana grafika do obrócenia, to tworzymy now¹ powierzchniê z obrócon¹ t¹ grafik¹.
		pRotatedSurf = rotozoomSurface(pSourceSurf, fAngle, fZoom, iSmooth);
	return pRotatedSurf;  // Zwrócenie wskaŸnika do utworzonej powierzchni.
}

void CGraphicMgr::Show(void) {
	g_WorldArea.Show();  // Rendering pod³o¿a.
	g_ObjectMgr.ShowObjects();  // Rendering obiektów: rycerzy i przeszkód.
	g_User.ShowTarget();  // Wyœwietlenie celu u¿ytkownika.
	g_DebugWindow.Show();  // Rendering okna debugera.
}

void CGraphicMgr::ShowIntro(void) {
	// Wype³nienie ekranu kolorem czarnym.
	SDL_FillRect(AppWindow.GetScreenSurf(), &AppWindow.GetScreenSurf()->clip_rect,
							 SDL_MapRGB(AppWindow.GetScreenSurf()->format, 0x00, 0x00, 0x00));

	// Pobranie powierzchni z logiem.
	CImage* pIntroImage = ImageMgr.GetImage("intro");
	if(!pIntroImage)
		assert(!"Failed to get image!");
	SDL_Surface* pIntroSurf = pIntroImage->GetSurface();

	// Zblittingowanie pobranej grafiki na œrodek ekranu.
	ApplySurface(((AppWindow.GetScreenWidth() - pIntroSurf->w) / 2), ((AppWindow.GetScreenHeight() - pIntroSurf->h) / 2),
							 pIntroSurf, AppWindow.GetScreenSurf());

	AppWindow.FlipScreen();  // Odœwie¿enie ekranu.
}	
