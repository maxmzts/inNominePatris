#include "InGame.h"
#include "../Game.h"
#include "Character.h"
#include "HUD.h"
#include "Hitbox.h"
#include "Shop.h"
#include "Interaction.h"
#include "MainMenu.h"
#include "KarmaSystem.h"
#include "PauseMenu.h"
#include "KoScreen.h"
#include "InteractionFactory.h"
#include "ItemManager.h"
#include "SpawnPlayerInteraction.h"
#include "WorldChangeInteraction.h"
#include "UiTriggerInteraction.h"
#include "RoomManager.h"
#include <iostream>
#include <algorithm>
#include "EnemyBat.h"
#include "LobbyState.h"
#include "World1State.h"
#include "World2State.h"
#include "World3State.h"
#include <EnemyManager.h>
#include <EnemyNecromancer.h>
#include <MusicManager.h>
#include <SaveSystem.h>

InGame* InGame::instance = nullptr;

/** 
 * Constructor de InGame. Carga el motor y el lobby con el jugador.
 */
InGame::InGame(GameEngine& engine, bool loadSaveFile)
    : engine(engine), player(), karmaSystem(player), shop(engine.getWindow(), karmaSystem) {
    // Cargar el mapa
    if (!tileMap.loadFromFile("./maps/lobby.tmx", engine)) {
        std::cerr << "Error cargando el mapa\n";
        exit(-1);
    }

    Character::setInstance(&player); // Establecer la instancia del jugador

    // Cargar la fuente
    if (!font.loadFromFile("./assets/fonts/IMPACT.TTF")) {
        std::cerr << "Error al cargar la fuente para los mensajes de proximidad\n";
    }

    // Hacer spawn al jugador
    player.spawnAt(tileMap, 20, 44);
    MusicManager::getInstance().transitionTo("resources/music/sanity.ogg");

    // Inicializar los subestados del mundo
    worldStates["lobby"] = std::make_unique<LobbyState>();
    worldStates["world_1"] = std::make_unique<World1State>();
    worldStates["world_2"] = std::make_unique<World2State>();
    worldStates["world_3"] = std::make_unique<World3State>();

    currentWorldState = worldStates["lobby"].get(); // Comienza en el lobby
    spawnWeaponsInLobby();

    // SETUP SAVESYSTEM
    SaveSystem::getInstance().setSaveFilePath("./savefile.txt");

    if (!loadSaveFile || !SaveSystem::getInstance().loadGameState(karmaSystem)) {
        // Si no hay partida guardada, inicializar con valores por defecto
        Character::getInstance()->addKarma(0);
        SaveSystem::getInstance().saveGameState(karmaSystem);
        // No hay mejoras iniciales
        //std::cout << "No se encontró partida guardada. Usando valores por defecto." << std::endl;
    } else {
        // std::cout << "Partida cargada correctamente." << std::endl;    
    }

    m_isPlayerInAnyUiTriggerArea = false;
    m_lastUiTriggerTileId = -1;
}

InGame* InGame::getInstance(GameEngine& engine, bool loadSaveFile) {
    if (!instance) {
        instance = new InGame(engine, loadSaveFile);
    } 
    // else {
    //     // Reinicia el estado del juego si ya existe
    //     instance->reset(engine);
    // }
    return instance;
}

/**
 * Update de InGame
 * Captura eventos del jugador (armas, cambiar de arma, etc).
 * Se ejecuta cada frame.
 */
