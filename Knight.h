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

	enum Type { user, leader, serf };  // Typ rycerza: "user" - u¿ytkownik, "leader" - lider, "serf" - poddany.
	enum Status { stand, walk };  // Status rycerza: "stand" - rycerz stoi, "walk" - rycerz porusza siê.

	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	IKnight(void);
	virtual ~IKnight(void);

	//-------------------------------------
	// Metody zwi¹zane z algorytmem armii.
	//-------------------------------------

	// Aktualizacja, metoda wykorzystywana jest w ka¿dej klatce. Metoda czysto wirtualna.
	virtual void ArmyIt(int iArmyId, std::vector<IKnight*>* pComradesContainer, const Uint32 iDeltaTicks) = 0;

	//----------------------------------------------------
	// Metody zajmuj¹ce siê wyœwietlaniem czêœci rycerza.
	//----------------------------------------------------

	void ShowShoesprints(void);  // Wyœwietlenie œladów pozostawionych przez rycerza.

	// Pokazanie podœwietlenia.
	void ShowBacklight(void) { if(m_bVisibilityBacklight) m_pBacklight->Show(&m_vPos); }

	// Wyœwietlenie butów rycerza.
	void ShowShoes(void) { m_Shoes.Show(&m_vPos, m_iStatus, static_cast<short>(m_fFrame), static_cast<int>(m_fAng)); }

	// Wyœwietlenie miecza nale¿¹cego do rycerza.
	void ShowSword(void) { m_Sword.Show(&m_vPos, m_iStatus, static_cast<short>(m_fFrame), static_cast<int>(m_fAng)); }

	// Wyœwietlenie cia³a rycerza.
	void ShowBody(void) { m_Body.Show(&m_vPos, m_iStatus, static_cast<short>(m_fFrame), static_cast<int>(m_fAng)); }

	//-------------------
	// Metody dostêpowe.
	//-------------------

	CVector* GetPos(void) { return &m_vPos; }  // Dostêp do wektora pozycji rycerza.
	CVector* GetVel(void) { return &m_vVel; }  // Dostêp do wektora prêdkoœci rycerza.
	float GetAng(void) const { return m_fAng; }  // Dostêp do orientacji rycerza.
	CCircle* GetCollisionCircle(void) { return &m_CollisionCircle; }  // Dostêp do ko³a kolizji rycerza.
	short GetStatus(void) const { return m_iStatus; }  // Dostêp do statusu rycerza.

	//-------------------
	// Pozosta³e metody.
	//-------------------

	void PrintData(void);  // Wyœwietla dane rycerza.
protected:
	//--------------------
	// Zmienne statyczne.
	//--------------------

	static bool m_bVisibilityBacklight;  // Flaga widocznoœci podœwietlenia, u¿ywana przez ka¿dego rycerza.

	//----------
	// Zmienne.
	//----------

	short m_iId;  // Identyfikator rycerza.
	CVector m_vPos;  // Po³o¿enie.
	CVector m_vVel;  // Prêdkoœæ.
	float m_fAng;  // Orientacja.
	float m_fSpeed;  // £¹czna prêdkoœæ rycerza.
	short m_iStatus;  // Status rycerza.
	float m_fFrame;  // Klatka animacji do wyœwietlenia.
	short m_iPreviousFrame;  // Poprzednio wyswietlana klatka animacji.
	CCircle m_CollisionCircle;  // Ko³o kolizji.
	CVector m_vOldPos;  // Poprzednie po³o¿enie.
	CVector m_vOldVel;  // Poprzednia prêdkoœæ.
	float m_fOldSpeed;  // £¹czna prêdkoœæ rycerza, poprzedzaj¹ca jego bezruch.
	std::vector<CShoesprint*> m_ShoesprintsContainer;  // Kontener zostawianych œladów przez rycerza.
	bool m_bIsShoesprint;  // Flaga zapobiegaj¹ca powstawaniu w klatce animacji wielu œladów po butach rycerza.

	//-----------------
	// Czêœci rycerza.
	//-----------------

	IBacklight* m_pBacklight;  // Podœwietlenie.
	CShoes m_Shoes;  // Buty.
	CSword m_Sword;  // Miecz.
	CBody m_Body;  // Cia³o.

	//-------------------------------------
	// Metody zwi¹zane z algorytmem armii.
	//-------------------------------------

	virtual void WorldBound(void) = 0;  // Implementacja granic œwiata. Metoda czysto wirtualna.

	//---------------------------
	// Metody dotycz¹ce kolizji.
	//---------------------------

	void CollisionWithObstacles(void);  // Implementacja kolizji z przeszkodami.
	void CollisionWithOthers(void);  // Implementacja kolizji z innymi rycerzami.

	// Zsynchronizowanie pozycji ko³a kolizji z pozycj¹ rycerza.
	void ShiftCollisionCircle(void) { m_CollisionCircle.pos = m_vPos; }

	//----------------------------
	// Metody dotycz¹ce animacji.
	//----------------------------

  void UpdateFrameAndStatus(const Uint32 iDeltaTicks);  // Uaktualnienie klatki animacji i statusu rycerza.

	// Sprawdzenie, czy nast¹pi³a zmiana klatki animacji.
	bool IsFrameChanged(void) { return (m_iPreviousFrame != static_cast<short>(m_fFrame)); }

	//-------------------
	// Pozosta³e metody.
	//-------------------

	void ComputeYaw(const Uint32 iDeltaTicks);  // Oblicza orientacjê rycerza.
	void FetchToAng(float fSuggestedAng, const Uint32 iDeltaTicks);  // Sprowadza k¹t rycerza do k¹ta podanego.
	void UpdateShoesprintsContainer(const Uint32 iDeltaTicks);  // Tworzenie i usuwanie œladów po butach.
};
