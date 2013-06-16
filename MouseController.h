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
	// Metody aktualizuj�ce dane.
	//----------------------------

	void UpdateMousePosition(void) { SDL_GetMouseState(&m_iX, &m_iY); }  // Pobiera po�o�enie kursora.
	void UpdateMouseButtons(void);  // Aktualizuje informacje odno�nie przycisk�w myszki.

	//-------------------
	// Metody dost�powe.
	//-------------------

	bool GetIsLeftButDown(void) const { return m_bIsLeftButDown; }
	bool GetIsRightButDown(void) const { return m_bIsRightButDown; }
	int GetX(void) const { return m_iX; }
	int GetY(void) const { return m_iY; }
private:
	//----------
	// Zmienne.
	//----------

	int m_iX, m_iY;  // Wsp�rz�dne kursora.
	bool m_bIsLeftButDown, m_bIsRightButDown;  // Czy lewy/prawy przycisk myszki jest wci�ni�ty.
};
