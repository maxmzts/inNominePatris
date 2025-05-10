#include "WorldChangeInteraction.h"
#include "Character.h"
#include "InGame.h"
#include <iostream>

WorldChangeInteraction::WorldChangeInteraction(int id, const std::string& name, 
                                              const std::string& targetWorldState,
                                              const std::string& mapFilePath,
                                              bool autoTrigger)
    : Interaction(id, name), 
      m_targetWorldState(targetWorldState),
      m_mapFilePath(mapFilePath),
      m_autoTrigger(autoTrigger) {
}

void WorldChangeInteraction::execute(Character& character, TileMap& tilemap) {
    std::cout << "Cambiando al estado del mundo: " << m_targetWorldState << std::endl;
    
    // El cambio de estado actual debe ser manejado por InGame
    // Este método se llama en checkAutoInteractions() en InGame.cpp
}

bool WorldChangeInteraction::isAvailable(Character& character, TileMap& tilemap) const {
    // Siempre disponible
    return true;
}

void WorldChangeInteraction::changeWorldState(InGame& inGame, GameEngine& engine) {
    // Cargar el nuevo mapa
    // if (!inGame.getTileMap().loadFromFile(m_mapFilePath, engine)) {
    //     std::cerr << "Error cargando el mapa: " << m_mapFilePath << std::endl;
    //     return;
    // }
    
    // Cambiar al estado del mundo especificado
    inGame.changeWorldState(m_targetWorldState);
    std::cout << "Estado cambiado a: " << m_targetWorldState << std::endl;

}