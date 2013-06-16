#include "Sword.h"
#include "singletons.h"

CSword::CSword(void) {
	// Za³adowanie wskaŸników do obrazków do tablic.
	for(int i = 0; i < 360; i++) {
		m_apStandSurfaces[i] = g_KnightMgr.GetKnightSwordStandSurface(i);

		// Okreœlenie szerokoœci i wysokoœci prostok¹ta opisuj¹cego obrazek.
		m_aStandSurfRects[i].w = m_apStandSurfaces[i]->w;
		m_aStandSurfRects[i].h = m_apStandSurfaces[i]->h;
	}
	for(int i = 0; i < 8; i++) {
		for(int j = 0; j < 360; j++) {
			m_apWalkSurfaces[i][j] = g_KnightMgr.GetKnightSwordWalkSurface(i, j);

			// Okreœlenie szerokoœci i wysokoœci prostok¹ta opisuj¹cego obrazek.
			m_aWalkSurfRects[i][j].w = m_apWalkSurfaces[i][j]->w;
			m_aWalkSurfRects[i][j].h = m_apWalkSurfaces[i][j]->h;
		}
	}
}
