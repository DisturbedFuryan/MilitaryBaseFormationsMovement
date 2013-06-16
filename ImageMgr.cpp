#include "ImageMgr.h"

CImageMgr::CImageMgr(void) {
	CreateAllImages();  // Stworzenie wszystkich obrazków.
}

CImageMgr::~CImageMgr(void) {
	// Opró¿nienie kontenera z obrazkami.
	std::vector<CImage*>::iterator It;
	for(It = m_ImagesContainer.begin(); It != m_ImagesContainer.end(); ) {
			delete *It;
			It = m_ImagesContainer.erase(It);
			// Poniewa¿ elementy bêd¹ce za elementem usuniêtym zostan¹ przesuniête odpowiednio na jego miejsce,
			// nie ma potrzeby inkrementowania iteratora.
	}
}

void CImageMgr::GetImgSurfaces(SDL_Surface* apSurfaces[], int iQuantity, std::string strName) {
	// Wype³nienie podanej tablicy wskaŸników podan¹ iloœci¹ powierzchni.
	for(int i = 0; i < iQuantity; i++) {
		std::stringstream ImgNameSS;  // Strumieñ do napisów.

		// Wys³anie nazwy pliku wraz z odpowiednim numerem do strumienia.
		if(i < 9)
			ImgNameSS << strName << "0" << (i + 1);
		else
			ImgNameSS << strName << (i + 1);

		CImage* pImage = GetImage(ImgNameSS.str());  // Pobranie wskaŸnika do obrazka o podanej nazwie z numerem.
		if(!pImage)
			assert(!"Failed to get image!");
		apSurfaces[i] = pImage->GetSurface();  // Pobranie powierzchni z obrazka i zapisanie jej w tablicy wskaŸników.
	}
}

CImage* CImageMgr::GetImage(std::string strName) {
	// Poszukiwanie obrazka w kontenerze o podanej nazwie.
	std::vector<CImage*>::iterator It = m_ImagesContainer.begin();
	while(It != m_ImagesContainer.end()) {
		if((*It)->GetName() == strName)
			return *It;  // Obrazek zosta³ znaleziony.
		++It;
	}
	return NULL;  // Ca³y kontener zosta³ przejrzany, nie znaleziono obrazka o podanej nazwie.
}

void CImageMgr::AddImage(std::string strPath, std::string strName) {
	CImage* pNewImage = new CImage(strPath, strName);  // Stworzenie nowego obrazka.
	m_ImagesContainer.push_back(pNewImage);  // Umieszczenie go na koñcu kontenera.
}

void CImageMgr::AddSeriesOfImages(std::string strImgSeriesPath, std::string strTemplateName, int iQuantity) {
	// Utworzenie zadanej iloœci obrazków.
	for(int i = 0; i < iQuantity; i++) {
		std::stringstream ImgNameSS;  // Strumieñ do napisów.

		// Wys³anie nazwy pliku wraz z odpowiednim numerem do strumienia.
		if(i < 9)
			ImgNameSS << strTemplateName << "0" << (i + 1);
		else
			ImgNameSS << strTemplateName << (i + 1);

		AddImage(strImgSeriesPath, ImgNameSS.str());  // Dodanie obrazka o podanej nazwie i odpowiednim numerze.
	}
}

void CImageMgr::CreateAllImages() {
	//-------------------------------------------------------
	// Stworzenie obrazków loga, trawy, palisady i kamienia.
	//-------------------------------------------------------

	AddImage("images/", "intro");
	AddImage("images/land/", "grass");
	AddImage("images/obstacles/", "palisade");
	AddImage("images/obstacles/", "stone");

	//---------------------------------------
	// Stworzenie obrazków œladów po butach.
	//---------------------------------------

	// Lewy œlad po bucie.
	AddImage("images/knight/shoesprints/", "knight_shoesprint_left");
	AddSeriesOfImages("images/knight/shoesprints/", "knight_shoesprint_left_disappear_", 4);

	// Prawy œlad po bucie.
	AddImage("images/knight/shoesprints/", "knight_shoesprint_right");
	AddSeriesOfImages("images/knight/shoesprints/", "knight_shoesprint_right_disappear_", 4);

	//--------------------------------------------
	// Stworzenie obrazków podœwietlenia rycerzy.
	//--------------------------------------------

	// ¯ó³te podœwietlenie dla dowódcy i poddanego.
	AddImage("images/knight/backlights/", "commander_backlight_yellow");
	AddImage("images/knight/backlights/", "serf_backlight_yellow");

	// Niebieskie podœwietlenie dla dowódcy i poddanego.
	AddImage("images/knight/backlights/", "commander_backlight_blue");
	AddImage("images/knight/backlights/", "serf_backlight_blue");

	// Zielone podœwietlenie dla dowódcy i poddanego.
	AddImage("images/knight/backlights/", "commander_backlight_green");
	AddImage("images/knight/backlights/", "serf_backlight_green");

	//------------------------------------
	// Stworzenie obrazków butów rycerza.
	//------------------------------------

	AddSeriesOfImages("images/knight/shoes/", "knight_shoes_", 8);

	//-------------------------------------
	// Stworzenie obrazków miecza rycerza.
	//-------------------------------------

	AddImage("images/knight/sword/", "knight_sword_stand");
	AddSeriesOfImages("images/knight/sword/", "knight_sword_walk_", 8);

	//------------------------------------
	// Stworzenie obrazków cia³a rycerza.
	//------------------------------------

	AddImage("images/knight/body/", "knight_body_stand");
	AddSeriesOfImages("images/knight/body/", "knight_body_walk_", 8);
}
