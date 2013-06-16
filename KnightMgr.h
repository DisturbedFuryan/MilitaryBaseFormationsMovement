#pragma once
#include "Army.h"
#include "Image.h"
#include "Knight.h"
#include "Leader.h"
#include "ProgressBar.h"
#include "SDL.h"
#include "Serf.h"
#include "Singleton.h"
#include "User.h"
#include "Vector.h"
#include <cassert>
#include <vector>

#define g_User CUser::GetSingleton()

class CKnightMgr : public Singleton<CKnightMgr> {
public:
	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	CKnightMgr(void);
	~CKnightMgr(void);

	//------------------------------
	// Wyœwietlanie czêœci rycerzy.
	//------------------------------

	void ShowShoesprints(void);
	void ShowBacklights(void);
	void ShowShoes(void);
	void ShowSwords(void);
	void ShowBodies(void);

	//-------------------
	// Metody dostêpowe.
	//-------------------

	std::vector<IKnight*>* GetKnightsContainer(void) { return &m_KnightsContainer; }
	std::vector<CArmy*>* GetArmiesContainer(void) { return &m_ArmiesContainer; }

	// Metody dostêpowe do poobracanych obrazków.
	SDL_Surface* GetKnightShoesprintSurface(int iType, int iAngle) {
		if(iType == CShoesprint::left) return m_apKnightShoesprintLeftSurfacesAllAngles[iAngle]; 
		else if(iType == CShoesprint::right) return m_apKnightShoesprintRightSurfacesAllAngles[iAngle];
		return NULL;
	}
	SDL_Surface* GetKnightShoesprintDisappearSurface(int iFrame, int iType, int iAngle) {
		if(iType == CShoesprint::left) return m_apKnightShoesprintLeftDisappearSurfacesAllAngles[iFrame][iAngle]; 
		else if(iType == CShoesprint::right) return m_apKnightShoesprintRightDisappearSurfacesAllAngles[iFrame][iAngle];
		return NULL;
	}
	SDL_Surface* GetCommanderBacklightSurface(short iType, short iAngle) {
		switch(iType) {
			case IBacklight::yellow: return m_apCommanderBacklightYellowSurfaces[iAngle];
			case IBacklight::blue: return m_apCommanderBacklightBlueSurfaces[iAngle];
			case IBacklight::green: return m_apCommanderBacklightGreenSurfaces[iAngle];
		}
		return NULL;
	}
	SDL_Surface* GetKnightShoesSurface(int iFrame, int iAngle) { return m_apKnightShoesSurfacesAllAngles[iFrame][iAngle]; }
	SDL_Surface* GetKnightSwordStandSurface(int iAngle) { return m_apKnightSwordStandSurfacesAllAngles[iAngle]; }
	SDL_Surface* GetKnightSwordWalkSurface(int iFrame, int iAngle)
		{ return m_apKnightSwordWalkSurfacesAllAngles[iFrame][iAngle]; }
	SDL_Surface* GetKnightBodyStandSurface(int iAngle) { return m_apKnightBodyStandSurfacesAllAngles[iAngle]; }
	SDL_Surface* GetKnightBodyWalkSurface(int iFrame, int iAngle)
		{ return m_apKnightBodyWalkSurfacesAllAngles[iFrame][iAngle]; }

	//-------------------
	// Pozosta³e metody.
	//-------------------

	bool AddArmy(bool bAddUser, int iNumLeaders, int iNumSerfs, CVector* pvPos = NULL);  // Tworzy now¹ armiê.
	void UpdateArmies(const Uint32 iDeltaTicks);  // Uaktualnia wszystkie armie.
	void HandleInput(void);  // Obs³uga zdarzeñ dla wszystkich rycerzy.
private:
	//------------
	// Kontenery.
	//------------

	std::vector<IKnight*> m_KnightsContainer;  // Kontener wszystkich rycerzy.
	std::vector<CArmy*> m_ArmiesContainer;  // Kontener wszystkich armii.

	//----------------------------------------------
	// Tablice wskaŸników do poobracanych obrazków.
	//----------------------------------------------

	// Œlady zostawiane przez rycerza.
	SDL_Surface* m_apKnightShoesprintLeftSurfacesAllAngles[360];
	SDL_Surface* m_apKnightShoesprintLeftDisappearSurfacesAllAngles[4][360];
	SDL_Surface* m_apKnightShoesprintRightSurfacesAllAngles[360];
	SDL_Surface* m_apKnightShoesprintRightDisappearSurfacesAllAngles[4][360];

	// Podœwietlenia rycerza.
	SDL_Surface* m_apCommanderBacklightYellowSurfaces[360];
	SDL_Surface* m_apCommanderBacklightBlueSurfaces[360];
	SDL_Surface* m_apCommanderBacklightGreenSurfaces[360];

	SDL_Surface* m_apKnightShoesSurfacesAllAngles[8][360];  // Buty rycerza.

	// Miecz rycerza.
	SDL_Surface* m_apKnightSwordStandSurfacesAllAngles[360];
	SDL_Surface* m_apKnightSwordWalkSurfacesAllAngles[8][360];

	// Cia³o rycerza.
	SDL_Surface* m_apKnightBodyStandSurfacesAllAngles[360];
	SDL_Surface* m_apKnightBodyWalkSurfacesAllAngles[8][360];

	//-------------------
	// Pozosta³e metody.
	//-------------------

	IKnight* AddKnight(short iId, CVector* pvPos, short iArmyId, short iType);  // Dodaje rycerza podanego typu.
	bool LoadSurfaces(void);  // Obraca obrazki.
};
