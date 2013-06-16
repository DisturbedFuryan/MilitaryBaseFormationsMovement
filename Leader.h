#pragma once
#include "Backlight.h"
#include "Boid.h"
#include "CommanderBacklight.h"
#include "Vector.h"
#include <vector>

class CLeader : public IBoid {
public:
	//-----------------------------
	// Konstruktory i destruktory. 
	//-----------------------------

	// Konstruktor tworz�cy rycerza w podanym po�o�eniu, o losowej pr�dko�ci i orientacji.
	CLeader(short iId, CVector* vPos, short iColour);

	//-------------------------------------
	// Metody zwi�zane z algorytmem armii.
	//-------------------------------------

	// Aktualizacja, metoda wykorzystywana jest w ka�dej klatce.
	void ArmyIt(int iArmyId, std::vector<IKnight*>* pComradesContainer, const Uint32 iDeltaTicks);
};
