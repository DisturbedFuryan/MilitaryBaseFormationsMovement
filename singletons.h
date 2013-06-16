#pragma once
#include "Application.h"
#include "AppWindow.h"
#include "Camera.h"
#include "CollisionController.h"
#include "DebugWindow.h"
#include "DefaultsMgr.h"
#include "EventMgr.h"
#include "GraphicMgr.h"
#include "Grass.h"
#include "ImageMgr.h"
#include "KnightMgr.h"
#include "MouseController.h"
#include "ObjectMgr.h"
#include "ObstacleMgr.h"
#include "User.h"
#include "WorldArea.h"

#define g_Application CApplication::GetSingleton()
#define g_AppWindow CAppWindow::GetSingleton()
#define g_Camera CCamera::GetSingleton()
#define g_CollisionController CCollisionController::GetSingleton()
#define g_DebugWindow CDebugWindow::GetSingleton()
#define g_DefaultsMgr CDefaultsMgr::GetSingleton()
#define g_EventMgr CEventMgr::GetSingleton()
#define g_GraphicMgr CGraphicMgr::GetSingleton()
#define g_Grass CGrass::GetSingleton()
#define g_ImageMgr CImageMgr::GetSingleton()
#define g_KnightMgr CKnightMgr::GetSingleton()
#define g_MouseController CMouseController::GetSingleton()
#define g_ObjectMgr CObjectMgr::GetSingleton()
#define g_ObstacleMgr CObstacleMgr::GetSingleton()
#define g_User CUser::GetSingleton()
#define g_WorldArea CWorldArea::GetSingleton()
