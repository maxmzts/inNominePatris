#include "UiTriggerInteraction.h"
#include "InGame.h"
#include <iostream>

UiTriggerInteraction::UiTriggerInteraction(int id, const std::string& name, const std::string& uiName)
    : Interaction(id, name), m_uiName(uiName) {
}

void UiTriggerInteraction::execute(Character& character, TileMap& tilemap) {
    // Esta función se llama desde checkAutoInteractions en InGame.cpp
    // Ya no necesitamos obtener la instancia de InGame aquí
    // InGame será pasado por parámetro en setUiTriggerState desde checkAutoInteractions
    //std::cout << "UiTrigger activado: " << m_uiName << std::endl;
    // La lógica real de cambio de estado se hace en setUiTriggerState
}

bool UiTriggerInteraction::isAvailable(Character& character, TileMap& tilemap) const {
    return true; // Siempre disponible si el jugador está en el tile
}

void UiTriggerInteraction::setUiTriggerState(InGame& inGame, bool state) {
    // Accedemos al flag en InGame y lo actualizamos
    if (m_uiName == "Tienda") {
        inGame.setPlayerInShopArea(state);
    }
    // Puedes añadir más condiciones para diferentes tipos de UIs
    std::cout << "Estado de UI '" << m_uiName << "' actualizado a: " << (state ? "activado" : "desactivado") << std::endl;
}

std::string UiTriggerInteraction::getProximityMessage() const {
    return "Presiona B para abrir la " + m_uiName;
}

std::string UiTriggerInteraction::getUiName() const {
    return m_uiName;
}