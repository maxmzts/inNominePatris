#include "DoorInteraction.h"
#include "Character.h"
#include "InteractionManager.h"
#include <iostream>

DoorInteraction::DoorInteraction(int id, const std::string& name, bool isOpen, int centerX, int centerY)
    : Interaction(id, name),
    m_centerX(centerX), 
    m_centerY(centerY),
    m_isOpen(isOpen) {
}

void DoorInteraction::execute(Character& character, TileMap& tilemap) {
    // Verificar si todos los botones requeridos están activados
    if (InteractionManager::getInstance()->checkDoorRequirements(getId())) {
        if (!m_isOpen) {
            std::cout << "¡Abriendo puerta " << getName() << "!" << std::endl;
            m_isOpen = true;
            
            // Eliminar barrera de decoración
            tilemap.setLocalTile("deco", m_centerX, m_centerY, -1); 
            tilemap.setLocalTile("deco", m_centerX + 1, m_centerY, -1); 
            tilemap.setLocalTile("deco", m_centerX + 2, m_centerY, -1); 
            tilemap.setLocalTile("deco", m_centerX + 3, m_centerY, -1); 
            tilemap.setLocalTile("deco", m_centerX + 4, m_centerY, -1); 
            tilemap.setLocalTile("deco", m_centerX + 5, m_centerY, -1); 
            
            // Eliminar barrera de colisiones
            // Aquí puedes añadir el código para quitar los bloques de colisión
        } else {
            std::cout << "La puerta " << getName() << " ya está abierta." << std::endl;
        }
    } else {
        std::cout << "No se puede abrir la puerta " << getName() << ". Faltan botones por activar." << std::endl;
    }
}

bool DoorInteraction::isAvailable(Character& character, TileMap& tilemap) const {
    // La puerta está disponible para interacción siempre,
    // pero solo se abrirá si se cumplen los requisitos
    return true;
}