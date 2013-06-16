#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <cassert>
#include <string>

class CImage {
public:
	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	CImage(std::string strPath, std::string strName);
	~CImage(void);

	//-------------------
	// Metody dostêpowe.
	//-------------------

	SDL_Surface* GetSurface(void) const { return m_pSurface; }
	int GetWidth(void) const { return m_iWidth; }
	int GetHeight(void) const { return m_iHeight; }
	std::string GetName(void) const { return m_strName; }
private:
	//----------
	// Zmienne.
	//----------

	SDL_Surface* m_pSurface;  // Powierzchnia obrazka.
	int m_iWidth, m_iHeight;  // Wymiary obrazka.
	std::string m_strName;  // Nazwa obrazka.

	//---------
	// Metody.
	//---------

	SDL_Surface* Load(std::string strFileName);  // £aduje obrazek o podanej nazwie.
};
