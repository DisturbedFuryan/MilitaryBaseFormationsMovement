#include "AppWindow.h"
#include "singletons.h"

CAppWindow::CAppWindow(void) : m_pScreenSurf(NULL), m_iScreenWidth(g_DefaultsMgr.GetScreenWidth()),
															 m_iScreenHeight(g_DefaultsMgr.GetScreenHeight()), m_iScreenBPP(32) {
	m_pScreenSurf = SDL_SetVideoMode(m_iScreenWidth, m_iScreenHeight, m_iScreenBPP, SDL_SWSURFACE);  // Ustawienie ekranu.
	if(!m_pScreenSurf)  // W przypadku niepowodzenia.
		assert(!"Failed to set up the screen!");
	SetCaption("Movement Formation");  // Ustawienie nag³ówka.
}
