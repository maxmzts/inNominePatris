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
    std::string name;
    KarmaType type;
    int cost;
    bool isUnlocked;
    bool isBlocked;
    std::function<void()> action;

    Upgrade(std::string name, KarmaType type, int cost, std::function<void()> action)
        : name(name), type(type), cost(cost), isUnlocked(false), isBlocked(false), action(action) {}
};

class KarmaSystem {
private:
    int karmaPoints;
    std::vector<Upgrade> upgrades;
    int pecadoCount;
    int absolucionCount;
    Character* character; // Referencia al personaje

public:
    KarmaSystem(Character* character);

    void addKarma(int amount);
    bool purchaseUpgrade(int upgradeIndex);
    void displayUpgrades() const;
    int getPecadoCount() const;
    int getAbsolucionCount() const;
    int getKarmaPoints() const;
};

#endif // KARMASYSTEM_H