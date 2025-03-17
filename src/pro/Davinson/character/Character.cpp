#include "Character.h"
#include <iostream>

Character::Character(const std::string& textureFile) : speed(1.75f), acceleration(0.1f), deceleration(0.15f) {
    if (!texture.loadFromFile(textureFile)) {
        std::cerr << "Error cargando la textura" << std::endl;
        exit(1);
    }
    sprite.setTexture(texture);
    sprite.setOrigin(75 / 2, 75 / 2);
    sprite.setTextureRect(sf::IntRect(0, 0, 75, 75));
    sprite.setPosition(320, 240);
    sprite.setScale(0.6f, 0.6f);

    velocity = sf::Vector2f(0.f, 0.f);
}

void Character::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::D: movingRight = true; break;
            case sf::Keyboard::A: movingLeft = true; break;
            case sf::Keyboard::W: movingUp = true; break;
            case sf::Keyboard::S: movingDown = true; break;
        }
    } else if (event.type == sf::Event::KeyReleased) {
        switch (event.key.code) {
            case sf::Keyboard::D: movingRight = false; break;
            case sf::Keyboard::A: movingLeft = false; break;
            case sf::Keyboard::W: movingUp = false; break;
            case sf::Keyboard::S: movingDown = false; break;
        }
    }
}

void Character::update() {
    sf::Vector2f moveDirection(0.f, 0.f);

    // Detectar entrada del usuario
    if (movingRight) moveDirection.x += 1;
    if (movingLeft) moveDirection.x -= 1;
    if (movingUp) moveDirection.y -= 1;
    if (movingDown) moveDirection.y += 1;

    // Normalizar el vector de movimiento en caso de diagonal
    if (moveDirection.x != 0 && moveDirection.y != 0) {
        moveDirection *= 0.7071f; // sqrt(2)/2 para mantener velocidad uniforme
    }

    // Aplicar aceleración si hay movimiento
    if (moveDirection.x != 0) {
        velocity.x += moveDirection.x * acceleration;
        if (velocity.x > speed) velocity.x = speed;
        if (velocity.x < -speed) velocity.x = -speed;
    } else {
        // Aplicar desaceleración si no hay entrada en X
        if (velocity.x > 0) {
            velocity.x -= deceleration;
            if (velocity.x < 0) velocity.x = 0;
        }
        if (velocity.x < 0) {
            velocity.x += deceleration;
            if (velocity.x > 0) velocity.x = 0;
        }
    }

    if (moveDirection.y != 0) {
        velocity.y += moveDirection.y * acceleration;
        if (velocity.y > speed) velocity.y = speed;
        if (velocity.y < -speed) velocity.y = -speed;
    } else {
        // Aplicar desaceleración si no hay entrada en Y
        if (velocity.y > 0) {
            velocity.y -= deceleration;
            if (velocity.y < 0) velocity.y = 0;
        }
        if (velocity.y < 0) {
            velocity.y += deceleration;
            if (velocity.y > 0) velocity.y = 0;
        }
    }

    // Mover el sprite según la velocidad calculada
    sprite.move(velocity);




    // // CODIGO DE COLISION DEL PERSONAJE CON LOS LIMITES DEL MAPA
    // // Obtener posición futura del personaje
    // sf::Vector2f newPosition = sprite.getPosition() + velocity;

    // // Obtener dimensiones del sprite
    // sf::FloatRect spriteBounds = sprite.getGlobalBounds();

    // // Limites de la ventana (asumiendo una resolución fija de 640x480)
    // float leftLimit = spriteBounds.width / 2;
    // float rightLimit = 640 - spriteBounds.width / 2;
    // float topLimit = spriteBounds.height / 2;
    // float bottomLimit = 480 - spriteBounds.height / 2;

    // // Restringir movimiento dentro de la pantalla
    // if (newPosition.x < leftLimit) {
    //     newPosition.x = leftLimit;
    //     velocity.x = 0; // Detener la velocidad en ese eje
    // }
    // if (newPosition.x > rightLimit) {
    //     newPosition.x = rightLimit;
    //     velocity.x = 0;
    // }
    // if (newPosition.y < topLimit) {
    //     newPosition.y = topLimit;
    //     velocity.y = 0;
    // }
    // if (newPosition.y > bottomLimit) {
    //     newPosition.y = bottomLimit;
    //     velocity.y = 0;
    // }

    // // Establecer la posición restringida
    // sprite.setPosition(newPosition);
















    // Ajustar la textura según la dirección del movimiento
    if (movingRight) {
        sprite.setTextureRect(sf::IntRect(0, 2 * 75, 75, 75));
        sprite.setScale(0.6f, 0.6f);
    }
    if (movingLeft) {
        sprite.setTextureRect(sf::IntRect(0, 2 * 75, 75, 75));
        sprite.setScale(-0.6f, 0.6f);
    }
    if (movingUp) sprite.setTextureRect(sf::IntRect(0, 3 * 75, 75, 75));
    if (movingDown) sprite.setTextureRect(sf::IntRect(0, 0 * 75, 75, 75));
}

void Character::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
