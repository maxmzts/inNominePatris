#pragma once

#include "Interaction.h"
#include "TileMap.h"
#include <unordered_map>

class Character;

class SpawnPlayerInteraction : public Interaction {
private:
    float m_destX1;    // Primera coordenada X de destino (en unidades de tile)
    float m_destY1;    // Primera coordenada Y de destino (en unidades de tile)
    float m_destX2;    // Segunda coordenada X de destino (en unidades de tile)
    float m_destY2;    // Segunda coordenada Y de destino (en unidades de tile)
    bool m_autoTrigger; // Si se activa automáticamente al pasar por encima (true) o requiere interacción (false)
    std::string m_roomId;
    std::string m_dest1RoomId;
    std::string m_dest2RoomId;
    
    // Mapa estático para rastrear el estado de cada portal (si se ha usado o no)
    static std::unordered_map<int, bool> s_portalUsageState;

public:
    /**
     * @brief Constructor de SpawnPlayerInteraction
     * @param id ID del tile de interacción
     * @param name Nombre descriptivo
     * @param destX1 Primera coordenada X de destino (en unidades de tile)
     * @param destY1 Primera coordenada Y de destino (en unidades de tile)
     * @param destX2 Segunda coordenada X de destino (en unidades de tile)
     * @param destY2 Segunda coordenada Y de destino (en unidades de tile)
     * @param autoTrigger Si se activa automáticamente al pasar por encima (true) o requiere presionar E (false)
     */
    SpawnPlayerInteraction(int id, const std::string& name, 
                          float destX1, float destY1, 
                          float destX2, float destY2,
                          const std::string& dest1RoomId,
                          const std::string& dest2RoomId, 
                          bool autoTrigger = true);
    
    void execute(Character& character, TileMap& tilemap) override;
    // Devuelve el id de la sala a la que hay que ir
    std::string getRoomInfo();
    bool isAvailable(Character& character, TileMap& tilemap) const override;
    
    // Método estático para reiniciar el estado de todos los portales
    static void resetPortalStates();
    
    // Getters
    float getDestX1() const { return m_destX1; }
    float getDestY1() const { return m_destY1; }
    float getDestX2() const { return m_destX2; }
    float getDestY2() const { return m_destY2; }
    bool getAutoTrigger() const { return m_autoTrigger; }
};