#pragma once
#include "AppWindow.h"
#include "SDL.h"
#include <cassert>

#define g_AppWindow CAppWindow::GetSingleton()

class CProgressBar {
public:
	//--------------------
	// Zmienne statyczne.
	//--------------------

	static int ms_iPrgBarCount; // Licznik pask�w post�pu.

	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	CProgressBar(void);
	~CProgressBar(void);

	//---------
	// Metody.
	//---------

	void SetProgress(float fProgress);  // Aktualizuje pasek post�pu. Argumentem jest warto�� procentowa.
	void Reset(void) { m_ProgressRect.w = 0; }  // Resetuje pasek post�pu.
	void Show(void) {  // Wy�wietla pasek post�pu.
		SDL_FillRect(g_AppWindow.GetScreenSurf(), &m_BackgroundRect,
								 SDL_MapRGB(g_AppWindow.GetScreenSurf()->format, 0x39, 0x39, 0x39));
		SDL_FillRect(g_AppWindow.GetScreenSurf(), &m_ProgressRect,
								 SDL_MapRGB(g_AppWindow.GetScreenSurf()->format, 0xF5, 0xF5, 0xF5));
	}
private:
	SDL_Rect m_ProgressRect;  // Pasek post�pu.
	SDL_Rect m_BackgroundRect;  // T�o paska post�pu.
};
