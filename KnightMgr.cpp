#include "KnightMgr.h"
#include "singletons.h"

CKnightMgr::CKnightMgr(void) {
	if(!LoadSurfaces())
		assert(!"Failed to rotate images!");
}

CKnightMgr::~CKnightMgr(void) {
	// Likwidacja wszystkich armii.
	std::vector<CArmy*>::iterator ItA;
	for(ItA = m_ArmiesContainer.begin(); ItA != m_ArmiesContainer.end(); ) {
			delete *ItA;
			ItA = m_ArmiesContainer.erase(ItA);
			// Poniewa¿ elementy bêd¹ce za elementem usuniêtym zostan¹ przesuniête odpowiednio na jego miejsce,
			// nie ma potrzeby inkrementowania iteratora.
	}

	// Likwidacja wszystkich rycerzy.
	std::vector<IKnight*>::iterator ItK;
	for(ItK = m_KnightsContainer.begin(); ItK != m_KnightsContainer.end(); ) {
			delete *ItK;
			ItK = m_KnightsContainer.erase(ItK);
	}

	// Zwolnienie pamiêci zajmowanej przez poobracane obrazki.
  if(g_DefaultsMgr.GetIsRotatedSurfaces()) {  // Je¿eli obrazki mia³y byæ poobracane.
		for(int i = 0; i < 360; i++) {  // Wyczyszczenie jednowymiarowych tablic.
			// Œlady po butach.
			SDL_FreeSurface(m_apKnightShoesprintLeftSurfacesAllAngles[i]);
			SDL_FreeSurface(m_apKnightShoesprintRightSurfacesAllAngles[i]);

			// Podœwietlenie.
			SDL_FreeSurface(m_apCommanderBacklightYellowSurfaces[i]);
			SDL_FreeSurface(m_apCommanderBacklightBlueSurfaces[i]);
			SDL_FreeSurface(m_apCommanderBacklightGreenSurfaces[i]);

			SDL_FreeSurface(m_apKnightSwordStandSurfacesAllAngles[i]);  // Miecz.
			SDL_FreeSurface(m_apKnightBodyStandSurfacesAllAngles[i]);  // Cia³o.
		}

		// Wyczyszczenie tablic dwuwymiarowych...
		for(int i = 0; i < 4; i++)  // ...zawieraj¹cych cztery obrazki pod wszystkimi k¹tami.
			for(int j = 0; j < 360; j++) {
				// Œlady po butach.
				SDL_FreeSurface(m_apKnightShoesprintLeftDisappearSurfacesAllAngles[i][j]);
				SDL_FreeSurface(m_apKnightShoesprintRightDisappearSurfacesAllAngles[i][j]);
			}
		for(int i = 0; i < 8; i++)  // ...zawieraj¹cych osiem obrazków pod wszystkimi k¹tami.
			for(int j = 0; j < 360; j++) {
				SDL_FreeSurface(m_apKnightShoesSurfacesAllAngles[i][j]);  // Buty.
				SDL_FreeSurface(m_apKnightSwordWalkSurfacesAllAngles[i][j]);  // Miecz.
				SDL_FreeSurface(m_apKnightBodyWalkSurfacesAllAngles[i][j]);  // Cia³o.
			}
	}
}

void CKnightMgr::ShowShoesprints(void) {
	std::vector<IKnight*>::iterator It;
	for(It = m_KnightsContainer.begin(); It != m_KnightsContainer.end(); ++It)
		(*It)->ShowShoesprints();
}

void CKnightMgr::ShowBacklights(void) {
	std::vector<IKnight*>::iterator It;
	for(It = m_KnightsContainer.begin(); It != m_KnightsContainer.end(); ++It)
		(*It)->ShowBacklight();
}

void CKnightMgr::ShowShoes(void) {
	std::vector<IKnight*>::iterator It;
	for(It = m_KnightsContainer.begin(); It != m_KnightsContainer.end(); ++It)
		(*It)->ShowShoes();
}

void CKnightMgr::ShowSwords(void) {
	std::vector<IKnight*>::iterator It;
	for(It = m_KnightsContainer.begin(); It != m_KnightsContainer.end(); ++It)
		(*It)->ShowSword();
}

