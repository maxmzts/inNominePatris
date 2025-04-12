#include "Shop.h"
#include <iostream>

Shop::Shop(sf::RenderWindow& window, int playerKarma)
    : window(window), openState(false), playerKarma(playerKarma), currentUpgradeIndex(0) {
    if (!font.loadFromFile("./assets/fonts/IMPACT.TTF")) {
        std::cerr << "Error loading font\n";
        exit(-1);
    }

    // Configurar el fondo para que ocupe toda la pantalla
    background.setSize(sf::Vector2f(window.getDefaultView().getSize().x, window.getDefaultView().getSize().y));
    background.setFillColor(sf::Color(50, 50, 50, 200));
    background.setPosition(0, 0); // Centrado automáticamente al ocupar toda la pantalla

    // Configurar el título
    titleText.setFont(font);
    titleText.setString("Tienda de Karma");
    titleText.setCharacterSize(36);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(window.getDefaultView().getSize().x / 2.f - titleText.getGlobalBounds().width / 2.f, 50);

    // Configurar las mejoras
    upgrade1Text.setFont(font);
    upgrade1Text.setCharacterSize(24);
    upgrade1Text.setFillColor(sf::Color::White);

    upgrade2Text.setFont(font);
    upgrade2Text.setCharacterSize(24);
    upgrade2Text.setFillColor(sf::Color::White);

    loadUpgrades();
}

void Shop::open() {
    openState = true;
}

void Shop::close() {
    openState = false;
}

bool Shop::isOpen() const {
    return openState;
}

void Shop::update(int playerKarma) {
    if (!openState) return;

    this->playerKarma = playerKarma;

    if (currentUpgradeIndex < upgrades.size() - 1) {
        // Configurar los textos de las mejoras
        upgrade1Text.setString(upgrades[currentUpgradeIndex].name + " - " + std::to_string(upgrades[currentUpgradeIndex].cost) + " Karma");
        upgrade2Text.setString(upgrades[currentUpgradeIndex + 1].name + " - " + std::to_string(upgrades[currentUpgradeIndex + 1].cost) + " Karma");

        // Centrar los textos en relación con el fondo
        float centerX = background.getPosition().x + background.getSize().x / 2.f;
        float centerY = background.getPosition().y + background.getSize().y / 2.f;

        upgrade1Text.setPosition(centerX - upgrade1Text.getGlobalBounds().width / 2.f, centerY - 50);
        upgrade2Text.setPosition(centerX - upgrade2Text.getGlobalBounds().width / 2.f, centerY + 50);
    }

    // Actualizar el texto del karma actual
    karmaText.setString("Karma actual: " + std::to_string(playerKarma));
    karmaText.setPosition(background.getPosition().x + 20, background.getPosition().y + 20);

    // Configurar el botón para mostrar las siguientes mejoras
    nextButtonText.setString("Siguiente");
    nextButtonText.setPosition(background.getPosition().x + background.getSize().x - 120, background.getPosition().y + background.getSize().y - 50);
}

void Shop::render() {
    if (!openState) return;

    // Guardar la vista actual
    sf::View originalView = window.getView();

    // Restablecer la vista predeterminada para usar coordenadas de pantalla
    window.setView(window.getDefaultView());

    // Dibujar los elementos de la tienda
    window.draw(background);
    window.draw(titleText);
    window.draw(upgrade1Text);
    window.draw(upgrade2Text);
    window.draw(karmaText); // Dibujar el karma actual
    window.draw(nextButtonText); // Dibujar el botón de siguiente

    // Restaurar la vista original
    window.setView(originalView);
}

void Shop::setOnUpgradeSelectedCallback(const std::function<void(const Upgrade&)>& callback) {
    onUpgradeSelected = callback;
}

void Shop::loadUpgrades() {
    upgrades.push_back(Upgrade("Danyo aumentado", KarmaType::Pecado, 100));
    upgrades.push_back(Upgrade("Velocidad de movimiento", KarmaType::Absolucion, 100));
    upgrades.push_back(Upgrade("Velocidad de ataque", KarmaType::Pecado, 200));
    upgrades.push_back(Upgrade("Reducción de recarga", KarmaType::Absolucion, 200));
    upgrades.push_back(Upgrade("Probabilidad de crítico", KarmaType::Pecado, 300));
    upgrades.push_back(Upgrade("Cantidad de vidas", KarmaType::Absolucion, 300));
    upgrades.push_back(Upgrade("Daño crítico aumentado", KarmaType::Pecado, 500));
    upgrades.push_back(Upgrade("Regeneración de vida", KarmaType::Absolucion, 500));
    upgrades.push_back(Upgrade("Área de ataque aumentada", KarmaType::Pecado, 700));
    upgrades.push_back(Upgrade("Escudo temporal", KarmaType::Absolucion, 700));
}

void Shop::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // Verificar si se hace clic en la primera mejora
        if (upgrade1Text.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            if (playerKarma >= upgrades[currentUpgradeIndex].cost) {
                playerKarma -= upgrades[currentUpgradeIndex].cost;
                if (onUpgradeSelected) onUpgradeSelected(upgrades[currentUpgradeIndex]);
                currentUpgradeIndex += 2; // Pasar a las siguientes mejoras
            } else {
                insufficientKarmaText.setString("Karma insuficiente");
                insufficientKarmaText.setPosition(upgrade1Text.getPosition().x, upgrade1Text.getPosition().y + 30);
            }
        }

        // Verificar si se hace clic en la segunda mejora
        if (upgrade2Text.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            if (playerKarma >= upgrades[currentUpgradeIndex + 1].cost) {
                playerKarma -= upgrades[currentUpgradeIndex + 1].cost;
                if (onUpgradeSelected) onUpgradeSelected(upgrades[currentUpgradeIndex + 1]);
                currentUpgradeIndex += 2; // Pasar a las siguientes mejoras
            } else {
                insufficientKarmaText.setString("Karma insuficiente");
                insufficientKarmaText.setPosition(upgrade2Text.getPosition().x, upgrade2Text.getPosition().y + 30);
            }
        }

        // Verificar si se hace clic en el botón de siguiente
        if (nextButtonText.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            currentUpgradeIndex += 2; // Pasar a las siguientes mejoras
        }
    } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::B) {
        close(); // Cerrar la tienda con Escape
    }
}