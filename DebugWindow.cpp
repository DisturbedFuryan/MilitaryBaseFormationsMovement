#include "DebugWindow.h"
#include "singletons.h"

CDebugWindow::CDebugWindow(void) : m_pFont(NULL), m_bIsVisible(false), m_iMaxMsgs(1000), m_bIsPermToGoUp(false),
																   m_strSpace("> ") {
	if(TTF_Init() == -1)  // Inicjalizacja SDL_ttf.
		assert(!"Failed to initialize SDL_ttf!");

	m_pFont = TTF_OpenFont("Fixedsys500c.ttf", 14);  // Otwarcie czcionki.
	if(!m_pFont)
		assert(!"Failed to open the font!");

	// Ustalenie koloru czcionki; kolor bia³y.
	m_TextColor.r = 255;
	m_TextColor.g = 255;
	m_TextColor.b = 255;

	// Stworzenie pierwszego komunikatu w celu odczytania wysokoœci pojedynczego komunikatu.
	SDL_Surface* pFirstMsgSurf = TTF_RenderText_Solid(m_pFont, "Movement Formation by Marcin Rainka, v. 1.0", m_TextColor);
	m_iMsgHeight = pFirstMsgSurf->h;  // Wykorzystanie stworzonego komunikatu do odczytania wysokoœci.
	AddToContainer(pFirstMsgSurf);  // Dodanie pierwszego komunikatu do kontenera.

	// Ustalenie po³o¿enia i wymiarów okna debugera.
	m_WindowRect.x = 0;
	m_WindowRect.y = 0;
	m_WindowRect.w = g_DefaultsMgr.GetScreenWidth();
	m_WindowRect.h = ((static_cast<int>(static_cast<float>(g_DefaultsMgr.GetScreenHeight()) * 0.75f) / m_iMsgHeight)
									 * m_iMsgHeight);

	// Ustalenie po³o¿enia i wymiarów stopki.
	m_WindowFooterRect.x = 0;
	m_WindowFooterRect.y = (m_WindowRect.h - 1);
	m_WindowFooterRect.w = m_WindowRect.w;
	m_WindowFooterRect.h = 1;
}

CDebugWindow::~CDebugWindow(void) {
	// Zwolnienie pamiêci zajmowanej przez komunikaty.
	std::vector<SDL_Surface*>::iterator It;
	for(It = m_MessagesContainer.begin(); It != m_MessagesContainer.end(); ) {
			SDL_FreeSurface(*It);
			It = m_MessagesContainer.erase(It);
			// Poniewa¿ elementy bêd¹ce za elementem usuniêtym zostan¹ przesuniête odpowiednio na jego miejsce,
			// nie ma potrzeby inkrementowania iteratora.
	}

	TTF_CloseFont(m_pFont);  // Zamkniêcie czcionki.
	TTF_Quit();
}

void CDebugWindow::Print(std::string strMessage) {
	SDL_Surface* pNewMsgSurf = NULL;  // Powierzchnia, na której zostanie utworzony napis.

	// Stworzenie napisu ze spacj¹ na pocz¹tku.
	pNewMsgSurf = TTF_RenderText_Solid(m_pFont,  (m_strSpace + strMessage).c_str(), m_TextColor);

	if(pNewMsgSurf)  // Dodanie nowego komunikatu do kontenera.
		AddToContainer(pNewMsgSurf);
}

void CDebugWindow::Print(std::string strFirstMsg, int iVal, std::string strSecondMsg) {
	SDL_Surface* pNewMsgSurf = NULL;  // Powierzchnia, na której zostanie utworzony napis.
	std::stringstream NewMsgSS;  // Strumieñ do napisów.
	NewMsgSS << m_strSpace << strFirstMsg << iVal << strSecondMsg;  // Wys³anie odpowiednich elementów do strumienia.
	pNewMsgSurf = TTF_RenderText_Solid(m_pFont,  NewMsgSS.str().c_str(), m_TextColor);  // Stworzenie napisu.
	if(pNewMsgSurf)  // Dodanie nowego komunikatu do kontenera.
		AddToContainer(pNewMsgSurf);
}

