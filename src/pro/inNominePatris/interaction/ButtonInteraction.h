#pragma once

#include "Interaction.h"
#include "TileMap.h"

class Character;

class ButtonInteraction : public Interaction {
private:
    int m_centerX;
    int m_centerY;
    bool m_isActivated; // para saber si el botón ya fue activado

public:
    ButtonInteraction(int id, const std::string& name, int centerX, int centerY);
    
    void execute(Character& character, TileMap& tilemap) override;
    bool isAvailable(Character& character, TileMap& tilemap) const override;
    
    // Getters
    bool isActivated() const { return m_isActivated; }
    int getCenterX() const { return m_centerX; }
    int getCenterY() const { return m_centerY; }
    std::string getProximityMessage() const;
    sf::FloatRect getBounds() const;
};