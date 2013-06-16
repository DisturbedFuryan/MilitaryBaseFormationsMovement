#pragma once
#include "Grass.h"
#include "Singleton.h"

class CWorldArea : public Singleton<CWorldArea> {
public:
	//-------------
	// Singletony.
	//-------------

	CGrass Grass;

	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	CWorldArea(void);

	//-------------------
	// Metody dostêpowe.
	//-------------------

	int GetWidth(void) const { return m_iWidth; }
	int GetHeight(void) const { return m_iHeight; }

	//-------------------
	// Pozosta³e metody.
	//-------------------

	void Show(void) { Grass.Show(); }  // Wyœwietlenie obszaru œwiata ogranicza siê do wyœwietlenia trawy.
private:
	int m_iWidth, m_iHeight;  // Wymiary obszaru.
};
