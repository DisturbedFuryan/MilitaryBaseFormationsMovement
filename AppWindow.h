#pragma once
#include "DebugWindow.h"
#include "SDL.h"
#include "Singleton.h"
#include <cassert>
#include <string>

class CAppWindow : public Singleton<CAppWindow> {
public:
	//-------------
	// Singletony.
	//-------------

	CDebugWindow DebugWindow;

	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	CAppWindow(void);

	//---------
	// Metody.
	//---------
	void FlipScreen(void) { if(SDL_Flip(m_pScreenSurf) == -1) assert(!"Failed to flip screen!"); }  // Od�wie�a ekran.
	void SetCaption(std::string strCaption) { SDL_WM_SetCaption(strCaption.c_str(), NULL); }  // Ustawia nag��wek.

	//-------------------
	// Metody dost�powe.
	//-------------------

	SDL_Surface* GetScreenSurf(void) const { return m_pScreenSurf; }
	int GetScreenWidth(void) const { return m_iScreenWidth; }
	int GetScreenHeight(void) const { return m_iScreenHeight; }
	int GetScreenBPP(void) const { return m_iScreenBPP; }
private:
	//----------
	// Zmienne.
	//----------

	SDL_Surface* m_pScreenSurf;  // Powierzchnia reprezentuj�ca ekran.
	int m_iScreenWidth, m_iScreenHeight, m_iScreenBPP;  // Szeroko��, wysoko�� i g��bia kolor�w ekranu.
};
