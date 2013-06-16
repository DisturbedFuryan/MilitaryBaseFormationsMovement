#pragma once
#include "GrassTile.h"
#include "Image.h"
#include "SDL.h"
#include "Singleton.h"
#include <cassert>

class CGrass : public Singleton<CGrass> {
public:
	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	CGrass(void);
	~CGrass(void);

	//-------------------
	// Metody dost�powe.
	//-------------------

	SDL_Surface* GetGrassTileSurf() const { return m_pGrassTileSurf; }

	//-------------------
	// Pozosta�e metody.
	//-------------------

	void Show(void);  // Wy�wietla traw�.
private:
	//----------
	// Zmienne.
	//----------

	SDL_Surface* m_pGrassTileSurf;  // Powierzchnia skrawka trawy.
	CGrassTile* m_apGrassTiles;  // Tablica ze wska�nikami do skrawk�w trawy.
	int m_iNumTiles;  // Liczba skrawk�w.
	int m_iGrassTileWidth, m_iGrassTileHeight;  // Wymiary skrawka trawy.

	//---------
	// Metody.
	//---------

	int ComputeNumTiles(void);  // Liczy ilo�� skrawk�w trawy mieszcz�cych si� w obszarze �wiata.
	void SetGrassTiles(void);  // Przypisuje ka�demu skrawkowi trawy odpowiednie wymiary i po�o�enie.
};
