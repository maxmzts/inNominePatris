#include "DungeonRoom.h"
#include "EnemyManager.h"
#include "MusicManager.h"
#include "Character.h"
#include "Item.h"

DungeonRoom::DungeonRoom(const std::string& id, std::vector<std::shared_ptr<Enemy>> enemies_list, std::function<void(TileMap&)> openDoors) 
    : RoomState(), roomId(id), openDoors(openDoors) {
    // Asignar los enemigos recibidos a la variable heredada
    this->enemies = std::move(enemies_list);
}

DungeonRoom::~DungeonRoom() {
}

void DungeonRoom::enter() {
    if (!completed) {
        // Spawnear enemigos solo si la sala no ha sido completada
        if(enemies.size() != 0){
            EnemyManager::getInstance()->addEnemies(enemies);
            MusicManager::getInstance().transitionTo("./resources/music/hunt-the-hunter.ogg", 80.f);
        }
    }
    std::cout << "Entré en la sala " << roomId << std::endl;
}

void DungeonRoom::setItemPositions(const std::vector<sf::Vector2f>& positions) {
    itemPositions = positions;
}

void DungeonRoom::generateItems(Character* player) {
    auto weapons = player->getEquippedWeapons();
    std::cout << "Armas equipadas: " << weapons.size() << std::endl;
    std::cout << "Posiciones de ítems: " << itemPositions.size() << std::endl;

    for (size_t i = 0; i < std::min(itemPositions.size(), weapons.size()); ++i) {
        ItemType weaponType = weapons[i]->getItemType();
        Item* newItem = Item::generateRandomItemforWeapon(weaponType);
        std::cout << "Generando ítem para el arma: " << static_cast<int>(weaponType) << std::endl;
        if (newItem) {
            newItem->setPosition(itemPositions[i].x, itemPositions[i].y);
            items.emplace_back(newItem);
            std::cout << "Generé un ítem en la posición: " << itemPositions[i].x << ", " << itemPositions[i].y << std::endl;
        }
    }
}

void DungeonRoom::update(TileMap& tileMap) {   
    // Si no quedan enemigos, la sala está completada
    if (!hasEnemies() && !completed) {
        completed = true;
        std::cout << "Maté a todos los enemigos, puedo salir" << std::endl;

        // Generar ítems en las posiciones especificadas
        Character* player = Character::getInstance();
        if (player == nullptr) {
            std::cerr << "Error: No se pudo obtener la instancia del jugador." << std::endl;
            return;
        }
        generateItems(player);

        MusicManager::getInstance().transitionTo("./resources/music/who_killed_them.ogg", 80.f);
    }

    // Verificar si el jugador pasa por encima de algún ítem
    Character* player = Character::getInstance();
    sf::FloatRect playerBounds = player->getBounds();

    for (auto& item : items) {
        if (!item->GetIsPickedUp() && item->getBounds().intersects(playerBounds)) {
            
            // Marcar todos los ítems como recogidos
            for (auto& otherItem : items) {
                otherItem->Picked();
                openDoors(tileMap);
            }

            auto weapons = player->getEquippedWeapons();
            for (auto* weapon : weapons) {
                if (weapon->getItemType() == item->getType()) { // Comparar el tipo del arma con el tipo del ítem
                    item->applyEffect(*weapon); // Aplicar el efecto al arma correspondiente
                    std::cout << "Ítem aplicado al arma de tipo: " << static_cast<int>(weapon->getItemType()) << std::endl;
                    break;
                }
            }
            
            std::cout << "Ítem recogido en posición: " << item->getBounds().left << ", " << item->getBounds().top << std::endl;
        }
    }
}

void DungeonRoom::exit() {
    // Lógica al salir de la sala
    // Por ejemplo, guardar el estado de la sala, etc.
}