#include "DebugWindow.h"
#include "singletons.h"

CDebugWindow::CDebugWindow(void) : m_pFont(NULL), m_bIsVisible(false), m_iMaxMsgs(1000), m_bIsPermToGoUp(false),
																   m_strSpace("> ") {
	if(TTF_Init() == -1)  // Inicjalizacja SDL_ttf.
		assert(!"Failed to initialize SDL_ttf!");

	m_pFont = TTF_OpenFont("Fixedsys500c.ttf", 14);  // Otwarcie czcionki.
	if(!m_pFont)
		assert(!"Failed to open the font!");

	// Ustalenie koloru czcionki; kolor bia�y.
	m_TextColor.r = 255;
	m_TextColor.g = 255;
	m_TextColor.b = 255;

	// Stworzenie pierwszego komunikatu w celu odczytania wysoko�ci pojedynczego komunikatu.
	SDL_Surface* pFirstMsgSurf = TTF_RenderText_Solid(m_pFont, "Movement Formation by Marcin Rainka, v. 1.0", m_TextColor);
	m_iMsgHeight = pFirstMsgSurf->h;  // Wykorzystanie stworzonego komunikatu do odczytania wysoko�ci.
	AddToContainer(pFirstMsgSurf);  // Dodanie pierwszego komunikatu do kontenera.

	// Ustalenie po�o�enia i wymiar�w okna debugera.
	m_WindowRect.x = 0;
	m_WindowRect.y = 0;
	m_WindowRect.w = g_DefaultsMgr.GetScreenWidth();
	m_WindowRect.h = ((static_cast<int>(static_cast<float>(g_DefaultsMgr.GetScreenHeight()) * 0.75f) / m_iMsgHeight)
									 * m_iMsgHeight);

	// Ustalenie po�o�enia i wymiar�w stopki.
	m_WindowFooterRect.x = 0;
	m_WindowFooterRect.y = (m_WindowRect.h - 1);
	m_WindowFooterRect.w = m_WindowRect.w;
	m_WindowFooterRect.h = 1;
}

CDebugWindow::~CDebugWindow(void) {
	// Zwolnienie pami�ci zajmowanej przez komunikaty.
	std::vector<SDL_Surface*>::iterator It;
	for(It = m_MessagesContainer.begin(); It != m_MessagesContainer.end(); ) {
			SDL_FreeSurface(*It);
			It = m_MessagesContainer.erase(It);
			// Poniewa� elementy b�d�ce za elementem usuni�tym zostan� przesuni�te odpowiednio na jego miejsce,
			// nie ma potrzeby inkrementowania iteratora.
	}

	TTF_CloseFont(m_pFont);  // Zamkni�cie czcionki.
	TTF_Quit();
}

void CDebugWindow::Print(std::string strMessage) {
	SDL_Surface* pNewMsgSurf = NULL;  // Powierzchnia, na kt�rej zostanie utworzony napis.

	// Stworzenie napisu ze spacj� na pocz�tku.
	pNewMsgSurf = TTF_RenderText_Solid(m_pFont,  (m_strSpace + strMessage).c_str(), m_TextColor);

	if(pNewMsgSurf)  // Dodanie nowego komunikatu do kontenera.
		AddToContainer(pNewMsgSurf);
}

void CDebugWindow::Print(std::string strFirstMsg, int iVal, std::string strSecondMsg) {
	SDL_Surface* pNewMsgSurf = NULL;  // Powierzchnia, na kt�rej zostanie utworzony napis.
	std::stringstream NewMsgSS;  // Strumie� do napis�w.
	NewMsgSS << m_strSpace << strFirstMsg << iVal << strSecondMsg;  // Wys�anie odpowiednich element�w do strumienia.
	pNewMsgSurf = TTF_RenderText_Solid(m_pFont,  NewMsgSS.str().c_str(), m_TextColor);  // Stworzenie napisu.
	if(pNewMsgSurf)  // Dodanie nowego komunikatu do kontenera.
		AddToContainer(pNewMsgSurf);
}

void CDebugWindow::Print(std::string strFirstMsg, float fVal, std::string strSecondMsg) {
	SDL_Surface* pNewMsgSurf = NULL;  // Powierzchnia, na kt�rej zostanie utworzony napis.
	std::stringstream NewMsgSS;  // Strumie� do napis�w.
	NewMsgSS << m_strSpace << strFirstMsg << fVal << strSecondMsg;  // Wys�anie odpowiednich element�w do strumienia.
	pNewMsgSurf = TTF_RenderText_Solid(m_pFont,  NewMsgSS.str().c_str(), m_TextColor);  // Stworzenie napisu.
	if(pNewMsgSurf)  // Dodanie nowego komunikatu do kontenera.
		AddToContainer(pNewMsgSurf);
}