void CDebugWindow::Print(std::string strFirstMsg, float fVal, std::string strSecondMsg) {
	SDL_Surface* pNewMsgSurf = NULL;  // Powierzchnia, na której zostanie utworzony napis.
	std::stringstream NewMsgSS;  // Strumieñ do napisów.
	NewMsgSS << m_strSpace << strFirstMsg << fVal << strSecondMsg;  // Wys³anie odpowiednich elementów do strumienia.
	pNewMsgSurf = TTF_RenderText_Solid(m_pFont,  NewMsgSS.str().c_str(), m_TextColor);  // Stworzenie napisu.
	if(pNewMsgSurf)  // Dodanie nowego komunikatu do kontenera.
		AddToContainer(pNewMsgSurf);
}

void CDebugWindow::Print(std::string strFirstMsg, int iFirstVal, std::string strSecondMsg,
												 int iSecondVal, std::string strThirdMsg) {
	SDL_Surface* pNewMsgSurf = NULL;  // Powierzchnia, na której zostanie utworzony napis.
	std::stringstream NewMsgSS;  // Strumieñ do napisów.

	// Wys³anie odpowiednich elementów do strumienia.
	NewMsgSS << m_strSpace << strFirstMsg << iFirstVal << strSecondMsg << iSecondVal << strThirdMsg;

	pNewMsgSurf = TTF_RenderText_Solid(m_pFont,  NewMsgSS.str().c_str(), m_TextColor);  // Stworzenie napisu.
	if(pNewMsgSurf)  // Dodanie nowego komunikatu do kontenera.
		AddToContainer(pNewMsgSurf);
}

void CDebugWindow::Print(std::string strFirstMsg, float fFirstVal, std::string strSecondMsg,
												 float fSecondVal, std::string strThirdMsg) {
	SDL_Surface* pNewMsgSurf = NULL;  // Powierzchnia, na której zostanie utworzony napis.
	std::stringstream NewMsgSS;  // Strumieñ do napisów.

	// Wys³anie odpowiednich elementów do strumienia.
	NewMsgSS << m_strSpace << strFirstMsg << fFirstVal << strSecondMsg << fSecondVal << strThirdMsg;

	pNewMsgSurf = TTF_RenderText_Solid(m_pFont,  NewMsgSS.str().c_str(), m_TextColor);  // Stworzenie napisu.
	if(pNewMsgSurf)  // Dodanie nowego komunikatu do kontenera.
		AddToContainer(pNewMsgSurf);
}

void CDebugWindow::Print(std::string strFirstMsg, int iFirstVal, std::string strSecondMsg,
												 float fSecondVal, std::string strThirdMsg) {
	SDL_Surface* pNewMsgSurf = NULL;  // Powierzchnia, na której zostanie utworzony napis.
	std::stringstream NewMsgSS;  // Strumieñ do napisów.

	// Wys³anie odpowiednich elementów do strumienia.
	NewMsgSS << m_strSpace << strFirstMsg << iFirstVal << strSecondMsg << fSecondVal << strThirdMsg;

	pNewMsgSurf = TTF_RenderText_Solid(m_pFont,  NewMsgSS.str().c_str(), m_TextColor);  // Stworzenie napisu.
	if(pNewMsgSurf)  // Dodanie nowego komunikatu do kontenera.
		AddToContainer(pNewMsgSurf);
}

void CDebugWindow::Print(std::string strFirstMsg, float fFirstVal, std::string strSecondMsg,
												 int iSecondVal, std::string strThirdMsg) {
	SDL_Surface* pNewMsgSurf = NULL;  // Powierzchnia, na której zostanie utworzony napis.
	std::stringstream NewMsgSS;  // Strumieñ do napisów.

	// Wys³anie odpowiednich elementów do strumienia.
	NewMsgSS << m_strSpace << strFirstMsg << fFirstVal << strSecondMsg << iSecondVal << strThirdMsg;

	pNewMsgSurf = TTF_RenderText_Solid(m_pFont,  NewMsgSS.str().c_str(), m_TextColor);  // Stworzenie napisu.
	if(pNewMsgSurf)  // Dodanie nowego komunikatu do kontenera.
		AddToContainer(pNewMsgSurf);
}

