#pragma once
#include "SDL.h"
#include "Singleton.h"

class CCamera : public Singleton<CCamera> {
public:
	//----------------------------
	// Konstruktory i destrktory.
	//----------------------------

	CCamera(int iX = 0, int iY = 0);

	//-------------------
	// Metody dostêpowe.
	//-------------------

	void SetX(int iX) { m_CameraRect.x = iX; }
	void SetY(int iY) { m_CameraRect.y = iY; }
	void Set(int iX, int iY) { m_CameraRect.x = iX; m_CameraRect.y = iY; }
	int GetX(void) const { return m_CameraRect.x; }
	int GetY(void) const { return m_CameraRect.y; }
	int GetWidth(void) const { return m_CameraRect.w; }
	int GetHeight(void) const { return m_CameraRect.h; }
	SDL_Rect* GetRect(void) { return &m_CameraRect; }
private:
	//----------
	// Zmienne.
	//----------

	SDL_Rect m_CameraRect;  // Prostok¹t opisuj¹cy kamerê.
};
