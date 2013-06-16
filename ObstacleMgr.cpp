#include "ObstacleMgr.h"
#include "singletons.h"

CObstacleMgr::~CObstacleMgr(void) {
	// Likwidacja wszystkich przeszk�d.
	std::vector<CObstacle*>::iterator It;
	for(It = m_ObstaclesContainer.begin(); It != m_ObstaclesContainer.end(); ) {
			delete *It;
			It = m_ObstaclesContainer.erase(It);
			// Poniewa� elementy b�d�ce za elementem usuni�tym zostan� przesuni�te odpowiednio na jego miejsce,
			// nie ma potrzeby inkrementowania iteratora.
	}
}

void CObstacleMgr::Show(void) {
	// Wy�wietlenie wszystkich przeszk�d b�d�cych w kontenerze.
	std::vector<CObstacle*>::iterator It;
	for(It = m_ObstaclesContainer.begin(); It != m_ObstaclesContainer.end(); ++It)
		(*It)->Show();
}

void CObstacleMgr::Show(short iType) {
	// Wy�wietlenie przeszk�d podanego typu, b�d�cych w kontenerze.
	std::vector<CObstacle*>::iterator It;
	for(It = m_ObstaclesContainer.begin(); It != m_ObstaclesContainer.end(); ++It)
		if(iType == (*It)->GetType())
			(*It)->Show();
}

void CObstacleMgr::CreatePalisadesAndStones(int iNumPalisades, int iNumStones) {
	if(!iNumPalisades && !iNumStones)
		// Wy�wietlenie odpowiedniego komunikatu, je�eli podana ilo�� przeszk�d do stworzenia jest zerowa.
		g_DebugWindow.Print("WARNING: Failed to create ", iNumPalisades, " obstacles.");
	else {
		CreateObstacles(iNumPalisades, CObstacle::palisade);  // Stworzenie palisad.
		CreateObstacles(iNumStones, CObstacle::stone);  // Stworzenie kamieni.
	}
}

