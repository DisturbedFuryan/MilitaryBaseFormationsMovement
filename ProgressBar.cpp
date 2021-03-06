#include "ProgressBar.h"

int CProgressBar::ms_iPrgBarCount = 0;  // Inicjalizacja zmiennej statycznej.

CProgressBar::CProgressBar(void) {
	ms_iPrgBarCount++;
	if(ms_iPrgBarCount > (g_AppWindow.GetScreenHeight() / 20))
		assert(!"Failed to create a progress bar!");
	// Przypisanie wartości prostokątowi reprezentującemu pasek postępu.
	m_ProgressRect.x = 0;
	m_ProgressRect.y = (g_AppWindow.GetScreenHeight() - (ms_iPrgBarCount * 20));
	m_ProgressRect.w = 0;
	m_ProgressRect.h = 20;

	// Przypisanie wartości prostokątowi reprezentującemu tło paska postępu.
	m_BackgroundRect.x = 0;
	m_BackgroundRect.y = m_ProgressRect.y;
	m_BackgroundRect.w = g_AppWindow.GetScreenWidth();
	m_BackgroundRect.h = 20;
}

CProgressBar::~CProgressBar(void) {
	ms_iPrgBarCount--;
}

void CProgressBar::SetProgress(float fProgress) {
	// Obliczenie długości paska postępu.
	m_ProgressRect.w = static_cast<int>(fProgress * static_cast<float>(m_BackgroundRect.w));

	// Zabezpieczenie przed przekroczeniem dopuszczalnych wartości przez pasek postępu.
	if(m_ProgressRect.w > m_BackgroundRect.w) 
		m_ProgressRect.w = m_BackgroundRect.w;
	else if(m_ProgressRect.w < 0)
		m_ProgressRect.w = 0;
}
