#include "InteractionManager.h"
#include <iostream>

InteractionManager* InteractionManager::instance = nullptr;

InteractionManager::InteractionManager() {
    // Configurar los requisitos iniciales para la puerta principal (ID 69) en mundo 1
    // Requiere los tres botones (IDs 772, 773, 774)
    std::vector<int> requiredButtonsWorld1 = {772, 773, 774};
    setDoorRequirements(69, requiredButtonsWorld1);
    
    // Configurar los requisitos para la puerta principal (ID 1242) en mundo 2
    // Requiere los tres botones (IDs 1233, 1234, 1235)
    std::vector<int> requiredButtonsWorld2 = {1233, 1234, 1235};
    setDoorRequirements(1242, requiredButtonsWorld2);
}

InteractionManager* InteractionManager::getInstance() {
    if (!instance) {
        instance = new InteractionManager();
    }
    return instance;
}

void InteractionManager::registerButtonPress(int buttonId) {
    m_activatedButtons[buttonId] = true;
    
    // Imprimir el estado actual para depuración
    std::cout << "Botón " << buttonId << " activado." << std::endl;
    
    // Verificar todas las puertas que puedan verse afectadas por este botón
    for (const auto& pair : m_doorRequirements) {
        int doorId = pair.first;
        const std::set<int>& requirements = pair.second;
        
        // Verificar si este botón es relevante para esta puerta
        if (requirements.find(buttonId) != requirements.end()) {
            if (checkDoorRequirements(doorId)) {
                std::cout << "¡Todos los botones activados para la puerta " << doorId << "!" << std::endl;
            }
        }
    }
}

bool InteractionManager::checkDoorRequirements(int doorId) {
    // Si la puerta no tiene requisitos, devolver false
    if (m_doorRequirements.find(doorId) == m_doorRequirements.end()) {
        return false;
    }
    
    // Verificar si todos los botones requeridos para esta puerta están activados
    const std::set<int>& requiredButtons = m_doorRequirements[doorId];
    for (int buttonId : requiredButtons) {
        if (m_activatedButtons.find(buttonId) == m_activatedButtons.end() || !m_activatedButtons[buttonId]) {
            return false; // Al menos un botón requerido no está activado
        }
    }
    
    return true; // Todos los botones requeridos están activados
}

void InteractionManager::addDoorRequirement(int doorId, int buttonId) {
    if (m_doorRequirements.find(doorId) == m_doorRequirements.end()) {
        m_doorRequirements[doorId] = std::set<int>();
    }
    m_doorRequirements[doorId].insert(buttonId);
}

void InteractionManager::setDoorRequirements(int doorId, const std::vector<int>& buttonIds) {
    m_doorRequirements[doorId] = std::set<int>(buttonIds.begin(), buttonIds.end());
}

void InteractionManager::resetButtons() {
    m_activatedButtons.clear();
}

InteractionManager::~InteractionManager() {
    // Cleanup if needed
}