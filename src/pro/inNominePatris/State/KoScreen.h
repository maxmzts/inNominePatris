#ifndef KOSCREEN_H
#define KOSCREEN_H

#include "State.h"

class KoScreen : public State {
private:
    static KoScreen* instance;

    KoScreen() = default;

public:
    static KoScreen* getInstance();

    void update(Game& game) override;
    void render(Game& game, sf::RenderWindow& window) override;
};

#endif // KOSCREEN_H