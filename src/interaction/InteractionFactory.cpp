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
        // WRLD 2 puzzle 1
        case 1233:
            return std::make_shared<ButtonInteraction>(tileId, "Botón Arriba", 151, 8);
        case 1234:
            return std::make_shared<ButtonInteraction>(tileId, "Botón Derecha", 190, 88);
        case 1235:
            return std::make_shared<ButtonInteraction>(tileId, "Botón Abajo", 99, 205);
        // AVISOS PANTALLA (INPUT VISUALES HOVERING)-------------------------------------------------------------------------------------------------
        // WRLD 1 puerta 1
        case 69: // Si tienes una puerta con este ID
            return std::make_shared<DoorInteraction>(tileId, "Puerta Principal", false, 27, 7);
        // WRLD 2 puerta 1
        case 1242: // Si tienes una puerta con este ID
            return std::make_shared<DoorInteraction>(tileId, "Puerta Principal", false, 41, 65);
        // INTERFACES-------------------------------------------------------------------------------------------------------
        case 872: // tienda
            return std::make_shared<UiTriggerInteraction>(tileId, "Tienda", "Tienda");
        // SPAWN POINTS----------------------------------------------------------------------------------------------------------------
        // WRLD 1
        case 68: // Nuevo tile para teletransporte
            return std::make_shared<SpawnPlayerInteraction>(tileId, "Teletransporte 1-2", 119, 40, 29, 10, "room2", "room1");
        case 862: // Nuevo tile para teletransporte
            return std::make_shared<SpawnPlayerInteraction>(tileId, "Teletransporte 2-3", 193, 26, 142, 25, "room3", "room2");
        case 863:
            return std::make_shared<SpawnPlayerInteraction>(tileId, "Teletransporte 3-4", 214, 82, 213, 46, "room4", "room3");
        case 864:
            return std::make_shared<SpawnPlayerInteraction>(tileId, "Teletransporte 4-5", 191, 190, 223, 117, "room5", "room4");
         // WRLD 2
        case 1105: // Nuevo tile para teletransporte
            return std::make_shared<SpawnPlayerInteraction>(tileId, "Teletransporte 1-2", 128, 31, 89, 76, "room2", "room1");
        case 1106: // Nuevo tile para teletransporte
            return std::make_shared<SpawnPlayerInteraction>(tileId, "Teletransporte 1-3", 154, 87, 105, 91, "room3", "room1");
        case 1107:
            return std::make_shared<SpawnPlayerInteraction>(tileId, "Teletransporte 1-4", 97, 163, 89, 104, "room4", "room1");
        case 1108:
            return std::make_shared<SpawnPlayerInteraction>(tileId, "Teletransporte 1-5", 176, 268, 223, 117, "room5", "room1");
        // WRLD 3
        case 1241:
            return std::make_shared<SpawnPlayerInteraction>(tileId, "Teletransporte 1-2", 45, 65, 45, 116, "room2", "room1");
        
        

         // CAMBIOS DE MUNDO
        case 936: // Cambio al World 1
            return std::make_shared<WorldChangeInteraction>(tileId, "Entrada al Mundo 1", "world_1", "./src/maps/world_1.tmx", "./src/resources/music/who-killed-them.ogg", 20, 44, true);
        case 937: // Cambio al World 2
            return std::make_shared<WorldChangeInteraction>(tileId, "Entrada al Mundo 2", "world_2", "./src/maps/world_2.tmx", "./src/resources/music/dungeon-crawler.ogg", 41, 114, true);
        case 1001: // Cambio al World 3
            return std::make_shared<WorldChangeInteraction>(tileId, "Entrada al Mundo 3", "world_3", "./src/maps/world_3.tmx", "./src/resources/music/dungeon-crawler.ogg", 45, 116, true);
        case 1492: // Cambio al lobby
            return std::make_shared<WorldChangeInteraction>(tileId, "Entrada al lobby", "lobby", "./src/maps/lobby.tmx", "./src/resources/music/lobby_track.ogg", 20, 44, true);
        
        default:
            return nullptr;
    }
}