#include "Body.h"
#include "singletons.h"

CBody::CBody(void) {
	// Załadowanie wskaźników do obrazków do tablic.
	for(int i = 0; i < 360; i++) {
		m_apStandSurfaces[i] = g_KnightMgr.GetKnightBodyStandSurface(i);

		// Określenie szerokości i wysokości prostokąta opisującego obrazek.
		m_aStandSurfRects[i].w = m_apStandSurfaces[i]->w;
		m_aStandSurfRects[i].h = m_apStandSurfaces[i]->h;
	}
	for(int i = 0; i < 8; i++) {
		for(int j = 0; j < 360; j++) {
			m_apWalkSurfaces[i][j] = g_KnightMgr.GetKnightBodyWalkSurface(i, j);

			// Określenie szerokości i wysokości prostokąta opisującego obrazek.
			m_aWalkSurfRects[i][j].w = m_apWalkSurfaces[i][j]->w;
			m_aWalkSurfRects[i][j].h = m_apWalkSurfaces[i][j]->h;
		}
	}
}
