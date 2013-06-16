#include "Camera.h"
#include "singletons.h"

CCamera::CCamera(int iX, int iY) {
	m_CameraRect.x = iX;
	m_CameraRect.y = iY;
	m_CameraRect.w = g_AppWindow.GetScreenWidth();
	m_CameraRect.h = g_AppWindow.GetScreenHeight();
}
