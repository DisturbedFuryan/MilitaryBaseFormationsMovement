#pragma once
#include "SDL.h"
#include "Singleton.h"

class CMouseController : public Singleton<CMouseController> {
public:
	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	CMouseController(void);

	//----------------------------
	// Metody aktualizuj¹ce dane.
	//----------------------------

	void UpdateMousePosition(void) { SDL_GetMouseState(&m_iX, &m_iY); }  // Pobiera po³o¿enie kursora.
	void UpdateMouseButtons(void);  // Aktualizuje informacje odnoœnie przycisków myszki.

	//-------------------
	// Metody dostêpowe.
	//-------------------

	bool GetIsLeftButDown(void) const { return m_bIsLeftButDown; }
	bool GetIsRightButDown(void) const { return m_bIsRightButDown; }
	int GetX(void) const { return m_iX; }
	int GetY(void) const { return m_iY; }
private:
	//----------
	// Zmienne.
	//----------

	int m_iX, m_iY;  // Wspó³rzêdne kursora.
	bool m_bIsLeftButDown, m_bIsRightButDown;  // Czy lewy/prawy przycisk myszki jest wciœniêty.
};
