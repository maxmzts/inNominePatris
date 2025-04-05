#include "Lance.h"
#include <iostream>
#include "Character.h"
#include <SFML/Window/Mouse.hpp>
#include <cmath>

Lance::Lance(GameEngine* engine) : Weapon(engine), isPortalDropped(false), attackRange(100.0f), PortalRange(300.0f) {
    spriteFacade.loadTexture("resources/lance.png"); // Cargar textura usando el Façade
}

void Lance::attack(Character& character, std::vector<Enemy>& enemies){
    std::cout << "Lance attack!" << std::endl;

    // Obtiene la posición y dirección del personaje
    sf::Vector2f position = character.getPosition();
    sf::Vector2f direction = character.getDirection(); // Detecta si el personaje mira a la izquierda o derecha

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

    // Verifica colisiones con enemigos
    for (Enemy& enemy : enemies) {
        if (hitbox.intersects(enemy.getBounds())) {
            std::cout << "Enemigo golpeado!" << std::endl;
            enemy.takeDamage(10);
        }
    }
}

void Lance::useAbility(Character& character,  sf::RenderWindow& window){
    if (!isPortalDropped) {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window); // Posición del ratón en pantalla
        sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePosition);
        
        // Calcular la distancia entre el personaje y la posición del ratón
        float distance = std::sqrt(std::pow(worldMousePos.x - character.getPosition().x, 2) + 
                                   std::pow(worldMousePos.y - character.getPosition().y, 2));
        
        if (distance <= PortalRange) {
            std::cout << "Dropping Portal at: " << worldMousePos.x << ", " << worldMousePos.y << std::endl;
            portal = Portal(worldMousePos);
            portal.setVisible(true);
            isPortalDropped = true;
        } else {
            std::cout << "Portal placement out of range!" << "Distance - PortalRange" << distance << "-" << PortalRange << std::endl;
        }
    } else {
        std::cout << "Teleporting to Portal!" << std::endl;
        character.setPosition(portal.getPosition().x, portal.getPosition().y);
        portal.setVisible(false);
        isPortalDropped = false;
    }
}

Portal::Portal() : position(0.f, 0.f), visible(false) {
    loadAnimationFrames();
    sprite.setScale(1.0f, 1.0f);
}
Portal::Portal(const sf::Vector2f& pos) : position(pos) {
    loadAnimationFrames();
    sprite.setScale(1.0f, 1.0f);
    setPosition(pos);
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
    if (visible) {
        engine->drawSprite(sprite);
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
    for (int i = 1; i <= 7; i++){
        sf::Texture texture;
        if (texture.loadFromFile("./resources/portal-animation/portal1_frame_" + std::to_string(i) + ".png")) {
            animationFrames.push_back(texture);
        } else {
            std::cerr << "Error loading portal_frame" << i << ".png" << std::endl;
        }
    }
    if (!animationFrames.empty()) {
        sprite.setTexture(animationFrames[0]); // Usar el primer frame por defecto
    }
}

void Portal::update(float deltaTime) {
    if (!visible || animationFrames.empty()) return;

    animationTimer += deltaTime;
    if (animationTimer >= frameDuration){
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

void Lance::draw() {
    spriteFacade.draw(engine->getWindow()); // Dibujar usando el Façade
    if (isPortalDropped) {
        portal.draw(engine->getWindow());
    }
}