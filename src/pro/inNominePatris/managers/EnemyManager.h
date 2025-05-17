// EnemyManager.h
#pragma once

#include <vector>
#include <memory>
#include "Enemy.h"
#include "Character.h"
#include "Projectile.h"



class EnemyManager {
private:
    // Instancia única del singleton
    static std::unique_ptr<EnemyManager> instance;
    
    // Lista de enemigos con punteros inteligentes
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::vector<Projectile*> activeProjectiles;  // Lista de proyectiles activos

    
    // Constructor privado (patrón singleton)
    EnemyManager() {}
    
    // Impedir copia y asignación
    EnemyManager(const EnemyManager&) = delete;
    EnemyManager& operator=(const EnemyManager&) = delete;
    
public:
    // Obtener instancia única
    static EnemyManager* getInstance();
    
    // Destructor
    ~EnemyManager() = default;
    
    // Añadir un enemigo individual
    void addEnemy(std::shared_ptr<Enemy> enemy);
    
    // Añadir múltiples enemigos
    void addEnemies(const std::vector<std::shared_ptr<Enemy>>& newEnemies);
    
    // Eliminar un enemigo específico
    void removeEnemy(std::shared_ptr<Enemy> enemy);
    
    // Eliminar todos los enemigos
    void clearEnemies();
    
    // Actualizar todos los enemigos
    void updateEnemies(float deltaTime, Character* player, const TileMap* tileMap);
    
    // Renderizar todos los enemigos
    void renderEnemies(sf::RenderWindow& window);
    
    // Eliminar enemigos muertos
    void removeDeadEnemies(Character* player);
    
    // Obtener lista de enemigos
    const std::vector<std::shared_ptr<Enemy>>& getEnemyList() const;
    
    // Encontrar enemigo por nombre o ID
    std::shared_ptr<Enemy> findEnemyByName(const std::string& name);
    
    // Obtener cantidad de enemigos
    size_t getEnemyCount() const;
    
    // Comprobar colisiones entre todos los enemigos y otro objeto
    bool checkCollisions(Hitbox* hitbox);

    // Crear proyectil
    void createProjectile(sf::Vector2f position, sf::Vector2f velocity, float damage, float lifetime);

    void updateProjectiles(float deltaTime, Character* player);
};