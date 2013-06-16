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
	// Metody dostêpowe.
	//-------------------

	SDL_Surface* GetGrassTileSurf() const { return m_pGrassTileSurf; }

	//-------------------
	// Pozosta³e metody.
	//-------------------

	void Show(void);  // Wyœwietla trawê.
private:
	//----------
	// Zmienne.
	//----------

	SDL_Surface* m_pGrassTileSurf;  // Powierzchnia skrawka trawy.
	CGrassTile* m_apGrassTiles;  // Tablica ze wskaŸnikami do skrawków trawy.
	int m_iNumTiles;  // Liczba skrawków.
	int m_iGrassTileWidth, m_iGrassTileHeight;  // Wymiary skrawka trawy.

	//---------
	// Metody.
	//---------

	int ComputeNumTiles(void);  // Liczy iloœæ skrawków trawy mieszcz¹cych siê w obszarze œwiata.
	void SetGrassTiles(void);  // Przypisuje ka¿demu skrawkowi trawy odpowiednie wymiary i po³o¿enie.
};
