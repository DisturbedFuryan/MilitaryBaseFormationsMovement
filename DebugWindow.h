#pragma once
#include "SDL.h"
#include "SDL_ttf.h"
#include "Singleton.h"
#include <cassert>
#include <sstream>
#include <string>
#include <vector>

class CDebugWindow : public Singleton<CDebugWindow> {
public:
	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	CDebugWindow(void);
	~CDebugWindow(void);

	//---------------------------
	// Wy�wietlanie komunikat�w.
	//---------------------------
	
	// Rozwi�zanie odno�nie wy�wietlania komunikat�w jest trywialne, ale wystarczaj�ce do podstawowych zastosowa�.
	void Print(std::string strMessage);
	void Print(std::string strFirstMsg, int iVal, std::string strSecondMsg = "");
	void Print(std::string strFirstMsg, float fVal, std::string strSecondMsg = "");
	void Print(std::string strFirstMsg, int iFirstVal, std::string strSecondMsg,
						 int iSecondVal, std::string strThirdMsg = "");
	void Print(std::string strFirstMsg, float fFirstVal, std::string strSecondMsg,
						 float fSecondVal, std::string strThirdMsg = "");
	void Print(std::string strFirstMsg, int iFirstVal, std::string strSecondMsg,
						 float fSecondVal, std::string strThirdMsg = "");
	void Print(std::string strFirstMsg, float fFirstVal, std::string strSecondMsg,
						 int iSecondVal, std::string strThirdMsg = "");

	//-------------------
	// Pozosta�e metody.
	//-------------------

	void HandleInput(void);  // Obs�uga urz�dze� wej�ciowych.
	void Show(void);  // Wy�wietlenie okna debugera.
private:
	//----------
	// Zmienne.
	//----------

	TTF_Font* m_pFont;  // Czcionka, kt�ra zostanie u�yta.
	SDL_Color m_TextColor;  // Kolor czcionki.
	SDL_Rect m_WindowRect;  // Okno debugera. 
	SDL_Rect m_WindowFooterRect;  // Stopka okna debugera.
	std::vector<SDL_Surface*> m_MessagesContainer;  // Kontener komunikat�w.
	std::vector<SDL_Surface*>::reverse_iterator LastMsgRIt;  // Iterator odwrotny wskazuj�cy ostatni komunikat.

	// Iterator odwrotny wskazuj�cy ostatni� widoczn� wiadomo��, patrz�c od g�ry.
	std::vector<SDL_Surface*>::reverse_iterator LastVisMsgRIt;

	bool m_bIsVisible;  // Czy okno jest widoczne.
	int m_iMaxMsgs; // Maksymalna liczba zapami�tanych komunikat�w.
	int m_iMsgHeight;  // Wysoko�� pojedynczej powierzchni z komunikatem.
	bool m_bIsPermToGoUp;  // Czy jest pozwolenie na przewijanie komunikat�w w g�r�.
	std::string m_strSpace;  // Spacja b�d�ca na pocz�tku ka�dego komunikatu.

	//---------
	// Metody.
	//---------

	void AddToContainer(SDL_Surface* pMessageSurf);  // Dodaje podany komunikat do kontenera.
};
