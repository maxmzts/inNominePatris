#include "InGame.h"
#include "../Game.h"
#include "../character/Character.h"
#include <iostream>
#include <algorithm> // Para std::remove_if

InGame* InGame::instance = nullptr;

/** 
 * Constructor de InGame. Carga el motor y el lobby con el jugador.
 */
InGame::InGame(GameEngine& engine)
    : engine(engine), player("./resources/sprites.png") {
    // Cargar el mapa
    if (!tileMap.loadFromFile("./maps/lobby.tmx", engine)) {
        std::cerr << "Error cargando el mapa\n";
        exit(-1);
    }
    sf::Texture enemy_tex;
    if (!enemy_tex.loadFromFile("resources/sprites.png")) {
        std::cerr << "Error cargando la imagen sprites.png";
        exit(-1);
    }

    // Hacer spawn al jugador
    player.spawnAt(tileMap, 500, 600);

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
    EnemyA* enemy = nullptr;
    //cargar enemigos
    for (size_t i = 0; i < 3; i++)
    {
        enemy = new EnemyA("Goblin", 180.f, 100.f, sf::Vector2f(100.f*i,100.f*i));
        enemy->setTexture(enemy_tex);
        enemies.push_back(enemy);
    }
    
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
        } else {
            player.handleInput(event);
        }

        // Respawn del jugador (tecla R)
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
            float randomX = rand() % 500;
            float randomY = rand() % 500;
            player.spawnAt(tileMap, randomX, randomY);
        }

        // Interacción con armas (tecla E)
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) {
            sf::Vector2f playerPosition = player.getPosition();

            auto it = std::find_if(weaponsOnGround.begin(), weaponsOnGround.end(), [&](Weapon* weapon) {
                sf::Vector2f weaponPosition = weapon->getPosition();
                return std::abs(playerPosition.x - weaponPosition.x) < 50 &&
                       std::abs(playerPosition.y - weaponPosition.y) < 50;
            });

            if (it != weaponsOnGround.end()) {
                Weapon* weapon = *it;
                sf::Vector2f weaponPos = weapon->getPosition();

                if (player.getWeaponCount() < 2) {
                    player.addWeaponWithPosition(weapon, weaponPos);
                    weaponsOnGround.erase(it);
                    player.equipWeapon();
                } else {
                    sf::Vector2f oldWeaponPos;
                    Weapon* oldWeapon = player.removeFirstWeapon(oldWeaponPos);
                    player.addWeaponWithPosition(weapon, weaponPos);
                    weaponsOnGround.erase(it);
                    oldWeapon->setPosition(oldWeaponPos.x, oldWeaponPos.y);
                    weaponsOnGround.push_back(oldWeapon);
                    player.equipWeapon();
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
                // Ataque normal
                ///////////////////////////////////////////////////////////player.attack(enemies);
            } else if (event.mouseButton.button == sf::Mouse::Right) {
                // Usar habilidad especial
                //////////////////////////////////////////////////////////player.useAbility(window, enemies);
            }
        }
    }

    // Actualizar el jugador y otros elementos
    float deltaTime = engine.getDeltaTime();
    player.update(tileMap, deltaTime);
    for(EnemyA* enemy : enemies){
        enemy->update(deltaTime,&player,&tileMap);
    }

    // Actualizar la posición de la cámara para seguir al jugador
    engine.setViewCenter(player.getPosition());

    // Actualizar armas específicas si están equipadas
    Weapon* equippedWeapon = player.getEquippedWeapon();
    if (equippedWeapon != nullptr) {
        if (Bow* bow = dynamic_cast<Bow*>(equippedWeapon)) {
            ////////////////////////////////////////////////////////bow->update(deltaTime, enemies); // Actualizar el arco
        } else if (Lance* lance = dynamic_cast<Lance*>(equippedWeapon)) {
            lance->PortalUpdate(deltaTime); // Actualizar la lanza
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
void InGame::render(Game& game, sf::RenderWindow& window) {
    engine.clear();
    tileMap.draw(engine);

    for (Weapon* weapon : weaponsOnGround) {
        weapon->draw(engine, nullptr);
    }

    for (EnemyA* enemy : enemies){
        enemy->render(engine.getWindow());
    }

    player.draw(engine);
    engine.display();
}
