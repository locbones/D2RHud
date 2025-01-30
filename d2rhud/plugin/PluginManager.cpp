#include "PluginManager.h"
#include "D2RHUD/D2RHUD.h"
#include <imgui.h>

PluginManager::PluginManager() {
    m_Plugins.push_back(new D2RHUD());
}

void PluginManager::Present() {
	for (auto& plugin : m_Plugins) {
		plugin->OnDraw();
	}
}

//plugin/PluginManager.cpp
void PluginManager::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_KEYDOWN:
        for (auto& plugin : m_Plugins) {
            if (plugin->OnKeyPressed(wParam)) {
                break;
            }
        }
        break;
    case WM_KEYUP:
        if (wParam == VK_DELETE) {
            m_ShowSettings = !m_ShowSettings;
        }
        for (auto& plugin : m_Plugins) {
            plugin->OnKeyReleased(wParam);
        }
        break;
    default:
        break;
    }
}

