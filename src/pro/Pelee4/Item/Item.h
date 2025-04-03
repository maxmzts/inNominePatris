#ifndef ITEM_H
#define ITEM_H

#include <SFML/Graphics.hpp>
#include "Character.h"
#include "Sword.h"
#include "Lance.h"
#include "Bow.h"
#include "ItemType.h"

class Item {
    protected:
        sf::Sprite sprite;
        sf::Texture texture;
        bool isPickedUp = false;
        ItemType type;
    public:
        Item(ItemType type) : type(type) {}
        virtual void applyEffect(Weapon& weapon) = 0;
        void markAsPicked() { isPickedUp = true; }
        Item* generateRandomItemforWeapon(ItemType weapontype);
        void setTexture(const sf::Texture& tex);
        void setTextureRect(int left, int top, int width, int height);
        void setPosition(float x, float y);
        void draw(sf::RenderWindow& window);
        sf::FloatRect getBounds() const;
        virtual ~Item() = default;
};
class AttackRangeSword : public Item {
    public:
        AttackRangeSword() : Item(ItemType::Sword) {}
        void applyEffect(Weapon& weapon) override;
        void Picked();
};

class DashBoostItem : public Item {
    public:
        DashBoostItem() : Item(ItemType::Sword) {}
        void applyEffect(Weapon& weapon) override;
        void Picked();
};

class AttackRangeLance : public Item {
    public:
        AttackRangeLance() : Item(ItemType::Lance) {}
        void applyEffect(Weapon& weapon) override;
        void Picked();
};


class PortalRangeItem: public Item {
    public:
        PortalRangeItem() : Item(ItemType::Lance) {}
        void applyEffect(Weapon& weapon) override;
        void Picked();
};

class ArrowSpeedItem: public Item {
    public:
        ArrowSpeedItem() : Item(ItemType::Bow) {}
        void applyEffect(Weapon& weapon) override;
        void Picked();
};

class AbilityArrowCountItem: public Item {
    public:
        AbilityArrowCountItem() : Item(ItemType::Bow) {}
        void applyEffect(Weapon& weapon) override;
        void Picked();
};

#endif // !ITEM_H