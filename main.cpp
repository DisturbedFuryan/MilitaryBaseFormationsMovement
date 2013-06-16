#include "Application.h"
#include "Timer.h"
#include <sstream>
#include <string>

int main(int argc, char* args[]) {
	CApplication Application;  // Definicja egzemplarza obiektu klasy CApplication.
	Application.ObjectMgr.CreateObjects();  // Stworzenie rycerzy i innych obiektów aplikacji.

	// Timery.

	CTimer DeltaTmr;  // Odmierza czas od ostatniego renderowania.
	CTimer* pFPSTmr = NULL;  // WskaŸnik na timer u¿ywany do obliczania liczby klatek na sekundê.
	CTimer* pUpdateTmr = NULL;  // WskaŸnik na timer u¿ywany do odœwie¿ania nag³ówka.

	// Stworzenie odpowiednich timerów, jeœli w nag³ówku ma byæ wyœwietlana œrednia liczba klatek na sekundê.
	if(Application.DefaultsMgr.GetIsFPS()) {
		pFPSTmr = new CTimer;
		pUpdateTmr = new CTimer;
	}

	long int iFrameCount = 0;  // Licznik klatek.

	// Wystartowanie timerów.
	if(Application.DefaultsMgr.GetIsFPS()) {
		pFPSTmr->Start();
		pUpdateTmr->Start();
	}
	DeltaTmr.Start();

	// G³ówna pêtla aplikacji, dzia³a dopóki flaga m_bIsQuit obiektu Application jest fa³szywa.
	while(!Application.GetIsQuit()) {
		Application.EventMgr.HandleEvents();  // Obs³uga zdarzeñ z urz¹dzeñ wejœciowych.
		Uint32 iDeltaTicks = DeltaTmr.GetTicks();
		DeltaTmr.Start();  // Restart timera DeltaTmr.
		Application.ObjectMgr.UpdateObjects(iDeltaTicks);  // Uaktualnienie rycerzy i innych obiektów.
		Application.GraphicMgr.Show();  // Wyœwietlenie elementów sceny.
		Application.GraphicMgr.AppWindow.FlipScreen();  // Odœwie¿enie ekranu.

		// Wyœwieltenie w nag³ówku œredniej liczby klatek na sekundê.
		if(Application.DefaultsMgr.GetIsFPS()) {
			iFrameCount++;  // Inkrementacja licznika klatek.
			if(pUpdateTmr->GetTicks() > 1000) {  // Je¿eli minê³a sekunda od ostatniej aktualizacji nag³ówka.
				std::stringstream CaptionSS;  // Strumieñ do napisów.

				// Do strumienia wysy³amy odpowiedni napis i œredni¹ liczbê klatek na sekundê.
				CaptionSS << "Average Frames Per Second: " << (iFrameCount / (pFPSTmr->GetTicks() / 1000.0f));

				Application.GraphicMgr.AppWindow.SetCaption(CaptionSS.str());  // Ustawienie nag³ówka.
				pUpdateTmr->Start();  // Restart timera.
			}
		}
	}

	// Je¿eli by³a wyœwietlana œrednia liczba klatek na sekundê, to usuwamy timery temu s³u¿¹ce.
	if(Application.DefaultsMgr.GetIsFPS()) {
		delete pFPSTmr;
		delete pUpdateTmr;
	}

	return 0;  // Wyjœcie z programu.
}
