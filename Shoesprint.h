#pragma once
#include "SDL.h"
#include "Timer.h"
#include "Vector.h"

class CShoesprint {
public:
	//--------------------
	// Typy wyliczeniowe.
	//--------------------

	enum Type { left, right };  // Typ �ladu: po lewym bucie, po prawym bucie.
	enum Status { visible, disappear, invisible };  // Aktualny status �ladu: widoczny, zanika, niewidoczny.

	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	// Pierwszy argument b�dzie s�u�y� tylko do odczytu, st�d "const".
	CShoesprint(const CVector* vPos, float fAng, short iType);

	//---------
	// Metody.
	//---------

	void Update(const Uint32 iDeltaTicks);  // Metoda uaktualniaj�ca.

	void Show(void);  // Wy�wietlenie �ladu.

	// Czy �lad ju� znikn��.
	bool IsDead(void) {
		return (m_iStatus == invisible); // Je�eli status jest "niewidoczny", to w�a�ciwie �ladu po bucie ju� nie ma.
	}
private:
	//----------
	// Zmienne.
	//----------

	CVector m_vPos;  // Pozycja �ladu.

	// Wska�niki do odpowiednich obrazk�w sk�adaj�cych si� na �lad.
	SDL_Surface* m_pAppearanceSurf;  // Wygl�d "�wie�ego �ladu".
	SDL_Surface* m_apDisapAnimSurfs[4];  // Klatki animacji zanikania �ladu.

	// Prostok�ty opisuj�ce obrazki.
	SDL_Rect m_AppearanceRect;
	SDL_Rect m_aDisapAnimRects[4];

	CTimer m_LifeTimer;  // Timer odmierzaj�cy czas �ycia �ladu.

	float m_fLifeTime;  // Czas �ycia �ladu.
	float m_fAnimationVelocity;  // Pr�dko�� animacji �ladu podczas znikania.
	short m_iStatus;  // Status �ladu.
	float m_fFrame;  // Klatka do wy�wietlenia.
};
