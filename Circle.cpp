#include "Circle.h"

CCircle::CCircle(void) : r(0.0f) { 
	pos.x = 0.0f;
	pos.y = 0.0f;
}

CCircle::CCircle(float fX, float fY, float fR) : r(fR) { 
	pos.x = fX;
	pos.y = fY;
}

CCircle::CCircle(const CVector& vPos, float fR) : r(fR) { 
	pos = vPos;
}

CCircle::CCircle(const CVector* pvPos, float fR) : r(fR) {
	pos = *pvPos;
}

CCircle::CCircle(const CCircle& Circle) {
	*this = Circle;
}

CCircle::CCircle(const CCircle* pCircle) {
	*this = *pCircle;
}

CCircle& CCircle::operator=(const CCircle& Circle) {
	pos = Circle.pos;
	r = Circle.r;
	return *this;
}