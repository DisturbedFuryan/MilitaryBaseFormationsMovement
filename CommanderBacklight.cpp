#include "CommanderBacklight.h"
#include "singletons.h"

CCommanderBacklight::CCommanderBacklight(short iColour) : m_fAng(0.0f), m_fAnimationVel(20.0f) {
	// Przypisanie odpowiednich powierzchni z podœwietleniem wskaŸnikom w tablicy.
	for(int i = 0; i < 360; i++) {
		m_aBacklightSurfaces[i] = g_KnightMgr.GetCommanderBacklightSurface(iColour, i);

		// Okreœlenie szerokoœci i wysokoœci prostok¹ta opisuj¹cego powierzchniê z podœwietleniem.
		m_aBacklightSurfacesRects[i].w = m_aBacklightSurfaces[i]->w;
		m_aBacklightSurfacesRects[i].h = m_aBacklightSurfaces[i]->h;
	}
}

void CCommanderBacklight::Update(const Uint32 iDeltaTicks) {
	m_fAng += (m_fAnimationVel * (iDeltaTicks / 1000.0f));  // Uaktualnienie k¹ta.
	if(m_fAng >= 360.0f)  // Zapêtlenie, k¹t nie mo¿e przekroczyæ 360 stopni.
		m_fAng = 0.0f;
}

void CCommanderBacklight::Show(const CVector* pvPos) {
	int iAng = static_cast<int>(m_fAng);  // Rzutowanie k¹ta na typ ca³kowity.

	// Przypisanie odpowiedniej pozycji prostok¹towi opisuj¹cemu powierzchniê z podœwietleniem.
	m_aBacklightSurfacesRects[iAng].x = (static_cast<int>(pvPos->x) - (m_aBacklightSurfacesRects[iAng].w / 2));
	m_aBacklightSurfacesRects[iAng].y = (static_cast<int>(pvPos->y) - (m_aBacklightSurfacesRects[iAng].h / 2));

	// Je¿eli nast¹pi kolizja z obszarem kamery, podœwietlenie zostanie wyœwietlone.
	if(g_CollisionController.CheckCollision(&m_aBacklightSurfacesRects[iAng], g_Camera.GetRect()))
		g_GraphicMgr.ApplySurface((m_aBacklightSurfacesRects[iAng].x - g_Camera.GetX()),
															(m_aBacklightSurfacesRects[iAng].y - g_Camera.GetY()),
															m_aBacklightSurfaces[iAng], g_AppWindow.GetScreenSurf());
}
