#include "InteractionManager.h"
#include "Character.h"
#include "DoorInteraction.h"
#include <iostream>

InteractionManager::InteractionManager() {
    // Registrar interacciones iniciales (si las hay)
}

void InteractionManager::registerInteraction(std::shared_ptr<Interaction> interaction) {
    m_interactions[interaction->getId()] = interaction;
}

bool InteractionManager::executeInteraction(int id, Character& character, TileMap& tilemap) {
    auto it = m_interactions.find(id);
    if (it != m_interactions.end() && it->second->isAvailable(character, tilemap)) {
        it->second->execute(character, tilemap);
        
        // Trackear botones presionados
        if (id == 772) m_button1Pressed = true;
        if (id == 773) m_button2Pressed = true;
        if (id == 774) m_button3Pressed = true;
        
        // Si todos los botones están presionados, ejecutar la interacción de la puerta
        if (areAllButtonsPressed()) {
            // Crear temporalmente una interacción de puerta
            DoorInteraction doorInteraction(69, "Puerta Principal", true, 27, 7);
            doorInteraction.execute(character, tilemap);
        }
        
        return true;
    }
    return false;
}

bool InteractionManager::checkInteraction(Character& character, TileMap& tilemap) {
    sf::FloatRect playerBounds = character.getBounds();
    int tileId = -1;
    
    if (tilemap.isPlayerInteractingWithTile(playerBounds, tileId)) {
        return executeInteraction(tileId, character, tilemap);
    } else {
        std::cout << "No hay nada con lo que interactuar aquí." << std::endl;
        return false;
    }
}

bool InteractionManager::areAllButtonsPressed() const {
    return m_button1Pressed && m_button2Pressed && m_button3Pressed;
}