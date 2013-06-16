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

	static int ms_iPrgBarCount; // Licznik pasków postêpu.

	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	CProgressBar(void);
	~CProgressBar(void);

	//---------
	// Metody.
	//---------

	void SetProgress(float fProgress);  // Aktualizuje pasek postêpu. Argumentem jest wartoœæ procentowa.
	void Reset(void) { m_ProgressRect.w = 0; }  // Resetuje pasek postêpu.
	void Show(void) {  // Wyœwietla pasek postêpu.
		SDL_FillRect(g_AppWindow.GetScreenSurf(), &m_BackgroundRect,
								 SDL_MapRGB(g_AppWindow.GetScreenSurf()->format, 0x39, 0x39, 0x39));
		SDL_FillRect(g_AppWindow.GetScreenSurf(), &m_ProgressRect,
								 SDL_MapRGB(g_AppWindow.GetScreenSurf()->format, 0xF5, 0xF5, 0xF5));
	}
private:
	SDL_Rect m_ProgressRect;  // Pasek postêpu.
	SDL_Rect m_BackgroundRect;  // T³o paska postêpu.
};
