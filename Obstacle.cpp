#include "Obstacle.h"
#include "singletons.h"

CObstacle::CObstacle(int iId, CVector* vPos, short iType) : m_iId(iId), m_iType(iType) {
	m_vPos = *vPos;  // Przypisanie podanej pozycji przeszkodzie.
	switch(m_iType) {
		case palisade: m_CollisionCircle.r = g_DefaultsMgr.GetPalisadeRadius();
									 m_pObstacleSurface = g_ImageMgr.GetImage("palisade")->GetSurface(); break;
		case stone: m_CollisionCircle.r = g_DefaultsMgr.GetStoneRadius();
							  m_pObstacleSurface = g_ImageMgr.GetImage("stone")->GetSurface();
	}
	ShiftCollisionCircle();  // Synchronizacja ko�a kolizji z pozycj�.

	// Przypisanie szeroko�ci i wysoko�ci prostok�towi opisuj�cemu powierzchni�.
	m_SurfaceRect.w = m_pObstacleSurface->w;
	m_SurfaceRect.h = m_pObstacleSurface->h;
}

void CObstacle::Show(void) {
	// Przypisanie odpowiedniej pozycji prostok�towi opisuj�cemu powierzchni�.
	m_SurfaceRect.x = (static_cast<int>(m_vPos.x) - (m_SurfaceRect.w / 2));
	m_SurfaceRect.y = (static_cast<int>(m_vPos.y) - (m_SurfaceRect.h / 2));

	// Je�eli powierzchnia przeszkody znajdzie si� w obszarze kamery, to zostanie wy�wietlona.
	if(g_CollisionController.CheckCollision(&m_SurfaceRect, g_Camera.GetRect()))
		g_GraphicMgr.ApplySurface((m_SurfaceRect.x - g_Camera.GetX()), (m_SurfaceRect.y - g_Camera.GetY()),
															m_pObstacleSurface, g_AppWindow.GetScreenSurf());
}