void CObstacleMgr::CreateObstacles(int iNumObstacles, short iType) {
	// Zmienna do przypisywania przeszkodom identyfikatora. Warto�ci� startow� jest rozmiar kontenera.
	short iId = m_ObstaclesContainer.size();

	int iObstacleCount = 0;  // Licznik stworzonych przeszk�d.

	// Ko�o kolizji nie stworzonej jeszcze przeszkody.
	CCircle ObstCollCircle;

	// Flaga okre�laj�ca, czy nast�pi�a kolizja pomi�dzy ko�em kolizji a kt�r�� z istniej�cych ju� przeszk�d.
	bool bIsCollision;

	int iHitCount;  // Zlicza pr�by znalezienia wolnej pozycji dla nowej przeszkody.

	// Stworzenie zadanej ilo�ci przeszk�d.
	for(int i = 0; i < iNumObstacles; i++) {
		iHitCount = 0;  // Licznik pr�b zostaje ustawiony na zero.

		// Flaga zostaje pocz�tkowo ustawiona na warto�� "true". Inaczej warunek znajduj�cej si� poni�ej p�tli "while"
		// by�by fa�szywy ju� na samym pocz�tku. Przypadek ten by�by niepo��dany.
		bIsCollision = true;

		// Pr�bujemy stworzy� przeszkod�, dop�ki kolejne wylosowane pozycje okazuj� si� by� kolizyjnymi
		// wobec innych przeszk�d, lub licznik pr�b nie przekroczy tysi�ca.
		while((iHitCount < 1000) && bIsCollision) {
			// Wylosowanie pozycji.
			ObstCollCircle.pos.x = (Rand() * static_cast<float>(g_DefaultsMgr.GetAreaWidth()));
			ObstCollCircle.pos.y = (Rand() * static_cast<float>(g_DefaultsMgr.GetAreaHeight()));

			// Zwi�kszenie wra�enia losowo�ci poprzez ewentualne odbicie sk�adowych wektora pozycji wzgl�dem "�rodka �wiata".
			if(Rand() > 0.5f)
				ObstCollCircle.pos.x = (static_cast<float>(g_DefaultsMgr.GetAreaWidth()) - ObstCollCircle.pos.x);
			if(Rand() > 0.5f)
				ObstCollCircle.pos.y = (static_cast<float>(g_DefaultsMgr.GetAreaHeight()) - ObstCollCircle.pos.y);

			// Przypisanie odpowiedniego promienia ko�owi kolizji dla odpowiedniego typu przeszkody.
			if(iType == CObstacle::palisade)  // Dla palisady.
				ObstCollCircle.r = g_DefaultsMgr.GetPalisadeRadius();
			else if(iType == CObstacle::stone)  // Dla kamienia.
				ObstCollCircle.r = g_DefaultsMgr.GetStoneRadius();

			//---------------------------------------------------------------------------------------------
			// Upewnienie si�, �e przeszkoda nie b�dzie kolidowa� z istniej�cymi rycerzami i przeszkodami.
			//---------------------------------------------------------------------------------------------

			bIsCollision = false;  // Zmiana flagi na wartos� "false". Nie stwierdzono jeszcze kolizji.

			// Je�eli wyst�pi�a kolizja z rycerzami, to nie ma potrzeby szuka� kolizji z istniej�cymi przeszkodami.
			if(!(bIsCollision = g_CollisionController.CheckCollisionWithKnights(&ObstCollCircle))) {
				// Powi�kszenie promienia ko�a kolizji o trzykrotno�� promienia rycerza.
				// Zabieg ten zapewni dostateczn� ilo�� miejsca pomi�dzy przeszkodami.
				ObstCollCircle.r += (g_DefaultsMgr.GetKnightRadius() * 3.0f);

				// Szukamy kolizji pomi�dzy ko�em o �rodku le��cym na wylosowanej pozycji
				// i adekwatnym do typu przeszkody powi�kszonym promieniem, a kt�r�� z istniej�cych przeszk�d.
				bIsCollision = g_CollisionController.CheckCollisionWithObstacles(&ObstCollCircle);
			}

			iHitCount++;
		}

		// Je�eli nie ma kolizji, przeszkoda zostaje stworzona. Czemu ten warunek jest sprawdzany?
		// Przecie� znajduj�ca si� wy�ej p�tla "while" mia�a funkcjonowa� dop�ki kolizja b�dzie wyst�powa�.
		// Skoro znajdujemy si� teraz poza p�tl�, czy to nie znaczy, �e kolizja nie wyst�puje? Ot� nie!
		// P�tla mog�a zosta� przerwana ze wzgl�du na przekroczenie przez licznik maksymalnej warto�ci.
		// Niekoniecznie ostatnia wylosowana pozycja jest pozycj� niekolizyjn�. Zatem upewniamy si�.
		if(!bIsCollision) {
			CObstacle* pObstacle = new CObstacle(iId, &ObstCollCircle.pos, iType);  // Stworzenie przeszkody.
			m_ObstaclesContainer.push_back(pObstacle);  // Umieszczenie stworzonej przeszkody na ko�cu kontenera.
			iObstacleCount++;  // Inkrementacja licznika stworzonych przeszk�d.
			iId++;  // Inkrementacja identyfikatora dla kolejnych nowych przeszk�d.
		}
	}

	//---------------------------------------------------------
	// Wy�wietlenie informacji na temat stworzonych przeszk�d.
	//---------------------------------------------------------

	if(iNumObstacles > 0)
		if(iType == CObstacle::palisade) {  // Dla palisady.
			if(iObstacleCount == iNumObstacles)
				g_DebugWindow.Print("All ", iNumObstacles, " palisades have been created.");
			else {
				g_DebugWindow.Print("WARNING: ", (iNumObstacles - iObstacleCount), " of ", iNumObstacles,
															" palisades have not been created.");
				g_DebugWindow.Print("         Probably not enough space.");
			}
		}
		else if(iType == CObstacle::stone)  // Dla kamienia.
			if(iObstacleCount == iNumObstacles)
				g_DebugWindow.Print("All ", iNumObstacles, " stones have been created.");
			else {
				g_DebugWindow.Print("WARNING: ", (iNumObstacles - iObstacleCount), " of ", iNumObstacles,
														  " stones have not been created.");
				g_DebugWindow.Print("         Probably not enough space.");
			}
}
