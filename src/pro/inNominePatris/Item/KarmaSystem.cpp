#include "KarmaSystem.h"
#include "Weapon.h"
KarmaSystem::KarmaSystem(Character* character)
    : karmaPoints(0), pecadoCount(0), absolucionCount(0), character(character) {
    // Inicializar las mejoras permanentes
    upgrades.push_back(Upgrade("Daño aumentado", KarmaType::Pecado, 100, [&](){
        Weapon::increaseGlobalDamageMultiplier(0.35f); // Aumentar el daño global
    })); // Mejora de Pecado
    upgrades.push_back(Upgrade("Velocidad de movimiento", KarmaType::Absolucion, 100, [&]() {
        character->increaseMovementSpeed(75);
    })); // Mejora de Absolución
    upgrades.push_back(Upgrade("Probabilidad de esquivar ataques", KarmaType::Pecado, 200, [&](){
        character->increaseDodgeChance(0.05f); // Aumentar la probabilidad de esquivar ataques
    })); // Mejora de Pecado
    upgrades.push_back(Upgrade("Bonus de monedas", KarmaType::Absolucion, 200, [&]() {})); // Mejora de Absolución
    upgrades.push_back(Upgrade("Probabilidad de crítico", KarmaType::Pecado, 300, [&](){
        Weapon::increaseGlobalCriticalChanceBonus(0.05f); // Aumentar la probabilidad de crítico global
    })); // Mejora de Pecado
    upgrades.push_back(Upgrade("Cantidad de vidas", KarmaType::Absolucion, 300, [&](){
        character->increaseMaxHealth(1);
    })); // Mejora de Absolución
    upgrades.push_back(Upgrade("Daño crítico aumentado", KarmaType::Pecado, 500, [&](){
        Weapon::increaseGlobalCriticalMultiplier(0.25f); // Aumentar el daño crítico global
    })); // Mejora avanzada de Pecado
    upgrades.push_back(Upgrade("Regeneración de vida", KarmaType::Absolucion, 500, [&](){
        character->enableHealthRegeneration(); // Habilitar regeneración de vida
    })); // Mejora avanzada de Absolución
    upgrades.push_back(Upgrade("Combo de ataques", KarmaType::Pecado, 700, [&](){
        Weapon::enableComboDamageBonus(0.05f); // Aumentar el daño por combo
    })); // Mejora avanzada de Pecado
    upgrades.push_back(Upgrade("Escudo temporal", KarmaType::Absolucion, 700, [&](){
        character->enableTemporalyShield(4.0f); // Habilitar escudo temporal durante 4 segundos
    })); // Mejora avanzada de Absolución
}

void KarmaSystem::addKarma(int amount) {
    karmaPoints += amount;
    std::cout << "Karma añadido: " << amount << ". Total: " << karmaPoints << std::endl;
}

bool KarmaSystem::purchaseUpgrade(int upgradeIndex) {
    if (upgradeIndex < 0 || upgradeIndex >= upgrades.size()) {
        std::cout << "Índice de mejora inválido." << std::endl;
        return false;
    }

    Upgrade& upgrade = upgrades[upgradeIndex];

    if (upgrade.isUnlocked || upgrade.isBlocked) {
        std::cout << "Esta mejora ya está desbloqueada o bloqueada." << std::endl;
        return false;
    }

    if (karmaPoints < upgrade.cost) {
        std::cout << "No tienes suficiente karma para esta mejora." << std::endl;
        return false;
    }

    // Desbloquear la mejora seleccionada
    karmaPoints -= upgrade.cost;
    upgrade.isUnlocked = true;

    // Bloquear la mejora opuesta
    for (size_t i = 0; i < upgrades.size(); i++) {
        if (i != upgradeIndex && upgrades[i].type != upgrade.type && !upgrades[i].isUnlocked) {
            upgrades[i].isBlocked = true;
            break;
        }
    }

    // Incrementar el contador de mejoras según el tipo
    if (upgrade.type == KarmaType::Pecado) {
        pecadoCount++;
    } else {
        absolucionCount++;
    }

    std::cout << "Mejora desbloqueada: " << upgrade.name << std::endl;
    return true;
}

void KarmaSystem::displayUpgrades() const {
    std::cout << "Mejoras disponibles:" << std::endl;
    for (size_t i = 0; i < upgrades.size(); i++) {
        const Upgrade& upgrade = upgrades[i];
        std::cout << i << ". " << upgrade.name
                  << " (Costo: " << upgrade.cost << ", Tipo: "
                  << (upgrade.type == KarmaType::Pecado ? "Pecado" : "Absolución") << ")"
                  << (upgrade.isUnlocked ? " [Desbloqueada]" : "")
                  << (upgrade.isBlocked ? " [Bloqueada]" : "")
                  << std::endl;
    }
}

int KarmaSystem::getPecadoCount() const {
    return pecadoCount;
}

int KarmaSystem::getAbsolucionCount() const {
    return absolucionCount;
}

int KarmaSystem::getKarmaPoints() const {
    return karmaPoints;
}