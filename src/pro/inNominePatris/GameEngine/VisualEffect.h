#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "SpriteFacade.h"
#include "AnimatedSprite.h"

class VisualEffect {
private:
    SpriteFacade sprite;
    std::unique_ptr<AnimatedSprite> animator;
    sf::Vector2i frameSize;
    int totalFrames;
    float frameDuration;
    float elapsedTime;
    int currentFrame;

    bool finished;
public:
    VisualEffect(
        const std::string& texturePath,
        sf::Vector2f position,
        sf::Vector2i frameSize,
        int frameCount,
        float frameDuration
    );

    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;

    bool isFinished() const;
};
