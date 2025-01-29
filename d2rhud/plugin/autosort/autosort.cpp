//plugin/autosort/autosort.cpp
#include "autosort.h"
#include <imgui.h>
#include <Windows.h>
#include "../../D2/D2Ptrs.h"
#include "../KeyMappings.h"
#include <iostream>
#include <fstream>
#include <string>


bool AutoSort::OnKeyPressed(short key)
{
    std::string filename = "./D2RHUD_Config.txt";
    std::string searchString = "Sort Stash: ";
    std::string searchStringB = "Sort Inventory: ";
    std::string searchStringC = "Sort Cube: ";
    std::string result = readTextFollowingString(filename, searchString);
    std::string resultB = readTextFollowingString(filename, searchStringB);
    std::string resultC = readTextFollowingString(filename, searchStringC);

    //Sort Stash
    if (!result.empty()) {
        result.erase(result.find_last_not_of(" \n\r\t") + 1);
        result.erase(0, result.find_first_not_of(" \n\r\t"));

        auto it = keyMap.find(result);
        if (it != keyMap.end() && key == it->second) {
            INVENTORY_AutoSort(1, INVPAGE_STASH);
            return true;
        }
    }

    //Sort Inventory
    if (!resultB.empty()) {
        resultB.erase(resultB.find_last_not_of(" \n\r\t") + 1);
        resultB.erase(0, resultB.find_first_not_of(" \n\r\t"));

        auto it = keyMap.find(resultB);
        if (it != keyMap.end() && key == it->second) {
            INVENTORY_AutoSort(1, INVPAGE_INVENTORY);
            return true;
        }
    }

    //Sort Cube
    if (!resultC.empty()) {
        resultC.erase(resultC.find_last_not_of(" \n\r\t") + 1);
        resultC.erase(0, resultC.find_first_not_of(" \n\r\t"));

        auto it = keyMap.find(resultC);
        if (it != keyMap.end() && key == it->second) {
            INVENTORY_AutoSort(1, INVPAGE_CUBE);
            return true;
        }
    }

    return false;
}
