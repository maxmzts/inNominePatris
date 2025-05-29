// DoorInteraction.h
#pragma once

#include "Interaction.h"
#include "TileMap.h"

class Character;

class DoorInteraction : public Interaction {
public:
    DoorInteraction(int id, const std::string& name, bool isOpen, int centerX, int centerY);
    virtual void execute(Character& character, TileMap& tilemap) override;
    virtual bool isAvailable(Character& character, TileMap& tilemap) const override;
    
    // Nuevo método para establecer el estado de la puerta
    void setIsOpen(bool isOpen) { m_isOpen = isOpen; }
    std::string getProximityMessage() const;
    sf::FloatRect getBounds() const;
    
private:
    int m_centerX;
    int m_centerY;
    bool m_isOpen;
};