#include "Grass.h"
#include "singletons.h"

CGrass::CGrass(void) {
	// Pobranie obrazka skrawka trawy.
	CImage* pGrassImage = g_ImageMgr.GetImage("grass");
	if(!pGrassImage)
		assert(!"Failed to get image!");
	m_pGrassTileSurf = pGrassImage->GetSurface();

	// Pobranie wymiar�w skrawka trawy.
	m_iGrassTileWidth = pGrassImage->GetWidth();
	m_iGrassTileHeight = pGrassImage->GetHeight();

	m_iNumTiles = ComputeNumTiles();  // Obliczenie ilo�ci skrawk�w mieszcz�cych si� w obszarze �wiata.

	// Stworzenie dynamicznej tablicy o rozmiarze odpowiadaj�cym wy�ej obliczonej ilo�ci skrawk�w.
	m_apGrassTiles = new CGrassTile[m_iNumTiles];

	SetGrassTiles();  // Rozmieszczenie skrawk�w, przypisanie im odpowiednich wymiar�w.
}

CGrass::~CGrass(void) {
	delete []m_apGrassTiles;  // Zwolnienie pami�ci zajmowanej przez tablic� skrawk�w trawy.
}

void CGrass::Show() {
	for(int i = 0; i < m_iNumTiles; i++)
		m_apGrassTiles[i].Show();
}

int CGrass::ComputeNumTiles(void) {
	// Liczba mieszcz�cych si� skrawk�w w szeroko�ci obszaru, przemno�ona przez ilo�� skrawk�w mieszcz�cych si�
	// w wysoko�ci obszaru �wiata. Zastosowanie instrukcji warunkowych ma na celu obliczenie ilo�ci skrawk�w potrzebnych
	// do pokrycia ca�ej powierzchni �wiata, nawet je�li ko�c�wki skrawk�w mia�yby wykracza� poza jego granice.
	int iNumWidth = (g_DefaultsMgr.GetAreaWidth() / m_iGrassTileWidth);
	if((iNumWidth * m_iGrassTileWidth) < g_DefaultsMgr.GetAreaWidth())
		iNumWidth++;
	int iNumHeight = (g_DefaultsMgr.GetAreaHeight() / m_iGrassTileHeight);
	if((iNumHeight * m_iGrassTileHeight) < g_DefaultsMgr.GetAreaHeight())
		iNumHeight++;
	return (iNumWidth * iNumHeight);
}

void CGrass::SetGrassTiles(void) {
	// Rozmieszczamy kolejne skrawki trawy poczynaj�c od pozycji (0; 0).
	int iX = 0, iY = 0;
	for(int i = 0; i < m_iNumTiles; i++) {
		m_apGrassTiles[i].SetRect(iX, iY, m_iGrassTileWidth, m_iGrassTileHeight);
		iX += m_iGrassTileWidth;
		if(iX >= g_DefaultsMgr.GetAreaWidth()) {
			iX = 0;
			iY += m_iGrassTileHeight;
		}
	}
}
