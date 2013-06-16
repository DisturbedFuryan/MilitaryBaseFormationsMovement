#pragma once
#include "SDL.h"

class CTimer {
public:
	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	CTimer(void);

	//------------------------
	// Kontrola pracy timera.
	//------------------------

	// Rozpoczyna odliczanie czasu.
	void Start(void) {
		m_bIsStarted = true;  // Rozpoczêcie odliczania.
		m_bIsPaused = false;  // Odblokowanie timera.
		m_iStartTicks = SDL_GetTicks();  // Pobranie aktualnego czasu przy u¿yciu SDL_GetTicks().
	}

	void Stop(void) { m_bIsStarted = m_bIsPaused = false; }  // Zatrzymuje odmierzanie czasu.

	// Wstrzymuje odliczanie.
	void Pause(void) { 
		if(m_bIsStarted && !m_bIsPaused) {  // Je¿eli timer jest uruchomiony i nie jest wstrzymany.
			m_bIsPaused = true;  // Wstrzymanie timera.
			m_iPausedTicks = (SDL_GetTicks() - m_iStartTicks);  // Obliczenie czasu, jaki min¹³ od pocz¹tku odliczania.
		}
	}

	// Wznawia odliczanie.
	void Unpause(void) {
		if(m_bIsPaused) {  // Je¿eli timer jest zatrzymany...
			m_bIsPaused = false;  // ...to odliczanie zostaje wznowione.

			// Aktualizacja punktu startowego. Od aktualnego czasu zostaje odjêty odmierzony czas, kiedy timer siê zatrzyma³.
			m_iStartTicks = (SDL_GetTicks() - m_iPausedTicks);

			m_iPausedTicks = 0;
		}
	}

	//-------------------
	// Metody dostêpowe.
	//-------------------

	bool GetIsStarted(void) const { return m_bIsStarted; }
	bool GetIsPaused(void) const { return m_bIsPaused; }

	//-------------------
	// Pozosta³e metody.
	//-------------------

	// Zwraca czas w milisekundach.
	int GetTicks(void) {
		if(m_bIsStarted)  // Je¿eli timer jest uruchomiony.
			// ...to zwracamy czas, jaki pokazywa³ timer w trakcie wstrzymania jego pracy,
			// lub czas, jaki up³yn¹³ do tej pory, jeœli timer nie jest wstrzymany.
			return (m_bIsPaused ? m_iPausedTicks : (SDL_GetTicks() - m_iStartTicks));
		return 0;
	}
private:
	//----------
	// Zmienne.
	//----------

	int m_iStartTicks;  // Punkt startowy timera.
	int m_iPausedTicks;  // Odmierzony czas, kiedy timer zosta³ zatrzymany.
	bool m_bIsPaused, m_bIsStarted;  // Status timera.
};
