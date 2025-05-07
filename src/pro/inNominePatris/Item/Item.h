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

class Item {
public:
    Item(ItemType type) : type(type) {
            spriteFacade.loadTexture("./resources/Items/cubo.png"); // Cargar textura por defecto
            spriteFacade.setOrigin(16.0f, 16.0f); // Establecer el origen
    }
    virtual ~Item() = default;

    virtual void applyEffect(Weapon& weapon) = 0; // Efecto específico del ítem
    void setTexture(const std::string& texturePath);
    void setPosition(float x, float y);
    void render(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    Item* generateRandomItemforWeapon(ItemType weapontype);

protected:
    SpriteFacade spriteFacade;
    bool isPickedUp = false;
    ItemType type;
};

//--------------Espada-------------------//
class DashBoostItem : public Item {
    public:
        DashBoostItem() : Item(ItemType::Sword) {}
        void applyEffect(Weapon& weapon) override;
        void Picked();
};

class DashCooldownItem : public Item {
    public:
        DashCooldownItem() : Item(ItemType::Sword) {}
        void applyEffect(Weapon& weapon) override;
        void Picked();
};

class AttackCooldownSwordItem : public Item {
    public:
        AttackCooldownSwordItem() : Item(ItemType::Sword) {}
        void applyEffect(Weapon& weapon) override;
        void Picked();
};

class AttackDamageSwordItem : public Item {
    public:
        AttackDamageSwordItem() : Item(ItemType::Sword) {}
        void applyEffect(Weapon& weapon) override;
        void Picked();
};

//--------------Lanza-------------------//
class AttackCooldownLanceItem : public Item {
    public:
        AttackCooldownLanceItem() : Item(ItemType::Lance) {}
        void applyEffect(Weapon& weapon) override;
        void Picked();
};

class AttackDamageLanceItem : public Item {
    public:
        AttackDamageLanceItem() : Item(ItemType::Lance) {}
        void applyEffect(Weapon& weapon) override;
        void Picked();
};
#endif // ITEM_H