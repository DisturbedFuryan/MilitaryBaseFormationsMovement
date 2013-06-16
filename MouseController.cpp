#include "MouseController.h"
#include "singletons.h"

CMouseController::CMouseController(void) : m_iX(0), m_iY(0), m_bIsLeftButDown(false), m_bIsRightButDown(false) {}

void CMouseController::UpdateMouseButtons(void) {
	// Je¿eli któryœ z przycisków myszki zosta³ przyciœniêty.
	if(g_EventMgr.GetEvent()->type == SDL_MOUSEBUTTONDOWN) {

		// Je¿eli jest to lewy przycisk myszki, to...
		if(g_EventMgr.GetEvent()->button.button == SDL_BUTTON_LEFT)
			m_bIsLeftButDown = true;  //  ...ustawiamy odpowiadaj¹c¹ za niego flagê na wartoœæ true.

		// Je¿eli jest to prawy przycisk myszki, to...
		if(g_EventMgr.GetEvent()->button.button == SDL_BUTTON_RIGHT)
			m_bIsRightButDown = true;  //  ...ustawiamy odpowiadaj¹c¹ za niego flagê na wartoœæ true.
	}

	// Je¿eli któryœ z przycisków myszki zosta³ zwolniony.
	if(g_EventMgr.GetEvent()->type == SDL_MOUSEBUTTONUP) {
		// Je¿eli jest to lewy przycisk myszki, to...
		if(g_EventMgr.GetEvent()->button.button == SDL_BUTTON_LEFT)
			m_bIsLeftButDown = false;  //  ...ustawiamy odpowiadaj¹c¹ za niego flagê na wartoœæ false.

		// Je¿eli jest to prawy przycisk myszki, to...
		if(g_EventMgr.GetEvent()->button.button == SDL_BUTTON_RIGHT)
			m_bIsRightButDown = false;  //  ...ustawiamy odpowiadaj¹c¹ za niego flagê na wartoœæ false.
	}
}
