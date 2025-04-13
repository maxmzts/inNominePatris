#include "InteractionFactory.h"
#include "ButtonInteraction.h"
#include "DoorInteraction.h"
#include "UiTriggerInteraction.h"


std::shared_ptr<Interaction> InteractionFactory::createInteraction(int tileId) {
    switch (tileId) {
        case 772:
            return std::make_shared<ButtonInteraction>(tileId, "Botón Abajo", 31, 29);
        case 773:
            return std::make_shared<ButtonInteraction>(tileId, "Botón Izquierda", 17, 20);
        case 774:
            return std::make_shared<ButtonInteraction>(tileId, "Botón Derecha", 44, 20);
        case 69: // Si tienes una puerta con este ID
            return std::make_shared<DoorInteraction>(tileId, "Puerta Principal", false, 27, 7);
        case 873: // Tile para la tienda
            return std::make_shared<UiTriggerInteraction>(tileId, "Tienda", "Tienda");
        default:
            return nullptr;
    }
}