void InGame::update(Game& game) {
    sf::RenderWindow& window = game.getWindow();
    sf::Event event;

    currentWorldState->update(tileMap);

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } 

        if(shop.isOpen()){
            shop.handleInput(event);
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::B){
                shop.close();
            }
            continue; // No procesar otros eventos
        }
        
        player.handleInput(event);

        // CHEATCODE PARA LA PRESENTACION
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num0) {
            const std::vector<std::shared_ptr<Enemy>> enemies = EnemyManager::getInstance()->getEnemyList();
            for( std::shared_ptr<Enemy> enemy : enemies ){
                enemy->takeDamage(50000.f, {0.f,0.f});
            }
        }
        // CHEATCODE PARA LA PRESENTACION
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num2) {
            LobbyState::setWorld1completed();
        }
        // CHEATCODE PARA LA PRESENTACION
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num3) {
            LobbyState::setWorld2completed();
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num7) {
            player.addKarma(2000);
        }

        // Abrir la tienda (tecla B)
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::B) {
            if (m_playerInShopArea) {  // Solo abre la tienda si el jugador está en una zona de tienda
                std::cout << "Abriendo tienda..." << std::endl;
                shop.open();
            }
        }

        if (shop.isOpen()) {
            shop.update(player.getKarma()); // Actualizar la tienda si está abierta
            return; // No actualizar el resto del juego
        }


        // Respawn del jugador (tecla R)
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
            float randomX = rand() % 500;
            float randomY = rand() % 500;
            player.spawnAt(tileMap, randomX, randomY);
        }

        // Menu pausa (tecla esc)
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            game.changeState(PauseMenu::getInstance(800, 600)); // Cambiar al menú de pausa
            return;
        }
        
        // Capturar posición del ratón y actualizar la dirección de apuntado
        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePosition);
            player.setMousePosition(worldMousePos);
        }


        // Interacción con armas (tecla E)
        
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) {
            sf::Vector2f playerPosition = player.getPosition();
            
            // Primero comprobar interacción con tiles
            player.interact(tileMap);
            
            // Verificar si el jugador está cerca de un arma en el suelo
            auto it = std::find_if(weaponsOnGround.begin(), weaponsOnGround.end(), [&](Weapon* weapon) {
                sf::Vector2f weaponPosition = weapon->getPosition();
                
                // Comprobar si el arma es accesible según el progreso
                bool canInteract = true;
                
                // Verificar que el jugador pueda interactuar con este tipo de arma
                if (dynamic_cast<Lance*>(weapon) && !LobbyState::isWorld2Completed()) {
                    canInteract = false; // La lanza no está disponible si no se ha completado el mundo 2
                } else if (dynamic_cast<Bow*>(weapon) && !LobbyState::isWorld1Completed()) {
                    canInteract = false; // El arco no está disponible si no se ha completado el mundo 1
                }
                
                return canInteract && 
                    std::abs(playerPosition.x - weaponPosition.x) < 50 &&
                    std::abs(playerPosition.y - weaponPosition.y) < 50;
            });

            if (it != weaponsOnGround.end()) {
                Weapon* weapon = *it;
                sf::Vector2f weaponPos = weapon->getPosition();
                
                if (player.getWeaponCount() < 2) {
                    // Player has 0 or 1 weapons, just add the new one
                    player.addWeaponWithPosition(weapon, weaponPos);
                    weaponsOnGround.erase(it); // Remove from ground
                    player.equipWeapon();
                    // std::cout << "Arma equipada!" << std::endl;
                } else {
                    // Player already has 2 weapons, replace the first one
                    sf::Vector2f oldWeaponPos;
                    Weapon* oldWeapon = player.removeFirstWeapon(oldWeaponPos);
                    
                    // Add the new weapon
                    player.addWeaponWithPosition(weapon, weaponPos);
                    weaponsOnGround.erase(it); // Remove from ground
                    
                    // Return old weapon to ground
                    oldWeapon->setPosition(oldWeaponPos.x, oldWeaponPos.y);
                    weaponsOnGround.push_back(oldWeapon);
                    
                    player.equipWeapon();
                    // std::cout << "Arma reemplazada y equipada!" << std::endl;
                }
            }
        }
        

        //  MOSTRAR MENSAJE SI EXISTE UNA INTERACCIÓN CON EL JUGADOR DISPONIBLE

        proximityMessage.clear(); // Limpiar el mensaje por defecto
        sf::FloatRect playerBounds = player.getBounds();
        int tileId = -1;

        if (tileMap.isPlayerInteractingWithTile(playerBounds, tileId)) {
            tileId -= 1;
            auto interaction = InteractionFactory::createInteraction(tileId);

            if (interaction && interaction->isAvailable(player, tileMap)) {
                // Verificar si la interacción es un ButtonInteraction
                if (auto buttonInteraction = std::dynamic_pointer_cast<ButtonInteraction>(interaction)) {
                    proximityMessage = buttonInteraction->getProximityMessage();
                }
                // Verificar si la interacción es un DoorInteraction
                else if (auto doorInteraction = std::dynamic_pointer_cast<DoorInteraction>(interaction)) {
                    proximityMessage = doorInteraction->getProximityMessage();
                }
            }
        }
        






        // Cambiar de arma (tecla Q)
        if (player.getEquippedWeapon() != nullptr) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q) {
                player.switchWeapon();
                player.equipWeapon();
            }
        }

        // Ataques con el ratón
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                //crear aqui la hitbox
                if (Weapon* equippedWeapon = player.getEquippedWeapon()) {
                    // Crear la hitbox del ataque
                    equippedWeapon->attack(player.getPosition(), player.getAimDirection());
                }
            } else if (event.mouseButton.button == sf::Mouse::Right) {
                // Usar habilidad especial
                if (Sword* sword = dynamic_cast<Sword*>(player.getEquippedWeapon())) {
                    bool Dash = sword->useAbility();
                    if(Dash)
                        player.startDash(sword->getDashSpeed(), 0.2f); // Usa los valores de la espada
                } else if (Lance* lance = dynamic_cast<Lance*>(player.getEquippedWeapon())) {
                    // Usar habilidad de la lanza
                    if (!lance->getIsPortalDropped()) {
                        // Colocar el portal
                        lance->useAbility(player.getPosition());
                    } else {
                        // Teletransportar al portal
                        sf::Vector2f playerPosition = player.getPosition();
                        playerPosition = lance->teleportToPortal();
                        player.setPosition(playerPosition.x, playerPosition.y);
                    }
                } else if (Bow* bow = dynamic_cast<Bow*>(player.getEquippedWeapon())) {
                    // Usar habilidad del arco
                    bow->useAbility(player.getPosition(), player.getAimDirection());
                }
                //player.useAbility(window, enemies);
            }
        }
    }

    // Actualizar el jugador y otros elementos
    float deltaTime = engine.getDeltaTime();
    player.update(tileMap, deltaTime);

    // Verificar si la vida del jugador es 0
    if (player.getHealth() <= 0) {
        // std::cout << "El jugador ha muerto. Pantalla KO..." << std::endl;
        game.changeState(KoScreen::getInstance());
        return; // Salir del método para evitar más actualizaciones
    }
    
    // Verificar interacciones automáticas (como los teletransportes)
    checkAutoInteractions();
    
    EnemyManager::getInstance()->updateEnemies(deltaTime, &player, &tileMap);

    if (Weapon* equippedWeapon = player.getEquippedWeapon()) {
        // Excluir el arco del procesamiento de la hitbox
        if (dynamic_cast<Bow*>(equippedWeapon) == nullptr) {
            // Actualizar la hitbox de ataque si el arma no es un arco
            if (equippedWeapon->getAttackHitbox()->isActive()) {
                // Solo infligir daño si es el primer frame en el que la hitbox está activa
                if (!equippedWeapon->hasDealtDamage()) {
                    for (auto enemy : EnemyManager::getInstance()->getEnemyList()) {
                        if (equippedWeapon->getAttackHitbox()->getGlobalBounds().intersects(enemy->getHurtbox()->getGlobalBounds())) {
                            //std::cout << "Enemigo golpeado!" << std::endl;
                            enemy->takeDamage(equippedWeapon->calculateDamage(), equippedWeapon->getAttackHitbox()->getPosition()); // Infligir daño al enemigo
                        }
                    }
                    // Marcar que el daño ya se ha aplicado
                    equippedWeapon->setDealtDamage(true);
                }
            }
        }
    }

    if (Bow* bow = dynamic_cast<Bow*>(player.getEquippedWeapon())) {
        for (Arrow& arrow : bow->getArrows()) {
            for (auto enemy : EnemyManager::getInstance()->getEnemyList()) {
                if (arrow.getBounds().intersects(enemy->getHurtbox()->getGlobalBounds())) {
                    // std::cout << "Enemy hit by arrow!" << std::endl;
                    enemy->takeDamage(bow->calculateDamage(), arrow.getPosition()); // Infligir daño al enemigo
                    arrow.markforRemoval(); // Marcar la flecha para eliminación
                    break; // Salir del bucle para evitar múltiples colisiones con la misma flecha
                }
            }
        }
    }

    // Actualizar la posición de la cámara para seguir al jugador
    engine.setViewCenter(player.getPosition());

    // Actualizar armas específicas si están equipadas
    Weapon* equippedWeapon = player.getEquippedWeapon();
    if (equippedWeapon != nullptr) {
        if (Bow* bow = dynamic_cast<Bow*>(equippedWeapon)) {
            bow->update(deltaTime, tileMap); // Actualizar el arco
        } else if (Lance* lance = dynamic_cast<Lance*>(equippedWeapon)) {
            lance->PortalUpdate(deltaTime); // Actualizar la lanza
            lance->update(deltaTime); // Actualizar la lanza
        } else if (Sword* sword = dynamic_cast<Sword*>(equippedWeapon)) {
            sword->update(deltaTime); // Actualizar la espada
        }
    }

    // Comprobar que el jugador recibe daño
    
    if(!player.getIsInvencible()){
        for(auto enemy : EnemyManager::getInstance()->getEnemyList()){
            if(enemy->getHitbox()->isActive() && checkPlayerWasHit(player, enemy))
                player.takeDamage(enemy->getAttackDamage());
        }
    }

    HUD::getInstance().update(player);
    MusicManager::getInstance().update(deltaTime);
    SFXManager::getInstance().update();
    VFXManager::getInstance().update(deltaTime);
}

