#ifndef LANCE_H
#define LANCE_H

#include <SFML/Graphics.hpp>
#include "SpriteFacade.h"
#include "Weapon.h"
#include "Hitbox.h"
#include "AnimatedSprite.h"

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
        void attack(sf::Vector2f position, sf::Vector2f direction) override;
        void createHitbox(sf::Vector2f position, sf::Vector2f direction);
        bool useAbility() override {}
        void useAbility(sf::Vector2f characterPosition, sf::Vector2f mousePosition);
        const sf::Vector2f& teleportToPortal();
        void DrawPortal(sf::RenderWindow& window);
        void PortalUpdate(float deltaTime);
        void update(float deltaTime) override;
        void increasePortalRange(float range);
        void decreaseAttackCooldown(float cooldown);
        void increaseAttackDamage(float damage);
        //void draw(GameEngine& engine, sf::Vector2f position, sf::Vector2f direction) override;
        void render(sf::RenderWindow& window) override;
        void renderOnPlayer(sf::Vector2f position, sf::Vector2f direction, sf::RenderWindow& window) override;
        void setPosition(float x, float y) override { spriteFacade.setPosition(x, y); };
        sf::Vector2f getPosition() const override { return spriteFacade.getPosition(); };
        AbilityType getAbilityType() const override { return AbilityType::Teleport; };
        bool getIsPortalDropped() const { return isPortalDropped; }
        std::shared_ptr<Hitbox> getAttackHitbox() const override;
    private:
        SpriteFacade spriteFacade;
        bool isPortalDropped;
        float PortalRange;
        Portal portal;
        float abilityCooldown = 4.0f; // Cooldown para la habilidad (en segundos)
        float abilityTimer = 0.f;     // Temporizador para la habilidad

        std::shared_ptr<Hitbox> attackHitbox;
        float attackCooldown;
        float attackTimer;

        SpriteFacade pinchSprite;
        AnimatedSprite pinchAnimation;
        bool isAnimating = false;
};



#endif // !LANCE_H