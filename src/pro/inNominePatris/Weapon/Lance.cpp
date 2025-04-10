#include "Lance.h"
#include <iostream>
#include "Character.h"
#include <SFML/Window/Mouse.hpp>
#include <cmath>

Lance::Lance(GameEngine* engine) : Weapon(engine), isPortalDropped(false), attackRange(100.0f), PortalRange(300.0f), portal() {
    spriteFacade.loadTexture("./resources/Weapons/lance.png"); // Cargar textura usando el Façade
}

void Lance::attack(sf::Vector2f position, sf::Vector2f direction){
    std::cout << "Lance attack!" << std::endl;
    

    // Define una hitbox dependiendo de la dirección
    sf::FloatRect hitbox;
    float attackWidth = attackRange * 0.5f;  // Ancho del ataque
    float attackHeight = attackRange * 1.5f; // Altura del ataque

    if (direction.x > 0) {  // Mirando a la derecha
        hitbox = sf::FloatRect(position.x + 40, position.y - attackHeight / 2, attackWidth, attackHeight);
    } else if (direction.x < 0) {  // Mirando a la izquierda
        hitbox = sf::FloatRect(position.x - attackWidth - 40, position.y - attackHeight / 2, attackWidth, attackHeight);
    } else if (direction.y < 0) {  // Mirando hacia arriba
        hitbox = sf::FloatRect(position.x - attackWidth / 2, position.y - attackHeight - 40, attackWidth, attackHeight);
    } else if (direction.y > 0) {  // Mirando hacia abajo
        hitbox = sf::FloatRect(position.x - attackWidth / 2, position.y + 40, attackWidth, attackHeight);
    } else { //hitbox estándar
        hitbox = sf::FloatRect(position.x, position.y, attackWidth, attackHeight);
    }

    // ESTO LO HACE INGAME
    // Verifica colisiones con enemigos
    // for (Enemy& enemy : enemies) {
    //     if (hitbox.intersects(enemy.getHurtbox()->getGlobalBounds())) {
    //         std::cout << "Enemigo golpeado!" << std::endl;
    //         enemy.takeDamage(10);
    //     }
    // }
}

// REIMPLEMENTAR
// void Lance::useAbility(Character& character,  sf::RenderWindow& window){
//     if (!isPortalDropped) {
//         sf::Vector2i mousePosition = sf::Mouse::getPosition(window); // Posición del ratón en pantalla
//         sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePosition);
        
//         // Calcular la distancia entre el personaje y la posición del ratón
//         float distance = std::sqrt(std::pow(worldMousePos.x - character.getPosition().x, 2) + 
//                                    std::pow(worldMousePos.y - character.getPosition().y, 2));
        
//         if (distance <= PortalRange) {
//             std::cout << "Dropping Portal at: " << worldMousePos.x << ", " << worldMousePos.y << std::endl;
//             portal.setPosition(worldMousePos);
//             portal.setVisible(true);
//             isPortalDropped = true;
//         } else {
//             std::cout << "Portal placement out of range!" << "Distance - PortalRange" << distance << "-" << PortalRange << std::endl;
//         }
//     } else {
//         std::cout << "Teleporting to Portal!" << std::endl;
//         character.setPosition(portal.getPosition().x, portal.getPosition().y);
//         portal.setVisible(false);
//         isPortalDropped = false;
//     }
// }

Portal::Portal() : position(0.f, 0.f), visible(false) {
    loadAnimationFrames();
    sprite.setScale(1.0f, 1.0f);

    if (animationFrames.empty()) {
        std::cerr << "Warning: Portal created without valid animation frames!" << std::endl;
    }
}

Portal::Portal(const sf::Vector2f& pos) : position(pos), visible(false) {
    loadAnimationFrames();
    sprite.setScale(1.0f, 1.0f);
    setPosition(pos);

    if (animationFrames.empty()) {
        std::cerr << "Warning: Portal created without valid animation frames!" << std::endl;
    }
}

void Portal::setPosition(const sf::Vector2f& pos) {
    position = pos;
    sprite.setPosition(position.x - sprite.getGlobalBounds().width / 2, 
    position.y - sprite.getGlobalBounds().height / 2);
}

const sf::Vector2f& Portal::getPosition() const {
    return position;
}

void Portal::draw(sf::RenderWindow& window) {
    if (visible && sprite.getTexture() != nullptr) {
        window.draw(sprite);
    }
}

void Lance::DrawPortal(sf::RenderWindow& window) {
    portal.draw(window);
}

void Lance::PortalUpdate(float deltaTime) {
    portal.update(deltaTime);
}

void Portal::setVisible(bool visible) {
    this->visible = visible;
}

bool Portal::isVisible() const {
    return visible;
}

void Portal::loadAnimationFrames() {
    for (int i = 1; i <= 7; i++) {
        sf::Texture texture;
        std::string filePath = "./resources/portal-animation/portal1_frame_" + std::to_string(i) + ".png";
        if (texture.loadFromFile(filePath)) {
            animationFrames.push_back(texture);
        } else {
            std::cerr << "Error loading " << filePath << std::endl;
        }
    }

    // Verificar si se cargaron frames correctamente
    if (!animationFrames.empty()) {
        sprite.setTexture(animationFrames[0]); // Usar el primer frame por defecto
    } else {
        std::cerr << "No animation frames loaded for portal!" << std::endl;
    }
}

void Portal::update(float deltaTime) {
    if (!visible || animationFrames.empty()) return;

    animationTimer += deltaTime;
    if (animationTimer >= frameDuration) {
        animationTimer = 0.0f;
        currentFrame = (currentFrame + 1) % animationFrames.size();
        sprite.setTexture(animationFrames[currentFrame]);
    }
}

void Lance::increaseAttackRange(float range) {
    attackRange += range;
    std::cout << "Attack range increased!" << std::endl;
}

void Lance::increasePortalRange(float range) {
    PortalRange += range;
    std::cout << "Portal range increased!" << std::endl;
}

/**
 * Ajusta la lanza en la posicion y con la direccion del jugador.
 * Luego llama a render para dibujar la lanza.
 */
void Lance::renderOnPlayer(sf::Vector2f position, sf::Vector2f direction) {
    // Ajustar la posición del arma en función de la dirección
    if (direction.x > 0) {  // Mirando a la derecha
        spriteFacade.setPosition(position.x + 20, position.y);
        spriteFacade.setRotation(0); // Sin rotación
    } else if (direction.x < 0) {  // Mirando a la izquierda
        spriteFacade.setPosition(position.x - 20, position.y);
        spriteFacade.setRotation(180); // Rotar 180 grados
    } else if (direction.y < 0) {  // Mirando hacia arriba
        spriteFacade.setPosition(position.x, position.y - 20);
        spriteFacade.setRotation(270); // Rotar 270 grados
    } else if (direction.y > 0) {  // Mirando hacia abajo
        spriteFacade.setPosition(position.x, position.y + 20);
        spriteFacade.setRotation(90); // Rotar 90 grados
    }

    render();
}

/**
 * Dibuja la lanza en la posicion y con la direccion que tiene su sprite por defecto.
 */
void Lance::render(){
    // Dibujar el sprite del arma
    spriteFacade.draw(engine->getWindow());
    if (isPortalDropped) {
        portal.draw(engine->getWindow());
    }
}