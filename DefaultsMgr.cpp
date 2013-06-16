#include "DefaultsMgr.h"

CDefaultsMgr::CDefaultsMgr(void) : m_iMaxArmies(3), m_iMaxKnights(30), m_iMaxComradesVisible(m_iMaxKnights / 2),
																	 m_fKnightRadius(33.0f), m_fPalisadeRadius(93.0f), m_fStoneRadius(23.0f),
																	 m_fMaxSpeed(160.0f), m_fDesiredSpeed(m_fMaxSpeed / 2.0f), m_fMinUrgency(0.05f),
																	 m_fMaxUrgency(0.1f), m_fMaxChange(m_fMaxSpeed * m_fMaxUrgency),
																	 m_fSeparationDist(110.0f), m_fEnemyKeepAwayDist(m_fKnightRadius * 3.5f),
																	 m_fObstacleKeepAwayDist(m_fKnightRadius * 0.5f), m_fDefaultPerceptionRange(5000.0f) {
	if(!ReadDefaults())  // Odczytanie pozosta³ych wartoœci domyœlnych z pliku.
		// W przypadku niepowodzenia, zatrzymanie programu ze stosownym komunikatem.
		assert(!"Failed to read the default values!");
}

bool CDefaultsMgr::ReadDefaults(void) {
	std::ifstream Defaults;  // Definicja egzemplarza obiektu klasy ifstream.
	Defaults.open("config.txt");  // Poinformowanie egzemplarza, z jakiego pliku ma p³yn¹æ strumieñ.
	if(!Defaults)  // Sprawdzenie, czy uda³o siê otworzyæ plik.
		return false;  // Zwrócenie fa³szu w przypadku niepowodzenia.
	std::string strWord;  // String, do którego bêd¹ wczytywane dane z pliku.

	//---------------------------------
	// Odczytanie wartoœci domyœlnych.
	//---------------------------------

	// W pliku "config.txt", w ka¿dym z wierszy, zawarte s¹ wartoœci domyœlne, zaraz po opisie danej wartoœci i dwukropku.
	// Wczytujemy do stringa kolejne wyrazy opisu, dopóki nie natrafimy na ostatni, koñcz¹cy siê ze znakiem dwukropka.
	// Wtedy odczytujemy wartoœæ domyœln¹ i zapisujemy j¹ do odpowiedniej zmiennej.

	// 1. Odczytanie szerokoœci ekranu.
	do {
		Defaults >> strWord;
	} while(strWord[strWord.length() - 1] != ':');
	Defaults >> m_iScreenWidth;
	if(m_iScreenWidth < 800)  // Szerokoœæ ekranu nie mo¿e byæ mniejsza od 800.
		m_iScreenWidth = 800;

	// 2. Odczytanie wysokoœci ekranu.
	do {
		Defaults >> strWord;
	} while(strWord[strWord.length() - 1] != ':');
	Defaults >> m_iScreenHeight;
	if(m_iScreenHeight < 450)  // Wysokoœæ ekranu nie mo¿e byæ mniejsza od 450.
		m_iScreenHeight = 450;

	// 3. Odczytanie szerokoœci œwiata.
	do {
		Defaults >> strWord;
	} while(strWord[strWord.length() - 1] != ':');
	Defaults >> m_iAreaWidth;
	if(m_iAreaWidth < m_iScreenWidth)  // Obszar œwiata nie mo¿e byæ mniejszy od ekranu.
		m_iAreaWidth = m_iScreenWidth;

	// 4. Odczytanie wysokoœci œwiata.
	do {
		Defaults >> strWord;
	} while(strWord[strWord.length() - 1] != ':');
	Defaults >> m_iAreaHeight;
	if(m_iAreaHeight < m_iScreenHeight)  // Obszar œwiata nie mo¿e byæ mniejszy od ekranu.
		m_iAreaHeight = m_iScreenHeight;

	// 5. Odczytanie iloœci armii do stworzenia.
	do {
		Defaults >> strWord;
	} while(strWord[strWord.length() - 1] != ':');
	Defaults >> m_iNumArmies;
	if(m_iNumArmies > m_iMaxArmies)  // Nie mo¿na stworzyæ wiêcej armii, ni¿ jest to dopuszczalne.
		m_iNumArmies = m_iMaxArmies;
	else if(m_iNumArmies < 1)  // Trzeba utworzyæ co najmniej jedn¹ armiê.
		m_iNumArmies = 1;

	// 6. Odczytanie iloœci liderów do stworzenia w ka¿dej z armii.
	do {
		Defaults >> strWord;
	} while(strWord[strWord.length() - 1] != ':');
	Defaults >> m_iNumLeadersArmy;
	if((m_iNumLeadersArmy * m_iNumArmies) > m_iMaxKnights)  // Nie mo¿na stworzyæ wiêcej rycerzy, ni¿ jest to dopuszczalne.
		m_iNumLeadersArmy = (m_iMaxKnights / m_iNumArmies);
	else if(m_iNumLeadersArmy < 1)
		m_iNumLeadersArmy = 1;  // W œwiecie musi istnieæ przynajmniej jeden lider, którego bêdzie reprezentowa³ u¿ytkownik.

	// 7. Odczytanie iloœci poddanych do stworzenia w ka¿dej z armii.

	do {
		Defaults >> strWord;
	} while(strWord[strWord.length() - 1] != ':');
	Defaults >> m_iNumSerfsArmy;

	// Nie mo¿na stworzyæ wiêcej rycerzy, ni¿ jest to dopuszczalne.
	if((m_iNumSerfsArmy * m_iNumArmies) > (m_iMaxKnights - (m_iNumLeadersArmy * m_iNumArmies)))
		m_iNumSerfsArmy = ((m_iMaxKnights - (m_iNumLeadersArmy * m_iNumArmies)) / m_iNumArmies);

	// 8. Odczytanie iloœci palisad do stworzenia.
	do {
		Defaults >> strWord;
	} while(strWord[strWord.length() - 1] != ':');
	Defaults >> m_iNumPalisades;

	// 9. Odczytanie iloœci kamieni do stworzenia.
	do {
		Defaults >> strWord;
	} while(strWord[strWord.length() - 1] != ':');
	Defaults >> m_iNumStones;

	// 10. Odczytanie informacji, czy obróciæ grafiki.
	do {
		Defaults >> strWord;
	} while(strWord[strWord.length() - 1] != ':');
	Defaults >> m_bIsRotatedSurfaces;

	// 11. Odczytanie informacji, czy wyœwietliæ œredni¹ liczbê klatek na sekundê.
	do {
		Defaults >> strWord;
	} while(strWord[strWord.length() - 1] != ':');
	Defaults >> m_bIsFPS;

	Defaults.close();  // Zamkniêcie strumienia.
	return true;  // Zwrócenie prawdy, pomyœlnie ustalono wartoœci domyœlne.
}
