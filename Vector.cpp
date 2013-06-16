#include "Vector.h"

CVector::CVector(void) : x(0.0f), y(0.0f) {}  // Przy domy�lnym konstruktorze zostanie stworzony wektor zerowy.

CVector::CVector(float fX, float fY) : x(fX), y(fY) {}

CVector::CVector(const CVector& v) : x(v.x), y(v.y) {}  // Przypisanie warto�ci wektora wej�ciowego.

CVector& CVector::operator=(const CVector& v) {
	x = v.x;
	y = v.y;
	return *this;
}

CVector& CVector::operator+=(const CVector& v) {
	x += v.x;
	y += v.y;
	return *this;
}

CVector& CVector::operator-=(const CVector& v) {
	x -= v.x;
	y -= v.y;
	return *this;
}

CVector& CVector::operator*=(float fVal) {
	x *= fVal;
	y *= fVal;
	return *this;
}

CVector& CVector::operator/=(float fVal) {
	x /= fVal;
	y /= fVal;
	return *this;
}

bool operator==(const CVector& vA, const CVector& vB) {
	return ((vA.x == vB.x) && (vA.y == vB.y));
}

bool operator!=(const CVector& vA, const CVector& vB) {
	return ((vA.x != vB.x) && (vA.y != vB.y));
}

CVector operator+(const CVector& vA, const CVector& vB) {
	CVector vSum(vA);
	vSum += vB;
	return vSum;
}

CVector operator-(const CVector& vA, const CVector& vB) {
	CVector vSum(vA);
	vSum -= vB;
	return vSum;
}

CVector operator-(const CVector& v) {
	return CVector(-v.x, -v.y);
}

CVector operator*(const CVector& v, float fVal) {
	return CVector(v.x * fVal, v.y * fVal);
}

CVector operator*(float fVal, const CVector& v) {
	return CVector(v.x * fVal, v.y * fVal);
}

float operator*(const CVector& vA, const CVector& vB) {
	return ((vA.x * vB.x) + (vA.y * vB.y));
}

CVector operator/(const CVector& v, float fVal) {
	return CVector(v.x / fVal, v.y / fVal);
}

CVector& CVector::Normalize(void) {
	float fM = Length();
	if(fM > 0.0f)  // Je�eli d�ugo�� jest wi�ksza od zera, zmienne zostan� pomno�one przez jej odwrotno��.
		fM = (1.0f / fM);
	else
		fM = 0.0f;
	x *= fM;
	y *= fM;
	return *this;
}

float CVector::Yaw(void) {
	float fYaw;  // Odchylenie.
	if(!this->IsZero()) {  // Je�eli wektor b�dzie zerowy, nie uda si� obliczy� k�ta.
		// Wektor biegn�cy pionowo w g�r�, celowo jest on wektorem jednostkowym. Zabieg ten upro�ci obliczenia.
		CVector vNorth(0.0f, -1.0f);
		// K�t mi�dzy wektorami w stopniach. Dzi�ki temu, �e wektor pomocniczy jest jednostkowy,
		// nie trzeba liczy� dodatkowo jego d�ugo�ci przy pierwszym dzieleniu.
		// "vNorth * *this" - iloczyn skalarny. Ostatnie mno�enie zamienia radiany na stopnie.
		fYaw = (acos((vNorth * *this) / this->Length()) * (180.0f / g_fPi));

		// Powy�szy wz�r da nam wynik z zakresu <0, 180>, bez wzgl�du na po�o�enie dw�ch wektor�w.
		// Chc�c uzyska� zakres k�ta pe�nego nale�y odj�� od 360 stopni wy�ej uzyskany wynik w przypadku,
		// gdy rozmiar k�ta przekroczy k�t p�pe�ny. W tym przypadku k�t ponad p�pe�ny powinien zaistnie�,
		// gdy zwrot wektora pr�dko�ci b�dzie bieg� "na prawo".
		if(x > 0.0f)
			fYaw = (360.0f - fYaw);
		// K�t 0 odpowiada k�towi 360 stopni. Nie ma potrzeby korzystania z obu definicji.
		// Co wi�cej, tablice z poobracanymi obrazkami zaczynaj� si� od 0, a ko�cz� na 359.
		// Z tego wzgl�du zostaje wybrana opcja pierwsza. Zatem celowo wy�ej stoi znak "wi�ksze", nie "wi�ksze, r�wne".
		// Dzi�ki temu wynikiem nigdy nie b�dzie k�t 360 stopni. Ewentualnie prawie, ale nie ca�kiem.
		// Teoretycznie... Praktycznie u�yty wz�r do obliczenia k�ta nie jest nazbyt dok�adny,
		// operuje na zmiennych typu float. Z tak� sam� precyzj� zdefiniowana zosta�a liczba Pi.
		// Tak�e nale�y si� zabezpieczy�:
		if(fYaw >= 360.0f) {
			// Ustawienie najwi�kszej mo�liwej liczby poprzedzaj�cej 360 dla typu float,
			// kt�ra po rzutowaniu na typ int przyjmie warto�� 359.
			fYaw = 359.99998f;
		}
	}
	else
		fYaw = 0.0f;  // W przypadku, gdy wektor jest zerowy, odchyleniem jest k�t 0 stopni.
	return fYaw;  // Zwr�cenie odchylenia.
}
