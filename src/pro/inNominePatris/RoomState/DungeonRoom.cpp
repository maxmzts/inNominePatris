#include "DungeonRoom.h"
#include "EnemyManager.h"

DungeonRoom::DungeonRoom(const std::string& id, std::vector<std::shared_ptr<Enemy>> enemies_list) 
    : RoomState(), roomId(id) {
    // Asignar los enemigos recibidos a la variable heredada
    this->enemies = std::move(enemies_list);
}

DungeonRoom::~DungeonRoom() {
}

void DungeonRoom::enter() {
    if (!completed) {
        // Spawnear enemigos solo si la sala no ha sido completada
        EnemyManager::getInstance()->addEnemies(enemies);
    }
    std::cout << "Entré en la sala " << roomId << std::endl;
}

void DungeonRoom::update() {   
    // Si no quedan enemigos, la sala está completada
    int count = EnemyManager::getInstance()->getEnemyCount();
    if (!hasEnemies() && !completed) {
        completed = true;
        std::cout << "Maté a todos los enemigos, puedo salir" << std::endl;
        // recompensas al completar la sala, etc.
    }
}

void DungeonRoom::exit() {
    // Lógica al salir de la sala
    // Por ejemplo, guardar el estado de la sala, etc.
}