#include "Shoesprint.h"
#include "singletons.h"

CShoesprint::CShoesprint(const CVector* vPos, float fAng, short iType) : m_fLifeTime(10000.0f), m_fFrame(0.0f), 
																																				 m_fAnimationVelocity(10.0f), m_iStatus(visible) {
	m_vPos = *vPos;
	int iAng = static_cast<int>(fAng);  // Rzutowanie k�ta na liczb� ca�kowit�.

	// Pobranie wska�nika do odpowiedniego obrazka przedstawiaj�cego �lad.
	m_pAppearanceSurf = g_KnightMgr.GetKnightShoesprintSurface(iType, iAng);

	// Znalezienie szeroko�ci i wysoko�ci obrazka.
	int iSurWidth = m_pAppearanceSurf->w;
	int iSurHeight = m_pAppearanceSurf->h;

	// Ustawienie prost�k�ta.
	m_AppearanceRect.x = (static_cast<int>(m_vPos.x) - (iSurWidth / 2));
	m_AppearanceRect.y = (static_cast<int>(m_vPos.y) - (iSurHeight / 2));
	m_AppearanceRect.w = iSurWidth;
	m_AppearanceRect.h = iSurHeight;

	// Za�adowanie obrazk�w przedstawiaj�cych znikanie oraz ustawienie prostok�t�w.
	for(int i = 0; i < 4; i++) {
		m_apDisapAnimSurfs[i] = g_KnightMgr.GetKnightShoesprintDisappearSurface(i, iType, iAng);

		// Znalezienie szeroko�ci i wysoko�ci obrazka.
		int iSurWidth = m_apDisapAnimSurfs[i]->w;
		int iSurHeight = m_apDisapAnimSurfs[i]->h;

		// Ustawienie prostok�ta.
		m_aDisapAnimRects[i].x = (static_cast<int>(m_vPos.x) - (iSurWidth / 2));
		m_aDisapAnimRects[i].y = (static_cast<int>(m_vPos.y) - (iSurHeight / 2));
		m_aDisapAnimRects[i].w = iSurWidth;
		m_aDisapAnimRects[i].h = iSurHeight;
	}

	m_LifeTimer.Start();  // Wystartowanie timera.
}

void CShoesprint::Update(const Uint32 iDeltaTicks) {
	if(m_iStatus != invisible) {  // Je�eli status �ladu jest r�ny od "niewidoczny".
		if(m_LifeTimer.GetTicks() > m_fLifeTime)  // Je�eli czas �ycia przeminie.
			m_iStatus = disappear;  // Ustawienie statusu na zanikanie.
		if(m_iStatus == disappear) {  // W przeciwnym przypadku, gdy powinna zosta� wy�wietlona animacja zanikania.
			m_fFrame += (m_fAnimationVelocity * (iDeltaTicks / 1000.0f));  // Uaktualnienie klatki.
			if(m_fFrame >= 4.0f)  // Je�eli klatka przekroczy�a warto�� 4, status zostaje zmieniony na "niewidoczny".
				m_iStatus = invisible;
		}
	}
}

void CShoesprint::Show(void) {
	if(m_iStatus != invisible) {  // Je�eli status �ladu jest r�ny od "niewidoczny".
		// Je�eli �lad jest widoczny, to zostaje wy�wietlony.
		if((m_iStatus == visible) && (g_CollisionController.CheckCollision(g_Camera.GetRect(), &m_AppearanceRect)))
			g_GraphicMgr.ApplySurface((m_AppearanceRect.x - g_Camera.GetX()), (m_AppearanceRect.y - g_Camera.GetY()), 
																m_pAppearanceSurf, g_AppWindow.GetScreenSurf());
		else if(m_iStatus == disappear) {  // W przeciwnym przypadku, gdy powinna zosta� wy�wietlona animacja zanikania.
			int iFrame = static_cast<int>(m_fFrame);
			if(g_CollisionController.CheckCollision(g_Camera.GetRect(), &m_aDisapAnimRects[iFrame]))
				g_GraphicMgr.ApplySurface((m_aDisapAnimRects[iFrame].x - g_Camera.GetX()), 
																	(m_aDisapAnimRects[iFrame].y - g_Camera.GetY()), 
																	m_apDisapAnimSurfs[iFrame], g_AppWindow.GetScreenSurf());
		}
	}
}
