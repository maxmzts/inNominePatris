#ifndef ITEM_H
#define ITEM_H

#include <SFML/Graphics.hpp>

class Item {
    public:
        virtual void applyEffect(Character& character) = 0;
        virtual ~Item() = default;
};

class DashBoost : public Item {
    public:
        void applyEffect(Character& character) override;
};

class SpeedBoost : public Item {
    public:
        void applyEffect(Character& character) override;
};