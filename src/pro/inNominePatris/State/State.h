#ifndef STATE_H
#define STATE_H

#include <SFML/Graphics.hpp>

class Game;

class State {
public:
    virtual ~State() = default;

    virtual void update(Game& game) = 0;
    virtual void render(Game& game, sf::RenderWindow& window) = 0;
};

#endif // STATE_H