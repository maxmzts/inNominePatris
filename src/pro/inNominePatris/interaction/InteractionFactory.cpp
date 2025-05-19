#include "InteractionFactory.h"
#include "ButtonInteraction.h"
#include "DoorInteraction.h"
#include "UiTriggerInteraction.h"
#include "SpawnPlayerInteraction.h"

std::shared_ptr<Interaction> InteractionFactory::createInteraction(int tileId) {
    switch (tileId) {
        // BUTTONS----------------------------------------------------------------------------------------------------
        // lvl 1 puzzle 1
        case 772:
            return std::make_shared<ButtonInteraction>(tileId, "Botón Abajo", 31, 29);
        case 773:
            return std::make_shared<ButtonInteraction>(tileId, "Botón Izquierda", 17, 20);
        case 774:
            return std::make_shared<ButtonInteraction>(tileId, "Botón Derecha", 44, 20);
        // PUERTAS-------------------------------------------------------------------------------------------------
        // lvl 1 puerta 1
        case 69: // Si tienes una puerta con este ID
            return std::make_shared<DoorInteraction>(tileId, "Puerta Principal", false, 27, 7);
        // INTERFACES-------------------------------------------------------------------------------------------------------
        case 873: // Tile para la tienda
            return std::make_shared<UiTriggerInteraction>(tileId, "Tienda", "Tienda");
        // SPAWN POINTS----------------------------------------------------------------------------------------------------------------
        case 68: // Nuevo tile para teletransporte
            return std::make_shared<SpawnPlayerInteraction>(tileId, "Teletransporte 1", 121, 45);        
        case 999: // Ejemplo de teletransporte que requiere interacción (presionar E)
            return std::make_shared<SpawnPlayerInteraction>(tileId, "Portal Manual", 90, 120, false);
        default:
            return nullptr;
    }
}