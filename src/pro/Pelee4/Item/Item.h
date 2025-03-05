#ifndef ITEM_H
#define ITEM_H

#include <SFML/Graphics.hpp>
#include "Character.h"
#include "Sword.h"

class Item {
    protected:
        sf::Sprite sprite;
        sf::Texture texture;
        bool isPickedUp = false;
    public:
        virtual void applyEffect(Sword& sword) = 0;
        void setTexture(const sf::Texture& tex);
        void setTextureRect(int left, int top, int width, int height);
        void setPosition(float x, float y);
        void draw(sf::RenderWindow& window);
        sf::FloatRect getBounds() const;
        virtual ~Item() = default;
};

class DashBoostItem : public Item {
    public:
        void applyEffect(Sword& sword) override;
};

class SpeedBoostItem : public Item {
    public:
        void applyEffect(Sword& sword) override;
};

#endif // !ITEM_H