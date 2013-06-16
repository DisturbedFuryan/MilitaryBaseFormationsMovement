#include "ObstacleMgr.h"
#include "singletons.h"

CObstacleMgr::~CObstacleMgr(void) {
	// Likwidacja wszystkich przeszkód.
	std::vector<CObstacle*>::iterator It;
	for(It = m_ObstaclesContainer.begin(); It != m_ObstaclesContainer.end(); ) {
			delete *It;
			It = m_ObstaclesContainer.erase(It);
			// Poniewa¿ elementy bêd¹ce za elementem usuniêtym zostan¹ przesuniête odpowiednio na jego miejsce,
			// nie ma potrzeby inkrementowania iteratora.
	}
}

void CObstacleMgr::Show(void) {
	// Wyœwietlenie wszystkich przeszkód bêd¹cych w kontenerze.
	std::vector<CObstacle*>::iterator It;
	for(It = m_ObstaclesContainer.begin(); It != m_ObstaclesContainer.end(); ++It)
		(*It)->Show();
}

void CObstacleMgr::Show(short iType) {
	// Wyœwietlenie przeszkód podanego typu, bêd¹cych w kontenerze.
	std::vector<CObstacle*>::iterator It;
	for(It = m_ObstaclesContainer.begin(); It != m_ObstaclesContainer.end(); ++It)
		if(iType == (*It)->GetType())
			(*It)->Show();
}

void CObstacleMgr::CreatePalisadesAndStones(int iNumPalisades, int iNumStones) {
	if(!iNumPalisades && !iNumStones)
		// Wyœwietlenie odpowiedniego komunikatu, je¿eli podana iloœæ przeszkód do stworzenia jest zerowa.
		g_DebugWindow.Print("WARNING: Failed to create ", iNumPalisades, " obstacles.");
	else {
		CreateObstacles(iNumPalisades, CObstacle::palisade);  // Stworzenie palisad.
		CreateObstacles(iNumStones, CObstacle::stone);  // Stworzenie kamieni.
	}
}

void CObstacleMgr::CreateObstacles(int iNumObstacles, short iType) {
	// Zmienna do przypisywania przeszkodom identyfikatora. Wartoœci¹ startow¹ jest rozmiar kontenera.
	short iId = m_ObstaclesContainer.size();

	int iObstacleCount = 0;  // Licznik stworzonych przeszkód.

	// Ko³o kolizji nie stworzonej jeszcze przeszkody.
	CCircle ObstCollCircle;

	// Flaga okreœlaj¹ca, czy nast¹pi³a kolizja pomiêdzy ko³em kolizji a któr¹œ z istniej¹cych ju¿ przeszkód.
	bool bIsCollision;

	int iHitCount;  // Zlicza próby znalezienia wolnej pozycji dla nowej przeszkody.

	// Stworzenie zadanej iloœci przeszkód.
	for(int i = 0; i < iNumObstacles; i++) {
		iHitCount = 0;  // Licznik prób zostaje ustawiony na zero.

		// Flaga zostaje pocz¹tkowo ustawiona na wartoœæ "true". Inaczej warunek znajduj¹cej siê poni¿ej pêtli "while"
		// by³by fa³szywy ju¿ na samym pocz¹tku. Przypadek ten by³by niepo¿¹dany.
		bIsCollision = true;

		// Próbujemy stworzyæ przeszkodê, dopóki kolejne wylosowane pozycje okazuj¹ siê byæ kolizyjnymi
		// wobec innych przeszkód, lub licznik prób nie przekroczy tysi¹ca.
		while((iHitCount < 1000) && bIsCollision) {
			// Wylosowanie pozycji.
			ObstCollCircle.pos.x = (Rand() * static_cast<float>(g_DefaultsMgr.GetAreaWidth()));
			ObstCollCircle.pos.y = (Rand() * static_cast<float>(g_DefaultsMgr.GetAreaHeight()));

			// Zwiêkszenie wra¿enia losowoœci poprzez ewentualne odbicie sk³adowych wektora pozycji wzglêdem "œrodka œwiata".
			if(Rand() > 0.5f)
				ObstCollCircle.pos.x = (static_cast<float>(g_DefaultsMgr.GetAreaWidth()) - ObstCollCircle.pos.x);
			if(Rand() > 0.5f)
				ObstCollCircle.pos.y = (static_cast<float>(g_DefaultsMgr.GetAreaHeight()) - ObstCollCircle.pos.y);

			// Przypisanie odpowiedniego promienia ko³owi kolizji dla odpowiedniego typu przeszkody.
			if(iType == CObstacle::palisade)  // Dla palisady.
				ObstCollCircle.r = g_DefaultsMgr.GetPalisadeRadius();
			else if(iType == CObstacle::stone)  // Dla kamienia.
				ObstCollCircle.r = g_DefaultsMgr.GetStoneRadius();

			//---------------------------------------------------------------------------------------------
			// Upewnienie siê, ¿e przeszkoda nie bêdzie kolidowaæ z istniej¹cymi rycerzami i przeszkodami.
			//---------------------------------------------------------------------------------------------

			bIsCollision = false;  // Zmiana flagi na wartosæ "false". Nie stwierdzono jeszcze kolizji.

			// Je¿eli wyst¹pi³a kolizja z rycerzami, to nie ma potrzeby szukaæ kolizji z istniej¹cymi przeszkodami.
			if(!(bIsCollision = g_CollisionController.CheckCollisionWithKnights(&ObstCollCircle))) {
				// Powiêkszenie promienia ko³a kolizji o trzykrotnoœæ promienia rycerza.
				// Zabieg ten zapewni dostateczn¹ iloœæ miejsca pomiêdzy przeszkodami.
				ObstCollCircle.r += (g_DefaultsMgr.GetKnightRadius() * 3.0f);

				// Szukamy kolizji pomiêdzy ko³em o œrodku le¿¹cym na wylosowanej pozycji
				// i adekwatnym do typu przeszkody powiêkszonym promieniem, a któr¹œ z istniej¹cych przeszkód.
				bIsCollision = g_CollisionController.CheckCollisionWithObstacles(&ObstCollCircle);
			}

			iHitCount++;
		}

		// Je¿eli nie ma kolizji, przeszkoda zostaje stworzona. Czemu ten warunek jest sprawdzany?
		// Przecie¿ znajduj¹ca siê wy¿ej pêtla "while" mia³a funkcjonowaæ dopóki kolizja bêdzie wystêpowaæ.
		// Skoro znajdujemy siê teraz poza pêtl¹, czy to nie znaczy, ¿e kolizja nie wystêpuje? Otó¿ nie!
		// Pêtla mog³a zostaæ przerwana ze wzglêdu na przekroczenie przez licznik maksymalnej wartoœci.
		// Niekoniecznie ostatnia wylosowana pozycja jest pozycj¹ niekolizyjn¹. Zatem upewniamy siê.
		if(!bIsCollision) {
			CObstacle* pObstacle = new CObstacle(iId, &ObstCollCircle.pos, iType);  // Stworzenie przeszkody.
			m_ObstaclesContainer.push_back(pObstacle);  // Umieszczenie stworzonej przeszkody na koñcu kontenera.
			iObstacleCount++;  // Inkrementacja licznika stworzonych przeszkód.
			iId++;  // Inkrementacja identyfikatora dla kolejnych nowych przeszkód.
		}
	}

	//---------------------------------------------------------
	// Wyœwietlenie informacji na temat stworzonych przeszkód.
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
