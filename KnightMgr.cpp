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
			// Poniewa� elementy b�d�ce za elementem usuni�tym zostan� przesuni�te odpowiednio na jego miejsce,
			// nie ma potrzeby inkrementowania iteratora.
	}

	// Likwidacja wszystkich rycerzy.
	std::vector<IKnight*>::iterator ItK;
	for(ItK = m_KnightsContainer.begin(); ItK != m_KnightsContainer.end(); ) {
			delete *ItK;
			ItK = m_KnightsContainer.erase(ItK);
	}

	// Zwolnienie pami�ci zajmowanej przez poobracane obrazki.
  if(g_DefaultsMgr.GetIsRotatedSurfaces()) {  // Je�eli obrazki mia�y by� poobracane.
		for(int i = 0; i < 360; i++) {  // Wyczyszczenie jednowymiarowych tablic.
			// �lady po butach.
			SDL_FreeSurface(m_apKnightShoesprintLeftSurfacesAllAngles[i]);
			SDL_FreeSurface(m_apKnightShoesprintRightSurfacesAllAngles[i]);

			// Pod�wietlenie.
			SDL_FreeSurface(m_apCommanderBacklightYellowSurfaces[i]);
			SDL_FreeSurface(m_apCommanderBacklightBlueSurfaces[i]);
			SDL_FreeSurface(m_apCommanderBacklightGreenSurfaces[i]);

			SDL_FreeSurface(m_apKnightSwordStandSurfacesAllAngles[i]);  // Miecz.
			SDL_FreeSurface(m_apKnightBodyStandSurfacesAllAngles[i]);  // Cia�o.
		}

		// Wyczyszczenie tablic dwuwymiarowych...
		for(int i = 0; i < 4; i++)  // ...zawieraj�cych cztery obrazki pod wszystkimi k�tami.
			for(int j = 0; j < 360; j++) {
				// �lady po butach.
				SDL_FreeSurface(m_apKnightShoesprintLeftDisappearSurfacesAllAngles[i][j]);
				SDL_FreeSurface(m_apKnightShoesprintRightDisappearSurfacesAllAngles[i][j]);
			}
		for(int i = 0; i < 8; i++)  // ...zawieraj�cych osiem obrazk�w pod wszystkimi k�tami.
			for(int j = 0; j < 360; j++) {
				SDL_FreeSurface(m_apKnightShoesSurfacesAllAngles[i][j]);  // Buty.
				SDL_FreeSurface(m_apKnightSwordWalkSurfacesAllAngles[i][j]);  // Miecz.
				SDL_FreeSurface(m_apKnightBodyWalkSurfacesAllAngles[i][j]);  // Cia�o.
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
	// Sprawdzenie warunk�w koniecznych do stworzenia armii.
	//-------------------------------------------------------

	// Licznik ustawiaj�cy identyfikatory nowo tworzonym rycerzom.
	// Warto�ci� pocz�tkow� jest ilo�� istniej�cych ju� rycerzy.
	short iIdCount = m_KnightsContainer.size();

	int iNumKnights = (static_cast<int>(bAddUser) + iNumLeaders + iNumSerfs);  // Liczba rycerzy do stworzenia.

	// Je�eli maksymalna liczba armii zosta�a przekroczona, lub suma wszystkich rycerzy maj�cych wej��
	// w sk�ad armii przekroczy�a maksymaln� liczb� rycerzy do stworzenia, lub nie b�dzie wystarczaj�ca,
	// to nie uda si� stworzy� armii. "iIdCount" aktualnie jest r�wny ilo�ci istniej�cych ju� rycerzy;
	// odejmuj�c jego warto�� od maksymalnej liczby rycerzy dowiadujemy si� ile jeszcze rycerzy mo�na stworzy�. 
	if((m_ArmiesContainer.size() > static_cast<unsigned>(g_DefaultsMgr.GetMaxArmies())) 
		 || (iNumKnights > (g_DefaultsMgr.GetMaxKnights() - iIdCount)) || (iNumKnights < 1)) {
		// Wy�wietlenie informacji o nieudanym stworzeniu armii.
		g_DebugWindow.Print("WARNING: Failed to form an army consisting of ", iNumKnights, " knights.");
		g_DebugWindow.Print("         Too much armies or wrong number of knights.");

		return false;
	}

	// Sprawdzenie czy wcze�niej nie zosta� ju� utworzony u�ytkownik. Nie mo�e istnie� kilku u�ytkownik�w.
	if(bAddUser) {
		std::vector<IKnight*>::iterator It = m_KnightsContainer.begin();
		while(It != m_KnightsContainer.end()) {
			if(dynamic_cast<CUser*>(*It)) {
				// Wy�wietlenie informacji o nieudanym stworzeniu armii.
				g_DebugWindow.Print("WARNING: Failed to form an army consisting of ", iNumKnights, " knights.");
				g_DebugWindow.Print("         User can not be created again.");

				return false;
			}
			++It;
		}
	}

	//------------------------------------------------
	// Wylosowanie pozycji, je�li nie zosta�a podana.
	//------------------------------------------------

	CVector vRandPos;  // Wylosowana pozycja.
	if(!pvPos) {
		// Je�eli szeroko�� �wiata jest mniejsza od wysoko�ci.
		if(g_DefaultsMgr.GetAreaWidth() < g_DefaultsMgr.GetAreaHeight()) {
			// Obliczenie d�ugo�ci obszaru, na kt�rym b�dzie losowana pozycja.
			float fSection = (static_cast<float>(g_DefaultsMgr.GetAreaHeight()) 
												/ static_cast<float>(g_DefaultsMgr.GetMaxArmies()));

			// Wylosowanie pozycji.
			vRandPos.x = (Rand() * static_cast<float>(g_DefaultsMgr.GetAreaWidth()));
			vRandPos.y = ((Rand() * fSection) + (fSection * static_cast<float>(CArmy::ms_iArmyCount)));
		}
		else {  // W przeciwnym wypadku: szeroko�� �wiata jest wi�ksza od wysoko�ci lub r�wna jej.
			// Obliczenie d�ugo�ci obszaru, na kt�rym b�dzie losowana pozycja.
			float fSection = (static_cast<float>(g_DefaultsMgr.GetAreaWidth()) 
												/ static_cast<float>(g_DefaultsMgr.GetMaxArmies()));

			// Wylosowanie pozycji.
			vRandPos.x = ((Rand() * fSection) + (fSection * static_cast<float>(CArmy::ms_iArmyCount)));
			vRandPos.y = (Rand() * static_cast<float>(g_DefaultsMgr.GetAreaHeight()));
		}
		pvPos = &vRandPos;  // Pozycji, na kt�rej zostanie stworzona armia, zostaje przypisana wylosowana pozycja.
	}

	//-------------------------
	// Utworzenie nowej armii.
	//-------------------------

	CArmy* pNewArmy = new CArmy;  // Utworzenie nowej armii, warunki konieczne do jej utworzenia zosta�y spe�nione.

	// Licznik tworzonych rycerzy. Do czego potrzebny? Mo�e si� zdarzy�, �e pomimo spe�nionych warunk�w,
	// obszar �wiata nie pomie�ci� ca�kowitej ilo�ci rycerzy maj�cych wej�� w sk�ad armii. Chcemy o tym wiedzie�.
	int iKnightCount = 0;

	IKnight* pNewKnight;

	// Dodanie u�ytkownika do armii.
	if(bAddUser) {
		pNewKnight = AddKnight(iIdCount, pvPos, pNewArmy->GetId(), IKnight::user);
		if(pNewKnight) {
			pNewArmy->AddTo(pNewKnight);
			iKnightCount++;
			iIdCount++;
		}
	}

	// Dodanie lider�w do armii.
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

		// Wy�wietlenie informacji o pozytywnym stworzeniu armii.
		g_DebugWindow.Print("Creating the Army ", pNewArmy->GetId(), ":");
		if(iKnightCount == iNumKnights)
			g_DebugWindow.Print("  All ", iNumKnights, " knights were created successfully.");
		else
			g_DebugWindow.Print("  WARNING: ", (iNumKnights - iKnightCount), " of ", iNumKnights,
													" knights had not been created.");

		return true;  // Armi� utworzono pomy�lnie.
	}
	delete pNewArmy;  // Usuni�cie armii, nie ma rycerzy, nie ma armii.

	// Wy�wietlenie informacji o nieudanym stworzeniu armii.
	g_DebugWindow.Print("WARNING: Failed to form an army consisting of ", iNumKnights, " knights.");
	g_DebugWindow.Print("         Probably not enough space.");

	return false;  // Tworzenie armii nie przebieg�o pomy�lnie.
}

