#include "KarmaSystem.h"
#include "Weapon.h"
KarmaSystem::KarmaSystem(Character& character)
    : pecadoCount(0), absolucionCount(0), character(character) {
    // Inicializar las mejoras permanentes
    upgrades.push_back(Upgrade("Daño aumentado", KarmaType::Pecado, 100, [&](){
        Weapon::increaseGlobalDamageMultiplier(0.2f); // Aumentar el daño global
    })); // Mejora de Pecado
    upgrades.push_back(Upgrade("Velocidad de movimiento", KarmaType::Absolucion, 100, [&]() {
        character.increaseMovementSpeed(75.0f);
    })); // Mejora de Absolución
    upgrades.push_back(Upgrade("Probabilidad de esquivar ataques", KarmaType::Pecado, 200, [&](){
        character.increaseDodgeChance(0.05f); // Aumentar la probabilidad de esquivar ataques
    })); // Mejora de Pecado
    upgrades.push_back(Upgrade("Bonus de monedas", KarmaType::Absolucion, 200, [&]() {})); // Mejora de Absolución
    
    upgrades.push_back(Upgrade("Probabilidad de crítico", KarmaType::Pecado, 300, [&](){
        Weapon::increaseGlobalCriticalChanceBonus(0.05f); // Aumentar la probabilidad de crítico global
    })); // Mejora de Pecado
    upgrades.push_back(Upgrade("Cantidad de vidas", KarmaType::Absolucion, 300, [&](){
        character.increaseMaxHealth(1);
    })); // Mejora de Absolución
    upgrades.push_back(Upgrade("Daño crítico aumentado", KarmaType::Pecado, 500, [&](){
        Weapon::increaseGlobalCriticalMultiplier(0.25f); // Aumentar el daño crítico global
    })); // Mejora avanzada de Pecado
    upgrades.push_back(Upgrade("Regeneración de vida", KarmaType::Absolucion, 500, [&](){
        character.enableHealthRegeneration(); // Habilitar regeneración de vida
    })); // Mejora avanzada de Absolución
    upgrades.push_back(Upgrade("Combo de ataques", KarmaType::Pecado, 700, [&](){
        Weapon::enableComboDamageBonus(0.1f); // Aumentar el daño por combo
    })); // Mejora avanzada de Pecado
    upgrades.push_back(Upgrade("Escudo temporal", KarmaType::Absolucion, 700, [&](){
        character.enableTemporalyShield(4.0f); // Habilitar escudo temporal durante 4 segundos
    })); // Mejora avanzada de Absolución
}

bool KarmaSystem::purchaseUpgrade(int upgradeIndex) {
    if (upgradeIndex < 0 || upgradeIndex >= upgrades.size()) {
        std::cout << "Índice de mejora inválido." << std::endl;
        return false;
    }

    Upgrade& upgrade = upgrades[upgradeIndex];

    // Verificar si la mejora ya está desbloqueada o bloqueada
    if (upgrade.isUnlocked) {
        std::cout << "Esta mejora ya está desbloqueada." << std::endl;
        return false;
    }
    
    if (upgrade.isBlocked) {
        std::cout << "Esta mejora está bloqueada y no puede ser comprada." << std::endl;
        return false;
    }

    // Verificar si hay suficiente karma
    if (character.getKarma() < upgrade.cost) {
        std::cout << "No tienes suficiente karma para esta mejora." << std::endl;
        return false;
    }

    // Verificar si se pueden desbloquear según el orden
    // Nivel 1: Mejoras iniciales disponibles siempre
    // Nivel 2+: Solo disponible si el nivel anterior tiene al menos una mejora comprada
    int level = upgradeIndex / 2;
    if (level > 0) {
        bool previousLevelUnlocked = false;
        for (int i = (level-1)*2; i < level*2 && i < upgrades.size(); i++) {
            if (upgrades[i].isUnlocked) {
                previousLevelUnlocked = true;
                break;
            }
        }
        
        if (!previousLevelUnlocked) {
            std::cout << "Debes desbloquear al menos una mejora del nivel anterior." << std::endl;
            return false;
        }
    }

    // Desbloquear la mejora seleccionada
    character.addKarma(-upgrade.cost); // Restar el costo de karma
    upgrade.isUnlocked = true;
    
    // Ejecutar la acción de la mejora
    if (upgrade.action) {
        std::cout << "Ejecutando acción para: " << upgrade.name << std::endl;
        upgrade.action();
        std::cout << "Acción ejecutada correctamente" << std::endl;
    }

    // Bloquear la mejora opuesta del mismo nivel
    int pairedIndex = (upgradeIndex % 2 == 0) ? upgradeIndex + 1 : upgradeIndex - 1;
    if (pairedIndex >= 0 && pairedIndex < upgrades.size() && !upgrades[pairedIndex].isUnlocked) {
        upgrades[pairedIndex].isBlocked = true;
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
    return character.getKarma(); // Obtener karma del personaje
}