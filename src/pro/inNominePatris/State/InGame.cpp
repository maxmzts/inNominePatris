#include "InGame.h"
#include "../Game.h"
#include "../character/Character.h"
#include "../interface/HUD.h"
#include "../hboxes/Hitbox.h"
#include "../Shop/Shop.h"
#include "../interaction/Interaction.h"
#include "MainMenu.h"
#include "KarmaSystem.h"
#include "PauseMenu.h"
#include "KoScreen.h"
#include "InteractionFactory.h"
#include "SpawnPlayerInteraction.h"
#include <iostream>
#include <algorithm>
#include "EnemyBat.h"
#include <EnemyManager.h>
#include <EnemyNecromancer.h>
#include <MusicManager.h>

InGame* InGame::instance = nullptr;

/** 
 * Constructor de InGame. Carga el motor y el lobby con el jugador.
 */
InGame::InGame(GameEngine& engine)
    : engine(engine), player(), hud(800, 600), karmaSystem(player), shop(engine.getWindow(), karmaSystem) {
    // Cargar el mapa
    if (!tileMap.loadFromFile("./maps/world_1.tmx", engine)) {
        std::cerr << "Error cargando el mapa\n";
        exit(-1);
    }

    // Cargar la fuente
    if (!font.loadFromFile("./assets/fonts/IMPACT.TTF")) {
        std::cerr << "Error al cargar la fuente para los mensajes de proximidad\n";
    }

    // Hacer spawn al jugador
    player.spawnAt(tileMap, 30, 44);

    // Crear las armas y colocarlas en los pilares
    Sword* sword = new Sword(&engine);
    Lance* lance = new Lance(&engine);
    Bow* bow = new Bow(&engine);

    weaponsOnGround = { sword, lance, bow };

    // Posiciones de los pilares
    sword->setPosition(183, 530);
    lance->setPosition(234, 500);
    bow->setPosition(163, 578);

    //TESTS ENEMIGOS
    std::shared_ptr<EnemyBat> enemy = nullptr;
    //cargar enemigos
    for (size_t i = 0; i < 3; i++)
    {
        enemy = std::make_shared<EnemyBat>(sf::Vector2f(100.f*i,100.f*i));
        EnemyManager::getInstance()->addEnemy(enemy);
    }
    EnemyManager::getInstance()->addEnemy(std::make_shared<EnemyNecromancer>(sf::Vector2f(400.f,400.f)));
    MusicManager::getInstance().addTrack("resources/music/lobby_track.ogg");
}

InGame* InGame::getInstance(GameEngine& engine) {
    if (!instance) {
        instance = new InGame(engine);
    }
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

        // Abrir la tienda (tecla B)
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::B) {
            std::cout << "Abriendo tienda..." << std::endl;
            shop.open();
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
                return std::abs(playerPosition.x - weaponPosition.x) < 50 &&
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
                    std::cout << "Arma equipada!" << std::endl;
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
                    std::cout << "Arma reemplazada y equipada!" << std::endl;
                }
            }
        }
    
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
                    bow->useAbility(player.getPosition(), player.getDirection());
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
        std::cout << "El jugador ha muerto. Pantalla KO..." << std::endl;
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
                            std::cout << "Enemigo golpeado!" << std::endl;
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
                    std::cout << "Enemy hit by arrow!" << std::endl;
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

    // TEST SISTEMA DE COMBATE
    /**
     * for (hitbox enemigo: enemigos){
     *     player->hurtbox.intersect(hitbox)
     * }
     * for (hurtbox enemigo: enemigos){
     *     player->arma->hitbox.intersect(hurtbox)
     * }
     */

    // Comprobar que el jugador recibe daño
    
    if(!player.getIsInvencible()){
        for(auto enemy : EnemyManager::getInstance()->getEnemyList()){
            if(enemy->getHitbox()->isActive() && checkPlayerWasHit(player, enemy))
                player.takeDamage(enemy->getAttackDamage());
        }
    }

    hud.update(player);

    VFXManager::getInstance().update(deltaTime);
}

// Método para verificar interacciones automáticas
void InGame::checkAutoInteractions() {
    sf::FloatRect playerBounds = player.getBounds();
    int tileId = -1;
    
    if (tileMap.isPlayerInteractingWithTile(playerBounds, tileId)) {
        // Ajustar el ID del tile
        tileId -= 1;
        
        // Crear la interacción correspondiente usando la fábrica
        auto interaction = InteractionFactory::createInteraction(tileId);
        
        if (interaction) {
            // Verificar si es una interacción de tipo SpawnPlayerInteraction con autoTrigger
            if (auto spawnInteraction = std::dynamic_pointer_cast<SpawnPlayerInteraction>(interaction)) {
                if (spawnInteraction->getAutoTrigger() && spawnInteraction->isAvailable(player, tileMap)) {
                    // Ejecutar automáticamente el teletransporte
                    spawnInteraction->execute(player, tileMap);
                }
            }
            // Aquí podrían ir otras tipos de interacciones automáticas
        }
    }
}

InGame::~InGame() {
    for (Weapon* weapon : weaponsOnGround) {
        delete weapon;
    }
    weaponsOnGround.clear();
}

/**
 * Renderiza los elementos de InGame
 */
// void InGame::render(Game& game, sf::RenderWindow& window) {
//     engine.clear();
//     tileMap.draw(engine);

//     for (Weapon* weapon : weaponsOnGround) {
//         weapon->render();
//     }

//     for (auto enemy : EnemyManager::getInstance()->getEnemyList()){
//         enemy->render(window);
//     }

//     player.draw(engine);

//     VFXManager::getInstance().render(window);

//     // Mostrar el HUD
//     hud.draw(window, player);

//     // Renderizar la tienda si está abierta
//     if (shop.isOpen()) {
//         shop.render();
//     }

//     engine.display();
// }


void InGame::render(Game& game, sf::RenderWindow& window) {
    GameEngine& engine = game.getEngine();

    engine.clear();
    tileMap.draw(engine);

    for (Weapon* weapon : weaponsOnGround) {
        weapon->render(engine.getRenderWindow()); // Usa el RenderWindow directamente
    }
    
    for (auto enemy : EnemyManager::getInstance()->getEnemyList()) {
        enemy->render(engine.getRenderWindow()); // Usa el RenderWindow directamente
    }
    
    VFXManager::getInstance().render(engine.getRenderWindow());

    player.draw(engine);

    // Mostrar el HUD
    hud.draw(engine.getRenderWindow(), player);

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


