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
	// Wyœwietlanie komunikatów.
	//---------------------------
	
	// Rozwi¹zanie odnoœnie wyœwietlania komunikatów jest trywialne, ale wystarczaj¹ce do podstawowych zastosowañ.
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
	// Pozosta³e metody.
	//-------------------

	void HandleInput(void);  // Obs³uga urz¹dzeñ wejœciowych.
	void Show(void);  // Wyœwietlenie okna debugera.
private:
	//----------
	// Zmienne.
	//----------

	TTF_Font* m_pFont;  // Czcionka, która zostanie u¿yta.
	SDL_Color m_TextColor;  // Kolor czcionki.
	SDL_Rect m_WindowRect;  // Okno debugera. 
	SDL_Rect m_WindowFooterRect;  // Stopka okna debugera.
	std::vector<SDL_Surface*> m_MessagesContainer;  // Kontener komunikatów.
	std::vector<SDL_Surface*>::reverse_iterator LastMsgRIt;  // Iterator odwrotny wskazuj¹cy ostatni komunikat.

	// Iterator odwrotny wskazuj¹cy ostatni¹ widoczn¹ wiadomoœæ, patrz¹c od góry.
	std::vector<SDL_Surface*>::reverse_iterator LastVisMsgRIt;

	bool m_bIsVisible;  // Czy okno jest widoczne.
	int m_iMaxMsgs; // Maksymalna liczba zapamiêtanych komunikatów.
	int m_iMsgHeight;  // Wysokoœæ pojedynczej powierzchni z komunikatem.
	bool m_bIsPermToGoUp;  // Czy jest pozwolenie na przewijanie komunikatów w górê.
	std::string m_strSpace;  // Spacja bêd¹ca na pocz¹tku ka¿dego komunikatu.

	//---------
	// Metody.
	//---------

	void AddToContainer(SDL_Surface* pMessageSurf);  // Dodaje podany komunikat do kontenera.
};
