#pragma once
#include "Backlight.h"
#include "Boid.h"
#include "DefaultsMgr.h"
#include "Knight.h"
#include "Leader.h"
#include "User.h"
#include "utilities.h"
#include "SerfBacklight.h"
#include "Vector.h"
#include <vector>

#define g_DefaultsMgr CDefaultsMgr::GetSingleton()

class CSerf : public IBoid {
public:
	//-----------------------------
	// Konstruktory i destruktory. 
	//-----------------------------

	// Konstruktor tworz�cy rycerza w podanym po�o�eniu, o losowej pr�dko�ci i orientacji.
	CSerf(short iId, CVector* vPos, short iColour);

	//-------------------------------------
	// Metody zwi�zane z algorytmem armii.
	//-------------------------------------

	// Aktualizacja, metoda wykorzystywana jest w ka�dej klatce.
	void ArmyIt(int iArmyId, std::vector<IKnight*>* pComradesContainer, const Uint32 iDeltaTicks);

	//-------------------
	// Pozosta�e metody.
	//-------------------

	void ClearVisibleContainer(void);  // Wyczyszczenie kontenera widoczno�ci.
private:
	//----------
	// Zmienne.
	//----------

	IKnight* m_pNearestCommander;  // Wska�nik na najbli�szego dow�dc�, je�li istnieje.
	float m_fDistToNearestCommander;  // Odleg�o�� do najbli�szego dow�dcy, je�li istnieje.

	//-------------------------------------
	// Metody zwi�zane z algorytmem armii.
	//-------------------------------------

	// Generuje wektor b�d�cy kompromisem pomi�dzy kierunkiem najbli�szego dow�dcy, a kierunkiem najbli�szego kompana.
	CVector CommanderMatchHeading(void) {
		// Obliczenie wektora zmiany, kompromisu pomi�dzy kierunkiem najbli�szego dow�dcy a kierunkiem najbli�szego kompana.
		CVector vChange = (*m_pNearestCommander->GetVel() + *m_pNearestComrade->GetVel());
		vChange.SetMagnitude(g_DefaultsMgr.GetMinUrgency() * 100.0f);  // Ustawienie odpowiedniej wielko�ci wektora zmiany.

		return vChange;  // Zwr�cenie wektora zmiany.
	}

	// Okre�la, kt�rych kompan�w widzi rycerz. Dodatkowo lokalizuje najbli�szego dow�dc�, je�li to mo�liwe.
	int SeeComrades(std::vector<IKnight*>* pComradesContainer);

	CVector SteerToCommander(void);  // Generuje wektor zapobiegaj�cy oddaleniu si� rycerza od dow�dcy.

	// Ewentualne zatrzymanie si� rycerza w przypadku bezruchu dow�dcy.
	void CommanderStop(void) {
		// Zatrzymanie si� rycerza, kiedy najbli�szy widoczny dow�dca stoi w miejscu i jest nieopodal
		// lub najbli�szy widoczny kompan stoi w miejscu, podobnie jak dow�dca, i jest nieopodal.
		// Uwaga, mo�e si� zdarzy�, �e wska�nik na najbli�szego widocznego kompana b�dzie wskazywa� na tego samego rycerza,
		// co wska�nik na najbli�szego widocznego dow�dc�. Dlatego w instrukcji warunkowej znajduje si� por�wnanie
		// tych dw�ch wska�nik�w, poniewa� w przytoczonym wypadku dalsze warunki by�yby analogiczne
		// do warunk�w poprzedzaj�cych to por�wnanie. Nie ma potrzeby sprawdzania ich ponownie.
		if(m_pNearestCommander && m_pNearestCommander->GetVel()->IsZero()
			 && ((m_fDistToNearestCommander <= g_DefaultsMgr.GetSeparationDist())
			 || ((m_pNearestComrade != m_pNearestCommander) && m_pNearestComrade->GetVel()->IsZero()
			 && (m_fDistToNearestComrade <= g_DefaultsMgr.GetSeparationDist()))))
			m_vVel.Set(0.0f, 0.0f);  // Wyzerowanie wektora pr�dko�ci rycerza.
	}

	//-------------------
	// Pozosta�e metody.
	//-------------------

	// Sprawdza, czy podany rycerz jest dow�dc�.
	bool IsCommander(IKnight* pKnight) {
		// Je�eli wska�nik do rycerzy jest ustawiony na typ rycerza: lider lub u�ytkownik, zostaje zwr�cona warto�� "true".
		if(dynamic_cast<CLeader*>(pKnight) || dynamic_cast<CUser*>(pKnight)) 
			return true;
		return false;  // Wska�nik do rycerzy jest ustawiony na inny typ rycerza.
	}
};
