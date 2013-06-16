#include "Image.h"

CImage::CImage(std::string strPath, std::string strName) : m_strName(strName) {
	std::string strFileName = (strPath + m_strName + ".png");  // Pe³na nazwa pliku wraz ze œcie¿k¹.
	m_pSurface = Load(strFileName);  // Za³adowanie obrazka.
	if(!m_pSurface)
		assert(!"Failed to load image!");

	// Pobranie wymiarów obrazka.
	m_iWidth = m_pSurface->w;
	m_iHeight = m_pSurface->h;
}

CImage::~CImage(void) {
	SDL_FreeSurface(m_pSurface);  // Zwolnienie pamiêci zajmowanej przez powierzchniê obrazka.
}

SDL_Surface* CImage::Load(std::string strFileName) {
  SDL_Surface* pLoadedImgSurf = NULL;  // Pobrana powierzchnia.
	SDL_Surface* pOptimizedImgSurf = NULL;  // Zoptymalizowana powierzchnia.
	pLoadedImgSurf = IMG_Load(strFileName.c_str());  // Za³adowanie obrazka.
	if(pLoadedImgSurf) {  // Je¿eli obrazek zosta³ poprawnie za³adowany to...
		pOptimizedImgSurf = SDL_DisplayFormatAlpha(pLoadedImgSurf);  // ...optymalizujemy powierzchniê.
		SDL_FreeSurface(pLoadedImgSurf);  // Zwolnienie pamiêci zajmowanej przez niezoptymalizowan¹ powierzchniê.
	}
	return pOptimizedImgSurf;  // Zwrócenie zoptymalizowanej powierzchni.
}