// Método para verificar interacciones automáticas
void InGame::checkAutoInteractions() {
    sf::FloatRect playerBounds = player.getBounds();
    int tileId = -1;
    bool foundUiTrigger = false; // Flag para controlar si hemos encontrado algún UiTrigger activo
    
    if (tileMap.isPlayerInteractingWithTile(playerBounds, tileId)) {
        // Ajustar el ID del tile
        if(tileId != 1492){
            tileId -= 1;
        }
        
        // Crear la interacción correspondiente usando la fábrica
        auto interaction = InteractionFactory::createInteraction(tileId);
        
        if (interaction) {
            // Verificar si es una interacción de tipo SpawnPlayerInteraction con autoTrigger
            if (auto spawnInteraction = std::dynamic_pointer_cast<SpawnPlayerInteraction>(interaction)) {
                if (spawnInteraction->getAutoTrigger() && spawnInteraction->isAvailable(player, tileMap)) {
                    // Ejecutar automáticamente el teletransporte
                    spawnInteraction->execute(player, tileMap);
                    //cambiar de sala
                    RoomManager::getInstance()->changeState(spawnInteraction->getRoomInfo());
                }
            }
            // Verificar si es una interacción de tipo WorldChangeInteraction con autoTrigger
            else if (auto worldChangeInteraction = std::dynamic_pointer_cast<WorldChangeInteraction>(interaction)) {
                if (worldChangeInteraction->getAutoTrigger() && worldChangeInteraction->isAvailable(player, tileMap)) {
                    // Ejecutar automáticamente el cambio de mundo
                    std::string targetWorldState, mapFilePath, musicFilePath;
                    sf::Vector2i spawnPosition;

                    worldChangeInteraction->getWorldInfo(targetWorldState, mapFilePath, musicFilePath, spawnPosition);
                    changeWorldState(targetWorldState, mapFilePath, musicFilePath, spawnPosition);
                }
            }
            else if (auto uiTriggerInteraction = std::dynamic_pointer_cast<UiTriggerInteraction>(interaction)) {
                if (uiTriggerInteraction->isAvailable(player, tileMap)) {
                    // Marcar que hemos encontrado un UiTrigger
                    foundUiTrigger = true;
                    m_lastUiTriggerTileId = tileId;
                    
                    // Solo activamos si no estaba ya activado o si es un nuevo UiTrigger
                    if (!m_isPlayerInAnyUiTriggerArea) {
                        m_isPlayerInAnyUiTriggerArea = true;
                        
                        // Primero ejecutamos la lógica básica de la interacción
                        uiTriggerInteraction->execute(player, tileMap);
                        
                        // Luego actualizamos el estado de la UI pasando la instancia de InGame por parámetro
                        uiTriggerInteraction->setUiTriggerState(*this, true);
                    }
                }
            }
            // Aquí podrían ir otras tipos de interacciones automáticas
        }
    }
    
    // Si no encontramos ningún UiTrigger activo pero antes estábamos en uno, desactivarlo
    if (!foundUiTrigger && m_isPlayerInAnyUiTriggerArea) {
        // Desactivamos todos los UiTriggers
        m_isPlayerInAnyUiTriggerArea = false;
        
        // Crear la interacción anterior para desactivarla
        auto previousInteraction = InteractionFactory::createInteraction(m_lastUiTriggerTileId);
        if (auto uiTriggerInteraction = std::dynamic_pointer_cast<UiTriggerInteraction>(previousInteraction)) {
            // Actualizar el estado de la UI a false
            uiTriggerInteraction->setUiTriggerState(*this, false);
        }
        
        m_lastUiTriggerTileId = -1;
    }
}


