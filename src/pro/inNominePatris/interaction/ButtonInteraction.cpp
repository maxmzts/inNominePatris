#include "ButtonInteraction.h"
#include "Character.h"
#include "InteractionManager.h"
#include <iostream>
#include "SFML/Graphics.hpp"

ButtonInteraction::ButtonInteraction(int id, const std::string& name, int centerX, int centerY)
    : Interaction(id, name), m_centerX(centerX), m_centerY(centerY), m_isActivated(false) {
}

void ButtonInteraction::execute(Character& character, TileMap& tilemap) {
    if (!m_isActivated) {
        std::cout << "Activando " << getName() << " (ID: " << getId() << ")" << std::endl;
        
        // Actualizar la capa de decoración (abrir jaula)
        tilemap.setLocalTile("deco", m_centerX - 2, m_centerY, 64);     // izquierda
        tilemap.setLocalTile("deco", m_centerX - 1, m_centerY, 65);     // centro
        tilemap.setLocalTile("deco", m_centerX, m_centerY, 66);         // derecha
        tilemap.setLocalTile("deco", m_centerX - 2, m_centerY + 1, 80); // izquierda abajo
        tilemap.setLocalTile("deco", m_centerX - 1, m_centerY + 1, 81); // centro abajo
        tilemap.setLocalTile("deco", m_centerX, m_centerY + 1, 82);     // derecha abajo
            
        // Registrar que este botón ha sido activado
        m_isActivated = true;
        
        // Registrar en el InteractionManager
        InteractionManager::getInstance()->registerButtonPress(getId());
    } else {
        std::cout << "El botón " << getName() << " ya está activado." << std::endl;
    }
}

bool ButtonInteraction::isAvailable(Character& character, TileMap& tilemap) const {
    return !m_isActivated; // Solo está disponible si no ha sido activado
}


std::string ButtonInteraction::getProximityMessage() const {
    if (!m_isActivated) {
        return "Pulsar con la \"E\"";
    }
    return ""; // No mostrar mensaje si ya está activado
}

sf::FloatRect ButtonInteraction::getBounds() const {
    return sf::FloatRect(m_centerX - 1, m_centerY - 1, 2, 2); // Ajusta el tamaño del área de proximidad
}