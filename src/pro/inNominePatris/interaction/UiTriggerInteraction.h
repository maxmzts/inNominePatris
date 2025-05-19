#ifndef UI_TRIGGER_INTERACTION_H
#define UI_TRIGGER_INTERACTION_H

#include "Interaction.h"
#include "Character.h"
#include "TileMap.h"
#include <string>

// Forward declaration para evitar dependencias circulares
class InGame;
class GameEngine;

class UiTriggerInteraction : public Interaction {
private:
    std::string m_uiName; // Nombre descriptivo de la interfaz
    bool m_isActivated; // para saber si el botón ya fue activado

public:
    UiTriggerInteraction(int id, const std::string& name, const std::string& uiName);
    
    virtual void execute(Character& character, TileMap& tilemap) override;
    virtual bool isAvailable(Character& character, TileMap& tilemap) const override;
    
    // Método para activar/desactivar la UI en InGame con InGame pasado por parámetro
    void setUiTriggerState(InGame& inGame, bool state);
    
    // Método para obtener el mensaje de proximidad
    std::string getProximityMessage() const;
    
    // Método para obtener el nombre de la UI
    std::string getUiName() const;
};

#endif // UI_TRIGGER_INTERACTION_H