#pragma once
#include <cassert>

template<typename T>
class Singleton {
public:
	//-----------------------------
	// Konstruktory i destruktory.
	//-----------------------------

	Singleton(void) {
		assert(!ms_Singleton);

		// Zapisanie obliczonego wzgl�dnego adresu pochodnej klasy we wska�niku ms_pSingleton.

		// Pochodna klasa mo�e si� wywodzi� z wielu klas Singleton. W takim przypadku this klasy SomeClass
		// mo�e by� r�ne od this klasy Singleton. Dlatego zostaje sprawdzona r�nica rzutowania nieistniej�cego obiektu,
		// znajduj�cego si� pod adresem 0x1, na obydwa typy. R�nica ta b�dzie odleg�o�ci� mi�dzy Singleton<SomeClass>
		// i jego pochodnym typem SomeClass, kt�rego potem mo�na u�y� przy obliczaniu wska�nika singletonu.

		int iOffset = ((int)(T*)1 - (int)(Singleton<T>*)(T*)1);
		ms_Singleton = (T*)((int)this + iOffset);
	}
	~Singleton(void) { 
		assert(ms_Singleton);
		ms_Singleton = 0;
	}

	//-------------------
	// Metody dost�powe.
	//-------------------

	static T& GetSingleton(void) {
		assert(ms_Singleton);
		return *ms_Singleton;
	}
	static T* GetSingletonPtr(void) { return ms_Singleton; }
private:
	//--------------------
	// Zmienne statyczne.
	//--------------------

	static T* ms_Singleton;
};

template<typename T>
T* Singleton<T>::ms_Singleton = 0;
