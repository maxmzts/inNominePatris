#include "SpawnPlayerInteraction.h"
#include "Character.h"
#include <iostream>

// Inicialización de la variable estática
std::unordered_map<int, bool> SpawnPlayerInteraction::s_portalUsageState;

SpawnPlayerInteraction::SpawnPlayerInteraction(int id, const std::string& name, 
                                              float destX1, float destY1, 
                                              float destX2, float destY2, 
                                              bool autoTrigger)
    : Interaction(id, name), 
      m_destX1(destX1), 
      m_destY1(destY1), 
      m_destX2(destX2), 
      m_destY2(destY2), 
      m_autoTrigger(autoTrigger) {
}

void SpawnPlayerInteraction::execute(Character& character, TileMap& tilemap) {
    // Obtener el ID del portal
    int portalId = getId();
    
    float targetX, targetY;
    
    // Comprobar si este portal ha sido usado antes
    bool useFirstDest = true;
    auto it = s_portalUsageState.find(portalId);
    if (it != s_portalUsageState.end()) {
        useFirstDest = !it->second;
    }
    
    if (useFirstDest) {
        targetX = m_destX1;
        targetY = m_destY1;
        //std::cout << "Teletransportando jugador al primer destino (" << targetX << ", " << targetY << ") con " << getName() << std::endl;
    } else {
        targetX = m_destX2;
        targetY = m_destY2;
        //std::cout << "Teletransportando jugador de regreso (" << targetX << ", " << targetY << ") con " << getName() << std::endl;
    }
    
    // Actualizar el estado para este portal
    s_portalUsageState[portalId] = useFirstDest;
    
    // Llama a la función spawnAt del personaje para teletransportarlo
    character.spawnAt(tilemap, targetX, targetY);
}

bool SpawnPlayerInteraction::isAvailable(Character& character, TileMap& tilemap) const {
    // Siempre disponible
    return true;
}

void SpawnPlayerInteraction::resetPortalStates() {
    s_portalUsageState.clear();
}