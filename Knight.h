#pragma once
#include "Backlight.h"
#include "Body.h"
#include "Circle.h"
#include "Shoes.h"
#include "Shoesprint.h"
#include "Sword.h"
#include "Vector.h"
#include <vector>

class IKnight {
public:
	//--------------------
	// Typy wyliczeniowe.
	//--------------------

	enum Type { user, leader, serf };  // Typ rycerza: "user" - u�ytkownik, "leader" - lider, "serf" - poddany.
	enum Status { stand, walk };  // Status rycerza: "stand" - rycerz stoi, "walk" - rycerz porusza si�.

	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	IKnight(void);
	virtual ~IKnight(void);

	//-------------------------------------
	// Metody zwi�zane z algorytmem armii.
	//-------------------------------------

	// Aktualizacja, metoda wykorzystywana jest w ka�dej klatce. Metoda czysto wirtualna.
	virtual void ArmyIt(int iArmyId, std::vector<IKnight*>* pComradesContainer, const Uint32 iDeltaTicks) = 0;

	//----------------------------------------------------
	// Metody zajmuj�ce si� wy�wietlaniem cz�ci rycerza.
	//----------------------------------------------------

	void ShowShoesprints(void);  // Wy�wietlenie �lad�w pozostawionych przez rycerza.

	// Pokazanie pod�wietlenia.
	void ShowBacklight(void) { if(m_bVisibilityBacklight) m_pBacklight->Show(&m_vPos); }

	// Wy�wietlenie but�w rycerza.
	void ShowShoes(void) { m_Shoes.Show(&m_vPos, m_iStatus, static_cast<short>(m_fFrame), static_cast<int>(m_fAng)); }

	// Wy�wietlenie miecza nale��cego do rycerza.
	void ShowSword(void) { m_Sword.Show(&m_vPos, m_iStatus, static_cast<short>(m_fFrame), static_cast<int>(m_fAng)); }

	// Wy�wietlenie cia�a rycerza.
	void ShowBody(void) { m_Body.Show(&m_vPos, m_iStatus, static_cast<short>(m_fFrame), static_cast<int>(m_fAng)); }

	//-------------------
	// Metody dost�powe.
	//-------------------

	CVector* GetPos(void) { return &m_vPos; }  // Dost�p do wektora pozycji rycerza.
	CVector* GetVel(void) { return &m_vVel; }  // Dost�p do wektora pr�dko�ci rycerza.
	float GetAng(void) const { return m_fAng; }  // Dost�p do orientacji rycerza.
	CCircle* GetCollisionCircle(void) { return &m_CollisionCircle; }  // Dost�p do ko�a kolizji rycerza.
	short GetStatus(void) const { return m_iStatus; }  // Dost�p do statusu rycerza.

	//-------------------
	// Pozosta�e metody.
	//-------------------

	void PrintData(void);  // Wy�wietla dane rycerza.
protected:
	//--------------------
	// Zmienne statyczne.
	//--------------------

	static bool m_bVisibilityBacklight;  // Flaga widoczno�ci pod�wietlenia, u�ywana przez ka�dego rycerza.

	//----------
	// Zmienne.
	//----------

	short m_iId;  // Identyfikator rycerza.
	CVector m_vPos;  // Po�o�enie.
	CVector m_vVel;  // Pr�dko��.
	float m_fAng;  // Orientacja.
	float m_fSpeed;  // ��czna pr�dko�� rycerza.
	short m_iStatus;  // Status rycerza.
	float m_fFrame;  // Klatka animacji do wy�wietlenia.
	short m_iPreviousFrame;  // Poprzednio wyswietlana klatka animacji.
	CCircle m_CollisionCircle;  // Ko�o kolizji.
	CVector m_vOldPos;  // Poprzednie po�o�enie.
	CVector m_vOldVel;  // Poprzednia pr�dko��.
	float m_fOldSpeed;  // ��czna pr�dko�� rycerza, poprzedzaj�ca jego bezruch.
	std::vector<CShoesprint*> m_ShoesprintsContainer;  // Kontener zostawianych �lad�w przez rycerza.
	bool m_bIsShoesprint;  // Flaga zapobiegaj�ca powstawaniu w klatce animacji wielu �lad�w po butach rycerza.

	//-----------------
	// Cz�ci rycerza.
	//-----------------

	IBacklight* m_pBacklight;  // Pod�wietlenie.
	CShoes m_Shoes;  // Buty.
	CSword m_Sword;  // Miecz.
	CBody m_Body;  // Cia�o.

	//-------------------------------------
	// Metody zwi�zane z algorytmem armii.
	//-------------------------------------

	virtual void WorldBound(void) = 0;  // Implementacja granic �wiata. Metoda czysto wirtualna.

	//---------------------------
	// Metody dotycz�ce kolizji.
	//---------------------------

	void CollisionWithObstacles(void);  // Implementacja kolizji z przeszkodami.
	void CollisionWithOthers(void);  // Implementacja kolizji z innymi rycerzami.

	// Zsynchronizowanie pozycji ko�a kolizji z pozycj� rycerza.
	void ShiftCollisionCircle(void) { m_CollisionCircle.pos = m_vPos; }

	//----------------------------
	// Metody dotycz�ce animacji.
	//----------------------------

  void UpdateFrameAndStatus(const Uint32 iDeltaTicks);  // Uaktualnienie klatki animacji i statusu rycerza.

	// Sprawdzenie, czy nast�pi�a zmiana klatki animacji.
	bool IsFrameChanged(void) { return (m_iPreviousFrame != static_cast<short>(m_fFrame)); }

	//-------------------
	// Pozosta�e metody.
	//-------------------

	void ComputeYaw(const Uint32 iDeltaTicks);  // Oblicza orientacj� rycerza.
	void FetchToAng(float fSuggestedAng, const Uint32 iDeltaTicks);  // Sprowadza k�t rycerza do k�ta podanego.
	void UpdateShoesprintsContainer(const Uint32 iDeltaTicks);  // Tworzenie i usuwanie �lad�w po butach.
};
