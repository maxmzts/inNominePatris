#ifndef WEAPON_H
#define WEAPON_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include "GameEngine.h"
#include "Hitbox.h"
#include "SFXManager.h"

#include "ItemType.h"
#include "AbilityType.h"

class Weapon {
    protected:
        GameEngine* engine;
        std::string name;
        bool dealtDamage = false; // Indica si el arma ha causado daño en el último ataque

        // Atributos específicos del arma
        float baseDamage = 10.0f;
        float baseAttackSpeed = 1.0f;
        float baseCriticalChance = 0.1f;
        float criticalMultiplier = 1.5f;
        float baseCooldownReduction = 0.0f;

        //Para el combo de ataques
        int consecutiveAttacks = 0; // Contador de ataques consecutivos
        float attackResetTimer = 0.0f; // Temporizador para reiniciar el contador de ataques
        const float attackResetTime = 2.0f; // Tiempo para reiniciar el contador de ataques
        inline static float comboDamageBonus = 0.0f; // Daño adicional por combos

        // Atributos estáticos (globales para todas las armas)
        inline static float globalDamageMultiplier = 1.0f;
        inline static float globalAttackSpeedMultiplier = 1.0f;
        inline static float globalCriticalChanceBonus = 0.50f;
        inline static float globalCriticalMultiplier = 0.0f;
        inline static float globalCooldownReduction = 0.0f;
    
    public:
        Weapon(GameEngine* engine) : engine(engine) {}
        virtual ~Weapon() = default;
        
        virtual void attack(sf::Vector2f position, sf::Vector2f direction) = 0;
        // REIMPLEMENTAR PORQUE CAUSA REFERENCIAS CIRCULARES POR TODOS LADOS
        virtual bool useAbility() = 0;
        virtual void useAbility(sf::Vector2f characterPosition) = 0;
        virtual void useAbility(sf::Vector2f position, sf::Vector2f direction) = 0;
        virtual AbilityType getAbilityType() const = 0;
        virtual ItemType getItemType() const = 0;

        // Método para dibujar el arma, ajustado al personaje
        //virtual void draw(sf::Vector2f position, sf::Vector2f direction) = 0;
        virtual void render(sf::RenderWindow& window) = 0;
        virtual void update(float deltaTime) = 0;
        virtual void renderOnPlayer(sf::Vector2f position, sf::Vector2f direction, sf::RenderWindow& window) = 0;
        virtual sf::Vector2f getPosition() const = 0;
        virtual void setPosition(float x, float y) = 0;
        virtual std::shared_ptr<Hitbox> getAttackHitbox() const = 0; // Método para obtener la hitbox de ataque
        bool hasDealtDamage() const { return dealtDamage; }
        void setDealtDamage(bool value) { dealtDamage = value; }


        // Métodos para obtener y establecer el nombre del arma
        const std::string& getName() const { return name; }

        float calculateDamage() const {
            float damage = getDamage() * globalDamageMultiplier;
            damage *= (1.0f + comboDamageBonus * consecutiveAttacks); // Aumentar el daño por ataques consecutivos
            float randomValue = static_cast<float>(rand()) / RAND_MAX; // Generar un número aleatorio entre 0 y 1
            if (randomValue < baseCriticalChance + globalCriticalChanceBonus) {
                float pitch = 0.8f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (1.2f - 0.8f);
                SFXManager::getInstance().addEffect("./resources/sfx/critico.wav", 60.f, pitch);
                return damage * getCriticalMultiplier(); // Daño crítico
            }
            return damage; // Daño normal
        }

        //Para las mejoras globales de las armas 
        static void increaseGlobalDamageMultiplier(float amount) {
            globalDamageMultiplier += amount;
        }

        static void increaseGlobalAttackSpeedMultiplier(float amount) {
            globalAttackSpeedMultiplier += amount;
        }

        static void increaseGlobalCriticalChanceBonus(float amount) {
            globalCriticalChanceBonus += amount;
        }

        static void increaseGlobalCriticalMultiplier(float amount) {
            globalCriticalMultiplier += amount;
        }

        static void increaseGlobalCooldownReduction(float amount) {
            globalCooldownReduction += amount;
        }

        static void enableComboDamageBonus(float amount) {
            comboDamageBonus += amount;
            std::cout << "Daño por combo aumentado en " << amount * 100 << "%." << std::endl;
        }

        float getDamage() const {
            return baseDamage * globalDamageMultiplier;
        }

        float getAttackSpeed() const {
            return baseAttackSpeed * globalAttackSpeedMultiplier;
        }

        float getCriticalChance() const {
            return baseCriticalChance + globalCriticalChanceBonus;
        }

        float getCooldownReduction() const {
            return baseCooldownReduction + globalCooldownReduction;
        }

        float getCriticalMultiplier() const {
            return criticalMultiplier + globalCriticalMultiplier;
        }

        void increaseConsecutiveAttacks() {
            consecutiveAttacks++;
            attackResetTimer = 0.0f;
        }

        void resetConsecutiveAttacks() {
            consecutiveAttacks = 0;
        }

        void updateConsecutiveAttacks(float deltaTime) {
            if (consecutiveAttacks > 0){
                attackResetTimer += deltaTime;
                if (attackResetTimer >= attackResetTime) {
                    resetConsecutiveAttacks();
                }
            }
        }
    };

#endif // !WEAPON_H