void CKnightMgr::UpdateArmies(const Uint32 iDeltaTicks) {
	std::vector<CArmy*>::iterator It;
	for(It = m_ArmiesContainer.begin(); It != m_ArmiesContainer.end(); ++It)
		(*It)->Update(iDeltaTicks);
}

void CKnightMgr::HandleInput(void) { 
	g_User.HandleInput();  // Obs�uga zdarze� dla u�ytkownika.

	//------------------------------------
	// Wy�wietlenie informacji o armiach.
	//------------------------------------

	// Je�eli zostanie wci�ni�ta jedynka, zostan� wy�wietlone informacje na temat armii "0".
	if((g_EventMgr.GetEvent()->type == SDL_KEYDOWN) && (g_EventMgr.GetEvent()->key.keysym.sym == SDLK_1))
		if(m_ArmiesContainer.size())  // Je�eli istnieje armia o identyfikatorze "0".
			m_ArmiesContainer[0]->PrintData();
		else
			g_DebugWindow.Print("WARNING: First Army does not exist.");

	// Je�eli zostanie wci�ni�ta dw�jka, zostan� wy�wietlone informacje na temat armii "1".
	else if((g_EventMgr.GetEvent()->type == SDL_KEYDOWN) && (g_EventMgr.GetEvent()->key.keysym.sym == SDLK_2))
		if(m_ArmiesContainer.size() > 1)  // Je�eli istnieje armia o identyfikatorze "1".
			m_ArmiesContainer[1]->PrintData();
		else
			g_DebugWindow.Print("WARNING: Second Army does not exist.");

	// Je�eli zostanie wci�ni�ta tr�jka, zostan� wy�wietlone informacje na temat armii "2".
	else if((g_EventMgr.GetEvent()->type == SDL_KEYDOWN) && (g_EventMgr.GetEvent()->key.keysym.sym == SDLK_3))
		if(m_ArmiesContainer.size() > 2)  // Je�eli istnieje armia o identyfikatorze "2".
			m_ArmiesContainer[2]->PrintData();
		else
			g_DebugWindow.Print("WARNING: Third Army does not exist.");
}

