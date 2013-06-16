#include "MouseController.h"
#include "singletons.h"

CMouseController::CMouseController(void) : m_iX(0), m_iY(0), m_bIsLeftButDown(false), m_bIsRightButDown(false) {}

void CMouseController::UpdateMouseButtons(void) {
	// Je�eli kt�ry� z przycisk�w myszki zosta� przyci�ni�ty.
	if(g_EventMgr.GetEvent()->type == SDL_MOUSEBUTTONDOWN) {

		// Je�eli jest to lewy przycisk myszki, to...
		if(g_EventMgr.GetEvent()->button.button == SDL_BUTTON_LEFT)
			m_bIsLeftButDown = true;  //  ...ustawiamy odpowiadaj�c� za niego flag� na warto�� true.

		// Je�eli jest to prawy przycisk myszki, to...
		if(g_EventMgr.GetEvent()->button.button == SDL_BUTTON_RIGHT)
			m_bIsRightButDown = true;  //  ...ustawiamy odpowiadaj�c� za niego flag� na warto�� true.
	}

	// Je�eli kt�ry� z przycisk�w myszki zosta� zwolniony.
	if(g_EventMgr.GetEvent()->type == SDL_MOUSEBUTTONUP) {
		// Je�eli jest to lewy przycisk myszki, to...
		if(g_EventMgr.GetEvent()->button.button == SDL_BUTTON_LEFT)
			m_bIsLeftButDown = false;  //  ...ustawiamy odpowiadaj�c� za niego flag� na warto�� false.

		// Je�eli jest to prawy przycisk myszki, to...
		if(g_EventMgr.GetEvent()->button.button == SDL_BUTTON_RIGHT)
			m_bIsRightButDown = false;  //  ...ustawiamy odpowiadaj�c� za niego flag� na warto�� false.
	}
}
