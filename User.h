#pragma once
#include "Camera.h"
#include "Circle.h"
#include "Backlight.h"
#include "CommanderBacklight.h"
#include "DefaultsMgr.h"
#include "Knight.h"
#include "Obstacle.h"
#include "Singleton.h"
#include "Vector.h"
#include <vector>

#define g_DefaultsMgr CDefaultsMgr::GetSingleton()

class CUser : public IKnight, public Singleton<CUser> {
public:
	//-----------------------------
	// Konstruktory i destruktory. 
	//-----------------------------

	CUser(short iId, CVector* vPos, short iColour);  // Konstruktor tworz�cy rycerza u�ytkownika w podanym po�o�eniu.

	//-------------------------------------
	// Metody zwi�zane z algorytmem armii.
	//-------------------------------------

	// Aktualizacja, metoda wykorzystywana jest w ka�dej klatce.
	void ArmyIt(int iArmyId, std::vector<IKnight*>* pComradesContainer, const Uint32 iDeltaTicks);

	//-------------------
	// Pozosta�e metody.
	//-------------------

	void HandleInput(void);  // Obs�uga zdarze� z urz�dze� wej�ciowych.
	void ShowTarget(void);  // Wy�wietla cel i ko�o kontroluj�ce ruch.
private:
	//----------
	// Zmienne.
	//----------

	CVector m_vTarget;  // Wektor pozycji, do jakiej chce si� uda� rycerz u�ytkownika.
	CCamera m_UserCamera;  // Kamera poruszaj�ca si� wraz z u�ytkownikiem, ustawiona centralnie nad nim.
	CCircle m_MoveControlCircle; // Ko�o kolizji, kt�re zapobiegnie poruszaniu si� rycerza przy najmniejszym ruchu myszk�.
	std::vector<CObstacle*> m_CloseObstaclesContainer;  // Kontener przeszk�d b�d�cych tu� obok u�ytkownika.
	bool m_bIsTargetVisible;  // Czy cel jest przedstawiony w formie graficznej.
	SDL_Rect MoveControlCircRect;  // Prostok�t opisuj�cy ko�o kontroluj�ce ruch.
	SDL_Rect TargetRect;  // Prostok�t opisuj�cy cel u�ytkownika.

	//-------------------------------------
	// Metody zwi�zane z algorytmem armii.
	//-------------------------------------

	// Obliczenie wektora pr�dko�ci przy pomocy wektora celu.
	void ComputeVel(void) {
		m_vVel = (m_vTarget - m_vPos);  // Przypisanie wektorowi pr�dko�ci r�nicy wekt�r�w: celu z pozycji.

		// Ustawienie wielko�ci wektora. Rycerz u�ytkownika porusza si� z optymaln� pr�dko�ci�.
		m_vVel.SetMagnitude(g_DefaultsMgr.GetDesiredSpeed());
	}

	void LocalizeTarget(void);  // Zlokalizowanie nowego celu, do kt�rego b�dzie pod��a� rycerz u�ytkownika.
	void WorldBound(void);  // Implementacja granic �wiata.
	void Target(void);  // Implementacja celu rycerza u�ytkownika.

	//---------------------------
	// Metody dotycz�ce kolizji.
	//---------------------------

	void ShiftMoveControlCircle(void);  // Zsynchronizowanie pozycji ko�a kontroluj�cego ruch z pozycj� rycerza.

	//-------------------
	// Pozosta�e metody.
	//-------------------

	void SearchCloseObstacles(void);  // Wyszukanie przeszk�d b�d�cych tu� obok u�ytkownika.
	void SetCamera(void);  // Ustawienie kamery centralnie nad rycerzem, o ile to jest mo�liwe.
};