IKnight* CKnightMgr::AddKnight(short iId, CVector* pvPos, short iArmyId, short iType) {
	CCircle KnightCollCircle(pvPos, g_DefaultsMgr.GetKnightRadius());  // Ko�o kolizji rycerza ustawione na podanej pozycji.
	bool bIsCollision, bIsOutWorld;  // Flagi: jest/nie ma kolizji, jest/nie jest poza obszarem �wiata.

	// Je�eli utworzone ko�o koliduje z kt�rym� z obiekt�w lub wysz�o poza obszar �wiata,
	// to b�dziemy losowa� kolejne pozycje w obszarze b�d�cym wok� podanego po�o�enia,
	// dop�ki nie natrafimy na bezkolizyjn� pozycj� w obszarze �wiata. Obszar b�dziemy
	// powi�ksza� stopniowo o optymaln� odleg�o�� mi�dzy rycerzami.
	if((bIsCollision = g_CollisionController.CheckCollisionWithObjects(&KnightCollCircle))
			|| (bIsOutWorld = g_CollisionController.IsOutWorld(&KnightCollCircle))) {
		int iHitCount, iExpansionCount = 0;  // Liczniki: pr�b znalezienia wolnej pozycji, powi�ksze� obszaru losowania.

		// Maksymalna ilo�� powi�ksze� obszaru losowania i optymalny odst�p mi�dzy rycerzami jako typ ca�kowity.
		int iMaxExpantion, iSepDist = static_cast<int>(g_DefaultsMgr.GetSeparationDist());

		// Maksymalna liczba powi�ksze� jest maksymaln� ilo�ci� odst�p�w mi�dzy rycerzami,
		// jak� da si� pomie�ci� w d�u�szej kraw�dzi �wiata, r�wnolegle do niej.
		iMaxExpantion = ((g_DefaultsMgr.GetAreaWidth() > g_DefaultsMgr.GetAreaHeight()) ? 
									  (g_DefaultsMgr.GetAreaWidth() / iSepDist) : (g_DefaultsMgr.GetAreaHeight() / iSepDist));

		CVector vSniper;  // "Rami�", kt�re wyci�gni�te z podanego po�o�enia wska�e kandydata na bazkolizyjn� pozycj�.
		float fSniperMagnitude = 0.0f;  // Wielko�� "ramienia".

		// Dop�ki b�dzie wyst�powa�a kolizja, lub ko�o b�dzie poza granicami �wiata,
		// i licznik powi�ksze� obszaru nie przekroczy dopuszczalnej ilo�ci,
		// to obszar losowania b�dzie powi�kszany za ka�dym razem o optymalny odst�p mi�dzy rycerzami.
		while((bIsCollision || bIsOutWorld) && (iExpansionCount < iMaxExpantion)) {
			iHitCount = 0;  // Wyzerowanie licznika pr�b.

			// Pr�bujemy znale�� bezkolizyjn� pozycj� w obecnym oddaleniu od podanej pozycji.
			// Kontynuujemy, dop�ki licznik pr�b nie przekroczy maksymalnej ilo�ci i wyst�puje kolizja
			// ko�a o wylosowanej pozycji z istniej�cymi obiektami, lub znajduje si� ono poza obszarem �wiata.
			while((iHitCount < 1000) && ((bIsCollision = g_CollisionController.CheckCollisionWithObjects(&KnightCollCircle))
					|| (bIsOutWorld = g_CollisionController.IsOutWorld(&KnightCollCircle)))) {
				vSniper.Set(Rand(), Rand());  // Wylosowanie sk�adowych "ramienia".

				// Zwi�kszenie losowo�ci.
				if(Rand() > 0.5f)
					vSniper.x *= -1;
				if(Rand() > 0.5f)
					vSniper.y *= -1;

				// Okre�lenie d�ugo�ci "ramienia".
				vSniper.SetMagnitude(fSniperMagnitude + (Rand() * g_DefaultsMgr.GetSeparationDist()));

				KnightCollCircle.pos = (*pvPos + vSniper);  // Wskazanie przez "rami�" kandydata na bezkolizyjn� pozycj�.
				iHitCount++;  // Inkrementacja licznika pr�b;
			}
			fSniperMagnitude += g_DefaultsMgr.GetSeparationDist();  // Powi�kszenie "ramienia".
			iExpansionCount++;  // Inkrementacja licznika powi�ksze�.
		}
	}

	// Sprawdzamy, czy ko�o nie koliduje z obiektami i nie znajduje si� poza �wiatem, p�tle mog�y zosta� przerwane
	// ze wzgl�du na osi�gni�cie przez liczniki warto�ci maksymalnej, nie przez odnalezienie odpowiedniej pozycji.
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

		if(pNewKnight)  // Je�eli typ by� podany b��dnie, to rycerz nie zosta� stworzony. St�d ta instrukcja warunkowa.
			m_KnightsContainer.push_back(pNewKnight);  // Umieszczenie stworzonego rycerza w kontenerze.
		return pNewKnight;  // Zwr�cenie wska�nika na stworzonego rycerza.
	}
	return NULL;  // Nie uda�o si� stworzy� rycerza.
}

