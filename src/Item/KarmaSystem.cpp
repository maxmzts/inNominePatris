#include "KarmaSystem.h"
#include "Weapon.h"
KarmaSystem::KarmaSystem(Character& character)
    : pecadoCount(0), absolucionCount(0), character(character) {
    // Inicializar las mejoras permanentes
    upgrades.push_back(Upgrade(1, "Dano aumentado", KarmaType::Pecado, 100, [&](){
        Weapon::increaseGlobalDamageMultiplier(0.2f); // Aumentar el daño global
    })); // Mejora de Pecado
    upgrades.push_back(Upgrade(2, "Velocidad de movimiento", KarmaType::Absolucion, 100, [&]() {
        character.increaseMovementSpeed(75.0f);
    })); // Mejora de Absolución
    upgrades.push_back(Upgrade(3, "Probabilidad de esquivar ataques", KarmaType::Pecado, 200, [&](){
        character.increaseDodgeChance(0.1f); // Aumentar la probabilidad de esquivar ataques
    })); // Mejora de Pecado
    upgrades.push_back(Upgrade(4, "Bonus de monedas", KarmaType::Absolucion, 200, [&]() {
        character.enableCoinBonus(); // Habilitar el bonus de monedas
    })); // Mejora de Absolución
    upgrades.push_back(Upgrade(5, "Probabilidad de critico", KarmaType::Pecado, 300, [&](){
        Weapon::increaseGlobalCriticalChanceBonus(0.05f); // Aumentar la probabilidad de crítico global
    })); // Mejora de Pecado
    upgrades.push_back(Upgrade(6, "Cantidad de vidas", KarmaType::Absolucion, 300, [&](){
        character.increaseMaxHealth(1);
    })); // Mejora de Absolución
    upgrades.push_back(Upgrade(7, "Dano critico aumentado", KarmaType::Pecado, 500, [&](){
        Weapon::increaseGlobalCriticalMultiplier(0.25f); // Aumentar el daño crítico global
    })); // Mejora avanzada de Pecado
    upgrades.push_back(Upgrade(8, "Regeneracion de vida", KarmaType::Absolucion, 500, [&](){
        character.enableHealthRegeneration(); // Habilitar regeneración de vida
    })); // Mejora avanzada de Absolución
    upgrades.push_back(Upgrade(9, "Combo de ataques", KarmaType::Pecado, 700, [&](){
        Weapon::enableComboDamageBonus(0.1f); // Aumentar el daño por combo
    })); // Mejora avanzada de Pecado
    upgrades.push_back(Upgrade(10, "Escudo temporal", KarmaType::Absolucion, 700, [&](){
        character.enableTemporalyShield(4.0f); // Habilitar escudo temporal durante 4 segundos
    })); // Mejora avanzada de Absolución
}

bool KarmaSystem::purchaseUpgrade(int upgradeIndex) {
    if (upgradeIndex < 0 || upgradeIndex >= static_cast<int>(upgrades.size())) {
        // std::cout << "Índice de mejora inválido." << std::endl;
        return false;
    }

    Upgrade& upgrade = upgrades[upgradeIndex];

    // Verificar si la mejora ya está desbloqueada, bloqueada o no hay suficiente karma
    if (upgrade.isUnlocked || upgrade.isBlocked || character.getKarma() < upgrade.cost) {
        return false;
    }

    if(setUpgrade(upgradeIndex)) {
        character.QuitKarma(upgrade.cost);
        return true;
    }
    return false;
}

bool KarmaSystem::setUpgrade(int upgradeIndex){
    Upgrade& upgrade = upgrades[upgradeIndex];
    // Verificar si se pueden desbloquear según el orden
    // Nivel 1: Mejoras iniciales disponibles siempre
    // Nivel 2+: Solo disponible si el nivel anterior tiene al menos una mejora comprada
    int level = upgradeIndex / 2;
    if (level > 0) {
        bool previousLevelUnlocked = false;
        for (int i = (level-1)*2; i < level*2 && i < static_cast<int>(upgrades.size()); i++) {
            if (upgrades[i].isUnlocked) {
                previousLevelUnlocked = true;
                break;
            }
        }
        
        if (!previousLevelUnlocked) {
            // std::cout << "Debes desbloquear al menos una mejora del nivel anterior." << std::endl;
            return false;
        }
    }

    // Desbloquear la mejora seleccionada
    upgrade.isUnlocked = true;
    
    // Ejecutar la acción de la mejora
    if (upgrade.action) {
        // std::cout << "Ejecutando acción para: " << upgrade.name << std::endl;
        upgrade.action();
        // std::cout << "Acción ejecutada correctamente" << std::endl;
    }

    // Bloquear la mejora opuesta del mismo nivel
    int pairedIndex = (upgradeIndex % 2 == 0) ? upgradeIndex + 1 : upgradeIndex - 1;
    if (pairedIndex >= 0 && pairedIndex < static_cast<int>(upgrades.size()) && !upgrades[pairedIndex].isUnlocked) {
        upgrades[pairedIndex].isBlocked = true;
    }

    // Incrementar el contador de mejoras según el tipo
    if (upgrade.type == KarmaType::Pecado) {
        pecadoCount++;
    } else {
        absolucionCount++;
    }

    // std::cout << "Mejora desbloqueada: " << upgrade.name << std::endl;

    // std::cout << "Aplicada mejora: " << upgradeIndex << std::endl;
    // std::cout << "Aplicada mejora: " << upgrade.id << std::endl;

    return true;
}

void KarmaSystem::displayUpgrades() const {
    // std::cout << "Mejoras disponibles:" << std::endl;
    for (size_t i = 0; i < upgrades.size(); i++) {
        const Upgrade& upgrade = upgrades[i];
        std::cout << i << ". " << upgrade.name
                  << " (Costo: " << upgrade.cost << ", Tipo: "
                  << (upgrade.type == KarmaType::Pecado ? "Pecado" : "Absolucion") << ")"
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