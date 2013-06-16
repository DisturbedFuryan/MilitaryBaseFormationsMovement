#include "EventMgr.h"
#include "singletons.h"

void CEventMgr::HandleEvents(void) {
	// Dopóki zdarzenia s¹ "uchwytne".
	while(SDL_PollEvent(&m_Event)) {
		MouseController.UpdateMouseButtons();  // Aktualizacja danych na temat przycisków myszki.
		g_ObjectMgr.HandleInput();  // Obs³uga zdarzeñ obiektów, np. rycerzy.
		g_DebugWindow.HandleInput();  // Obs³uga zdarzeñ okna debugera.

		// Je¿eli u¿ytkownik wcisn¹³ Esc...
		if((m_Event.type == SDL_KEYDOWN) && (m_Event.key.keysym.sym == SDLK_ESCAPE))
			g_Application.SetIsQuit(true);  // ...to koñczymy pracê programu.

		// Je¿eli u¿ytkownik wyrazi³ chêæ zamkniêcia programu to...
		if(m_Event.type == SDL_QUIT)
			g_Application.SetIsQuit(true);  // ...spe³niamy j¹.
	}
	MouseController.UpdateMousePosition();  // Aktualizacja pozycji, w jakiej znajduje siê kursor myszki.
}
