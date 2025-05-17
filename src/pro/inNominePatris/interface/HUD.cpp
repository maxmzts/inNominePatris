#include "HUD.h"
#include <iostream>

// Constructor privado
HUD::HUD(float width, float height) {
    // Configurar inventario
    inventoryBackground.setSize(sf::Vector2f(300, 100));
    inventoryBackground.setFillColor(sf::Color(50, 50, 50, 200));
    inventoryBackground.setPosition(10, height - 150);

    // Configurar texto del arma
    weaponText.setFont(font);
    weaponText.setCharacterSize(20);
    weaponText.setFillColor(sf::Color::White);
    weaponText.setPosition(20, height - 140);

    // Configurar texto del arma secundaria
    secondaryWeaponText.setFont(font);
    secondaryWeaponText.setCharacterSize(20);
    secondaryWeaponText.setFillColor(sf::Color::White);
    secondaryWeaponText.setPosition(20, height - 110);

// Configurar notificación de ítem
itemNotificationBackground.setSize(sf::Vector2f(400, 100));
itemNotificationBackground.setFillColor(sf::Color(20, 20, 20, 220));
itemNotificationBackground.setOutlineColor(sf::Color(255, 215, 0, 255));
itemNotificationBackground.setOutlineThickness(2.0f);

itemNotificationBackground.setPosition(width - 410, height - 110);

itemNotificationTitle.setFont(font);
itemNotificationTitle.setCharacterSize(24);
itemNotificationTitle.setFillColor(sf::Color(255, 215, 0, 255));
itemNotificationTitle.setPosition(width - 400, height - 100);

itemNotificationDesc.setFont(font);
itemNotificationDesc.setCharacterSize(18);
itemNotificationDesc.setFillColor(sf::Color(255, 255, 255, 255));
itemNotificationDesc.setPosition(width - 400, height - 70);

    itemNotificationActive = false;
    itemNotificationDuration = 0.0f;

    // Cargar fuente
    if (!font.loadFromFile("./assets/fonts/IMPACT.TTF")) {
        std::cerr << "Error cargando la fuente\n";
        exit(-1);
    }
}

// Obtener la instancia única
HUD& HUD::getInstance(float width, float height) {
    static HUD instance(width, height);
    return instance;
}

void HUD::update(const Character& character) {
    // Calcular el delta time
    float deltaTime = deltaClock.restart().asSeconds();

    // Actualizar texto del arma equipada
    Weapon* equippedWeapon = character.getEquippedWeapon();
    if (equippedWeapon) {
        weaponText.setString("Arma equipada: " + equippedWeapon->getName() + ", Karma: " + std::to_string(character.getKarma()));
    } else {
        weaponText.setString("Sin arma equipada");
    }

    // Actualizar texto del arma secundaria
    if (character.getWeaponCount() > 1) {
        int secondaryIndex = 1 - character.getEquippedIndex(); // Índice del arma secundaria
        Weapon* secondaryWeapon = character.getWeaponAtIndex(secondaryIndex);
        if (secondaryWeapon) {
            secondaryWeaponText.setString("Arma secundaria: " + secondaryWeapon->getName());
        }
    } else {
        secondaryWeaponText.setString("");
    }

    // Actualizar temporizador de notificación de ítem
    if (itemNotificationActive) {
        itemNotificationDuration -= deltaTime;
        if (itemNotificationDuration <= 0.0f) {
            itemNotificationActive = false;
        }
        
        // Efecto de desvanecimiento al final de la notificación
        if (itemNotificationDuration < 1.0f) {
            float alpha = 255.0f * (itemNotificationDuration);
            sf::Color bgColor = itemNotificationBackground.getFillColor();
            bgColor.a = static_cast<sf::Uint8>(alpha * 0.86f);  // Mantener la transparencia original
            itemNotificationBackground.setFillColor(bgColor);
            
            sf::Color outlineColor = itemNotificationBackground.getOutlineColor();
            outlineColor.a = static_cast<sf::Uint8>(alpha);
            itemNotificationBackground.setOutlineColor(outlineColor);
            
            sf::Color titleColor = itemNotificationTitle.getFillColor();
            titleColor.a = static_cast<sf::Uint8>(alpha);
            itemNotificationTitle.setFillColor(titleColor);
            
            sf::Color descColor = itemNotificationDesc.getFillColor();
            descColor.a = static_cast<sf::Uint8>(alpha);
            itemNotificationDesc.setFillColor(descColor);
        }
    }
}

void HUD::showItemNotification(const std::string& itemName, const std::string& itemDescription) {
    itemNotificationActive = true;
    itemNotificationDuration = 4.0f; // Mostrar durante 4 segundos
    
    // Restablecer colores con opacidad completa
    itemNotificationBackground.setFillColor(sf::Color(20, 20, 20, 220));
    itemNotificationBackground.setOutlineColor(sf::Color(255, 215, 0, 255));
    itemNotificationTitle.setFillColor(sf::Color(255, 215, 0, 255));
    itemNotificationDesc.setFillColor(sf::Color(255, 255, 255, 255));
    
    // Actualizar textos
    itemNotificationTitle.setString(itemName);
    itemNotificationDesc.setString(itemDescription);
}

void HUD::draw(sf::RenderWindow& window, const Character& character) {
    // Guardar la vista actual
    sf::View originalView = window.getView();

    // Restablecer la vista para usar coordenadas de pantalla
    window.setView(window.getDefaultView());

    // Dibujar inventario
    window.draw(inventoryBackground);

    // Dibujar texto del arma equipada
    window.draw(weaponText);

    // Dibujar texto del arma secundaria
    window.draw(secondaryWeaponText);
    
    // Dibujar notificación de ítem si está activa
    if (itemNotificationActive) {
        window.draw(itemNotificationBackground);
        window.draw(itemNotificationTitle);
        window.draw(itemNotificationDesc);
    }

    // Restaurar la vista original
    window.setView(originalView);
}