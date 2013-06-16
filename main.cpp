#include "Application.h"
#include "Timer.h"
#include <sstream>
#include <string>

int main(int argc, char* args[]) {
	CApplication Application;  // Definicja egzemplarza obiektu klasy CApplication.
	Application.ObjectMgr.CreateObjects();  // Stworzenie rycerzy i innych obiekt�w aplikacji.

	// Timery.

	CTimer DeltaTmr;  // Odmierza czas od ostatniego renderowania.
	CTimer* pFPSTmr = NULL;  // Wska�nik na timer u�ywany do obliczania liczby klatek na sekund�.
	CTimer* pUpdateTmr = NULL;  // Wska�nik na timer u�ywany do od�wie�ania nag��wka.

	// Stworzenie odpowiednich timer�w, je�li w nag��wku ma by� wy�wietlana �rednia liczba klatek na sekund�.
	if(Application.DefaultsMgr.GetIsFPS()) {
		pFPSTmr = new CTimer;
		pUpdateTmr = new CTimer;
	}

	long int iFrameCount = 0;  // Licznik klatek.

	// Wystartowanie timer�w.
	if(Application.DefaultsMgr.GetIsFPS()) {
		pFPSTmr->Start();
		pUpdateTmr->Start();
	}
	DeltaTmr.Start();

	// G��wna p�tla aplikacji, dzia�a dop�ki flaga m_bIsQuit obiektu Application jest fa�szywa.
	while(!Application.GetIsQuit()) {
		Application.EventMgr.HandleEvents();  // Obs�uga zdarze� z urz�dze� wej�ciowych.
		Uint32 iDeltaTicks = DeltaTmr.GetTicks();
		DeltaTmr.Start();  // Restart timera DeltaTmr.
		Application.ObjectMgr.UpdateObjects(iDeltaTicks);  // Uaktualnienie rycerzy i innych obiekt�w.
		Application.GraphicMgr.Show();  // Wy�wietlenie element�w sceny.
		Application.GraphicMgr.AppWindow.FlipScreen();  // Od�wie�enie ekranu.

		// Wy�wieltenie w nag��wku �redniej liczby klatek na sekund�.
		if(Application.DefaultsMgr.GetIsFPS()) {
			iFrameCount++;  // Inkrementacja licznika klatek.
			if(pUpdateTmr->GetTicks() > 1000) {  // Je�eli min�a sekunda od ostatniej aktualizacji nag��wka.
				std::stringstream CaptionSS;  // Strumie� do napis�w.

				// Do strumienia wysy�amy odpowiedni napis i �redni� liczb� klatek na sekund�.
				CaptionSS << "Average Frames Per Second: " << (iFrameCount / (pFPSTmr->GetTicks() / 1000.0f));

				Application.GraphicMgr.AppWindow.SetCaption(CaptionSS.str());  // Ustawienie nag��wka.
				pUpdateTmr->Start();  // Restart timera.
			}
		}
	}

	// Je�eli by�a wy�wietlana �rednia liczba klatek na sekund�, to usuwamy timery temu s�u��ce.
	if(Application.DefaultsMgr.GetIsFPS()) {
		delete pFPSTmr;
		delete pUpdateTmr;
	}

	return 0;  // Wyj�cie z programu.
}