void CKnightMgr::ShowBodies(void) {
	std::vector<IKnight*>::iterator It;
	for(It = m_KnightsContainer.begin(); It != m_KnightsContainer.end(); ++It)
		(*It)->ShowBody();
}

bool CKnightMgr::AddArmy(bool bAddUser, int iNumLeaders, int iNumSerfs, CVector* pvPos) {
	//-------------------------------------------------------
	// Sprawdzenie warunków koniecznych do stworzenia armii.
	//-------------------------------------------------------

	// Licznik ustawiaj¹cy identyfikatory nowo tworzonym rycerzom.
	// Wartoœci¹ pocz¹tkow¹ jest iloœæ istniej¹cych ju¿ rycerzy.
	short iIdCount = m_KnightsContainer.size();

	int iNumKnights = (static_cast<int>(bAddUser) + iNumLeaders + iNumSerfs);  // Liczba rycerzy do stworzenia.

	// Je¿eli maksymalna liczba armii zosta³a przekroczona, lub suma wszystkich rycerzy maj¹cych wejœæ
	// w sk³ad armii przekroczy³a maksymaln¹ liczbê rycerzy do stworzenia, lub nie bêdzie wystarczaj¹ca,
	// to nie uda siê stworzyæ armii. "iIdCount" aktualnie jest równy iloœci istniej¹cych ju¿ rycerzy;
	// odejmuj¹c jego wartoœæ od maksymalnej liczby rycerzy dowiadujemy siê ile jeszcze rycerzy mo¿na stworzyæ. 
	if((m_ArmiesContainer.size() > static_cast<unsigned>(g_DefaultsMgr.GetMaxArmies())) 
		 || (iNumKnights > (g_DefaultsMgr.GetMaxKnights() - iIdCount)) || (iNumKnights < 1)) {
		// Wyœwietlenie informacji o nieudanym stworzeniu armii.
		g_DebugWindow.Print("WARNING: Failed to form an army consisting of ", iNumKnights, " knights.");
		g_DebugWindow.Print("         Too much armies or wrong number of knights.");

		return false;
	}

	// Sprawdzenie czy wczeœniej nie zosta³ ju¿ utworzony u¿ytkownik. Nie mo¿e istnieæ kilku u¿ytkowników.
	if(bAddUser) {
		std::vector<IKnight*>::iterator It = m_KnightsContainer.begin();
		while(It != m_KnightsContainer.end()) {
			if(dynamic_cast<CUser*>(*It)) {
				// Wyœwietlenie informacji o nieudanym stworzeniu armii.
				g_DebugWindow.Print("WARNING: Failed to form an army consisting of ", iNumKnights, " knights.");
				g_DebugWindow.Print("         User can not be created again.");

				return false;
			}
			++It;
		}
	}

	//------------------------------------------------
	// Wylosowanie pozycji, jeœli nie zosta³a podana.
	//------------------------------------------------

	CVector vRandPos;  // Wylosowana pozycja.
	if(!pvPos) {
		// Je¿eli szerokoœæ œwiata jest mniejsza od wysokoœci.
		if(g_DefaultsMgr.GetAreaWidth() < g_DefaultsMgr.GetAreaHeight()) {
			// Obliczenie d³ugoœci obszaru, na którym bêdzie losowana pozycja.
			float fSection = (static_cast<float>(g_DefaultsMgr.GetAreaHeight()) 
												/ static_cast<float>(g_DefaultsMgr.GetMaxArmies()));

			// Wylosowanie pozycji.
			vRandPos.x = (Rand() * static_cast<float>(g_DefaultsMgr.GetAreaWidth()));
			vRandPos.y = ((Rand() * fSection) + (fSection * static_cast<float>(CArmy::ms_iArmyCount)));
		}
		else {  // W przeciwnym wypadku: szerokoœæ œwiata jest wiêksza od wysokoœci lub równa jej.
			// Obliczenie d³ugoœci obszaru, na którym bêdzie losowana pozycja.
			float fSection = (static_cast<float>(g_DefaultsMgr.GetAreaWidth()) 
												/ static_cast<float>(g_DefaultsMgr.GetMaxArmies()));

			// Wylosowanie pozycji.
			vRandPos.x = ((Rand() * fSection) + (fSection * static_cast<float>(CArmy::ms_iArmyCount)));
			vRandPos.y = (Rand() * static_cast<float>(g_DefaultsMgr.GetAreaHeight()));
		}
		pvPos = &vRandPos;  // Pozycji, na której zostanie stworzona armia, zostaje przypisana wylosowana pozycja.
	}

	//-------------------------
	// Utworzenie nowej armii.
	//-------------------------

	CArmy* pNewArmy = new CArmy;  // Utworzenie nowej armii, warunki konieczne do jej utworzenia zosta³y spe³nione.

	// Licznik tworzonych rycerzy. Do czego potrzebny? Mo¿e siê zdarzyæ, ¿e pomimo spe³nionych warunków,
	// obszar œwiata nie pomieœci³ ca³kowitej iloœci rycerzy maj¹cych wejœæ w sk³ad armii. Chcemy o tym wiedzieæ.
	int iKnightCount = 0;

	IKnight* pNewKnight;

	// Dodanie u¿ytkownika do armii.
	if(bAddUser) {
		pNewKnight = AddKnight(iIdCount, pvPos, pNewArmy->GetId(), IKnight::user);
		if(pNewKnight) {
			pNewArmy->AddTo(pNewKnight);
			iKnightCount++;
			iIdCount++;
		}
	}

	// Dodanie liderów do armii.
	for(int i = 0; i < iNumLeaders; i++) {
		pNewKnight = AddKnight(iIdCount, pvPos, pNewArmy->GetId(), IKnight::leader);
		if(pNewKnight) {
			pNewArmy->AddTo(pNewKnight);
			iKnightCount++;
			iIdCount++;
		}
	}

	// Dodanie poddanych do armii do armii.
	for(int i = 0; i < iNumSerfs; i++) {
		pNewKnight = AddKnight(iIdCount, pvPos, pNewArmy->GetId(), IKnight::serf);
		if(pNewKnight) {
			pNewArmy->AddTo(pNewKnight);
			iKnightCount++;
			iIdCount++;
		}
	}

	if(iKnightCount) {
		m_ArmiesContainer.push_back(pNewArmy);  // Umieszczenie nowo utworzonej armii w kontenerze.

		// Wyœwietlenie informacji o pozytywnym stworzeniu armii.
		g_DebugWindow.Print("Creating the Army ", pNewArmy->GetId(), ":");
		if(iKnightCount == iNumKnights)
			g_DebugWindow.Print("  All ", iNumKnights, " knights were created successfully.");
		else
			g_DebugWindow.Print("  WARNING: ", (iNumKnights - iKnightCount), " of ", iNumKnights,
													" knights had not been created.");

		return true;  // Armiê utworzono pomyœlnie.
	}
	delete pNewArmy;  // Usuniêcie armii, nie ma rycerzy, nie ma armii.

	// Wyœwietlenie informacji o nieudanym stworzeniu armii.
	g_DebugWindow.Print("WARNING: Failed to form an army consisting of ", iNumKnights, " knights.");
	g_DebugWindow.Print("         Probably not enough space.");

	return false;  // Tworzenie armii nie przebieg³o pomyœlnie.
}

