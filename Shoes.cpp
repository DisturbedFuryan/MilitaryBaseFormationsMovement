#include "Shoes.h"
#include "singletons.h"

CShoes::CShoes(void) {
	// Za�adowanie wska�nik�w do obrazk�w do tablic.
	for(int i = 0; i < 360; i++) {
		m_apStandSurfaces[i] = NULL;  // Nie wida� but�w, kiedy rycerz stoi w miejscu.
	}
	for(int i = 0; i < 8; i++) {
		for(int j = 0; j < 360; j++) {
			m_apWalkSurfaces[i][j] = g_KnightMgr.GetKnightShoesSurface(i, j);

			// Okre�lenie szeroko�ci i wysoko�ci prostok�ta opisuj�cego obrazek.
			m_aWalkSurfRects[i][j].w = m_apWalkSurfaces[i][j]->w;
			m_aWalkSurfRects[i][j].h = m_apWalkSurfaces[i][j]->h;
		}
	}
}