void CDebugWindow::Print(std::string strFirstMsg, int iFirstVal, std::string strSecondMsg,
												 int iSecondVal, std::string strThirdMsg) {
	SDL_Surface* pNewMsgSurf = NULL;  // Powierzchnia, na kt�rej zostanie utworzony napis.
	std::stringstream NewMsgSS;  // Strumie� do napis�w.

	// Wys�anie odpowiednich element�w do strumienia.
	NewMsgSS << m_strSpace << strFirstMsg << iFirstVal << strSecondMsg << iSecondVal << strThirdMsg;

	pNewMsgSurf = TTF_RenderText_Solid(m_pFont,  NewMsgSS.str().c_str(), m_TextColor);  // Stworzenie napisu.
	if(pNewMsgSurf)  // Dodanie nowego komunikatu do kontenera.
		AddToContainer(pNewMsgSurf);
}

void CDebugWindow::Print(std::string strFirstMsg, float fFirstVal, std::string strSecondMsg,
												 float fSecondVal, std::string strThirdMsg) {
	SDL_Surface* pNewMsgSurf = NULL;  // Powierzchnia, na kt�rej zostanie utworzony napis.
	std::stringstream NewMsgSS;  // Strumie� do napis�w.

	// Wys�anie odpowiednich element�w do strumienia.
	NewMsgSS << m_strSpace << strFirstMsg << fFirstVal << strSecondMsg << fSecondVal << strThirdMsg;

	pNewMsgSurf = TTF_RenderText_Solid(m_pFont,  NewMsgSS.str().c_str(), m_TextColor);  // Stworzenie napisu.
	if(pNewMsgSurf)  // Dodanie nowego komunikatu do kontenera.
		AddToContainer(pNewMsgSurf);
}

void CDebugWindow::Print(std::string strFirstMsg, int iFirstVal, std::string strSecondMsg,
												 float fSecondVal, std::string strThirdMsg) {
	SDL_Surface* pNewMsgSurf = NULL;  // Powierzchnia, na kt�rej zostanie utworzony napis.
	std::stringstream NewMsgSS;  // Strumie� do napis�w.

	// Wys�anie odpowiednich element�w do strumienia.
	NewMsgSS << m_strSpace << strFirstMsg << iFirstVal << strSecondMsg << fSecondVal << strThirdMsg;

	pNewMsgSurf = TTF_RenderText_Solid(m_pFont,  NewMsgSS.str().c_str(), m_TextColor);  // Stworzenie napisu.
	if(pNewMsgSurf)  // Dodanie nowego komunikatu do kontenera.
		AddToContainer(pNewMsgSurf);
}

void CDebugWindow::Print(std::string strFirstMsg, float fFirstVal, std::string strSecondMsg,
												 int iSecondVal, std::string strThirdMsg) {
	SDL_Surface* pNewMsgSurf = NULL;  // Powierzchnia, na kt�rej zostanie utworzony napis.
	std::stringstream NewMsgSS;  // Strumie� do napis�w.

	// Wys�anie odpowiednich element�w do strumienia.
	NewMsgSS << m_strSpace << strFirstMsg << fFirstVal << strSecondMsg << iSecondVal << strThirdMsg;

	pNewMsgSurf = TTF_RenderText_Solid(m_pFont,  NewMsgSS.str().c_str(), m_TextColor);  // Stworzenie napisu.
	if(pNewMsgSurf)  // Dodanie nowego komunikatu do kontenera.
		AddToContainer(pNewMsgSurf);
}

