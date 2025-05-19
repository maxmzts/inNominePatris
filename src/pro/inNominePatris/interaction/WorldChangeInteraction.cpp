
#include "WorldChangeInteraction.h"
#include "Character.h"
#include "InGame.h"
#include "MusicManager.h"
#include <iostream>

WorldChangeInteraction::WorldChangeInteraction(int id, const std::string& name, 
                                              const std::string& targetWorldState,
                                              const std::string& mapFilePath,
                                              const std::string& musicFilePath,
                                              int spawnX, int spawnY,
                                              bool autoTrigger)
    : Interaction(id, name), 
      m_targetWorldState(targetWorldState),
      m_mapFilePath(mapFilePath),
      m_musicFilePath(musicFilePath),
      m_spawnPosition(spawnX, spawnY),
      m_autoTrigger(autoTrigger) {
}

void WorldChangeInteraction::execute(Character& character, TileMap& tilemap) {
  // std::cout << "Cambiando al estado del mundo: " << m_targetWorldState << std::endl;
    
    // El cambio de estado actual debe ser manejado por InGame
    // Este método se llama en checkAutoInteractions() en InGame.cpp
}

bool WorldChangeInteraction::isAvailable(Character& character, TileMap& tilemap) const {
    // Siempre disponible
    return true;
}

void WorldChangeInteraction::changeWorldState(InGame& inGame, GameEngine& engine) {
  // std::cout << "Iniciando cambio de mundo a: " << m_targetWorldState << std::endl;
    
    // 1. Cargar el nuevo mapa (la función loadFromFile ya incluye clear() ahora)
    if (!inGame.getTileMap().loadFromFile(m_mapFilePath, engine)) {
        std::cerr << "Error cargando el mapa: " << m_mapFilePath << std::endl;
        return;
    }
    
    // 2. Hacer spawn al jugador en la nueva posición
    inGame.getPlayer().spawnAt(inGame.getTileMap(), m_spawnPosition.x, m_spawnPosition.y);
  // std::cout << "Jugador reposicionado en: (" << m_spawnPosition.x << ", " << m_spawnPosition.y << ")" << std::endl;
    
    // 3. Cargar la música del nuevo mundo
    MusicManager::getInstance().clear(); // Limpia la música anterior
    MusicManager::getInstance().addTrack(m_musicFilePath);
  // std::cout << "Música cargada desde: " << m_musicFilePath << std::endl;
    
    // 4. Cambiar al estado del mundo especificado
    //inGame.changeWorldState(m_targetWorldState);
  // std::cout << "Estado cambiado a: " << m_targetWorldState << std::endl;
}

void WorldChangeInteraction::getWorldInfo(std::string& targetWorldState, std::string& mapFilePath, std::string& musicFilePath, sf::Vector2i& spawnPosition){
    targetWorldState = m_targetWorldState;
    mapFilePath = m_mapFilePath;
    musicFilePath = m_musicFilePath;
    spawnPosition = m_spawnPosition;
}

