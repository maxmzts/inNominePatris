#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(SpriteFacade& spriteFacade)
    : spriteFacade(spriteFacade),
    currentAnimation(nullptr),
    currentFrame(0),
    currentTime(0.0f),
    isPlaying(false),
    isPaused(false),
    speedFactor(1.0f)
{
}

void AnimatedSprite::addAnimation(
    const std::string& name,
    int frameCount,
    sf::Vector2i startPosition,
    sf::Vector2i frameSize,
    bool loop)
{
    Animation animation;
    animation.frameCount = frameCount;
    animation.startPosition = startPosition;
    animation.frameSize = frameSize;
    animation.frameDuration = 1.0f; // Se ajustará con el FPS en play()
    animation.isLooping = loop;
    
    animations[name] = animation;
}

void AnimatedSprite::play(const std::string& name, float fps, bool loop)
{
    // Verificar si la animación existe
    auto it = animations.find(name);
    if (it == animations.end()) {
        std::cerr << "Animación no encontrada: " << name << std::endl;
        return;
    }
    
    // Si es la misma animación que ya está en reproducción, solo actualizar parámetros
    if (isPlaying && currentAnimationName == name) {
        it->second.frameDuration = 1.0f / fps;
        it->second.isLooping = loop;
        return;
    }
    
    // Configurar la nueva animación
    currentAnimationName = name;
    currentAnimation = &it->second;
    currentAnimation->frameDuration = 1.0f / fps; // Convertir FPS a duración de frame
    currentAnimation->isLooping = loop;
    
    // Reiniciar estados
    currentFrame = 0;
    currentTime = 0.0f;
    isPlaying = true;
    isPaused = false;
    
    // Aplicar el primer frame
    sf::IntRect rect(
        currentAnimation->startPosition.x,
        currentAnimation->startPosition.y,
        currentAnimation->frameSize.x,
        currentAnimation->frameSize.y
    );
    spriteFacade.setTextureRect(rect);
}

void AnimatedSprite::pause()
{
    isPaused = true;
}

void AnimatedSprite::resume()
{
    isPaused = false;
}

void AnimatedSprite::stop()
{
    isPlaying = false;
    currentAnimation = nullptr;
    currentAnimationName = "";
}

void AnimatedSprite::setAnimationEndCallback(AnimationCallback callback)
{
    onAnimationEnd = callback;
}

void AnimatedSprite::update(float deltaTime)
{
    if (!currentAnimation || !isPlaying || isPaused) {
        return;
    }

    sf::Vector2f scale = spriteFacade.getScale();
    
    // Actualizar el tiempo acumulado
    currentTime += deltaTime;
    
    // Verificar si es hora de cambiar al siguiente frame
    if (currentTime >= currentAnimation->frameDuration) {
        // Reiniciar el contador de tiempo
        currentTime = 0.0f;
        
        // Avanzar al siguiente frame
        currentFrame++;
        
        // Verificar si hemos llegado al final de la animación
        if (currentFrame >= currentAnimation->frameCount) {
            if (currentAnimation->isLooping) {
                // Si es una animación en bucle, volver al primer frame
                currentFrame = 0;
            } else {
                // Si no es bucle, detener la animación y ejecutar callback
                currentFrame = currentAnimation->frameCount - 1;
                isPlaying = false;
                
                // Ejecutar el callback si existe
                if (onAnimationEnd) {
                    onAnimationEnd();
                }
                
                return;
            }
        }
        
        // Actualizar el rectángulo de textura para mostrar el frame actual
        int columnIndex = currentFrame % (currentAnimation->frameSize.x > 0 ? 
            currentAnimation->frameSize.x : 1);
        int rowIndex = currentFrame / (currentAnimation->frameSize.x > 0 ? 
            currentAnimation->frameSize.x : 1);
        
        sf::IntRect rect(
            currentAnimation->startPosition.x + columnIndex * currentAnimation->frameSize.x,
            currentAnimation->startPosition.y + rowIndex * currentAnimation->frameSize.y,
            currentAnimation->frameSize.x,
            currentAnimation->frameSize.y
        );
        
        spriteFacade.setTextureRect(rect);
        spriteFacade.setScale(scale.x, scale.y);
    }
}

bool AnimatedSprite::isAnimationPlaying() const
{
    return isPlaying && !isPaused;
}

std::string AnimatedSprite::getCurrentAnimationName() const
{
    return currentAnimationName;
}