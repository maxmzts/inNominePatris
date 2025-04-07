#ifndef LANCE_H
#define LANCE_H

#include <SFML/Graphics.hpp>
#include "SpriteFacade.h"
#include "Weapon.h"

class Portal {
    public:
        Portal();
        Portal(const sf::Vector2f& pos);
        void setPosition(const sf::Vector2f& pos);
        const sf::Vector2f& getPosition() const;

        void draw(sf::RenderWindow& window);
        void setVisible(bool visible); 
        bool isVisible() const;

        void update(float deltaTime);
    private:
        GameEngine* engine;
        sf::Vector2f position;
        sf::Sprite sprite;
        static sf::Texture portalTexture;
        std::vector<sf::Texture> animationFrames;
        int currentFrame = 0;
        float animationTimer = 0.0f;
        float frameDuration = 0.1f;
        bool visible;

        void loadAnimationFrames();
};

class Lance : public Weapon {
    public:
        Lance(GameEngine* engine);
        void attack(Character& character, std::vector<Enemy>& enemies) override;
        void useAbility(Character& character) override {}
        void useAbility(Character& character,  sf::RenderWindow& window) override;
        void DrawPortal(sf::RenderWindow& window);
        void PortalUpdate(float deltaTime);
        void increaseAttackRange(float range);
        void increasePortalRange(float range);
        void draw(GameEngine& engine, const Character* character) override;
        void setPosition(float x, float y) override { spriteFacade.setPosition(x, y); };
        sf::Vector2f getPosition() const override { return spriteFacade.getPosition(); };
        AbilityType getAbilityType() const override { return AbilityType::Teleport; };


    private:
        SpriteFacade spriteFacade;
        float attackRange;
        bool isPortalDropped;
        float PortalRange;
        Portal portal;
};



#endif // !LANCE_H