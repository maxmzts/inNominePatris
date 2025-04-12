// DoorInteraction.cpp
#include "DoorInteraction.h"
#include "Character.h"
#include <iostream>

DoorInteraction::DoorInteraction(int id, const std::string& name, bool isOpen, int centerX, int centerY)
    : Interaction(id, name),
    m_centerX(centerX), 
    m_centerY(centerY),
    m_isOpen(isOpen) {
}

void DoorInteraction::execute(Character& character, TileMap& tilemap) {
    if (m_isOpen) {
        std::cout << "La puerta " << getName() << " está abierta." << std::endl;
        m_isOpen = false; //reiniciamos bool para evitar que este codigo se realice repetidas veces
        // eliminamos barrera de deco
        tilemap.setLocalTile("deco", m_centerX, m_centerY, -1); 
        tilemap.setLocalTile("deco", m_centerX + 1, m_centerY, -1); 
        tilemap.setLocalTile("deco", m_centerX + 2, m_centerY, -1); 
        tilemap.setLocalTile("deco", m_centerX + 3, m_centerY, -1); 
        tilemap.setLocalTile("deco", m_centerX + 4, m_centerY, -1); 
        tilemap.setLocalTile("deco", m_centerX + 5, m_centerY, -1); 
        // eliminamos barrera de bounds
        // TENGO QUE ARREGLAR EL CODIGO PARA QUE PUEDA QUITAR BLOQUES PORQUE NO ESTÁ FUNCIONANDO
    } else {
        std::cout << "La puerta " << getName() << " está cerrada." << std::endl;
    }
}

bool DoorInteraction::isAvailable(Character& character, TileMap& tilemap) const {
    // Siempre disponible
    return true;
}