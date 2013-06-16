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
	// Wywo�anie metody ArmyIt() dla wszystkich cz�onk�w armii.
	std::vector<IKnight*>::iterator It;
	for(It = m_MembersContainer.begin(); It != m_MembersContainer.end(); ++It)
		(*It)->ArmyIt(m_iId, &m_MembersContainer, iDeltaTicks);
}

bool CArmy::RemoveFrom(IKnight* pKnight) {
	std::vector<IKnight*>::iterator It = m_MembersContainer.begin();
	while(It != m_MembersContainer.end()) {
		if(*It == pKnight) {
			// Usuni�cie delikwetna z armii. Uwaga! Obiekt usuwany, wskazywany jako cz�onek armii, nadal istnieje!
			m_MembersContainer.erase(It);

			return true;  // Usuni�cie podanego delikwenta powiod�o si�.
		}
		++It;
	}
	return false;  // Nie uda�o sie usun�� podanego delikwenta.
}

void CArmy::PrintData(void) {
	g_DebugWindow.Print("Data for Army ", m_iId, ":");
	switch(m_iId) {  // Z identyfikatora armii mo�na wyczyta� kolor rycerzy.
		case IBacklight::yellow: g_DebugWindow.Print("  Colour: yellow"); break;
		case IBacklight::blue: g_DebugWindow.Print("  Colour: blue"); break;
		case IBacklight::green: g_DebugWindow.Print("  Colour: green");
	}
	g_DebugWindow.Print("  Number of members: ", GetCount());
	g_DebugWindow.Print("  Data for members:");
	// Wy�wietlenie informacji na temat rycerzy bed�cych w armii.
	std::vector<IKnight*>::iterator It;
	for(It = m_MembersContainer.begin(); It != m_MembersContainer.end(); ++It)
		(*It)->PrintData();
}
