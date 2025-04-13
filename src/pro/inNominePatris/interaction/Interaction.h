#pragma once
#include "TileMap.h"
#include <string>

class Character; // Forward declaration

class Interaction {
public:
    Interaction(int id, const std::string& name);
    virtual ~Interaction() = default;
    
    int getId() const { return m_id; }
    std::string getName() const { return m_name; }
    
    // Método que se ejecuta al interactuar
    virtual void execute(Character& character, TileMap& tilemap) = 0;
    
    // Método opcional para comprobar si la interacción está disponible
    virtual bool isAvailable(Character& character, TileMap& tilemap) const { return true; }

protected:
    int m_id;
    std::string m_name;
};