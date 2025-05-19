#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <functional>
#include "SpriteFacade.h"

class AnimatedSprite {
private:
    // Estructura que define una animación
    struct Animation {
        int frameCount;            // Cantidad de frames en la animación
        sf::Vector2i startPosition; // Posición inicial en el spritesheet
        sf::Vector2i frameSize;    // Tamaño de cada frame
        float frameDuration;       // Duración de cada frame en segundos
        bool isLooping;            // Si la animación debe repetirse
    };

    SpriteFacade& spriteFacade;    // Referencia al SpriteFacade que será animado
    
    // Mapa de animaciones disponibles (nombre -> animación)
    std::map<std::string, Animation> animations;
    
    // Estado de la animación actual
    Animation* currentAnimation;
    std::string currentAnimationName;
    int currentFrame;
    float currentTime;
    bool isPlaying;
    bool isPaused;
    float speedFactor;
    
    // Callback para cuando una animación termina
    using AnimationCallback = std::function<void()>;
    AnimationCallback onAnimationEnd;

public:
    // Constructor que recibe una referencia al SpriteFacade
    AnimatedSprite(SpriteFacade& spriteFacade);
    
    // Añadir una nueva animación desde un spritesheet
    void addAnimation(
        const std::string& name,
        int frameCount,
        sf::Vector2i startPosition,
        sf::Vector2i frameSize,
        bool loop = true
    );
    
    // Reproducir una animación con una velocidad específica en FPS
    void play(const std::string& name, float fps = 12.0f, bool loop = true);
    
    // Pausar la animación actual
    void pause();
    
    // Reanudar la animación pausada
    void resume();
    
    // Detener la animación actual
    void stop();
    
    // Establecer callback para cuando termina la animación
    void setAnimationEndCallback(AnimationCallback callback);
    
    // Actualizar la animación (llamar en cada frame)
    void update(float deltaTime);
    
    // Verificar si hay una animación reproduciéndose
    bool isAnimationPlaying() const;
    
    // Obtener el nombre de la animación actual
    std::string getCurrentAnimationName() const;
};