void CKnightMgr::UpdateArmies(const Uint32 iDeltaTicks) {
	std::vector<CArmy*>::iterator It;
	for(It = m_ArmiesContainer.begin(); It != m_ArmiesContainer.end(); ++It)
		(*It)->Update(iDeltaTicks);
}

void CKnightMgr::HandleInput(void) { 
	g_User.HandleInput();  // Obs³uga zdarzeñ dla u¿ytkownika.

	//------------------------------------
	// Wyœwietlenie informacji o armiach.
	//------------------------------------

	// Je¿eli zostanie wciœniêta jedynka, zostan¹ wyœwietlone informacje na temat armii "0".
	if((g_EventMgr.GetEvent()->type == SDL_KEYDOWN) && (g_EventMgr.GetEvent()->key.keysym.sym == SDLK_1))
		if(m_ArmiesContainer.size())  // Je¿eli istnieje armia o identyfikatorze "0".
			m_ArmiesContainer[0]->PrintData();
		else
			g_DebugWindow.Print("WARNING: First Army does not exist.");

	// Je¿eli zostanie wciœniêta dwójka, zostan¹ wyœwietlone informacje na temat armii "1".
	else if((g_EventMgr.GetEvent()->type == SDL_KEYDOWN) && (g_EventMgr.GetEvent()->key.keysym.sym == SDLK_2))
		if(m_ArmiesContainer.size() > 1)  // Je¿eli istnieje armia o identyfikatorze "1".
			m_ArmiesContainer[1]->PrintData();
		else
			g_DebugWindow.Print("WARNING: Second Army does not exist.");

	// Je¿eli zostanie wciœniêta trójka, zostan¹ wyœwietlone informacje na temat armii "2".
	else if((g_EventMgr.GetEvent()->type == SDL_KEYDOWN) && (g_EventMgr.GetEvent()->key.keysym.sym == SDLK_3))
		if(m_ArmiesContainer.size() > 2)  // Je¿eli istnieje armia o identyfikatorze "2".
			m_ArmiesContainer[2]->PrintData();
		else
			g_DebugWindow.Print("WARNING: Third Army does not exist.");
}

