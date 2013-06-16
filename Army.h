#pragma once
#include "Backlight.h"
#include "Knight.h"
#include <vector>

class CArmy {
public:
	//--------------------
	// Zmienne statyczne.
	//--------------------

	static int ms_iArmyCount;  // Licznik armii.

	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	CArmy(void);
	~CArmy(void);

	//-------------------------------------
	// Metody zwi�zane z algorytmem armii.
	//-------------------------------------

	void Update(const Uint32 iDeltaTicks);  // Uaktualnia wszystkich rycerzy w armii.

	//-------------------
	// Metody dost�powe.
	//-------------------

	int GetCount(void) const { return m_MembersContainer.size(); }  // Zwraca liczb� rycerzy bed�cych w armii.
	std::vector<IKnight*>* GetMembersContainer(void) { return &m_MembersContainer; }
	short GetId(void) const { return m_iId; }

	//-------------------
	// Pozosta�e metody.
	//-------------------

	void AddTo(IKnight* pKnight) { m_MembersContainer.push_back(pKnight); }  // Dodaje wskazanego rycerza do armii.
	bool RemoveFrom(IKnight* pKnight);  // Usuwa wskazanego rycerza z armii.
	void PrintData(void);  // Wy�wietla dane armii.
private:
	//----------
	// Zmienne.
	//----------

	short m_iId;  // Identyfikator armii.
	std::vector<IKnight*> m_MembersContainer;  // Kontener wska�nik�w do rycerzy b�d�cych w armii.
};