InGame::~InGame() {
    for (Weapon* weapon : weaponsOnGround) {
        delete weapon;
    }
    weaponsOnGround.clear();
}


void InGame::render(Game& game, sf::RenderWindow& window) {
    GameEngine& engine = game.getEngine();

    engine.clear();
    tileMap.draw(engine);

    currentWorldState->render(engine);

    for (Weapon* weapon : weaponsOnGround) {
        weapon->render(engine.getRenderWindow()); // Usa el RenderWindow directamente
    }
    
    EnemyManager::getInstance()->renderEnemies(engine.getRenderWindow());


    // Obtener la sala actual desde el RoomManager
    auto currentRoom = RoomManager::getInstance()->getCurrentState();
    if (auto dungeonRoom = std::dynamic_pointer_cast<DungeonRoom>(currentRoom)) {
        const auto& items = dungeonRoom->getItems();
        for (const auto& item : items) {
            item->render(window);
        }
    }
    
    VFXManager::getInstance().render(engine.getRenderWindow());

    player.draw(engine);

    // Mostrar el HUD
    HUD::getInstance().draw(engine.getRenderWindow(), player);

    // Dibujar el mensaje de proximidad
    if (!proximityMessage.empty()) {
        sf::Text text;
        text.setFont(font); // Usa la fuente cargada
        text.setString(proximityMessage);
        text.setCharacterSize(18); // Tamaño de la letra
        text.setFillColor(sf::Color::White);

        // Calcular la posición del texto
        sf::Vector2f textPosition(player.getPosition().x - 50, player.getPosition().y - 70);
        text.setPosition(textPosition);

        // Crear un fondo rectangular detrás del texto
        sf::FloatRect textBounds = text.getGlobalBounds();
        sf::RectangleShape background(sf::Vector2f(textBounds.width + 10, textBounds.height + 10));
        background.setFillColor(sf::Color(50, 50, 50, 150)); // Gris translúcido
        background.setPosition(textBounds.left - 5, textBounds.top - 5);

        // Dibujar el fondo y luego el texto
        window.draw(background);
        window.draw(text);
    }

    // Renderizar la tienda si está abierta
    if (shop.isOpen()) {
        shop.render(engine.getRenderWindow()); // Usa el RenderWindow directamente
    }

    engine.display();
}


