#include "VisualEffect.h"

VisualEffect::VisualEffect(
    const std::string& texturePath,
    sf::Vector2f position,
    sf::Vector2i frameSize,
    int frameCount,
    float fps,
    bool invert,
    bool loop
) : finished(false)
{
    // Cargar textura
    sprite.loadTexture(texturePath);
    sprite.setOrigin(frameSize.x / 2.0f, frameSize.y / 2.0f);
    sprite.setPosition(position.x, position.y);
    if(invert)
        sprite.setScale(-1.f , 1.f);
    
    // Inicializar el sistema de animación
    animator = std::make_unique<AnimatedSprite>(sprite);
    
    // Añadir la animación del efecto (asumiendo que los frames están en una fila)
    animator->addAnimation("effect", frameCount, sf::Vector2i(0, 0), frameSize, false);
    
    if(loop){
        animator->addAnimation("effect", frameCount, sf::Vector2i(0, 0), frameSize, false);
    } else {
        // Configurar el callback para cuando termine la animación
        animator->addAnimation("effect", frameCount, sf::Vector2i(0, 0), frameSize, true);
    }

    animator->setAnimationEndCallback([this]() {
        this->finished = true;
    });

    // Iniciar la animación del efecto
    animator->play("effect", fps, false);
}

void VisualEffect::update(float deltaTime) {
    if (!finished) {
        // Actualizar la animación
        animator->update(deltaTime);
    }
}

void VisualEffect::draw(sf::RenderWindow& window) const {
    if (!finished) {
        sprite.draw(window);
    }
}

bool VisualEffect::isFinished() const {
    return finished;
}