IKnight* CKnightMgr::AddKnight(short iId, CVector* pvPos, short iArmyId, short iType) {
	CCircle KnightCollCircle(pvPos, g_DefaultsMgr.GetKnightRadius());  // Ko³o kolizji rycerza ustawione na podanej pozycji.
	bool bIsCollision, bIsOutWorld;  // Flagi: jest/nie ma kolizji, jest/nie jest poza obszarem œwiata.

	// Je¿eli utworzone ko³o koliduje z którymœ z obiektów lub wysz³o poza obszar œwiata,
	// to bêdziemy losowaæ kolejne pozycje w obszarze bêd¹cym wokó³ podanego po³o¿enia,
	// dopóki nie natrafimy na bezkolizyjn¹ pozycjê w obszarze œwiata. Obszar bêdziemy
	// powiêkszaæ stopniowo o optymaln¹ odleg³oœæ miêdzy rycerzami.
	if((bIsCollision = g_CollisionController.CheckCollisionWithObjects(&KnightCollCircle))
			|| (bIsOutWorld = g_CollisionController.IsOutWorld(&KnightCollCircle))) {
		int iHitCount, iExpansionCount = 0;  // Liczniki: prób znalezienia wolnej pozycji, powiêkszeñ obszaru losowania.

		// Maksymalna iloœæ powiêkszeñ obszaru losowania i optymalny odstêp miêdzy rycerzami jako typ ca³kowity.
		int iMaxExpantion, iSepDist = static_cast<int>(g_DefaultsMgr.GetSeparationDist());

		// Maksymalna liczba powiêkszeñ jest maksymaln¹ iloœci¹ odstêpów miêdzy rycerzami,
		// jak¹ da siê pomieœciæ w d³u¿szej krawêdzi œwiata, równolegle do niej.
		iMaxExpantion = ((g_DefaultsMgr.GetAreaWidth() > g_DefaultsMgr.GetAreaHeight()) ? 
									  (g_DefaultsMgr.GetAreaWidth() / iSepDist) : (g_DefaultsMgr.GetAreaHeight() / iSepDist));

		CVector vSniper;  // "Ramiê", które wyci¹gniête z podanego po³o¿enia wska¿e kandydata na bazkolizyjn¹ pozycjê.
		float fSniperMagnitude = 0.0f;  // Wielkoœæ "ramienia".

		// Dopóki bêdzie wystêpowa³a kolizja, lub ko³o bêdzie poza granicami œwiata,
		// i licznik powiêkszeñ obszaru nie przekroczy dopuszczalnej iloœci,
		// to obszar losowania bêdzie powiêkszany za ka¿dym razem o optymalny odstêp miêdzy rycerzami.
		while((bIsCollision || bIsOutWorld) && (iExpansionCount < iMaxExpantion)) {
			iHitCount = 0;  // Wyzerowanie licznika prób.

			// Próbujemy znaleŸæ bezkolizyjn¹ pozycjê w obecnym oddaleniu od podanej pozycji.
			// Kontynuujemy, dopóki licznik prób nie przekroczy maksymalnej iloœci i wystêpuje kolizja
			// ko³a o wylosowanej pozycji z istniej¹cymi obiektami, lub znajduje siê ono poza obszarem œwiata.
			while((iHitCount < 1000) && ((bIsCollision = g_CollisionController.CheckCollisionWithObjects(&KnightCollCircle))
					|| (bIsOutWorld = g_CollisionController.IsOutWorld(&KnightCollCircle)))) {
				vSniper.Set(Rand(), Rand());  // Wylosowanie sk³adowych "ramienia".

				// Zwiêkszenie losowoœci.
				if(Rand() > 0.5f)
					vSniper.x *= -1;
				if(Rand() > 0.5f)
					vSniper.y *= -1;

				// Okreœlenie d³ugoœci "ramienia".
				vSniper.SetMagnitude(fSniperMagnitude + (Rand() * g_DefaultsMgr.GetSeparationDist()));

				KnightCollCircle.pos = (*pvPos + vSniper);  // Wskazanie przez "ramiê" kandydata na bezkolizyjn¹ pozycjê.
				iHitCount++;  // Inkrementacja licznika prób;
			}
			fSniperMagnitude += g_DefaultsMgr.GetSeparationDist();  // Powiêkszenie "ramienia".
			iExpansionCount++;  // Inkrementacja licznika powiêkszeñ.
		}
	}

	// Sprawdzamy, czy ko³o nie koliduje z obiektami i nie znajduje siê poza œwiatem, pêtle mog³y zostaæ przerwane
	// ze wzglêdu na osi¹gniêcie przez liczniki wartoœci maksymalnej, nie przez odnalezienie odpowiedniej pozycji.
	if(!bIsCollision && !bIsOutWorld) {
		IKnight* pNewKnight;

		// Stworzenie odpowiedniego rycerza.
		switch(iType) {
			case IKnight::user: pNewKnight = new CUser(iId, &KnightCollCircle.pos, iArmyId);
													break;
			case IKnight::leader: pNewKnight = new CLeader(iId, &KnightCollCircle.pos, iArmyId);
														break;
			case IKnight::serf: pNewKnight = new CSerf(iId, &KnightCollCircle.pos, iArmyId);
													break;
			default: pNewKnight = NULL;
		}

		if(pNewKnight)  // Je¿eli typ by³ podany b³êdnie, to rycerz nie zosta³ stworzony. St¹d ta instrukcja warunkowa.
			m_KnightsContainer.push_back(pNewKnight);  // Umieszczenie stworzonego rycerza w kontenerze.
		return pNewKnight;  // Zwrócenie wskaŸnika na stworzonego rycerza.
	}
	return NULL;  // Nie uda³o siê stworzyæ rycerza.
}

