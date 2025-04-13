// UiTriggerInteraction.h
#ifndef UI_TRIGGER_INTERACTION_H
#define UI_TRIGGER_INTERACTION_H

#include "Interaction.h"
#include "Character.h"
#include "TileMap.h"
#include <string>
#include <functional>

// Forward declaration para evitar dependencias circulares
class UserInterface;

class UiTriggerInteraction : public Interaction {
private:
    UserInterface* m_ui; // Interfaz que se mostrará al activar esta interacción
    std::string m_uiName; // Nombre descriptivo de la interfaz

public:
    UiTriggerInteraction(int id, const std::string& name, const std::string& uiName);
    
    virtual void execute(Character& character, TileMap& tilemap) override;
    virtual bool isAvailable(Character& character, TileMap& tilemap) const override;
};

#endif // UI_TRIGGER_INTERACTION_H