// UiTriggerInteraction.cpp
#include "UiTriggerInteraction.h"
#include <iostream>

UiTriggerInteraction::UiTriggerInteraction(int id, const std::string& name, const std::string& uiName)
    : Interaction(id, name), m_uiName(uiName) {
}

void UiTriggerInteraction::execute(Character& character, TileMap& tilemap) {
  // std::cout << "Abriendo interfaz de " << m_uiName << std::endl;
    
    // Aquí se implementaría la lógica para mostrar la interfaz
    // Por ahora, solo imprimimos un mensaje para simular la acción
    // if (m_ui) {
    //     std::cout << "La interfaz " << m_uiName << " ha sido activada" << std::endl;
    //     // m_ui->show(); // Esto se implementaría cuando tengamos la clase UserInterface
    // } else {
    //     std::cout << "Error: La interfaz no está disponible" << std::endl;
    // }
}

bool UiTriggerInteraction::isAvailable(Character& character, TileMap& tilemap) const {
    // Podríamos añadir condiciones adicionales aquí, como requisitos de nivel
    // o ítems para acceder a ciertas interfaces
    return true;
}