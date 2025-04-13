#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>

enum class KarmaType {
    Pecado,
    Absolucion
};

struct Upgrade {
    std::string name;
    KarmaType type;
    int cost;

    Upgrade(const std::string& name, KarmaType type, int cost)
        : name(name), type(type), cost(cost) {}
};

class Shop {
public:
    Shop(sf::RenderWindow& window, int playerKarma);

    void open();
    void close();
    bool isOpen() const;

    void update(int playerKarma);
    void render();
    void handleInput(const sf::Event& event);

    void setOnUpgradeSelectedCallback(const std::function<void(const Upgrade&)>& callback);

private:
    sf::RenderWindow& window;
    bool openState;
    int playerKarma;

    std::vector<Upgrade> upgrades;
    size_t currentUpgradeIndex;

    sf::RectangleShape background;
    sf::Text titleText;
    sf::Text upgrade1Text;
    sf::Text upgrade2Text;
    sf::Text karmaText; // Texto para mostrar el karma actual
    sf::Text insufficientKarmaText; // Mensaje de karma insuficiente
    sf::Text nextButtonText; // Botón para mostrar las siguientes mejoras
    sf::Font font;

    std::function<void(const Upgrade&)> onUpgradeSelected;

    void loadUpgrades();
};