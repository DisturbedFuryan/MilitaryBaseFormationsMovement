#pragma once
#include "Image.h"
#include "SDL.h"
#include "Singleton.h"
#include <cassert>
#include <sstream>
#include <string>
#include <vector>

class CImageMgr : public Singleton<CImageMgr> {
public:
	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	CImageMgr(void);
	~CImageMgr(void);

	//-------------------
	// Metody dostêpowe.
	//-------------------

	// Pobiera powierzchnie kilku obrazków do tablicy podanej w argumencie.
	// Do czego mo¿e siê przydaæ? Powiedzmy, ¿e istniej¹ dwa obrazki: obrazek_01 i obrazek_02. Obydwa maj¹ takie same nazwy,
	// poza koñcówk¹ - numerem. W takim przypadku wystarczy podaæ jako argumenty: wskaŸnik do tablicy,
	// w której zostan¹ umieszczone ich powierzchnie, iloœæ obrazków, i nazwê bez numeru.
	// W ten wygodny sposób otrzymamy powierzchnie obydwu obrazków, bez potrzeby wyszukiwania ka¿dej z osobna.
	void GetImgSurfaces(SDL_Surface* apSurfaces[], int iQuantity, std::string strName);

	CImage* GetImage(std::string strName);
private:
	//----------
	// Zmienne.
	//----------

	std::vector<CImage*> m_ImagesContainer;  // Kontener obrazków.

	//---------
	// Metody.
	//---------

	// Tworzy obrazek. strPath - œcie¿ka dostêpu do grafiki, strName - nazwa pliku PNG podana bez rozszerzenia.
	void AddImage(std::string strPath, std::string strName);

	// Tworzy kilka obrazków. Grafiki musz¹ mieæ t¹ sam¹ œcie¿kê dostêpu i nazwê ró¿ni¹c¹ siê tylko koñcówk¹,
	// bêd¹c¹ numerem obrazka. Przyk³adowo, chc¹c stworzyæ obrazki z dwóch plików graficznych, z tego samego katalagu,
	// o nazwach: grafika_01.png, grafika_02.png, wygodnie bêdzie pos³u¿yæ siê t¹ metod¹.
	// W tym przypadku argumenty wygl¹da³yby nastêpuj¹co: "jakas/sciezka/", "grafika_", 2.
	// Uwaga! Nazwy plików musz¹ mieæ na koñcu dwucyfrowy numer, a numeracja powinna siê zaczynaæ od "01".
	void AddSeriesOfImages(std::string strImgSeriesPath, std::string strTemplateName, int iQuantity);

	void CreateAllImages(void);  // Tworzy wszystkie obrazki wystêpuj¹ce w aplikacji.
};
