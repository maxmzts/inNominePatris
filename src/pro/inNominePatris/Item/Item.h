#ifndef ITEM_H
#define ITEM_H

#include <SFML/Graphics.hpp>
#include "Weapon.h"
#include "SpriteFacade.h"
#include "GameEngine.h"
#include "ItemType.h"
#include "Character.h"
#include "Sword.h"
#include "Lance.h"
#include "Bow.h"
#include "HUD.h" // Añadido para mostrar notificaciones

class Item {
public:
    Item(ItemType type) : type(type) {
            spriteFacade.loadTexture("./resources/Items/cubo.png"); // Cargar textura por defecto
            spriteFacade.setOrigin(16.0f, 16.0f); // Establecer el origen

            // Inicializar la fuente para el texto
            if (!font.loadFromFile("./assets/fonts/IMPACT.TTF")) { // Asegúrate de tener esta fuente o cambia la ruta
                std::cerr << "Error cargando la fuente" << std::endl;
            }
            
            // Configurar el texto por defecto
            itemNameText.setFont(font);
            itemNameText.setCharacterSize(12);
            itemNameText.setFillColor(sf::Color::White);
            itemNameText.setOutlineColor(sf::Color::Black);
            itemNameText.setOutlineThickness(1.0f);
            
            itemDescription = "???";
    }

    virtual ~Item() = default;

