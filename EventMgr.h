#pragma once
#include "MouseController.h"
#include "SDL.h"
#include "Singleton.h"

class CEventMgr : public Singleton<CEventMgr> {
public:
	//-------------
	// Singletony.
	//-------------

	CMouseController MouseController;

	//-------------------
	// Metody dostêpowe.
	//-------------------

	SDL_Event* GetEvent(void) { return &m_Event; }

	//-------------------
	// Pozosta³e metody.
	//-------------------

	void HandleEvents(void);  // Obs³uga zdarzeñ z urz¹dzeñ wejœciowych.
private:
	//----------
	// Zmienne.
	//----------

	SDL_Event m_Event;  // Struktura przechowuj¹ca dane odnoœnie zdarzeñ.
};
