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
	// Metody dost�powe.
	//-------------------

	// Pobiera powierzchnie kilku obrazk�w do tablicy podanej w argumencie.
	// Do czego mo�e si� przyda�? Powiedzmy, �e istniej� dwa obrazki: obrazek_01 i obrazek_02. Obydwa maj� takie same nazwy,
	// poza ko�c�wk� - numerem. W takim przypadku wystarczy poda� jako argumenty: wska�nik do tablicy,
	// w kt�rej zostan� umieszczone ich powierzchnie, ilo�� obrazk�w, i nazw� bez numeru.
	// W ten wygodny spos�b otrzymamy powierzchnie obydwu obrazk�w, bez potrzeby wyszukiwania ka�dej z osobna.
	void GetImgSurfaces(SDL_Surface* apSurfaces[], int iQuantity, std::string strName);

	CImage* GetImage(std::string strName);
private:
	//----------
	// Zmienne.
	//----------

	std::vector<CImage*> m_ImagesContainer;  // Kontener obrazk�w.

	//---------
	// Metody.
	//---------

	// Tworzy obrazek. strPath - �cie�ka dost�pu do grafiki, strName - nazwa pliku PNG podana bez rozszerzenia.
	void AddImage(std::string strPath, std::string strName);

	// Tworzy kilka obrazk�w. Grafiki musz� mie� t� sam� �cie�k� dost�pu i nazw� r�ni�c� si� tylko ko�c�wk�,
	// b�d�c� numerem obrazka. Przyk�adowo, chc�c stworzy� obrazki z dw�ch plik�w graficznych, z tego samego katalagu,
	// o nazwach: grafika_01.png, grafika_02.png, wygodnie b�dzie pos�u�y� si� t� metod�.
	// W tym przypadku argumenty wygl�da�yby nast�puj�co: "jakas/sciezka/", "grafika_", 2.
	// Uwaga! Nazwy plik�w musz� mie� na ko�cu dwucyfrowy numer, a numeracja powinna si� zaczyna� od "01".
	void AddSeriesOfImages(std::string strImgSeriesPath, std::string strTemplateName, int iQuantity);

	void CreateAllImages(void);  // Tworzy wszystkie obrazki wyst�puj�ce w aplikacji.
};