    virtual void applyEffect(Weapon& weapon) = 0; // Efecto específico del ítem
    void setTexture(const std::string& texturePath);
    void setPosition(float x, float y);
    void render(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    static Item* generateRandomItemforWeapon(ItemType weapontype);
    
    void Picked(HUD* hud = nullptr) { 
        isPickedUp = true;
        if (hud != nullptr) {
            hud->showItemNotification(itemName, itemDescription);
        }
    }
    
    bool GetIsPickedUp() const { return isPickedUp; }
    ItemType getType() const { return type; }
    void setItemName(const std::string& name);
    const std::string& getItemName() const { return itemName; }
    const std::string& getItemDescription() const { return itemDescription; }

protected:
    SpriteFacade spriteFacade;
    bool isPickedUp = false;
    ItemType type;

    sf::Text itemNameText;
    sf::Font font;
    std::string itemName;
    std::string itemDescription;
    void setupItemName(const std::string& name);
    void setItemDescription(const std::string& description) { itemDescription = description; }
};

//--------------Espada-------------------//
class DashBoostItem : public Item {
    public:
        DashBoostItem() : Item(ItemType::Sword) {
            spriteFacade.loadTexture("./resources/Items/DashBoost.png");
            setupItemName("Slipstream Edge");
            setItemDescription("+ Velocidad de dash");
        }
        void applyEffect(Weapon& weapon) override;
        void Picked(HUD* hud = nullptr) { Item::Picked(hud); }
};

class DashCooldownItem : public Item {
    public:
        DashCooldownItem() : Item(ItemType::Sword) {
            spriteFacade.loadTexture("./resources/Items/DashCooldown.png");
            setupItemName("Chrono Flash");
            setItemDescription("- Recarga de dash");
        }
        void applyEffect(Weapon& weapon) override;
        void Picked(HUD* hud = nullptr) { Item::Picked(hud); }
};

class AttackCooldownSwordItem : public Item {
    public:
        AttackCooldownSwordItem() : Item(ItemType::Sword) {
            spriteFacade.loadTexture("./resources/Items/SwordAttackCooldown.png");
            setupItemName("Executioner's Hand");
            setItemDescription("- Tiempo entre ataques");
        }
        void applyEffect(Weapon& weapon) override;
        void Picked(HUD* hud = nullptr) { Item::Picked(hud); }
};

class AttackDamageSwordItem : public Item {
    public:
        AttackDamageSwordItem() : Item(ItemType::Sword) {
            spriteFacade.loadTexture("./resources/Items/SwordAttackDamage.png");
            setupItemName("Emery Tears");
            setItemDescription("+ Daño con espada");
        }
        void applyEffect(Weapon& weapon) override;
        void Picked(HUD* hud = nullptr) { Item::Picked(hud); }
};

class DoubleDashItem : public Item {
    public:
        DoubleDashItem() : Item(ItemType::Sword) {
            spriteFacade.loadTexture("./resources/Items/DoubleDash.png");
            setupItemName("Twin Specter");
            setItemDescription("Dash doble");
        }
        void applyEffect(Weapon& weapon) override;
        void Picked(HUD* hud = nullptr) { Item::Picked(hud); }
};

//--------------Lanza-------------------//
class AttackCooldownLanceItem : public Item {
    public:
        AttackCooldownLanceItem() : Item(ItemType::Lance) {
            spriteFacade.loadTexture("./resources/Items/LanceAttackCooldown.png");
            setupItemName("Deep Heartbeat");
            setItemDescription("Tiempo entre ataques");
        }
        void applyEffect(Weapon& weapon) override;
        void Picked(HUD* hud = nullptr) { Item::Picked(hud); }
};

class AttackDamageLanceItem : public Item {
    public:
        AttackDamageLanceItem() : Item(ItemType::Lance) {
            spriteFacade.loadTexture("./resources/Items/LanceAttackDamage.png");
            setupItemName("Titan's Fang");
            setItemDescription("+ Fuerza con lanza");
        }
        void applyEffect(Weapon& weapon) override;
        void Picked(HUD* hud = nullptr) { Item::Picked(hud); }
};

class AttackHitboxLanceItem : public Item {
    public:
        AttackHitboxLanceItem() : Item(ItemType::Lance) {
            spriteFacade.loadTexture("./resources/Items/LanceAttackHitbox.png");
            setupItemName("Abyssal Reach");
            setItemDescription("+ Alcance de ataques");
        }
        void applyEffect(Weapon& weapon) override;
        void Picked(HUD* hud = nullptr) { Item::Picked(hud); }
};

class RevengeReturnItem : public Item {
    public:
        RevengeReturnItem() : Item(ItemType::Lance) {
            spriteFacade.loadTexture("./resources/Items/RevengeReturn.png");
            setupItemName("Vengeance Echo");
            setItemDescription("+ Fuerza post-teletransporte");
        }
        void applyEffect(Weapon& weapon) override;
        void Picked(HUD* hud = nullptr) { Item::Picked(hud); }
};

//--------------Arco-------------------//

class AttackDamageBowItem : public Item {
    public:
        AttackDamageBowItem() : Item(ItemType::Bow) {
            spriteFacade.loadTexture("./resources/Items/BowAttackDamage.png");
            setupItemName("Deadly Whisper");
            setItemDescription("+ Fuerza con arco");
        }
        void applyEffect(Weapon& weapon) override;
        void Picked(HUD* hud = nullptr) { Item::Picked(hud); }
};

class ArrowSpeedItem : public Item {
    public:
        ArrowSpeedItem() : Item(ItemType::Bow) {
            spriteFacade.loadTexture("./resources/Items/ArrowSpeed.png");
            setupItemName("Celestial Breath");
            setItemDescription("+ Velocidad de flechas");
        }
        void applyEffect(Weapon& weapon) override;
        void Picked(HUD* hud = nullptr) { Item::Picked(hud); }
};

class ArrowCountItem : public Item {
    public:
        ArrowCountItem() : Item(ItemType::Bow) {
            spriteFacade.loadTexture("./resources/Items/ArrowCount.png");
            setupItemName("Needle Swarm");
            setItemDescription("+ Flechas por disparo");
        }
        void applyEffect(Weapon& weapon) override;
        void Picked(HUD* hud = nullptr) { Item::Picked(hud); }
};

class QuickShotItem : public Item {
    public:
        QuickShotItem() : Item(ItemType::Bow) {
            spriteFacade.loadTexture("./resources/Items/QuickShot.png");
            setupItemName("Mercury Fingers");
            setItemDescription("Disparos instantáneos");
        }
        void applyEffect(Weapon& weapon) override;
        void Picked(HUD* hud = nullptr) { Item::Picked(hud); }
};

class DecreaseAbilityCooldownBowItem : public Item {
    public:
        DecreaseAbilityCooldownBowItem() : Item(ItemType::Bow) {
            spriteFacade.loadTexture("./resources/Items/BowAbilityCooldown.png");
            setupItemName("Time Prism");
            setItemDescription("- Recarga de habilidad");
        }
        void applyEffect(Weapon& weapon) override;
        void Picked(HUD* hud = nullptr) { Item::Picked(hud); }
};

#endif // ITEM_H