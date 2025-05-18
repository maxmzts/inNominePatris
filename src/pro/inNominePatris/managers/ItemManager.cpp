#include "ItemManager.h"

ItemManager* ItemManager::instance = nullptr;

ItemManager* ItemManager::getInstance() {
    if (!instance) {
        instance = new ItemManager();
    }
    return instance;
}

void ItemManager::resetInstance() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

void ItemManager::addActiveItem(Item* item) {
    activeRunItems.push_back(item);
}

void ItemManager::clearRunItemEffects(const std::vector<Weapon*>& weapons) {
    for (Item* item : activeRunItems) {
        for (Weapon* weapon : weapons) {
            if (weapon->getItemType() == item->getType()) {
                item->revertEffect(*weapon);
                // DEBUG
                std::cout << "Efecto revertido para el arma de tipo: " << static_cast<int>(weapon->getItemType()) << std::endl;
            }
        }
    }
    activeRunItems.clear();
}