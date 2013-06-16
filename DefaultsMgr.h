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
	// Metody dostêpowe.
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

	int m_iMaxArmies;  // Maksymalna iloœæ armii.
	int m_iMaxKnights;  // Maksymalna iloœæ rycerzy.
	int m_iMaxComradesVisible;  // Maksymalna iloœæ widzinych kompanów przez rycerza.
	float m_fKnightRadius;  // Promieñ ko³a kolizji rycerza.
	float m_fPalisadeRadius;  // Promieñ ko³a kolizji palisady.
	float m_fStoneRadius;  // Promieñ ko³a kolizji kamienia.
	float m_fMaxSpeed;  // Maksymalna prêdkoœæ osi¹gana przez rycerza.
	float m_fDesiredSpeed;  // Optymalna prêdkoœæ rycerza.
	float m_fMinUrgency;  // Minimalna "waga".
	float m_fMaxUrgency;  // Maksymalna "waga".
	float m_fMaxChange;  // Maksymalna zmiana.
	float m_fSeparationDist;  // Optymalny dystans pomiêdzy rycerzami.
	float m_fEnemyKeepAwayDist;  // Odleg³oœæ, na jak¹ rycerz mo¿e zbli¿yæ siê do wroga.
	float m_fObstacleKeepAwayDist;  // Odleg³oœæ, na jak¹ rycerz mo¿e zbli¿yæ siê do przeszkody.
	float m_fDefaultPerceptionRange;  // Zasiêg wzroku rycerza.

	//------------------------------
	// Zmienne odczytywane z pliku.
	//------------------------------

  int m_iScreenWidth, m_iScreenHeight;  // Szerokoœæ i wysokoœæ ekranu.
	int m_iAreaWidth, m_iAreaHeight;  // Szerokoœæ i wysokoœæ œwiata.
	int m_iNumArmies;  // Liczba armii do stworzenia.
	int m_iNumLeadersArmy;  // Liczba liderów do stworzenia w ka¿dej z armii.
	int m_iNumSerfsArmy;  // Liczba poddanych do stworzenia w ka¿dej z armii.
	int m_iNumPalisades;  // Liczba palisad do stworzenia.
	int m_iNumStones;  // Liczba kamieni do stworzenia.
	bool m_bIsRotatedSurfaces;  // Czy obróciæ grafiki.
	bool m_bIsFPS;  // Czy wyœwietliæ œredni¹ liczbê klatek na sekundê.

	//---------
	// Metody.
	//---------

	bool ReadDefaults(void);  // Odczytuje wartoœci domyœlne z pliku.
};
