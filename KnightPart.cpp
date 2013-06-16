#include "KnightPart.h"
#include "singletons.h"

void IKnightPart::Show(const CVector* pvPos, short iStatus, short iFrame, int iAng) {
	if(iStatus == IKnight::stand && m_apStandSurfaces[iAng]) {  // Kiedy rycerz stoi w miejscu.
		// Przypisanie odpowiedniej pozycji prostok¹towi opisuj¹cemu obrazek.
		m_aStandSurfRects[iAng].x = (static_cast<int>(pvPos->x) - (m_aStandSurfRects[iAng].w / 2));
		m_aStandSurfRects[iAng].y = (static_cast<int>(pvPos->y) - (m_aStandSurfRects[iAng].h / 2));

		if(g_CollisionController.CheckCollision(g_Camera.GetRect(), &m_aStandSurfRects[iAng]))
			g_GraphicMgr.ApplySurface((m_aStandSurfRects[iAng].x - g_Camera.GetX()),
																(m_aStandSurfRects[iAng].y - g_Camera.GetY()), 
																m_apStandSurfaces[iAng], g_AppWindow.GetScreenSurf());
	}
	else if(iStatus == IKnight::walk && m_apWalkSurfaces[iFrame][iAng]) {  // W wypadku, kiedy rycerz idzie.
		// Przypisanie odpowiedniej pozycji prostok¹towi opisuj¹cemu obrazek.
		m_aWalkSurfRects[iFrame][iAng].x = (static_cast<int>(pvPos->x) - (m_aWalkSurfRects[iFrame][iAng].w / 2));
		m_aWalkSurfRects[iFrame][iAng].y = (static_cast<int>(pvPos->y) - (m_aWalkSurfRects[iFrame][iAng].h / 2));

		if(g_CollisionController.CheckCollision(g_Camera.GetRect(), &m_aWalkSurfRects[iFrame][iAng]))
			g_GraphicMgr.ApplySurface((m_aWalkSurfRects[iFrame][iAng].x - g_Camera.GetX()),
																(m_aWalkSurfRects[iFrame][iAng].y - g_Camera.GetY()), 
																m_apWalkSurfaces[iFrame][iAng], g_AppWindow.GetScreenSurf());
	}
}
