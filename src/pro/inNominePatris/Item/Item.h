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
    static Item* generateRandomItemforWeapon(ItemType weapontype);
    void Picked() { isPickedUp = true; }
    bool GetIsPickedUp() const { return isPickedUp; }
    ItemType getType() const { return type; }

protected:
    SpriteFacade spriteFacade;
    bool isPickedUp = false;
    ItemType type;
};

//--------------Espada-------------------//
class DashBoostItem : public Item {
    public:
        DashBoostItem() : Item(ItemType::Sword) {
            spriteFacade.loadTexture("./resources/Items/DashBoost.png");
        }
        void applyEffect(Weapon& weapon) override;
        void Picked();
};

class DashCooldownItem : public Item {
    public:
        DashCooldownItem() : Item(ItemType::Sword) {
            spriteFacade.loadTexture("./resources/Items/DashCooldown.png");
        }
        void applyEffect(Weapon& weapon) override;
        void Picked();
};

class AttackCooldownSwordItem : public Item {
    public:
        AttackCooldownSwordItem() : Item(ItemType::Sword) {
            spriteFacade.loadTexture("./resources/Items/SwordAttackCooldown.png");
        }
        void applyEffect(Weapon& weapon) override;
        void Picked();
};

class AttackDamageSwordItem : public Item {
    public:
        AttackDamageSwordItem() : Item(ItemType::Sword) {
            spriteFacade.loadTexture("./resources/Items/SwordAttackDamage.png");
        }
        void applyEffect(Weapon& weapon) override;
        void Picked();
};

class DoubleDashItem : public Item {
    public:
        DoubleDashItem() : Item(ItemType::Sword) {
            spriteFacade.loadTexture("./resources/Items/DoubleDash.png");
        }
        void applyEffect(Weapon& weapon) override;
        void Picked();
};

//--------------Lanza-------------------//
class AttackCooldownLanceItem : public Item {
    public:
        AttackCooldownLanceItem() : Item(ItemType::Lance) {
            spriteFacade.loadTexture("./resources/Items/LanceAttackCooldown.png");
        }
        void applyEffect(Weapon& weapon) override;
        void Picked();
};

class AttackDamageLanceItem : public Item {
    public:
        AttackDamageLanceItem() : Item(ItemType::Lance) {
            spriteFacade.loadTexture("./resources/Items/LanceAttackDamage.png");
        }
        void applyEffect(Weapon& weapon) override;
        void Picked();
};

class AttackHitboxLanceItem : public Item {
    public:
        AttackHitboxLanceItem() : Item(ItemType::Lance) {
            spriteFacade.loadTexture("./resources/Items/LanceAttackHitbox.png");
        }
        void applyEffect(Weapon& weapon) override;
        void Picked();
};

class RevengeReturnItem : public Item {
    public:
        RevengeReturnItem() : Item(ItemType::Lance) {
            spriteFacade.loadTexture("./resources/Items/RevengeReturn.png");
        }
        void applyEffect(Weapon& weapon) override;
        void Picked();
};

//--------------Arco-------------------//

class AttackDamageBowItem : public Item {
    public:
        AttackDamageBowItem() : Item(ItemType::Bow) {
            spriteFacade.loadTexture("./resources/Items/BowAttackDamage.png");
        }
        void applyEffect(Weapon& weapon) override;
        void Picked();
};

class ArrowSpeedItem : public Item {
    public:
        ArrowSpeedItem() : Item(ItemType::Bow) {
            spriteFacade.loadTexture("./resources/Items/ArrowSpeed.png");
        }
        void applyEffect(Weapon& weapon) override;
        void Picked();
};

class ArrowCountItem : public Item {
    public:
        ArrowCountItem() : Item(ItemType::Bow) {
            spriteFacade.loadTexture("./resources/Items/ArrowCount.png");
        }
        void applyEffect(Weapon& weapon) override;
        void Picked();
};

class QuickShotItem : public Item {
    public:
        QuickShotItem() : Item(ItemType::Bow) {
            spriteFacade.loadTexture("./resources/Items/QuickShot.png");
        }
        void applyEffect(Weapon& weapon) override;
        void Picked();
};

class DecreaseAbilityCooldownBowItem : public Item {
    public:
        DecreaseAbilityCooldownBowItem() : Item(ItemType::Bow) {
            spriteFacade.loadTexture("./resources/Items/BowAbilityCooldown.png");
        }
        void applyEffect(Weapon& weapon) override;
        void Picked();
};

#endif // ITEM_H