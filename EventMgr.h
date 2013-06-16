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
	// Metody dost�powe.
	//-------------------

	SDL_Event* GetEvent(void) { return &m_Event; }

	//-------------------
	// Pozosta�e metody.
	//-------------------

	void HandleEvents(void);  // Obs�uga zdarze� z urz�dze� wej�ciowych.
private:
	//----------
	// Zmienne.
	//----------

	SDL_Event m_Event;  // Struktura przechowuj�ca dane odno�nie zdarze�.
};
