#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "State.h"

class PauseMenu : public State {
private:
    static PauseMenu* instance;

    PauseMenu() = default;

public:
    static PauseMenu* getInstance();

    void update(Game& game) override;
    void render(Game& game, sf::RenderWindow& window) override;
};

#endif // PAUSEMENU_H