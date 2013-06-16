#pragma once
#include "DefaultsMgr.h"
#include "EventMgr.h"
#include "GraphicMgr.h"
#include "ObjectMgr.h"
#include "SDL.h"
#include "Singleton.h"
#include "WorldArea.h"
#include <cassert>

class CApplication : public Singleton<CApplication> {
public:
	//-------------
	// Singletony.
	//-------------

	CDefaultsMgr DefaultsMgr;
	CGraphicMgr GraphicMgr;
	CEventMgr EventMgr;
	CWorldArea WorldArea;
	CObjectMgr ObjectMgr;

	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	CApplication(void);
	~CApplication(void);

	//-------------------
	// Metody dostêpowe.
	//-------------------

	void SetIsQuit(bool bIsQuit) { m_bIsQuit = bIsQuit; }
	bool GetIsQuit(void) const { return m_bIsQuit; }
private:
	//----------
	// Zmienne.
	//----------

	bool m_bIsQuit;  // Czy zakoñczyæ dzia³anie aplikacji.

	//---------
	// Metody.
	//---------

	void InitSDL(void);  // Inicjalizacja SDL.
	void CleanUp(void);  // Metoda sprz¹taj¹ca.
};