void CDebugWindow::HandleInput(void) {
	int iNumMsgs = m_MessagesContainer.size();  // Obliczenie ilo�ci komunikat�w.

	//--------------------------
	// Wywo�anie okna debugera.
	//--------------------------

	// Je�eli zosta� wci�ni�ty znak "~", to zmieniamy widoczno�� okna debugera.
	if((g_EventMgr.GetEvent()->type == SDL_KEYDOWN) && (g_EventMgr.GetEvent()->key.keysym.sym == SDLK_BACKQUOTE)) {
		m_bIsVisible = !m_bIsVisible;
		if(iNumMsgs)  // Je�eli istniej� jakiekolwiek komunikaty...
			LastVisMsgRIt = LastMsgRIt;  // ...to ustawiamy ostatni widoczny komunikat na ostatnio dodany.
	}

	if(m_bIsVisible) {  // Je�eli okno debugera jest widoczne, to mo�emy kontrolowa� przewijanie komunikat�w.
		//------------------
		// Strza�ka w g�r�.
		//------------------

		// Je�eli zosta�a wci�ni�ta strza�ka w g�r� i jest pozwolenie na przewijanie komunikat�w w g�r�.
		if((g_EventMgr.GetEvent()->type == SDL_KEYDOWN) && (g_EventMgr.GetEvent()->key.keysym.sym == SDLK_UP)
			 && m_bIsPermToGoUp) {
			++LastVisMsgRIt;  // Inkrementacja iteratora odwrotnego wskazuj�cego na ostatni widoczny komunikat.

			// Je�eli znajdujemy si� po inkrementacji przed pierwszym elementem kontenera...
			if(LastVisMsgRIt == m_MessagesContainer.rend())
				LastVisMsgRIt = m_MessagesContainer.rbegin(); // ... to przerzucamy si� na ostatni element.

			// Je�eli jeste�my w miejscu ostatnio dodanego komunikatu, to cofamy si�.
			// Przewijaj�c komunikaty w g�r� do coraz to starszych nie chcemy wy�wietli� tego naj�wie�szego.
			if(LastVisMsgRIt == LastMsgRIt) {
				if(LastVisMsgRIt == m_MessagesContainer.rbegin())  // Zap�tlenie.
					LastVisMsgRIt = m_MessagesContainer.rend();
				--LastVisMsgRIt;  // Cofni�cie.
			}

			// Nale�y pami�ta�, �e inkrementuj�c iterator odwrotny przesuwamy si� od ko�ca kontenera do jego pocz�tku!
		}

		//-----------------
		// Strza�ka w d�.
		//-----------------

		// Je�eli zosta�a wci�ni�ta strza�ka w d� i s� jakiekolwiek komunikaty.
		else if((g_EventMgr.GetEvent()->type == SDL_KEYDOWN) && (g_EventMgr.GetEvent()->key.keysym.sym == SDLK_DOWN)
					  && iNumMsgs)
			// Je�eli ostatnim widocznym komunikatem jest najnowszy komunikat, to nie ma sensu przewija� komunikat�w w d�.
			if(LastVisMsgRIt != LastMsgRIt) {
				if(LastVisMsgRIt == m_MessagesContainer.rbegin())  // Zap�tlenie.
					LastVisMsgRIt = m_MessagesContainer.rend();
				--LastVisMsgRIt;  // Dekrementacja iteratora odwrotnego wskazuj�cego na ostatni widoczny komunikat.
			}
	}
}

