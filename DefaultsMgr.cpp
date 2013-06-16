#include "DefaultsMgr.h"

CDefaultsMgr::CDefaultsMgr(void) : m_iMaxArmies(3), m_iMaxKnights(30), m_iMaxComradesVisible(m_iMaxKnights / 2),
																	 m_fKnightRadius(33.0f), m_fPalisadeRadius(93.0f), m_fStoneRadius(23.0f),
																	 m_fMaxSpeed(160.0f), m_fDesiredSpeed(m_fMaxSpeed / 2.0f), m_fMinUrgency(0.05f),
																	 m_fMaxUrgency(0.1f), m_fMaxChange(m_fMaxSpeed * m_fMaxUrgency),
																	 m_fSeparationDist(110.0f), m_fEnemyKeepAwayDist(m_fKnightRadius * 3.5f),
																	 m_fObstacleKeepAwayDist(m_fKnightRadius * 0.5f), m_fDefaultPerceptionRange(5000.0f) {
	if(!ReadDefaults())  // Odczytanie pozosta�ych warto�ci domy�lnych z pliku.
		// W przypadku niepowodzenia, zatrzymanie programu ze stosownym komunikatem.
		assert(!"Failed to read the default values!");
}

bool CDefaultsMgr::ReadDefaults(void) {
	std::ifstream Defaults;  // Definicja egzemplarza obiektu klasy ifstream.
	Defaults.open("config.txt");  // Poinformowanie egzemplarza, z jakiego pliku ma p�yn�� strumie�.
	if(!Defaults)  // Sprawdzenie, czy uda�o si� otworzy� plik.
		return false;  // Zwr�cenie fa�szu w przypadku niepowodzenia.
	std::string strWord;  // String, do kt�rego b�d� wczytywane dane z pliku.

	//---------------------------------
	// Odczytanie warto�ci domy�lnych.
	//---------------------------------

	// W pliku "config.txt", w ka�dym z wierszy, zawarte s� warto�ci domy�lne, zaraz po opisie danej warto�ci i dwukropku.
	// Wczytujemy do stringa kolejne wyrazy opisu, dop�ki nie natrafimy na ostatni, ko�cz�cy si� ze znakiem dwukropka.
	// Wtedy odczytujemy warto�� domy�ln� i zapisujemy j� do odpowiedniej zmiennej.

	// 1. Odczytanie szeroko�ci ekranu.
	do {
		Defaults >> strWord;
	} while(strWord[strWord.length() - 1] != ':');
	Defaults >> m_iScreenWidth;
	if(m_iScreenWidth < 800)  // Szeroko�� ekranu nie mo�e by� mniejsza od 800.
		m_iScreenWidth = 800;

	// 2. Odczytanie wysoko�ci ekranu.
	do {
		Defaults >> strWord;
	} while(strWord[strWord.length() - 1] != ':');
	Defaults >> m_iScreenHeight;
	if(m_iScreenHeight < 450)  // Wysoko�� ekranu nie mo�e by� mniejsza od 450.
		m_iScreenHeight = 450;

	// 3. Odczytanie szeroko�ci �wiata.
	do {
		Defaults >> strWord;
	} while(strWord[strWord.length() - 1] != ':');
	Defaults >> m_iAreaWidth;
	if(m_iAreaWidth < m_iScreenWidth)  // Obszar �wiata nie mo�e by� mniejszy od ekranu.
		m_iAreaWidth = m_iScreenWidth;

	// 4. Odczytanie wysoko�ci �wiata.
	do {
		Defaults >> strWord;
	} while(strWord[strWord.length() - 1] != ':');
	Defaults >> m_iAreaHeight;
	if(m_iAreaHeight < m_iScreenHeight)  // Obszar �wiata nie mo�e by� mniejszy od ekranu.
		m_iAreaHeight = m_iScreenHeight;

	// 5. Odczytanie ilo�ci armii do stworzenia.
	do {
		Defaults >> strWord;
	} while(strWord[strWord.length() - 1] != ':');
	Defaults >> m_iNumArmies;
	if(m_iNumArmies > m_iMaxArmies)  // Nie mo�na stworzy� wi�cej armii, ni� jest to dopuszczalne.
		m_iNumArmies = m_iMaxArmies;
	else if(m_iNumArmies < 1)  // Trzeba utworzy� co najmniej jedn� armi�.
		m_iNumArmies = 1;

	// 6. Odczytanie ilo�ci lider�w do stworzenia w ka�dej z armii.
	do {
		Defaults >> strWord;
	} while(strWord[strWord.length() - 1] != ':');
	Defaults >> m_iNumLeadersArmy;
	if((m_iNumLeadersArmy * m_iNumArmies) > m_iMaxKnights)  // Nie mo�na stworzy� wi�cej rycerzy, ni� jest to dopuszczalne.
		m_iNumLeadersArmy = (m_iMaxKnights / m_iNumArmies);
	else if(m_iNumLeadersArmy < 1)
		m_iNumLeadersArmy = 1;  // W �wiecie musi istnie� przynajmniej jeden lider, kt�rego b�dzie reprezentowa� u�ytkownik.

	// 7. Odczytanie ilo�ci poddanych do stworzenia w ka�dej z armii.

	do {
		Defaults >> strWord;
	} while(strWord[strWord.length() - 1] != ':');
	Defaults >> m_iNumSerfsArmy;

	// Nie mo�na stworzy� wi�cej rycerzy, ni� jest to dopuszczalne.
	if((m_iNumSerfsArmy * m_iNumArmies) > (m_iMaxKnights - (m_iNumLeadersArmy * m_iNumArmies)))
		m_iNumSerfsArmy = ((m_iMaxKnights - (m_iNumLeadersArmy * m_iNumArmies)) / m_iNumArmies);

	// 8. Odczytanie ilo�ci palisad do stworzenia.
	do {
		Defaults >> strWord;
	} while(strWord[strWord.length() - 1] != ':');
	Defaults >> m_iNumPalisades;

	// 9. Odczytanie ilo�ci kamieni do stworzenia.
	do {
		Defaults >> strWord;
	} while(strWord[strWord.length() - 1] != ':');
	Defaults >> m_iNumStones;

	// 10. Odczytanie informacji, czy obr�ci� grafiki.
	do {
		Defaults >> strWord;
	} while(strWord[strWord.length() - 1] != ':');
	Defaults >> m_bIsRotatedSurfaces;

	// 11. Odczytanie informacji, czy wy�wietli� �redni� liczb� klatek na sekund�.
	do {
		Defaults >> strWord;
	} while(strWord[strWord.length() - 1] != ':');
	Defaults >> m_bIsFPS;

	Defaults.close();  // Zamkni�cie strumienia.
	return true;  // Zwr�cenie prawdy, pomy�lnie ustalono warto�ci domy�lne.
}
