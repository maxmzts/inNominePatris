#pragma once
#include <vector>
#include "Item.h"
#include "Weapon.h"

class ItemManager {
private:
    static ItemManager* instance;
    std::vector<Item*> activeRunItems;
    ItemManager() = default;
    ItemManager(const ItemManager&) = delete;
    ItemManager& operator=(const ItemManager&) = delete;
public:
    static ItemManager* getInstance();
    static void resetInstance();

    void addActiveItem(Item* item);
    void clearRunItemEffects(const std::vector<Weapon*>& weapons);
};