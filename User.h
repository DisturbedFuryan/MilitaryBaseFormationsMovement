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

	CUser(short iId, CVector* vPos, short iColour);  // Konstruktor tworz¹cy rycerza u¿ytkownika w podanym po³o¿eniu.

	//-------------------------------------
	// Metody zwi¹zane z algorytmem armii.
	//-------------------------------------

	// Aktualizacja, metoda wykorzystywana jest w ka¿dej klatce.
	void ArmyIt(int iArmyId, std::vector<IKnight*>* pComradesContainer, const Uint32 iDeltaTicks);

	//-------------------
	// Pozosta³e metody.
	//-------------------

	void HandleInput(void);  // Obs³uga zdarzeñ z urz¹dzeñ wejœciowych.
	void ShowTarget(void);  // Wyœwietla cel i ko³o kontroluj¹ce ruch.
private:
	//----------
	// Zmienne.
	//----------

	CVector m_vTarget;  // Wektor pozycji, do jakiej chce siê udaæ rycerz u¿ytkownika.
	CCamera m_UserCamera;  // Kamera poruszaj¹ca siê wraz z u¿ytkownikiem, ustawiona centralnie nad nim.
	CCircle m_MoveControlCircle; // Ko³o kolizji, które zapobiegnie poruszaniu siê rycerza przy najmniejszym ruchu myszk¹.
	std::vector<CObstacle*> m_CloseObstaclesContainer;  // Kontener przeszkód bêd¹cych tu¿ obok u¿ytkownika.
	bool m_bIsTargetVisible;  // Czy cel jest przedstawiony w formie graficznej.
	SDL_Rect MoveControlCircRect;  // Prostok¹t opisuj¹cy ko³o kontroluj¹ce ruch.
	SDL_Rect TargetRect;  // Prostok¹t opisuj¹cy cel u¿ytkownika.

	//-------------------------------------
	// Metody zwi¹zane z algorytmem armii.
	//-------------------------------------

	// Obliczenie wektora prêdkoœci przy pomocy wektora celu.
	void ComputeVel(void) {
		m_vVel = (m_vTarget - m_vPos);  // Przypisanie wektorowi prêdkoœci ró¿nicy wektórów: celu z pozycji.

		// Ustawienie wielkoœci wektora. Rycerz u¿ytkownika porusza siê z optymaln¹ prêdkoœci¹.
		m_vVel.SetMagnitude(g_DefaultsMgr.GetDesiredSpeed());
	}

	void LocalizeTarget(void);  // Zlokalizowanie nowego celu, do którego bêdzie pod¹¿a³ rycerz u¿ytkownika.
	void WorldBound(void);  // Implementacja granic œwiata.
	void Target(void);  // Implementacja celu rycerza u¿ytkownika.

	//---------------------------
	// Metody dotycz¹ce kolizji.
	//---------------------------

	void ShiftMoveControlCircle(void);  // Zsynchronizowanie pozycji ko³a kontroluj¹cego ruch z pozycj¹ rycerza.

	//-------------------
	// Pozosta³e metody.
	//-------------------

	void SearchCloseObstacles(void);  // Wyszukanie przeszkód bêd¹cych tu¿ obok u¿ytkownika.
	void SetCamera(void);  // Ustawienie kamery centralnie nad rycerzem, o ile to jest mo¿liwe.
};
