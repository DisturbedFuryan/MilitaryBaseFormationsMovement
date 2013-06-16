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
	// Metody dost�powe.
	//-------------------

	int GetWidth(void) const { return m_iWidth; }
	int GetHeight(void) const { return m_iHeight; }

	//-------------------
	// Pozosta�e metody.
	//-------------------

	void Show(void) { Grass.Show(); }  // Wy�wietlenie obszaru �wiata ogranicza si� do wy�wietlenia trawy.
private:
	int m_iWidth, m_iHeight;  // Wymiary obszaru.
};
