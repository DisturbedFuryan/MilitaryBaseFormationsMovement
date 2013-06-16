#pragma once
#include "AppWindow.h"
#include "Camera.h"
#include "CollisionController.h"
#include "GraphicMgr.h"
#include "SDL.h"

#define g_AppWindow CAppWindow::GetSingleton()
#define g_Camera CCamera::GetSingleton()
#define g_CollisionController CCollisionController::GetSingleton()
#define g_GraphicMgr CGraphicMgr::GetSingleton()

class CGrassTile {
public:
	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	CGrassTile(void);

	//-------------------
	// Metody dost�powe.
	//-------------------

	void SetRect(int iX, int iY, int iWidth, int iHeight)
		{ m_Rect.x = iX; m_Rect.y = iY; m_Rect.w = iWidth; m_Rect.h = iHeight; } 

	//-------------------
	// Pozosta�e metody.
	//-------------------

	// Wy�wietlenie skrawka trawy.
	void Show(void) {
		// Je�eli skrawek trawy nie jest widoczny przez kamer�, to nie ma potrzeby go wy�wietla�.
		if(g_CollisionController.CheckCollision(&m_Rect, g_Camera.GetRect()))
			g_GraphicMgr.ApplySurface((m_Rect.x - g_Camera.GetX()), (m_Rect.y - g_Camera.GetY()),
																m_pSurface, g_AppWindow.GetScreenSurf());
	}
private:
	//----------
	// Zmienne.
	//----------

	SDL_Surface* m_pSurface;  // Obrazek skrawka trawy.
	SDL_Rect m_Rect;  // Prostok�t opisuj�cy skrawek trawy.
};
