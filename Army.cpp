#include "Army.h"
#include "singletons.h"

int CArmy::ms_iArmyCount = 0;  // Inicjalizacja zmiennej statycznej.

CArmy::CArmy(void) {
	m_iId = ms_iArmyCount;
	ms_iArmyCount++;  // Inkrementacja licznika.
}

CArmy::~CArmy(void) {
	ms_iArmyCount--;  // Dekrementacja licznika.
}

void CArmy::Update(const Uint32 iDeltaTicks) {
	// Wywo³anie metody ArmyIt() dla wszystkich cz³onków armii.
	std::vector<IKnight*>::iterator It;
	for(It = m_MembersContainer.begin(); It != m_MembersContainer.end(); ++It)
		(*It)->ArmyIt(m_iId, &m_MembersContainer, iDeltaTicks);
}

bool CArmy::RemoveFrom(IKnight* pKnight) {
	std::vector<IKnight*>::iterator It = m_MembersContainer.begin();
	while(It != m_MembersContainer.end()) {
		if(*It == pKnight) {
			// Usuniêcie delikwetna z armii. Uwaga! Obiekt usuwany, wskazywany jako cz³onek armii, nadal istnieje!
			m_MembersContainer.erase(It);

			return true;  // Usuniêcie podanego delikwenta powiod³o siê.
		}
		++It;
	}
	return false;  // Nie uda³o sie usun¹æ podanego delikwenta.
}

void CArmy::PrintData(void) {
	g_DebugWindow.Print("Data for Army ", m_iId, ":");
	switch(m_iId) {  // Z identyfikatora armii mo¿na wyczytaæ kolor rycerzy.
		case IBacklight::yellow: g_DebugWindow.Print("  Colour: yellow"); break;
		case IBacklight::blue: g_DebugWindow.Print("  Colour: blue"); break;
		case IBacklight::green: g_DebugWindow.Print("  Colour: green");
	}
	g_DebugWindow.Print("  Number of members: ", GetCount());
	g_DebugWindow.Print("  Data for members:");
	// Wyœwietlenie informacji na temat rycerzy bed¹cych w armii.
	std::vector<IKnight*>::iterator It;
	for(It = m_MembersContainer.begin(); It != m_MembersContainer.end(); ++It)
		(*It)->PrintData();
}
