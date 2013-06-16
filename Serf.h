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

	// Konstruktor tworz¹cy rycerza w podanym po³o¿eniu, o losowej prêdkoœci i orientacji.
	CSerf(short iId, CVector* vPos, short iColour);

	//-------------------------------------
	// Metody zwi¹zane z algorytmem armii.
	//-------------------------------------

	// Aktualizacja, metoda wykorzystywana jest w ka¿dej klatce.
	void ArmyIt(int iArmyId, std::vector<IKnight*>* pComradesContainer, const Uint32 iDeltaTicks);

	//-------------------
	// Pozosta³e metody.
	//-------------------

	void ClearVisibleContainer(void);  // Wyczyszczenie kontenera widocznoœci.
private:
	//----------
	// Zmienne.
	//----------

	IKnight* m_pNearestCommander;  // WskaŸnik na najbli¿szego dowódcê, jeœli istnieje.
	float m_fDistToNearestCommander;  // Odleg³oœæ do najbli¿szego dowódcy, jeœli istnieje.

	//-------------------------------------
	// Metody zwi¹zane z algorytmem armii.
	//-------------------------------------

	// Generuje wektor bêd¹cy kompromisem pomiêdzy kierunkiem najbli¿szego dowódcy, a kierunkiem najbli¿szego kompana.
	CVector CommanderMatchHeading(void) {
		// Obliczenie wektora zmiany, kompromisu pomiêdzy kierunkiem najbli¿szego dowódcy a kierunkiem najbli¿szego kompana.
		CVector vChange = (*m_pNearestCommander->GetVel() + *m_pNearestComrade->GetVel());
		vChange.SetMagnitude(g_DefaultsMgr.GetMinUrgency() * 100.0f);  // Ustawienie odpowiedniej wielkoœci wektora zmiany.

		return vChange;  // Zwrócenie wektora zmiany.
	}

	// Okreœla, których kompanów widzi rycerz. Dodatkowo lokalizuje najbli¿szego dowódcê, jeœli to mo¿liwe.
	int SeeComrades(std::vector<IKnight*>* pComradesContainer);

	CVector SteerToCommander(void);  // Generuje wektor zapobiegaj¹cy oddaleniu siê rycerza od dowódcy.

	// Ewentualne zatrzymanie siê rycerza w przypadku bezruchu dowódcy.
	void CommanderStop(void) {
		// Zatrzymanie siê rycerza, kiedy najbli¿szy widoczny dowódca stoi w miejscu i jest nieopodal
		// lub najbli¿szy widoczny kompan stoi w miejscu, podobnie jak dowódca, i jest nieopodal.
		// Uwaga, mo¿e siê zdarzyæ, ¿e wskaŸnik na najbli¿szego widocznego kompana bêdzie wskazywa³ na tego samego rycerza,
		// co wskaŸnik na najbli¿szego widocznego dowódcê. Dlatego w instrukcji warunkowej znajduje siê porównanie
		// tych dwóch wskaŸników, poniewa¿ w przytoczonym wypadku dalsze warunki by³yby analogiczne
		// do warunków poprzedzaj¹cych to porównanie. Nie ma potrzeby sprawdzania ich ponownie.
		if(m_pNearestCommander && m_pNearestCommander->GetVel()->IsZero()
			 && ((m_fDistToNearestCommander <= g_DefaultsMgr.GetSeparationDist())
			 || ((m_pNearestComrade != m_pNearestCommander) && m_pNearestComrade->GetVel()->IsZero()
			 && (m_fDistToNearestComrade <= g_DefaultsMgr.GetSeparationDist()))))
			m_vVel.Set(0.0f, 0.0f);  // Wyzerowanie wektora prêdkoœci rycerza.
	}

	//-------------------
	// Pozosta³e metody.
	//-------------------

	// Sprawdza, czy podany rycerz jest dowódc¹.
	bool IsCommander(IKnight* pKnight) {
		// Je¿eli wskaŸnik do rycerzy jest ustawiony na typ rycerza: lider lub u¿ytkownik, zostaje zwrócona wartoœæ "true".
		if(dynamic_cast<CLeader*>(pKnight) || dynamic_cast<CUser*>(pKnight)) 
			return true;
		return false;  // WskaŸnik do rycerzy jest ustawiony na inny typ rycerza.
	}
};
