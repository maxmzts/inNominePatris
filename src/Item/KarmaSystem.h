#ifndef KARMASYSTEM_H
#define KARMASYSTEM_H

#include <vector>
#include <string>
#include <iostream>
#include <functional>
#include "Character.h"

enum class KarmaType {
    Pecado,
    Absolucion
};

struct Upgrade {
    int id;
    std::string name;
    KarmaType type;
    int cost;
    bool isUnlocked;
    bool isBlocked;
    std::function<void()> action;

    Upgrade(int id, std::string name, KarmaType type, int cost, std::function<void()> action)
        : id(id), name(name), type(type), cost(cost), isUnlocked(false), isBlocked(false), action(action) {}
};

class KarmaSystem {
private:
    std::vector<Upgrade> upgrades;
    int pecadoCount;
    int absolucionCount;
    Character& character; // Referencia al personaje

public:
    KarmaSystem(Character& character);

    bool purchaseUpgrade(int upgradeIndex);
    bool setUpgrade(int upgradeIndex);
    void displayUpgrades() const;
    int getPecadoCount() const;
    int getAbsolucionCount() const;
    int getKarmaPoints() const;
    const std::vector<Upgrade>& getUpgrades() const { return upgrades; }
    std::vector<int> getPurchasedUpgrades() const { 
        std::vector<int> upgradesIndexes = {};
        for(Upgrade upgrade : upgrades) {
            if(upgrade.isUnlocked) upgradesIndexes.push_back(upgrade.id);
        }
        return upgradesIndexes; 
    }
};

#endif // KARMASYSTEM_H