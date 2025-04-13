#include "ButtonInteraction.h"
#include "Character.h"
#include <iostream>

ButtonInteraction::ButtonInteraction(int id, const std::string& name, int centerX, int centerY)
    : Interaction(id, name), m_centerX(centerX), m_centerY(centerY) {
}

void ButtonInteraction::execute(Character& character, TileMap& tilemap) {
    std::cout << "Interactuando con " << getName() << std::endl;
    
    // Actualizar la capa de decoración (abrir jaula)
    tilemap.setLocalTile("deco", m_centerX - 2, m_centerY, 64);     // izquierda
    tilemap.setLocalTile("deco", m_centerX - 1, m_centerY, 65);     // centro
    tilemap.setLocalTile("deco", m_centerX, m_centerY, 66);         // derecha
    tilemap.setLocalTile("deco", m_centerX - 2, m_centerY + 1, 80); // izquierda abajo
    tilemap.setLocalTile("deco", m_centerX - 1, m_centerY + 1, 81); // centro abajo
    tilemap.setLocalTile("deco", m_centerX, m_centerY + 1, 82);     // derecha abajo
}