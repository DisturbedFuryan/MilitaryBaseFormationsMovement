#pragma once
#include "Singleton.h"
#include <cassert>
#include <fstream>
#include <string>

class CDefaultsMgr : public Singleton<CDefaultsMgr> {
public:
	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	CDefaultsMgr(void);

	//-------------------
	// Metody dost�powe.
	//-------------------

	int GetMaxArmies(void) const { return m_iMaxArmies; }
	int GetMaxKnights(void) const { return m_iMaxKnights; }
	int GetMaxComradesVisible(void) const { return m_iMaxComradesVisible; }
	float GetKnightRadius(void) const { return m_fKnightRadius; } 
	float GetPalisadeRadius(void) const { return m_fPalisadeRadius; } 
	float GetStoneRadius(void) const { return m_fStoneRadius; }
	float GetMaxSpeed(void) const { return m_fMaxSpeed; }
	float GetDesiredSpeed(void) const { return m_fDesiredSpeed; }
	float GetMinUrgency(void) const { return m_fMinUrgency; }
	float GetMaxUrgency(void) const { return m_fMaxUrgency; }
	float GetMaxChange(void) const { return m_fMaxChange; }
	float GetSeparationDist(void) const { return m_fSeparationDist; }
	float GetEnemyKeepAwayDist(void) const { return m_fEnemyKeepAwayDist; } 
	float GetObstacleKeepAwayDist(void) const { return m_fObstacleKeepAwayDist; }
	float GetDefaultPerceptionRange(void) const { return m_fDefaultPerceptionRange; }
	int GetScreenWidth(void) const { return m_iScreenWidth; }
	int GetScreenHeight(void) const { return m_iScreenHeight; }
	int GetAreaWidth(void) const { return m_iAreaWidth; }
	int GetAreaHeight(void) const { return m_iAreaHeight; }
	int GetNumArmies(void) const { return m_iNumArmies; }
	int GetNumLeadersArmy(void) const { return m_iNumLeadersArmy; }
	int GetNumSerfsArmy(void) const { return m_iNumSerfsArmy; } 
	int GetNumPalisades(void) const { return m_iNumPalisades; }
	int GetNumStones(void) const { return m_iNumStones; }
	bool GetIsRotatedSurfaces(void) const { return m_bIsRotatedSurfaces; }
	bool GetIsFPS(void) const { return m_bIsFPS; }
private:
	//---------------------------------------
	// Zmienne ustawiane przez konstruktory.
	//---------------------------------------

	int m_iMaxArmies;  // Maksymalna ilo�� armii.
	int m_iMaxKnights;  // Maksymalna ilo�� rycerzy.
	int m_iMaxComradesVisible;  // Maksymalna ilo�� widzinych kompan�w przez rycerza.
	float m_fKnightRadius;  // Promie� ko�a kolizji rycerza.
	float m_fPalisadeRadius;  // Promie� ko�a kolizji palisady.
	float m_fStoneRadius;  // Promie� ko�a kolizji kamienia.
	float m_fMaxSpeed;  // Maksymalna pr�dko�� osi�gana przez rycerza.
	float m_fDesiredSpeed;  // Optymalna pr�dko�� rycerza.
	float m_fMinUrgency;  // Minimalna "waga".
	float m_fMaxUrgency;  // Maksymalna "waga".
	float m_fMaxChange;  // Maksymalna zmiana.
	float m_fSeparationDist;  // Optymalny dystans pomi�dzy rycerzami.
	float m_fEnemyKeepAwayDist;  // Odleg�o��, na jak� rycerz mo�e zbli�y� si� do wroga.
	float m_fObstacleKeepAwayDist;  // Odleg�o��, na jak� rycerz mo�e zbli�y� si� do przeszkody.
	float m_fDefaultPerceptionRange;  // Zasi�g wzroku rycerza.

	//------------------------------
	// Zmienne odczytywane z pliku.
	//------------------------------

  int m_iScreenWidth, m_iScreenHeight;  // Szeroko�� i wysoko�� ekranu.
	int m_iAreaWidth, m_iAreaHeight;  // Szeroko�� i wysoko�� �wiata.
	int m_iNumArmies;  // Liczba armii do stworzenia.
	int m_iNumLeadersArmy;  // Liczba lider�w do stworzenia w ka�dej z armii.
	int m_iNumSerfsArmy;  // Liczba poddanych do stworzenia w ka�dej z armii.
	int m_iNumPalisades;  // Liczba palisad do stworzenia.
	int m_iNumStones;  // Liczba kamieni do stworzenia.
	bool m_bIsRotatedSurfaces;  // Czy obr�ci� grafiki.
	bool m_bIsFPS;  // Czy wy�wietli� �redni� liczb� klatek na sekund�.

	//---------
	// Metody.
	//---------

	bool ReadDefaults(void);  // Odczytuje warto�ci domy�lne z pliku.
};
