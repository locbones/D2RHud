#pragma once
#include "D2Structs.h"
#include "../Pattern.h"

FUNC_DEF(int32_t, __fastcall, STATLIST_GetUnitStatSigned, (D2UnitStrc* pUnit, uint32_t nStatId, uint16_t nLayer));


FUNC_DEF(D2UnitStrc*, __fastcall, UNITS_GetClientUnitByIdAndType, (uint32_t nUnitId, D2C_UnitTypes nUnitType));
FUNC_DEF(D2UnitStrc*, __fastcall, UNITS_GetServerUnitByTypeAndId, (D2GameStrc* pGame, D2C_UnitTypes nUnitType, uint32_t nUnitId));

VAR_DEF(D2MouseHoverStruct, gMouseHover);
VAR_DEF(D2GameStrc*, gCurrentSinglePlayerGame);

namespace D2Ptrs {
	// All verified and working on
	// 1.4.71510 
	static void Initialize() {
		FUNC_PATTERN(STATLIST_GetUnitStatSigned, NULL, "48 83 EC 28 45 0F B7 C8 48 85 C9 74 42 48 8B 89 ? ? ? ? 48 85 C9 74 2F");

		FUNC_PATTERNREF(UNITS_GetClientUnitByIdAndType, NULL, "E8 ? ? ? ? 8B 4F 05", 1);
		FUNC_PATTERNREF(UNITS_GetServerUnitByTypeAndId, NULL, "E8 ? ? ? ? 46 8D 04 37 ", 1);

		VAR_PATTERNREF(gMouseHover, NULL, "48 8D 3D ? ? ? ? BB ? ? ? ? 48 8B CF E8 ? ? ? ? 48 83 C7 10 ", 3);
		VAR_PATTERNREF(gCurrentSinglePlayerGame, NULL, "48 89 05 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 44 8B C7 ", 3);
	}
};