//  QUITAR
bool InGame::checkEnemyWasHit(std::shared_ptr<Enemy> enemy, Character player){
    if(player.getEquippedWeapon()->getAttackHitbox()->isActive())
        return enemy->getHurtbox()->getGlobalBounds().intersects(player.getEquippedWeapon()->getAttackHitbox()->getGlobalBounds());
}

bool InGame::checkPlayerWasHit(Character& player, std::shared_ptr<Enemy> enemy){
    if (enemy->getHitbox() == nullptr) return false;
    return player.getHurtbox()->getGlobalBounds().intersects(enemy->getHitbox()->getGlobalBounds());
}

void InGame::reset(GameEngine& engine) {
    // Reset the tilemap
    if (!tileMap.loadFromFile("./maps/lobby.tmx", engine)) {
        std::cerr << "Error cargando el mapa\n";
        exit(-1);
    }

    // Reset player
    player.spawnAt(tileMap, 20, 44); // Posición inicial del lobby
    player.setHealth(player.getMaxHealth());

    //Reset items
    ItemManager::getInstance()->clearRunItemEffects(player.getEquippedWeapons());
    
    // Clear enemies
    EnemyManager::getInstance()->clearEnemies();
    currentWorldState = worldStates["lobby"].get();
    // Repoblar armas en el lobby
    static_cast<LobbyState*>(currentWorldState)->spawnWeaponsOnGround(weaponsOnGround, &engine);
}