void CDebugWindow::HandleInput(void) {
	int iNumMsgs = m_MessagesContainer.size();  // Obliczenie iloœci komunikatów.

	//--------------------------
	// Wywo³anie okna debugera.
	//--------------------------

	// Je¿eli zosta³ wciœniêty znak "~", to zmieniamy widocznoœæ okna debugera.
	if((g_EventMgr.GetEvent()->type == SDL_KEYDOWN) && (g_EventMgr.GetEvent()->key.keysym.sym == SDLK_BACKQUOTE)) {
		m_bIsVisible = !m_bIsVisible;
		if(iNumMsgs)  // Je¿eli istniej¹ jakiekolwiek komunikaty...
			LastVisMsgRIt = LastMsgRIt;  // ...to ustawiamy ostatni widoczny komunikat na ostatnio dodany.
	}

	if(m_bIsVisible) {  // Je¿eli okno debugera jest widoczne, to mo¿emy kontrolowaæ przewijanie komunikatów.
		//------------------
		// Strza³ka w górê.
		//------------------

		// Je¿eli zosta³a wciœniêta strza³ka w górê i jest pozwolenie na przewijanie komunikatów w górê.
		if((g_EventMgr.GetEvent()->type == SDL_KEYDOWN) && (g_EventMgr.GetEvent()->key.keysym.sym == SDLK_UP)
			 && m_bIsPermToGoUp) {
			++LastVisMsgRIt;  // Inkrementacja iteratora odwrotnego wskazuj¹cego na ostatni widoczny komunikat.

			// Je¿eli znajdujemy siê po inkrementacji przed pierwszym elementem kontenera...
			if(LastVisMsgRIt == m_MessagesContainer.rend())
				LastVisMsgRIt = m_MessagesContainer.rbegin(); // ... to przerzucamy siê na ostatni element.

			// Je¿eli jesteœmy w miejscu ostatnio dodanego komunikatu, to cofamy siê.
			// Przewijaj¹c komunikaty w górê do coraz to starszych nie chcemy wyœwietliæ tego najœwie¿szego.
			if(LastVisMsgRIt == LastMsgRIt) {
				if(LastVisMsgRIt == m_MessagesContainer.rbegin())  // Zapêtlenie.
					LastVisMsgRIt = m_MessagesContainer.rend();
				--LastVisMsgRIt;  // Cofniêcie.
			}

			// Nale¿y pamiêtaæ, ¿e inkrementuj¹c iterator odwrotny przesuwamy siê od koñca kontenera do jego pocz¹tku!
		}

		//-----------------
		// Strza³ka w dó³.
		//-----------------

		// Je¿eli zosta³a wciœniêta strza³ka w dó³ i s¹ jakiekolwiek komunikaty.
		else if((g_EventMgr.GetEvent()->type == SDL_KEYDOWN) && (g_EventMgr.GetEvent()->key.keysym.sym == SDLK_DOWN)
					  && iNumMsgs)
			// Je¿eli ostatnim widocznym komunikatem jest najnowszy komunikat, to nie ma sensu przewijaæ komunikatów w dó³.
			if(LastVisMsgRIt != LastMsgRIt) {
				if(LastVisMsgRIt == m_MessagesContainer.rbegin())  // Zapêtlenie.
					LastVisMsgRIt = m_MessagesContainer.rend();
				--LastVisMsgRIt;  // Dekrementacja iteratora odwrotnego wskazuj¹cego na ostatni widoczny komunikat.
			}
	}
}