bool CKnightMgr::LoadSurfaces(void) {
	CProgressBar ProgressBar;  // Utworzenie paska post�pu.

	// Tworzenie kolejnych powierzchni zawieraj�cych odpowiednio poobracane obrazki.

	//------------------
	// �lady po butach.
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

			// Aktualizacja paska post�pu.
			ProgressBar.SetProgress(static_cast<float>(i + 1) / 360.0f);

			// Wy�wietlenie paska post�pu na ekranie.
			ProgressBar.Show();
			g_AppWindow.FlipScreen();
		}
		else
			m_apKnightShoesprintLeftSurfacesAllAngles[i] = m_pKnightShoesprintLeftSurface;
	}
	ProgressBar.Reset();  // Zresetowanie paska post�pu.
	SDL_Surface* m_apKnightShoesprintLeftDisappearSurfaces[4];
	g_ImageMgr.GetImgSurfaces(m_apKnightShoesprintLeftDisappearSurfaces, 4, "knight_shoesprint_left_disappear_");
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 360; j++) {
			if(g_DefaultsMgr.GetIsRotatedSurfaces()) {
				m_apKnightShoesprintLeftDisappearSurfacesAllAngles[i][j] = 
					g_GraphicMgr.RotateSurface(m_apKnightShoesprintLeftDisappearSurfaces[i], static_cast<float>(j));
				if(!m_apKnightShoesprintLeftDisappearSurfacesAllAngles[i][j])
					return false;

				// Aktualizacja paska post�pu.
				ProgressBar.SetProgress(static_cast<float>(i * 360.0f + j + 1.0f) / (4.0f * 360.0f));

				// Wy�wietlenie paska post�pu na ekranie.
				ProgressBar.Show();
				g_AppWindow.FlipScreen();
			}
			else
				m_apKnightShoesprintLeftDisappearSurfacesAllAngles[i][j] = m_apKnightShoesprintLeftDisappearSurfaces[i];
		}
	ProgressBar.Reset();  // Zresetowanie paska post�pu.
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

			// Aktualizacja paska post�pu.
			ProgressBar.SetProgress(static_cast<float>(i + 1) / 360.0f);

			// Wy�wietlenie paska post�pu na ekranie.
			ProgressBar.Show();
			g_AppWindow.FlipScreen();
		}
		else
			m_apKnightShoesprintRightSurfacesAllAngles[i] = m_pKnightShoesprintRightSurface;
	}
	ProgressBar.Reset();  // Zresetowanie paska post�pu.
	SDL_Surface* m_apKnightShoesprintRightDisappearSurfaces[4];
	g_ImageMgr.GetImgSurfaces(m_apKnightShoesprintRightDisappearSurfaces, 4, "knight_shoesprint_right_disappear_");
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 360; j++) {
			if(g_DefaultsMgr.GetIsRotatedSurfaces()) {
				m_apKnightShoesprintRightDisappearSurfacesAllAngles[i][j] = 
					g_GraphicMgr.RotateSurface(m_apKnightShoesprintRightDisappearSurfaces[i], static_cast<float>(j));
				if(!m_apKnightShoesprintRightDisappearSurfacesAllAngles[i][j])
					return false;

				// Aktualizacja paska post�pu.
				ProgressBar.SetProgress(static_cast<float>(i * 360.0f + j + 1.0f) / (4.0f * 360.0f));

				// Wy�wietlenie paska post�pu na ekranie.
				ProgressBar.Show();
				g_AppWindow.FlipScreen();
			}
			else
				m_apKnightShoesprintRightDisappearSurfacesAllAngles[i][j] = m_apKnightShoesprintRightDisappearSurfaces[i];
		}

	//----------------
	// Pod�wietlenie.
	//----------------

	ProgressBar.Reset();  // Zresetowanie paska post�pu.
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

			// Aktualizacja paska post�pu.
			ProgressBar.SetProgress(static_cast<float>(i + 1) / 360.0f);

			// Wy�wietlenie paska post�pu na ekranie.
			ProgressBar.Show();
			g_AppWindow.FlipScreen();
		}
		else
			m_apCommanderBacklightYellowSurfaces[i] = m_pCommanderBacklightYellowSurface;
	}
	ProgressBar.Reset();  // Zresetowanie paska post�pu.
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

			// Aktualizacja paska post�pu.
			ProgressBar.SetProgress(static_cast<float>(i + 1) / 360.0f);

			// Wy�wietlenie paska post�pu na ekranie.
			ProgressBar.Show();
			g_AppWindow.FlipScreen();
		}
		else
			m_apCommanderBacklightBlueSurfaces[i] = m_pCommanderBacklightBlueSurface;
	}
	ProgressBar.Reset();  // Zresetowanie paska post�pu.
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

			// Aktualizacja paska post�pu.
			ProgressBar.SetProgress(static_cast<float>(i + 1) / 360.0f);

			// Wy�wietlenie paska post�pu na ekranie.
			ProgressBar.Show();
			g_AppWindow.FlipScreen();
		}
		else
			m_apCommanderBacklightGreenSurfaces[i] = m_pCommanderBacklightGreenSurface;
	}

	//-------
	// Buty.
	//-------

	ProgressBar.Reset();  // Zresetowanie paska post�pu.
	SDL_Surface* m_apKnightShoesSurfaces[8];
	g_ImageMgr.GetImgSurfaces(m_apKnightShoesSurfaces, 8, "knight_shoes_");
	for(int i = 0; i < 8; i++)
		for(int j = 0; j < 360; j++) {
			if(g_DefaultsMgr.GetIsRotatedSurfaces()) {
				m_apKnightShoesSurfacesAllAngles[i][j] = g_GraphicMgr.RotateSurface(m_apKnightShoesSurfaces[i], 
																																						static_cast<float>(j));
				if(!m_apKnightShoesSurfacesAllAngles[i][j])
					return false;

				// Aktualizacja paska post�pu.
				ProgressBar.SetProgress(static_cast<float>(i * 360.0f + j + 1.0f) / (8.0f * 360.0f));

				// Wy�wietlenie paska post�pu na ekranie.
				ProgressBar.Show();
				g_AppWindow.FlipScreen();
			}
			else
				m_apKnightShoesSurfacesAllAngles[i][j] = m_apKnightShoesSurfaces[i];
		}

	//--------
	// Miecz.
	//--------

	ProgressBar.Reset();  // Zresetowanie paska post�pu.
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

			// Aktualizacja paska post�pu.
			ProgressBar.SetProgress(static_cast<float>(i + 1) / 360.0f);

			// Wy�wietlenie paska post�pu na ekranie.
			ProgressBar.Show();
			g_AppWindow.FlipScreen();
		}
		else
			m_apKnightSwordStandSurfacesAllAngles[i] = m_pKnightSwordStandSurface;
	}
	ProgressBar.Reset();  // Zresetowanie paska post�pu.
	SDL_Surface* m_apKnightSwordWalkSurfaces[8];
	g_ImageMgr.GetImgSurfaces(m_apKnightSwordWalkSurfaces, 8, "knight_sword_walk_");
	for(int i = 0; i < 8; i++)
		for(int j = 0; j < 360; j++) {
			if(g_DefaultsMgr.GetIsRotatedSurfaces()) {
				m_apKnightSwordWalkSurfacesAllAngles[i][j] = g_GraphicMgr.RotateSurface(m_apKnightSwordWalkSurfaces[i], 
																																								static_cast<float>(j));
				if(!m_apKnightSwordWalkSurfacesAllAngles[i][j])
					return false;

				// Aktualizacja paska post�pu.
				ProgressBar.SetProgress(static_cast<float>(i * 360.0f + j + 1.0f) / (8.0f * 360.0f));

				// Wy�wietlenie paska post�pu na ekranie.
				ProgressBar.Show();
				g_AppWindow.FlipScreen();
			}
			else
				m_apKnightSwordWalkSurfacesAllAngles[i][j] = m_apKnightSwordWalkSurfaces[i];
		}

	//--------
	// Cia�o.
	//--------

	ProgressBar.Reset();  // Zresetowanie paska post�pu.
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

			// Aktualizacja paska post�pu.
			ProgressBar.SetProgress(static_cast<float>(i + 1) / 360.0f);

			// Wy�wietlenie paska post�pu na ekranie.
			ProgressBar.Show();
			g_AppWindow.FlipScreen();
		}
		else
			m_apKnightBodyStandSurfacesAllAngles[i] = m_pKnightBodyStandSurface;
	}
	ProgressBar.Reset();  // Zresetowanie paska post�pu.
	SDL_Surface* m_apKnightBodyWalkSurfaces[8];
	g_ImageMgr.GetImgSurfaces(m_apKnightBodyWalkSurfaces, 8, "knight_body_walk_");
	for(int i = 0; i < 8; i++)
		for(int j = 0; j < 360; j++) {
			if(g_DefaultsMgr.GetIsRotatedSurfaces()) {
				m_apKnightBodyWalkSurfacesAllAngles[i][j] = g_GraphicMgr.RotateSurface(m_apKnightBodyWalkSurfaces[i], 
																																							 static_cast<float>(j));
				if(!m_apKnightBodyWalkSurfacesAllAngles[i][j])
					return false;

				// Aktualizacja paska post�pu.
				ProgressBar.SetProgress(static_cast<float>(i * 360.0f + j + 1.0f) / (8.0f * 360.0f));

				// Wy�wietlenie paska post�pu na ekranie.
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
