#include "WorldArea.h"
#include "singletons.h"

CWorldArea::CWorldArea(void) : m_iWidth(g_DefaultsMgr.GetAreaWidth()), m_iHeight(g_DefaultsMgr.GetAreaHeight()) {}