void InGame::changeWorldState(std::string& stateName, std::string& mapFilePath, std::string& musicFilePath, sf::Vector2i& spawnPosition) {
    // Eliminar todas las armas del suelo al cambiar de mundo
    for (Weapon* weapon : weaponsOnGround) {
        delete weapon;
    }
    weaponsOnGround.clear();

    if (worldStates.find(stateName) != worldStates.end()) {
        // 1. Cargar el nuevo mapa (la función loadFromFile ya incluye clear() ahora)
        if (!getTileMap().loadFromFile(mapFilePath, engine)) {
            std::cerr << "Error cargando el mapa: " << mapFilePath << std::endl;
            return;
        }

        // 2. Restablecer el mundo a su estado natural 
        // Reiniciar salas
        RoomManager::getInstance()->clearRooms();
        // Resetear estado de portales y botones
        SpawnPlayerInteraction::resetPortalStates();
        InteractionManager::getInstance()->resetButtons(); 
        // Reiniciar items y mejoras

        // Vaciar listas de enemigos y de efectos visuales permanentes
        VFXManager::getInstance().clear();
        EnemyManager::getInstance()->clearEnemies();
        
        // 3. Hacer spawn al jugador en la nueva posición
        getPlayer().spawnAt(getTileMap(), spawnPosition.x, spawnPosition.y);
        // DEBUG
        //std::cout << "Jugador reposicionado en: (" << spawnPosition.x << ", " << spawnPosition.y << ")" << std::endl;
        
        // 4. Cambiar el estado actual de mundo e inicializar
        currentWorldState = worldStates[stateName].get();
        currentWorldState->initialize();

        // Si entramos al lobby, crear las armas allí
        if (stateName == "lobby") {
            static_cast<LobbyState*>(currentWorldState)->spawnWeaponsOnGround(weaponsOnGround, &engine);
            ItemManager::getInstance()->clearRunItemEffects(getPlayer().getEquippedWeapons());
        }

        // 5. Guardar partida en cualquier caso
        SaveSystem::getInstance().saveGameState(karmaSystem);

        // DEBUG
        // std::cout << "Cambiando al estado del mundo: " << stateName << std::endl;
    } else {
        std::cerr << "Error: Estado del mundo no encontrado: " << stateName << std::endl;
    }
}


void InGame::resetInstance() {
    // Borra la instancia actual y la establece en nullptr
    delete instance;
    instance = nullptr;
}

// Añadir aquí el nuevo método para el LobbyState
void InGame::spawnWeaponsInLobby() {
    if (currentWorldState == worldStates["lobby"].get()) {
        static_cast<LobbyState*>(currentWorldState)->spawnWeaponsOnGround(weaponsOnGround, &engine);
    }
}


void InGame::resetUiTriggerStates() {
    // Resetear todos los flags relacionados con UIs
    m_playerInShopArea = false;
    m_isPlayerInAnyUiTriggerArea = false;
    m_lastUiTriggerTileId = -1;
}