#include "Vector.h"

CVector::CVector(void) : x(0.0f), y(0.0f) {}  // Przy domyœlnym konstruktorze zostanie stworzony wektor zerowy.

CVector::CVector(float fX, float fY) : x(fX), y(fY) {}

CVector::CVector(const CVector& v) : x(v.x), y(v.y) {}  // Przypisanie wartoœci wektora wejœciowego.

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
	if(fM > 0.0f)  // Je¿eli d³ugoœæ jest wiêksza od zera, zmienne zostan¹ pomno¿one przez jej odwrotnoœæ.
		fM = (1.0f / fM);
	else
		fM = 0.0f;
	x *= fM;
	y *= fM;
	return *this;
}

float CVector::Yaw(void) {
	float fYaw;  // Odchylenie.
	if(!this->IsZero()) {  // Je¿eli wektor bêdzie zerowy, nie uda siê obliczyæ k¹ta.
		// Wektor biegn¹cy pionowo w górê, celowo jest on wektorem jednostkowym. Zabieg ten uproœci obliczenia.
		CVector vNorth(0.0f, -1.0f);
		// K¹t miêdzy wektorami w stopniach. Dziêki temu, ¿e wektor pomocniczy jest jednostkowy,
		// nie trzeba liczyæ dodatkowo jego d³ugoœci przy pierwszym dzieleniu.
		// "vNorth * *this" - iloczyn skalarny. Ostatnie mno¿enie zamienia radiany na stopnie.
		fYaw = (acos((vNorth * *this) / this->Length()) * (180.0f / g_fPi));

		// Powy¿szy wzór da nam wynik z zakresu <0, 180>, bez wzglêdu na po³o¿enie dwóch wektorów.
		// Chc¹c uzyskaæ zakres k¹ta pe³nego nale¿y odj¹æ od 360 stopni wy¿ej uzyskany wynik w przypadku,
		// gdy rozmiar k¹ta przekroczy k¹t pó³pe³ny. W tym przypadku k¹t ponad pó³pe³ny powinien zaistnieæ,
		// gdy zwrot wektora prêdkoœci bêdzie bieg³ "na prawo".
		if(x > 0.0f)
			fYaw = (360.0f - fYaw);
		// K¹t 0 odpowiada k¹towi 360 stopni. Nie ma potrzeby korzystania z obu definicji.
		// Co wiêcej, tablice z poobracanymi obrazkami zaczynaj¹ siê od 0, a koñcz¹ na 359.
		// Z tego wzglêdu zostaje wybrana opcja pierwsza. Zatem celowo wy¿ej stoi znak "wiêksze", nie "wiêksze, równe".
		// Dziêki temu wynikiem nigdy nie bêdzie k¹t 360 stopni. Ewentualnie prawie, ale nie ca³kiem.
		// Teoretycznie... Praktycznie u¿yty wzór do obliczenia k¹ta nie jest nazbyt dok³adny,
		// operuje na zmiennych typu float. Z tak¹ sam¹ precyzj¹ zdefiniowana zosta³a liczba Pi.
		// Tak¿e nale¿y siê zabezpieczyæ:
		if(fYaw >= 360.0f) {
			// Ustawienie najwiêkszej mo¿liwej liczby poprzedzaj¹cej 360 dla typu float,
			// która po rzutowaniu na typ int przyjmie wartoœæ 359.
			fYaw = 359.99998f;
		}
	}
	else
		fYaw = 0.0f;  // W przypadku, gdy wektor jest zerowy, odchyleniem jest k¹t 0 stopni.
	return fYaw;  // Zwrócenie odchylenia.
}