void CDebugWindow::Show(void) {
	// Je�eli okno ma by� widoczne, to je renderujemy.
	if(m_bIsVisible) {
		//--------------------------
		// Rendering okna i stopki.
		//--------------------------

		// Wyrenderowanie okna debugera.
		SDL_FillRect(g_AppWindow.GetScreenSurf(), &m_WindowRect,
								 SDL_MapRGB(g_AppWindow.GetScreenSurf()->format, 0x00, 0x00, 0x00));

		// Wyrenderowanie stopki okna debugera.
		SDL_FillRect(g_AppWindow.GetScreenSurf(), &m_WindowFooterRect,
								 SDL_MapRGB(g_AppWindow.GetScreenSurf()->format, 0xF5, 0xF5, 0xF5));

		//------------------------
		// Rendering komunikat�w.
		//------------------------

		int iContainerSize;  // Rozmiar kontenera - ilo�� komunikat�w.
		if(iContainerSize = m_MessagesContainer.size()) {  // Je�eli istniej� jakiekolwiek kounikaty.
			// Obliczenie wysoko�ci, na jakiej b�d� pokazywane komunikaty.
			int iHeight;
			if((iHeight = (iContainerSize * m_iMsgHeight)) > m_WindowRect.h)
				iHeight = m_WindowRect.h;

			// Wy�wietlanie komunikat�w od ostatniego widocznego do coraz to starszych.
			std::vector<SDL_Surface*>::reverse_iterator RIt = LastVisMsgRIt;
			// Je�eli kontener jest pe�ny, b�dzie trzeba wzi�� pod uwag� ewentualne przej�cie z jego pocz�tku do ko�ca.
			if(iContainerSize == m_iMaxMsgs) {
				// Wy�wietlamy kolejne komunikaty, dop�ki iterator nie b�dzie wskazywa� na ostatni komunikat
				// i wysoko�� b�dzie wi�ksza, lub r�wna, od wysoko�ci komunikatu (nie chcemy wy�wietla� komunikat�w poza
				// oknem aplikacji). M�g�by nie zosta� wy�wietlony �aden komunikat, gdyby iterator ju� na pocz�tku wskazywa�
				// na ostatni komunikat. Aby tego unikn��, pos�u�ymy si� p�tl� do while. Przy sprawdzaniu warunku,
				// zinkrementowany iterator b�dzie w tym przypadku wskazywa� na komunikat poprzedzaj�cy ten ostatni.
				do {
					iHeight -= m_iMsgHeight;  // Zmniejszenie wysoko�ci o wysoko�� pojedynczego komunikatu.

					// Wy�wietlenie komunikatu na obliczonej wysoko�ci.
					g_GraphicMgr.ApplySurface(5, iHeight, *RIt, CAppWindow::GetSingletonPtr()->GetScreenSurf());

					++RIt;

					// Je�eli iterator po inkrementacji jest ustawiony zaraz przed elementem pierwszym kontenera...
					if(RIt == m_MessagesContainer.rend())
						RIt = m_MessagesContainer.rbegin();  // To ustawiamy go tak, aby wskazywa� na ostatni element.
				} while((RIt != LastMsgRIt) && (iHeight >= m_iMsgHeight));

				// Je�eli iterator wskazuje na ostatni komunikat, to znaczy, �e najstarszy komunikat zosta� wy�wietlony
				// i nie ma potrzeby przewija� komunikat�w w g�r�, bo starszych ju� nie zobaczymy.
				if(RIt == LastMsgRIt)
					m_bIsPermToGoUp = false;
				else
					m_bIsPermToGoUp = true;
			}
			// W przeciwnym wypadku wy�wietlamy komunikaty od ostatniego do coraz to starszego,
			// nie martwi�c si� przy tym o zap�tlenie przeszukiwania.
			else {
				// Wy�wietlamy kolejne komunikaty, dop�ki nie dojdziemy do elementu stoj�cego
				// przed pierwszym elementem kontenera i wysoko�� jest wi�ksza, lub r�wna wysoko�ci komunikatu.
				while((RIt != m_MessagesContainer.rend()) && (iHeight >= m_iMsgHeight)) {
					iHeight -= m_iMsgHeight;
					g_GraphicMgr.ApplySurface(5, iHeight, *RIt, CAppWindow::GetSingletonPtr()->GetScreenSurf());
					++RIt;
				}

				// Je�eli iterator wskazuje na element stoj�cy przed pierwszym elementem kontenera,
				// to znaczy, �e najstarszy komunikat zosta� wy�wietlony
				// i nie ma potrzeby przewija� komunikat�w w g�r�, bo starszych ju� nie zobaczymy.
				if(RIt == m_MessagesContainer.rend())
					m_bIsPermToGoUp = false;
				else
					m_bIsPermToGoUp = true;
			}
		}
	}
}

void CDebugWindow::AddToContainer(SDL_Surface* pMessageSurf) {
	// Je�eli zosta�a osi�gni�ta maksymalna liczba przechowywanych komunikat�w,
	// najstarszy komunikat zostanie zast�piony przez nowy.
	if(m_MessagesContainer.size() == m_iMaxMsgs) {
		// Sprawdzamy, czy ostatni komunikat nie jest ostatnim elementem kontenera.
		if(LastMsgRIt == m_MessagesContainer.rbegin())
			// Je�li jest, ustawiamy iterator bezpo�rednio przed pierwszym elementem w kontenerze.
			LastMsgRIt = m_MessagesContainer.rend();

		// Dekrementuj�c iterator odwrotny przechodzimy do kolejnego elementu,
		// kieruj�c si� od pocz�tku kontenera do jego ko�ca, tak jak by�my inkrementowali iterator nieodwrotny.
		--LastMsgRIt;

		SDL_FreeSurface(*LastMsgRIt);  // Zwolnienie pami�ci zajmowanej przez najstarszy komunikat.
		*LastMsgRIt = pMessageSurf;  // Umiejscowienie nowego komunikatu na miejscu najstarszego.
	}
	else {  // W przeciwnym wypadku umieszczamy komunikat na ko�cu kontenera.
		m_MessagesContainer.push_back(pMessageSurf);

		// Ustawiamy iterator tak, aby wskazywa� na umieszczony przed chwil� komunikat, czyli na ostatni element.
		LastMsgRIt = m_MessagesContainer.rbegin();
	}
	LastVisMsgRIt = LastMsgRIt;  // Umieszczony komunikat staje si� ostatnim widocznym.
}
