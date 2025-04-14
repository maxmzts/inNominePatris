#pragma once

#include "Interaction.h"
#include "TileMap.h"

class Character;

class SpawnPlayerInteraction : public Interaction {
private:
    float m_destX;     // Coordenada X de destino (en unidades de tile)
    float m_destY;     // Coordenada Y de destino (en unidades de tile)
    bool m_autoTrigger; // Si se activa automáticamente al pasar por encima (true) o requiere interacción (false)

public:
    /**
     * @brief Constructor de SpawnPlayerInteraction
     * @param id ID del tile de interacción
     * @param name Nombre descriptivo
     * @param destX Coordenada X de destino (en unidades de tile)
     * @param destY Coordenada Y de destino (en unidades de tile)
     * @param autoTrigger Si se activa automáticamente al pasar por encima (true) o requiere presionar E (false)
     */
    SpawnPlayerInteraction(int id, const std::string& name, float destX, float destY, bool autoTrigger = true);
    
    void execute(Character& character, TileMap& tilemap) override;
    bool isAvailable(Character& character, TileMap& tilemap) const override;
    
    // Getters
    float getDestX() const { return m_destX; }
    float getDestY() const { return m_destY; }
    bool getAutoTrigger() const { return m_autoTrigger; }
};