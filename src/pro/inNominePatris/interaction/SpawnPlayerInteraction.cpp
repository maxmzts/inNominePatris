#include "SpawnPlayerInteraction.h"
#include "Character.h"
#include <iostream>

SpawnPlayerInteraction::SpawnPlayerInteraction(int id, const std::string& name, float destX, float destY, bool autoTrigger)
    : Interaction(id, name), m_destX(destX), m_destY(destY), m_autoTrigger(autoTrigger) {
}

void SpawnPlayerInteraction::execute(Character& character, TileMap& tilemap) {
    std::cout << "Teletransportando jugador a la posición (" << m_destX << ", " << m_destY << ") con " << getName() << std::endl;
    
    // Llama a la función spawnAt del personaje para teletransportarlo
    character.spawnAt(tilemap, m_destX, m_destY);
}

bool SpawnPlayerInteraction::isAvailable(Character& character, TileMap& tilemap) const {
    // Siempre disponible
    return true;
}