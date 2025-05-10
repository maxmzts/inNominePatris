#pragma once

#include "Interaction.h"
#include "TileMap.h"
#include <string>

class Character;
class InGame;

class WorldChangeInteraction : public Interaction {
private:
    std::string m_targetWorldState;  // Nombre del estado al que se quiere cambiar
    std::string m_mapFilePath;       // Ruta del archivo del mapa a cargar
    bool m_autoTrigger;              // Si se activa automáticamente al pisar el tile

public:
    /**
     * @brief Constructor de WorldChangeInteraction
     * @param id ID del tile de interacción
     * @param name Nombre descriptivo de la interacción
     * @param targetWorldState Nombre del estado del mundo al que cambiar
     * @param mapFilePath Ruta del archivo del mapa a cargar
     * @param autoTrigger Si se activa automáticamente al pasar por encima (true) o requiere presionar E (false)
     */
    WorldChangeInteraction(int id, const std::string& name, 
                          const std::string& targetWorldState,
                          const std::string& mapFilePath,
                          bool autoTrigger = true);
    
    void execute(Character& character, TileMap& tilemap) override;
    bool isAvailable(Character& character, TileMap& tilemap) const override;
    
    // Getters
    std::string getTargetWorldState() const { return m_targetWorldState; }
    std::string getMapFilePath() const { return m_mapFilePath; }
    bool getAutoTrigger() const { return m_autoTrigger; }
    
    // Método para cambiar al estado del mundo especificado
    void changeWorldState(InGame& inGame, GameEngine& engine);
};