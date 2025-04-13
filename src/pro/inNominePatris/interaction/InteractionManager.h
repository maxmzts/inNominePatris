#pragma once
#include "Interaction.h"
#include "TileMap.h"
#include <unordered_map>
#include <memory>

class Character;

class InteractionManager {
public:
    InteractionManager();
    ~InteractionManager() = default;
    
    // Registrar una nueva interacción
    void registerInteraction(std::shared_ptr<Interaction> interaction);
    
    // Ejecutar una interacción por ID
    bool executeInteraction(int id, Character& character, TileMap& tilemap);
    
    // Comprobar interacciones disponibles en la posición del jugador
    bool checkInteraction(Character& character, TileMap& tilemap);
    
private:
    std::unordered_map<int, std::shared_ptr<Interaction>> m_interactions;
    
    // Variables para tracking del estado de los botones
    bool m_button1Pressed = false;
    bool m_button2Pressed = false;
    bool m_button3Pressed = false;
    
    // Comprobar si todos los botones han sido presionados
    bool areAllButtonsPressed() const;
};