#include "EventMgr.h"
#include "singletons.h"

void CEventMgr::HandleEvents(void) {
	// Dop�ki zdarzenia s� "uchwytne".
	while(SDL_PollEvent(&m_Event)) {
		MouseController.UpdateMouseButtons();  // Aktualizacja danych na temat przycisk�w myszki.
		g_ObjectMgr.HandleInput();  // Obs�uga zdarze� obiekt�w, np. rycerzy.
		g_DebugWindow.HandleInput();  // Obs�uga zdarze� okna debugera.

		// Je�eli u�ytkownik wcisn�� Esc...
		if((m_Event.type == SDL_KEYDOWN) && (m_Event.key.keysym.sym == SDLK_ESCAPE))
			g_Application.SetIsQuit(true);  // ...to ko�czymy prac� programu.

		// Je�eli u�ytkownik wyrazi� ch�� zamkni�cia programu to...
		if(m_Event.type == SDL_QUIT)
			g_Application.SetIsQuit(true);  // ...spe�niamy j�.
	}
	MouseController.UpdateMousePosition();  // Aktualizacja pozycji, w jakiej znajduje si� kursor myszki.
}
