#include "Application.h"

CApplication::CApplication() : m_bIsQuit(false) {
	InitSDL();  // Zainicjalizowanie SDL.
	srand(SDL_GetTicks());  // Ustawienie losowoœci.
}

CApplication::~CApplication() {
	CleanUp();
}

void CApplication::InitSDL() {
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
		// Zatrzymanie programu ze stosownym komunikatem w przypadku nieudanej inicjalizacji.
		assert(!"Failed to initialize SDL!");
}

void CApplication::CleanUp() {
	SDL_Quit();
}