bool CKnightMgr::LoadSurfaces(void) {
	CProgressBar ProgressBar;  // Utworzenie paska postêpu.

	// Tworzenie kolejnych powierzchni zawieraj¹cych odpowiednio poobracane obrazki.

	//------------------
	// Œlady po butach.
	//------------------

	CImage* pImage = g_ImageMgr.GetImage("knight_shoesprint_left");
	if(!pImage)
		return false;
	SDL_Surface* m_pKnightShoesprintLeftSurface = pImage->GetSurface();
	for(int i = 0; i < 360; i++) {
		if(g_DefaultsMgr.GetIsRotatedSurfaces()) {
			m_apKnightShoesprintLeftSurfacesAllAngles[i] = g_GraphicMgr.RotateSurface(m_pKnightShoesprintLeftSurface,
																																							  static_cast<float>(i));
			if(!m_apKnightShoesprintLeftSurfacesAllAngles[i])
				return false;

			// Aktualizacja paska postêpu.
			ProgressBar.SetProgress(static_cast<float>(i + 1) / 360.0f);

			// Wyœwietlenie paska postêpu na ekranie.
			ProgressBar.Show();
			g_AppWindow.FlipScreen();
		}
		else
			m_apKnightShoesprintLeftSurfacesAllAngles[i] = m_pKnightShoesprintLeftSurface;
	}
	ProgressBar.Reset();  // Zresetowanie paska postêpu.
	SDL_Surface* m_apKnightShoesprintLeftDisappearSurfaces[4];
	g_ImageMgr.GetImgSurfaces(m_apKnightShoesprintLeftDisappearSurfaces, 4, "knight_shoesprint_left_disappear_");
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 360; j++) {
			if(g_DefaultsMgr.GetIsRotatedSurfaces()) {
				m_apKnightShoesprintLeftDisappearSurfacesAllAngles[i][j] = 
					g_GraphicMgr.RotateSurface(m_apKnightShoesprintLeftDisappearSurfaces[i], static_cast<float>(j));
				if(!m_apKnightShoesprintLeftDisappearSurfacesAllAngles[i][j])
					return false;

				// Aktualizacja paska postêpu.
				ProgressBar.SetProgress(static_cast<float>(i * 360.0f + j + 1.0f) / (4.0f * 360.0f));

				// Wyœwietlenie paska postêpu na ekranie.
				ProgressBar.Show();
				g_AppWindow.FlipScreen();
			}
			else
				m_apKnightShoesprintLeftDisappearSurfacesAllAngles[i][j] = m_apKnightShoesprintLeftDisappearSurfaces[i];
		}
	ProgressBar.Reset();  // Zresetowanie paska postêpu.
	pImage = g_ImageMgr.GetImage("knight_shoesprint_right");
	if(!pImage)
		return false;
	SDL_Surface* m_pKnightShoesprintRightSurface = pImage->GetSurface();
	for(int i = 0; i < 360; i++) {
		if(g_DefaultsMgr.GetIsRotatedSurfaces()) {
			m_apKnightShoesprintRightSurfacesAllAngles[i] = g_GraphicMgr.RotateSurface(m_pKnightShoesprintRightSurface, 
																																								 static_cast<float>(i));
			if(!m_apKnightShoesprintRightSurfacesAllAngles[i])
				return false;

			// Aktualizacja paska postêpu.
			ProgressBar.SetProgress(static_cast<float>(i + 1) / 360.0f);

			// Wyœwietlenie paska postêpu na ekranie.
			ProgressBar.Show();
			g_AppWindow.FlipScreen();
		}
		else
			m_apKnightShoesprintRightSurfacesAllAngles[i] = m_pKnightShoesprintRightSurface;
	}
	ProgressBar.Reset();  // Zresetowanie paska postêpu.
	SDL_Surface* m_apKnightShoesprintRightDisappearSurfaces[4];
	g_ImageMgr.GetImgSurfaces(m_apKnightShoesprintRightDisappearSurfaces, 4, "knight_shoesprint_right_disappear_");
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 360; j++) {
			if(g_DefaultsMgr.GetIsRotatedSurfaces()) {
				m_apKnightShoesprintRightDisappearSurfacesAllAngles[i][j] = 
					g_GraphicMgr.RotateSurface(m_apKnightShoesprintRightDisappearSurfaces[i], static_cast<float>(j));
				if(!m_apKnightShoesprintRightDisappearSurfacesAllAngles[i][j])
					return false;

				// Aktualizacja paska postêpu.
				ProgressBar.SetProgress(static_cast<float>(i * 360.0f + j + 1.0f) / (4.0f * 360.0f));

				// Wyœwietlenie paska postêpu na ekranie.
				ProgressBar.Show();
				g_AppWindow.FlipScreen();
			}
			else
				m_apKnightShoesprintRightDisappearSurfacesAllAngles[i][j] = m_apKnightShoesprintRightDisappearSurfaces[i];
		}

	//----------------
	// Podœwietlenie.
	//----------------

	ProgressBar.Reset();  // Zresetowanie paska postêpu.
	pImage = g_ImageMgr.GetImage("commander_backlight_yellow");
	if(!pImage)
		return false;
	SDL_Surface* m_pCommanderBacklightYellowSurface = pImage->GetSurface();
	for(int i = 0; i < 360; i++) {
		if(g_DefaultsMgr.GetIsRotatedSurfaces()) {
			m_apCommanderBacklightYellowSurfaces[i] = g_GraphicMgr.RotateSurface(m_pCommanderBacklightYellowSurface, 
																																					 static_cast<float>(i));
			if(!m_apCommanderBacklightYellowSurfaces[i])
				return false;

			// Aktualizacja paska postêpu.
			ProgressBar.SetProgress(static_cast<float>(i + 1) / 360.0f);

			// Wyœwietlenie paska postêpu na ekranie.
			ProgressBar.Show();
			g_AppWindow.FlipScreen();
		}
		else
			m_apCommanderBacklightYellowSurfaces[i] = m_pCommanderBacklightYellowSurface;
	}
	ProgressBar.Reset();  // Zresetowanie paska postêpu.
	pImage = g_ImageMgr.GetImage("commander_backlight_blue");
	if(!pImage)
		return false;
	SDL_Surface* m_pCommanderBacklightBlueSurface = pImage->GetSurface();
	for(int i = 0; i < 360; i++) {
		if(g_DefaultsMgr.GetIsRotatedSurfaces()) {
			m_apCommanderBacklightBlueSurfaces[i] = g_GraphicMgr.RotateSurface(m_pCommanderBacklightBlueSurface, 
																																				 static_cast<float>(i));
			if(!m_apCommanderBacklightBlueSurfaces[i])
				return false;

			// Aktualizacja paska postêpu.
			ProgressBar.SetProgress(static_cast<float>(i + 1) / 360.0f);

			// Wyœwietlenie paska postêpu na ekranie.
			ProgressBar.Show();
			g_AppWindow.FlipScreen();
		}
		else
			m_apCommanderBacklightBlueSurfaces[i] = m_pCommanderBacklightBlueSurface;
	}
	ProgressBar.Reset();  // Zresetowanie paska postêpu.
	pImage = g_ImageMgr.GetImage("commander_backlight_green");
	if(!pImage)
		return false;
	SDL_Surface* m_pCommanderBacklightGreenSurface = pImage->GetSurface();
	for(int i = 0; i < 360; i++) {
		if(g_DefaultsMgr.GetIsRotatedSurfaces()) {
			m_apCommanderBacklightGreenSurfaces[i] = g_GraphicMgr.RotateSurface(m_pCommanderBacklightGreenSurface, 
																																				  static_cast<float>(i));
			if(!m_apCommanderBacklightGreenSurfaces[i])
				return false;

			// Aktualizacja paska postêpu.
			ProgressBar.SetProgress(static_cast<float>(i + 1) / 360.0f);

			// Wyœwietlenie paska postêpu na ekranie.
			ProgressBar.Show();
			g_AppWindow.FlipScreen();
		}
		else
			m_apCommanderBacklightGreenSurfaces[i] = m_pCommanderBacklightGreenSurface;
	}

	//-------
	// Buty.
	//-------

	ProgressBar.Reset();  // Zresetowanie paska postêpu.
	SDL_Surface* m_apKnightShoesSurfaces[8];
	g_ImageMgr.GetImgSurfaces(m_apKnightShoesSurfaces, 8, "knight_shoes_");
	for(int i = 0; i < 8; i++)
		for(int j = 0; j < 360; j++) {
			if(g_DefaultsMgr.GetIsRotatedSurfaces()) {
				m_apKnightShoesSurfacesAllAngles[i][j] = g_GraphicMgr.RotateSurface(m_apKnightShoesSurfaces[i], 
																																						static_cast<float>(j));
				if(!m_apKnightShoesSurfacesAllAngles[i][j])
					return false;

				// Aktualizacja paska postêpu.
				ProgressBar.SetProgress(static_cast<float>(i * 360.0f + j + 1.0f) / (8.0f * 360.0f));

				// Wyœwietlenie paska postêpu na ekranie.
				ProgressBar.Show();
				g_AppWindow.FlipScreen();
			}
			else
				m_apKnightShoesSurfacesAllAngles[i][j] = m_apKnightShoesSurfaces[i];
		}

	//--------
	// Miecz.
	//--------

	ProgressBar.Reset();  // Zresetowanie paska postêpu.
	pImage = g_ImageMgr.GetImage("knight_sword_stand");
	if(!pImage)
		return false;
	SDL_Surface* m_pKnightSwordStandSurface = pImage->GetSurface();
	for(int i = 0; i < 360; i++) {
		if(g_DefaultsMgr.GetIsRotatedSurfaces()) {
			m_apKnightSwordStandSurfacesAllAngles[i] = g_GraphicMgr.RotateSurface(m_pKnightSwordStandSurface, 
																																						static_cast<float>(i));
			if(!m_apKnightSwordStandSurfacesAllAngles[i])
				return false;

			// Aktualizacja paska postêpu.
			ProgressBar.SetProgress(static_cast<float>(i + 1) / 360.0f);

			// Wyœwietlenie paska postêpu na ekranie.
			ProgressBar.Show();
			g_AppWindow.FlipScreen();
		}
		else
			m_apKnightSwordStandSurfacesAllAngles[i] = m_pKnightSwordStandSurface;
	}
	ProgressBar.Reset();  // Zresetowanie paska postêpu.
	SDL_Surface* m_apKnightSwordWalkSurfaces[8];
	g_ImageMgr.GetImgSurfaces(m_apKnightSwordWalkSurfaces, 8, "knight_sword_walk_");
	for(int i = 0; i < 8; i++)
		for(int j = 0; j < 360; j++) {
			if(g_DefaultsMgr.GetIsRotatedSurfaces()) {
				m_apKnightSwordWalkSurfacesAllAngles[i][j] = g_GraphicMgr.RotateSurface(m_apKnightSwordWalkSurfaces[i], 
																																								static_cast<float>(j));
				if(!m_apKnightSwordWalkSurfacesAllAngles[i][j])
					return false;

				// Aktualizacja paska postêpu.
				ProgressBar.SetProgress(static_cast<float>(i * 360.0f + j + 1.0f) / (8.0f * 360.0f));

				// Wyœwietlenie paska postêpu na ekranie.
				ProgressBar.Show();
				g_AppWindow.FlipScreen();
			}
			else
				m_apKnightSwordWalkSurfacesAllAngles[i][j] = m_apKnightSwordWalkSurfaces[i];
		}

	//--------
	// Cia³o.
	//--------

	ProgressBar.Reset();  // Zresetowanie paska postêpu.
	pImage = g_ImageMgr.GetImage("knight_body_stand");
	if(!pImage)
		return false;
	SDL_Surface* m_pKnightBodyStandSurface = pImage->GetSurface();
	for(int i = 0; i < 360; i++) {
		if(g_DefaultsMgr.GetIsRotatedSurfaces()) {
			m_apKnightBodyStandSurfacesAllAngles[i] = g_GraphicMgr.RotateSurface(m_pKnightBodyStandSurface, 
																																					 static_cast<float>(i));
			if(!m_apKnightBodyStandSurfacesAllAngles[i])
				return false;

			// Aktualizacja paska postêpu.
			ProgressBar.SetProgress(static_cast<float>(i + 1) / 360.0f);

			// Wyœwietlenie paska postêpu na ekranie.
			ProgressBar.Show();
			g_AppWindow.FlipScreen();
		}
		else
			m_apKnightBodyStandSurfacesAllAngles[i] = m_pKnightBodyStandSurface;
	}
	ProgressBar.Reset();  // Zresetowanie paska postêpu.
	SDL_Surface* m_apKnightBodyWalkSurfaces[8];
	g_ImageMgr.GetImgSurfaces(m_apKnightBodyWalkSurfaces, 8, "knight_body_walk_");
	for(int i = 0; i < 8; i++)
		for(int j = 0; j < 360; j++) {
			if(g_DefaultsMgr.GetIsRotatedSurfaces()) {
				m_apKnightBodyWalkSurfacesAllAngles[i][j] = g_GraphicMgr.RotateSurface(m_apKnightBodyWalkSurfaces[i], 
																																							 static_cast<float>(j));
				if(!m_apKnightBodyWalkSurfacesAllAngles[i][j])
					return false;

				// Aktualizacja paska postêpu.
				ProgressBar.SetProgress(static_cast<float>(i * 360.0f + j + 1.0f) / (8.0f * 360.0f));

				// Wyœwietlenie paska postêpu na ekranie.
				ProgressBar.Show();
				g_AppWindow.FlipScreen();
			}
			else
				m_apKnightBodyWalkSurfacesAllAngles[i][j] = m_apKnightBodyWalkSurfaces[i];
		}

	if(!g_DefaultsMgr.GetIsRotatedSurfaces())
		g_DebugWindow.Print("WARNING: Surfaces have not been rotated.");
	return true;
}