void CDebugWindow::Show(void) {
	// Je¿eli okno ma byæ widoczne, to je renderujemy.
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
		// Rendering komunikatów.
		//------------------------

		int iContainerSize;  // Rozmiar kontenera - iloœæ komunikatów.
		if(iContainerSize = m_MessagesContainer.size()) {  // Je¿eli istniej¹ jakiekolwiek kounikaty.
			// Obliczenie wysokoœci, na jakiej bêd¹ pokazywane komunikaty.
			int iHeight;
			if((iHeight = (iContainerSize * m_iMsgHeight)) > m_WindowRect.h)
				iHeight = m_WindowRect.h;

			// Wyœwietlanie komunikatów od ostatniego widocznego do coraz to starszych.
			std::vector<SDL_Surface*>::reverse_iterator RIt = LastVisMsgRIt;
			// Je¿eli kontener jest pe³ny, bêdzie trzeba wzi¹æ pod uwagê ewentualne przejœcie z jego pocz¹tku do koñca.
			if(iContainerSize == m_iMaxMsgs) {
				// Wyœwietlamy kolejne komunikaty, dopóki iterator nie bêdzie wskazywa³ na ostatni komunikat
				// i wysokoœæ bêdzie wiêksza, lub równa, od wysokoœci komunikatu (nie chcemy wyœwietlaæ komunikatów poza
				// oknem aplikacji). Móg³by nie zostaæ wyœwietlony ¿aden komunikat, gdyby iterator ju¿ na pocz¹tku wskazywa³
				// na ostatni komunikat. Aby tego unikn¹æ, pos³u¿ymy siê pêtl¹ do while. Przy sprawdzaniu warunku,
				// zinkrementowany iterator bêdzie w tym przypadku wskazywa³ na komunikat poprzedzaj¹cy ten ostatni.
				do {
					iHeight -= m_iMsgHeight;  // Zmniejszenie wysokoœci o wysokoœæ pojedynczego komunikatu.

					// Wyœwietlenie komunikatu na obliczonej wysokoœci.
					g_GraphicMgr.ApplySurface(5, iHeight, *RIt, CAppWindow::GetSingletonPtr()->GetScreenSurf());

					++RIt;

					// Je¿eli iterator po inkrementacji jest ustawiony zaraz przed elementem pierwszym kontenera...
					if(RIt == m_MessagesContainer.rend())
						RIt = m_MessagesContainer.rbegin();  // To ustawiamy go tak, aby wskazywa³ na ostatni element.
				} while((RIt != LastMsgRIt) && (iHeight >= m_iMsgHeight));

				// Je¿eli iterator wskazuje na ostatni komunikat, to znaczy, ¿e najstarszy komunikat zosta³ wyœwietlony
				// i nie ma potrzeby przewijaæ komunikatów w górê, bo starszych ju¿ nie zobaczymy.
				if(RIt == LastMsgRIt)
					m_bIsPermToGoUp = false;
				else
					m_bIsPermToGoUp = true;
			}
			// W przeciwnym wypadku wyœwietlamy komunikaty od ostatniego do coraz to starszego,
			// nie martwi¹c siê przy tym o zapêtlenie przeszukiwania.
			else {
				// Wyœwietlamy kolejne komunikaty, dopóki nie dojdziemy do elementu stoj¹cego
				// przed pierwszym elementem kontenera i wysokoœæ jest wiêksza, lub równa wysokoœci komunikatu.
				while((RIt != m_MessagesContainer.rend()) && (iHeight >= m_iMsgHeight)) {
					iHeight -= m_iMsgHeight;
					g_GraphicMgr.ApplySurface(5, iHeight, *RIt, CAppWindow::GetSingletonPtr()->GetScreenSurf());
					++RIt;
				}

				// Je¿eli iterator wskazuje na element stoj¹cy przed pierwszym elementem kontenera,
				// to znaczy, ¿e najstarszy komunikat zosta³ wyœwietlony
				// i nie ma potrzeby przewijaæ komunikatów w górê, bo starszych ju¿ nie zobaczymy.
				if(RIt == m_MessagesContainer.rend())
					m_bIsPermToGoUp = false;
				else
					m_bIsPermToGoUp = true;
			}
		}
	}
}

void CDebugWindow::AddToContainer(SDL_Surface* pMessageSurf) {
	// Je¿eli zosta³a osi¹gniêta maksymalna liczba przechowywanych komunikatów,
	// najstarszy komunikat zostanie zast¹piony przez nowy.
	if(m_MessagesContainer.size() == m_iMaxMsgs) {
		// Sprawdzamy, czy ostatni komunikat nie jest ostatnim elementem kontenera.
		if(LastMsgRIt == m_MessagesContainer.rbegin())
			// Jeœli jest, ustawiamy iterator bezpoœrednio przed pierwszym elementem w kontenerze.
			LastMsgRIt = m_MessagesContainer.rend();

		// Dekrementuj¹c iterator odwrotny przechodzimy do kolejnego elementu,
		// kieruj¹c siê od pocz¹tku kontenera do jego koñca, tak jak byœmy inkrementowali iterator nieodwrotny.
		--LastMsgRIt;

		SDL_FreeSurface(*LastMsgRIt);  // Zwolnienie pamiêci zajmowanej przez najstarszy komunikat.
		*LastMsgRIt = pMessageSurf;  // Umiejscowienie nowego komunikatu na miejscu najstarszego.
	}
	else {  // W przeciwnym wypadku umieszczamy komunikat na koñcu kontenera.
		m_MessagesContainer.push_back(pMessageSurf);

		// Ustawiamy iterator tak, aby wskazywa³ na umieszczony przed chwil¹ komunikat, czyli na ostatni element.
		LastMsgRIt = m_MessagesContainer.rbegin();
	}
	LastVisMsgRIt = LastMsgRIt;  // Umieszczony komunikat staje siê ostatnim widocznym.
}
