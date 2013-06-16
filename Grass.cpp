#include "Grass.h"
#include "singletons.h"

CGrass::CGrass(void) {
	// Pobranie obrazka skrawka trawy.
	CImage* pGrassImage = g_ImageMgr.GetImage("grass");
	if(!pGrassImage)
		assert(!"Failed to get image!");
	m_pGrassTileSurf = pGrassImage->GetSurface();

	// Pobranie wymiarów skrawka trawy.
	m_iGrassTileWidth = pGrassImage->GetWidth();
	m_iGrassTileHeight = pGrassImage->GetHeight();

	m_iNumTiles = ComputeNumTiles();  // Obliczenie iloœci skrawków mieszcz¹cych siê w obszarze œwiata.

	// Stworzenie dynamicznej tablicy o rozmiarze odpowiadaj¹cym wy¿ej obliczonej iloœci skrawków.
	m_apGrassTiles = new CGrassTile[m_iNumTiles];

	SetGrassTiles();  // Rozmieszczenie skrawków, przypisanie im odpowiednich wymiarów.
}

CGrass::~CGrass(void) {
	delete []m_apGrassTiles;  // Zwolnienie pamiêci zajmowanej przez tablicê skrawków trawy.
}

void CGrass::Show() {
	for(int i = 0; i < m_iNumTiles; i++)
		m_apGrassTiles[i].Show();
}

int CGrass::ComputeNumTiles(void) {
	// Liczba mieszcz¹cych siê skrawków w szerokoœci obszaru, przemno¿ona przez iloœæ skrawków mieszcz¹cych siê
	// w wysokoœci obszaru œwiata. Zastosowanie instrukcji warunkowych ma na celu obliczenie iloœci skrawków potrzebnych
	// do pokrycia ca³ej powierzchni œwiata, nawet jeœli koñcówki skrawków mia³yby wykraczaæ poza jego granice.
	int iNumWidth = (g_DefaultsMgr.GetAreaWidth() / m_iGrassTileWidth);
	if((iNumWidth * m_iGrassTileWidth) < g_DefaultsMgr.GetAreaWidth())
		iNumWidth++;
	int iNumHeight = (g_DefaultsMgr.GetAreaHeight() / m_iGrassTileHeight);
	if((iNumHeight * m_iGrassTileHeight) < g_DefaultsMgr.GetAreaHeight())
		iNumHeight++;
	return (iNumWidth * iNumHeight);
}

void CGrass::SetGrassTiles(void) {
	// Rozmieszczamy kolejne skrawki trawy poczynaj¹c od pozycji (0; 0).
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
