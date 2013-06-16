#include "SerfBacklight.h"
#include "singletons.h"

CSerfBacklight::CSerfBacklight(short iColour) {
	// Przypisanie wska�nikowi do powierzchni z pod�wietleniem odpowiedniej powierzchni.
	switch(iColour) {
		case yellow: m_pBacklightSurface = g_ImageMgr.GetImage("serf_backlight_yellow")->GetSurface(); break;
		case blue: m_pBacklightSurface = g_ImageMgr.GetImage("serf_backlight_blue")->GetSurface(); break;
		case green: m_pBacklightSurface = g_ImageMgr.GetImage("serf_backlight_green")->GetSurface();
	}

	// Okre�lenie szeroko�ci i wysoko�ci prostok�ta opisuj�cego powierzchni� z pod�wietleniem.
	m_BacklightSurfaceRect.w = m_pBacklightSurface->w;
	m_BacklightSurfaceRect.h = m_pBacklightSurface->h;
}

void CSerfBacklight::Show(const CVector* pvPos) {
	// Przypisanie odpowiedniej pozycji prostok�towi opisuj�cemu powierzchni� z pod�wietleniem.
	m_BacklightSurfaceRect.x = (static_cast<int>(pvPos->x) - (m_BacklightSurfaceRect.w / 2));
	m_BacklightSurfaceRect.y = (static_cast<int>(pvPos->y) - (m_BacklightSurfaceRect.h / 2));

	// Je�eli nast�pi kolizja z obszarem kamery, pod�wietlenie zostanie wy�wietlone.
	if(g_CollisionController.CheckCollision(&m_BacklightSurfaceRect, g_Camera.GetRect()))
		g_GraphicMgr.ApplySurface((m_BacklightSurfaceRect.x - g_Camera.GetX()),
															(m_BacklightSurfaceRect.y - g_Camera.GetY()), 
															m_pBacklightSurface, g_AppWindow.GetScreenSurf());
}
