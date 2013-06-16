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

		// Zapisanie obliczonego wzglêdnego adresu pochodnej klasy we wskaŸniku ms_pSingleton.

		// Pochodna klasa mo¿e siê wywodziæ z wielu klas Singleton. W takim przypadku this klasy SomeClass
		// mo¿e byæ ró¿ne od this klasy Singleton. Dlatego zostaje sprawdzona ró¿nica rzutowania nieistniej¹cego obiektu,
		// znajduj¹cego siê pod adresem 0x1, na obydwa typy. Ró¿nica ta bêdzie odleg³oœci¹ miêdzy Singleton<SomeClass>
		// i jego pochodnym typem SomeClass, którego potem mo¿na u¿yæ przy obliczaniu wskaŸnika singletonu.

		int iOffset = ((int)(T*)1 - (int)(Singleton<T>*)(T*)1);
		ms_Singleton = (T*)((int)this + iOffset);
	}
	~Singleton(void) { 
		assert(ms_Singleton);
		ms_Singleton = 0;
	}

	//-------------------
	// Metody dostêpowe.
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
