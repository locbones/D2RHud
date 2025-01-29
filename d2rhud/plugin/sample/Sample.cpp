#include "Sample.h"
#include <imgui.h>
#include "../../D2/D2Ptrs.h"
#include <sstream>
#include <windows.h>
#include <vector>

const char* Version = "1.0.0";
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



void Sample::OnDraw() {

    //Adjust resistance and HP positioning based on screen resolution
    auto drawList = ImGui::GetBackgroundDrawList();
    auto min = drawList->GetClipRectMin();
    auto max = drawList->GetClipRectMax();
    auto center = (max.x - min.x) / 2.f;

    ImGuiIO& io = ImGui::GetIO();
    float ypercent1 = io.DisplaySize.y * 0.0745f;
    float ypercent2 = io.DisplaySize.y * 0.043f;
    int fontIndex = (io.DisplaySize.y <= 720) ? 0 : (io.DisplaySize.y <= 900) ? 1 : (io.DisplaySize.y <= 1080) ? 2 : (io.DisplaySize.y <= 1440) ? 3 : 4;

    ImGui::PushFont(io.Fonts->Fonts[fontIndex]);

    D2GameStrc* pGame = *gCurrentSinglePlayerGame;

    if (!gMouseHover->IsHovered) {
        return;
    }
    if (gMouseHover->HoveredUnitType > 0x1) {
        return;
    }


    //Retreive UNIT info using PTR functions (D2Ptrs.h)
    D2UnitStrc* pUnit = UNITS_GetClientUnitByIdAndType(gMouseHover->HoveredUnitId, gMouseHover->HoveredUnitType);
    D2UnitStrc* pUnitPlayer = UNITS_GetClientUnitByIdAndType(1, UNIT_PLAYER);
    D2UnitStrc* pUnitServer = UNITS_GetServerUnitByTypeAndId(pGame, gMouseHover->HoveredUnitType, gMouseHover->HoveredUnitId);


    //Check if HP is greater than 0 (avoid displaying NPC stats)
    if (STATLIST_GetUnitStatSigned(pUnitServer, STAT_HITPOINTS, 0) != 0)
    {
        //Retreive all Resistance stats and display them equally spaced and centered based on resolution variables
        if (pUnit) {
            float totalWidth = 0.f;
            float spaceWidth = ImGui::CalcTextSize(Seperator).x;
            std::string resistances[6];
            float widths[6];

            for (int i = 0; i < 6; i++) {
                int resistanceValue = STATLIST_GetUnitStatSigned(pUnit, ResistanceStats[i], 0);
                resistances[i] = std::format("{}", resistanceValue);
            }

            for (int i = 0; i < 6; i++) {
                if (i > 0) {
                    totalWidth += spaceWidth;
                }
                widths[i] = ImGui::CalcTextSize(resistances[i].c_str()).x;
                totalWidth += widths[i];
            }
            auto startX = center - (totalWidth / 2.f);
            for (int i = 0; i < 6; i++) {
                if (i > 0) {
                    startX += spaceWidth;
                }

                drawList->AddText({ startX, ypercent1 }, ResistanceColors[i], resistances[i].c_str());
                startX += widths[i];
            }

            //Retrieve HP stat and add both value and % of max to display
            if (pGame) {
                if (pUnitServer) {
                    auto hp = std::format("{} / {}",
                        STATLIST_GetUnitStatSigned(pUnitServer, STAT_HITPOINTS, 0) >> 8,
                        STATLIST_GetUnitStatSigned(pUnitServer, STAT_MAXHP, 0) >> 8);
                    auto width = ImGui::CalcTextSize(hp.c_str()).x;
                    drawList->AddText({ center - (width / 2.0f) + 1, ypercent2 }, IM_COL32(255, 255, 255, 255), hp.c_str());


                    /* Debug Example - Retrieves stat references from D2Enums.h */

                    //std::string playerStrength = std::format("Player Strength: {}", STATLIST_GetUnitStatSigned(pUnitPlayer, STAT_ITEM_MAGICARROW, 0));
                    //drawList->AddText({ 20, 10 }, IM_COL32(170, 50, 50, 255), playerStrength.c_str());                
                }
            }
        }
    }
}
