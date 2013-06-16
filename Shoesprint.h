#pragma once
#include "SDL.h"
#include "Timer.h"
#include "Vector.h"

class CShoesprint {
public:
	//--------------------
	// Typy wyliczeniowe.
	//--------------------

	enum Type { left, right };  // Typ œladu: po lewym bucie, po prawym bucie.
	enum Status { visible, disappear, invisible };  // Aktualny status œladu: widoczny, zanika, niewidoczny.

	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	// Pierwszy argument bêdzie s³u¿y³ tylko do odczytu, st¹d "const".
	CShoesprint(const CVector* vPos, float fAng, short iType);

	//---------
	// Metody.
	//---------

	void Update(const Uint32 iDeltaTicks);  // Metoda uaktualniaj¹ca.

	void Show(void);  // Wyœwietlenie œladu.

	// Czy œlad ju¿ znikn¹³.
	bool IsDead(void) {
		return (m_iStatus == invisible); // Je¿eli status jest "niewidoczny", to w³aœciwie œladu po bucie ju¿ nie ma.
	}
private:
	//----------
	// Zmienne.
	//----------

	CVector m_vPos;  // Pozycja œladu.

	// WskaŸniki do odpowiednich obrazków sk³adaj¹cych siê na œlad.
	SDL_Surface* m_pAppearanceSurf;  // Wygl¹d "œwie¿ego œladu".
	SDL_Surface* m_apDisapAnimSurfs[4];  // Klatki animacji zanikania œladu.

	// Prostok¹ty opisuj¹ce obrazki.
	SDL_Rect m_AppearanceRect;
	SDL_Rect m_aDisapAnimRects[4];

	CTimer m_LifeTimer;  // Timer odmierzaj¹cy czas ¿ycia œladu.

	float m_fLifeTime;  // Czas ¿ycia œladu.
	float m_fAnimationVelocity;  // Prêdkoœæ animacji œladu podczas znikania.
	short m_iStatus;  // Status œladu.
	float m_fFrame;  // Klatka do wyœwietlenia.
};
