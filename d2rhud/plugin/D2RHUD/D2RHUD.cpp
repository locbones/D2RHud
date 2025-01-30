#include "D2RHUD.h"
#include <imgui.h>
#include "../../D2/D2Ptrs.h"
#include <sstream>
#include <windows.h>
#include <vector>
#include <functional>
#include "../KeyMappings.h"
#include <string>

const char* Version = "1.0.1";
const char* ResistanceNames[6] = { "   ", "   ", "   ", "   ", "   ", "   " };
constexpr  uint32_t ResistanceStats[6] = { 39, 41, 43, 45, 36, 37 };
constexpr  uint32_t Alignment = { 172 };
constexpr  uint32_t ImmunityCold = { 187 };
constexpr  uint32_t ImmunityFire = { 189 };
constexpr  uint32_t ImmunityLight = { 190 };
constexpr  uint32_t ImmunityPoison = { 191 };
constexpr  uint32_t ImmunityPhysical = { 192 };
constexpr  uint32_t ImmunityMagic = { 193 };
constexpr  ImU32 ResistanceColors[6] = { IM_COL32(170,50,50,255) ,IM_COL32(170,170,50,255) ,IM_COL32(50,90,170,255) ,IM_COL32(50,170,50,255),IM_COL32(255,255,255,255), IM_COL32(255,175,0,255) };
constexpr  float HPLocationY = 2.f;
constexpr  float ResistanceLocationY = 80.f;
constexpr  const char* Seperator = "  ";
constexpr uint32_t Experience = { 21 };

//Draw monster stats based on resolution and hover settings
void D2RHUD::OnDraw() {
    auto drawList = ImGui::GetBackgroundDrawList();
    auto min = drawList->GetClipRectMin();
    auto max = drawList->GetClipRectMax();
    float center = (max.x - min.x) / 2.f;

    //Change font size based on resolution
    ImGuiIO& io = ImGui::GetIO();
    int fontIndex = (io.DisplaySize.y <= 720) ? 0 : (io.DisplaySize.y <= 900) ? 1 : (io.DisplaySize.y <= 1080) ? 2 : (io.DisplaySize.y <= 1440) ? 3 : 4;
    ImGui::PushFont(io.Fonts->Fonts[fontIndex]);

    // Ensure hover conditions are met
    if (!gMouseHover->IsHovered || gMouseHover->HoveredUnitType > 0x1) {
        return;
    }

    //D2 Pointers to data values
    D2GameStrc* pGame = *gCurrentSinglePlayerGame;
    D2UnitStrc* pUnit = UNITS_GetClientUnitByIdAndType(gMouseHover->HoveredUnitId, gMouseHover->HoveredUnitType);
    D2UnitStrc* pUnitServer = UNITS_GetServerUnitByTypeAndId(pGame, gMouseHover->HoveredUnitType, gMouseHover->HoveredUnitId);

    //Only display values if unit returns a valid HP value
    if (!pUnit || !pUnitServer || STATLIST_GetUnitStatSigned(pUnitServer, STAT_HITPOINTS, 0) == 0) {
        return;
    }

    //Calculate resistances display
    float totalWidth = 0.f;
    float spaceWidth = ImGui::CalcTextSize(Seperator).x;
    std::string resistances[6];
    float widths[6];

    //Generate the resistance values amd determine the total character width
    for (int i = 0; i < 6; i++) {
        resistances[i] = std::to_string(STATLIST_GetUnitStatSigned(pUnit, ResistanceStats[i], 0));
        widths[i] = ImGui::CalcTextSize(resistances[i].c_str()).x;
        totalWidth += widths[i] + (i > 0 ? spaceWidth : 0);
    }

    //Add stats to draw list with adjusted width values
    float startX = center - (totalWidth / 2.f);
    for (int i = 0; i < 6; i++) {
        drawList->AddText({ startX, ResistanceLocationY }, ResistanceColors[i], resistances[i].c_str());
        startX += widths[i] + spaceWidth;
    }

    // Display HP Stats
    int currentHP = STATLIST_GetUnitStatSigned(pUnitServer, STAT_HITPOINTS, 0) >> 8;
    int maxHP = STATLIST_GetUnitStatSigned(pUnitServer, STAT_MAXHP, 0) >> 8;
    std::string hpText = std::to_string(currentHP) + " / " + std::to_string(maxHP);
    float hpWidth = ImGui::CalcTextSize(hpText.c_str()).x;
    drawList->AddText({ center - (hpWidth / 2.f), HPLocationY }, IM_COL32(255, 255, 255, 255), hpText.c_str());

    /* Debug Example - Retrieves stat references from D2Enums.h, Remove the // at start of line to use */
    //std::string playerStrength = std::format("Player Strength: {}", STATLIST_GetUnitStatSigned(pUnitPlayer, STAT_ITEM_MAGICARROW, 0));
    //drawList->AddText({ 20, 10 }, IM_COL32(170, 50, 50, 255), playerStrength.c_str());
}

//Check Hotkeys and execute functions
bool D2RHUD::OnKeyPressed(short key)
{
    std::string filename = "D2RHUD_Config.txt";

    // Structure for string and keymap definitions
    struct KeyAction {
        std::string searchString;
        std::function<void()> action;
    };

    // Structure for keymap to function definitions
    std::vector<KeyAction> actions = {
        {"Transmute: ", [] { D2CLIENT_Transmute(); }},
        {"Sort Stash: ", [] { INVENTORY_AutoSort(1, INVPAGE_STASH); }},
        {"Sort Inventory: ", [] { INVENTORY_AutoSort(1, INVPAGE_INVENTORY); }},
        {"Sort Cube: ", [] { INVENTORY_AutoSort(1, INVPAGE_CUBE); }}
    };

    // Parse config entries to find matching keymap for each function (KeyMappings.cpp)
    for (const auto& action : actions) {
        std::string result = readTextFollowingString(filename, action.searchString);
        if (!result.empty()) {
            result.erase(result.find_last_not_of(" \n\r\t") + 1);
            result.erase(0, result.find_first_not_of(" \n\r\t"));

            auto it = keyMap.find(result);
            if (it != keyMap.end() && key == it->second) {
                action.action();
                return true;
            }
        }
    }

    // Track key presses for the version display
    if (key == VK_CONTROL) ctrlPressed = true;
    if (key == VK_MENU) altPressed = true;
    if (key == 'V') vPressed = true;

    // If CTRL + ALT + V are pressed together, show the message box
    if (ctrlPressed && altPressed && vPressed) {
        ShowVersionMessage();
        ctrlPressed = altPressed = vPressed = false; // Reset state
        return true;
    }

    return false;
}

//Show D2RHUD Version Info as a MessageBox Popup
void D2RHUD::ShowVersionMessage()
{
    std::string message = "Version: " + std::string(Version);
    MessageBoxA(nullptr, message.c_str(), "Debug Display", MB_OK | MB_ICONINFORMATION);
}