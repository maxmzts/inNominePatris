#include "InteractionFactory.h"
#include "ButtonInteraction.h"
#include "DoorInteraction.h"
#include "UiTriggerInteraction.h"
#include "SpawnPlayerInteraction.h"
#include "WorldChangeInteraction.h"

// bool going = true;

std::shared_ptr<Interaction> InteractionFactory::createInteraction(int tileId) {
    switch (tileId) {
        // BUTTONS----------------------------------------------------------------------------------------------------
        // WRLD 1 puzzle 1
        case 772:
            return std::make_shared<ButtonInteraction>(tileId, "Botón Abajo", 31, 29);
        case 773:
            return std::make_shared<ButtonInteraction>(tileId, "Botón Izquierda", 17, 20);
        case 774:
            return std::make_shared<ButtonInteraction>(tileId, "Botón Derecha", 44, 20);
        // AVISOS PANTALLA (INPUT VISUALES HOVERING)-------------------------------------------------------------------------------------------------
        // lvl 1 puerta 1
        case 69: // Si tienes una puerta con este ID
            return std::make_shared<DoorInteraction>(tileId, "Puerta Principal", false, 27, 7);
        // INTERFACES-------------------------------------------------------------------------------------------------------
        case 873: // Tile para la tienda
            return std::make_shared<UiTriggerInteraction>(tileId, "Tienda", "Tienda");
        // SPAWN POINTS----------------------------------------------------------------------------------------------------------------
        // WRLD 1
        case 68: // Nuevo tile para teletransporte
            return std::make_shared<SpawnPlayerInteraction>(tileId, "Teletransporte 1-2", 119, 40, 29, 10, "room2", "room1");
        case 862: // Nuevo tile para teletransporte
            return std::make_shared<SpawnPlayerInteraction>(tileId, "Teletransporte 2-3", 193, 26, 142, 25, "room3", "room1");
        case 863:
            return std::make_shared<SpawnPlayerInteraction>(tileId, "Teletransporte 3-4", 214, 82, 213, 46, "room4", "room3");
        case 864:
            return std::make_shared<SpawnPlayerInteraction>(tileId, "Teletransporte 4-5", 191, 190, 223, 117, "room5", "room4");
        
        

         // CAMBIOS DE MUNDO
        case 936: // Cambio al World 1
            return std::make_shared<WorldChangeInteraction>(tileId, "Entrada al Mundo 1", "world_1", "./maps/world_1.tmx", "resources/music/hunt-the-hunter.ogg", 20, 44, true);
        
        default:
            return nullptr;
    }
}