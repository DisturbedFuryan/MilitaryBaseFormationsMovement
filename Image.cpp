#include "Image.h"

CImage::CImage(std::string strPath, std::string strName) : m_strName(strName) {
	std::string strFileName = (strPath + m_strName + ".png");  // Pe�na nazwa pliku wraz ze �cie�k�.
	m_pSurface = Load(strFileName);  // Za�adowanie obrazka.
	if(!m_pSurface)
		assert(!"Failed to load image!");

	// Pobranie wymiar�w obrazka.
	m_iWidth = m_pSurface->w;
	m_iHeight = m_pSurface->h;
}

CImage::~CImage(void) {
	SDL_FreeSurface(m_pSurface);  // Zwolnienie pami�ci zajmowanej przez powierzchni� obrazka.
}

SDL_Surface* CImage::Load(std::string strFileName) {
  SDL_Surface* pLoadedImgSurf = NULL;  // Pobrana powierzchnia.
	SDL_Surface* pOptimizedImgSurf = NULL;  // Zoptymalizowana powierzchnia.
	pLoadedImgSurf = IMG_Load(strFileName.c_str());  // Za�adowanie obrazka.
	if(pLoadedImgSurf) {  // Je�eli obrazek zosta� poprawnie za�adowany to...
		pOptimizedImgSurf = SDL_DisplayFormatAlpha(pLoadedImgSurf);  // ...optymalizujemy powierzchni�.
		SDL_FreeSurface(pLoadedImgSurf);  // Zwolnienie pami�ci zajmowanej przez niezoptymalizowan� powierzchni�.
	}
	return pOptimizedImgSurf;  // Zwr�cenie zoptymalizowanej powierzchni.
}
