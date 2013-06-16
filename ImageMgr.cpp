#include "ImageMgr.h"

CImageMgr::CImageMgr(void) {
	CreateAllImages();  // Stworzenie wszystkich obrazk�w.
}

CImageMgr::~CImageMgr(void) {
	// Opr�nienie kontenera z obrazkami.
	std::vector<CImage*>::iterator It;
	for(It = m_ImagesContainer.begin(); It != m_ImagesContainer.end(); ) {
			delete *It;
			It = m_ImagesContainer.erase(It);
			// Poniewa� elementy b�d�ce za elementem usuni�tym zostan� przesuni�te odpowiednio na jego miejsce,
			// nie ma potrzeby inkrementowania iteratora.
	}
}

void CImageMgr::GetImgSurfaces(SDL_Surface* apSurfaces[], int iQuantity, std::string strName) {
	// Wype�nienie podanej tablicy wska�nik�w podan� ilo�ci� powierzchni.
	for(int i = 0; i < iQuantity; i++) {
		std::stringstream ImgNameSS;  // Strumie� do napis�w.

		// Wys�anie nazwy pliku wraz z odpowiednim numerem do strumienia.
		if(i < 9)
			ImgNameSS << strName << "0" << (i + 1);
		else
			ImgNameSS << strName << (i + 1);

		CImage* pImage = GetImage(ImgNameSS.str());  // Pobranie wska�nika do obrazka o podanej nazwie z numerem.
		if(!pImage)
			assert(!"Failed to get image!");
		apSurfaces[i] = pImage->GetSurface();  // Pobranie powierzchni z obrazka i zapisanie jej w tablicy wska�nik�w.
	}
}

CImage* CImageMgr::GetImage(std::string strName) {
	// Poszukiwanie obrazka w kontenerze o podanej nazwie.
	std::vector<CImage*>::iterator It = m_ImagesContainer.begin();
	while(It != m_ImagesContainer.end()) {
		if((*It)->GetName() == strName)
			return *It;  // Obrazek zosta� znaleziony.
		++It;
	}
	return NULL;  // Ca�y kontener zosta� przejrzany, nie znaleziono obrazka o podanej nazwie.
}

void CImageMgr::AddImage(std::string strPath, std::string strName) {
	CImage* pNewImage = new CImage(strPath, strName);  // Stworzenie nowego obrazka.
	m_ImagesContainer.push_back(pNewImage);  // Umieszczenie go na ko�cu kontenera.
}

void CImageMgr::AddSeriesOfImages(std::string strImgSeriesPath, std::string strTemplateName, int iQuantity) {
	// Utworzenie zadanej ilo�ci obrazk�w.
	for(int i = 0; i < iQuantity; i++) {
		std::stringstream ImgNameSS;  // Strumie� do napis�w.

		// Wys�anie nazwy pliku wraz z odpowiednim numerem do strumienia.
		if(i < 9)
			ImgNameSS << strTemplateName << "0" << (i + 1);
		else
			ImgNameSS << strTemplateName << (i + 1);

		AddImage(strImgSeriesPath, ImgNameSS.str());  // Dodanie obrazka o podanej nazwie i odpowiednim numerze.
	}
}

void CImageMgr::CreateAllImages() {
	//-------------------------------------------------------
	// Stworzenie obrazk�w loga, trawy, palisady i kamienia.
	//-------------------------------------------------------

	AddImage("images/", "intro");
	AddImage("images/land/", "grass");
	AddImage("images/obstacles/", "palisade");
	AddImage("images/obstacles/", "stone");

	//---------------------------------------
	// Stworzenie obrazk�w �lad�w po butach.
	//---------------------------------------

	// Lewy �lad po bucie.
	AddImage("images/knight/shoesprints/", "knight_shoesprint_left");
	AddSeriesOfImages("images/knight/shoesprints/", "knight_shoesprint_left_disappear_", 4);

	// Prawy �lad po bucie.
	AddImage("images/knight/shoesprints/", "knight_shoesprint_right");
	AddSeriesOfImages("images/knight/shoesprints/", "knight_shoesprint_right_disappear_", 4);

	//--------------------------------------------
	// Stworzenie obrazk�w pod�wietlenia rycerzy.
	//--------------------------------------------

	// ��te pod�wietlenie dla dow�dcy i poddanego.
	AddImage("images/knight/backlights/", "commander_backlight_yellow");
	AddImage("images/knight/backlights/", "serf_backlight_yellow");

	// Niebieskie pod�wietlenie dla dow�dcy i poddanego.
	AddImage("images/knight/backlights/", "commander_backlight_blue");
	AddImage("images/knight/backlights/", "serf_backlight_blue");

	// Zielone pod�wietlenie dla dow�dcy i poddanego.
	AddImage("images/knight/backlights/", "commander_backlight_green");
	AddImage("images/knight/backlights/", "serf_backlight_green");

	//------------------------------------
	// Stworzenie obrazk�w but�w rycerza.
	//------------------------------------

	AddSeriesOfImages("images/knight/shoes/", "knight_shoes_", 8);

	//-------------------------------------
	// Stworzenie obrazk�w miecza rycerza.
	//-------------------------------------

	AddImage("images/knight/sword/", "knight_sword_stand");
	AddSeriesOfImages("images/knight/sword/", "knight_sword_walk_", 8);

	//------------------------------------
	// Stworzenie obrazk�w cia�a rycerza.
	//------------------------------------

	AddImage("images/knight/body/", "knight_body_stand");
	AddSeriesOfImages("images/knight/body/", "knight_body_walk_", 8);
}
