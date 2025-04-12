#include "EnemyManager.h"
#include <algorithm>

// Inicialización del singleton
std::unique_ptr<EnemyManager> EnemyManager::instance = nullptr;

EnemyManager* EnemyManager::getInstance() {
    if (instance == nullptr) {
        instance = std::unique_ptr<EnemyManager>(new EnemyManager());
    }
    return instance.get();
}

void EnemyManager::addEnemy(std::shared_ptr<Enemy> enemy) {
    if (enemy) {
        enemies.push_back(enemy);
    }
}

void EnemyManager::addEnemies(const std::vector<std::shared_ptr<Enemy>>& newEnemies) {
    for (const auto& enemy : newEnemies) {
        addEnemy(enemy);
    }
}

void EnemyManager::removeEnemy(std::shared_ptr<Enemy> enemy) {
    enemies.erase(
        std::remove(enemies.begin(), enemies.end(), enemy),
        enemies.end()
    );
}

void EnemyManager::clearEnemies() {
    enemies.clear();
}

void EnemyManager::updateEnemies(float deltaTime, Character* player, const TileMap* tileMap) {
    for (auto& enemy : enemies) {
        enemy->update(deltaTime, player, tileMap);
    }
    
    // Eliminar enemigos muertos después de la actualización
    removeDeadEnemies();
}

void EnemyManager::renderEnemies(sf::RenderWindow& window) {
    for (const auto& enemy : enemies) {
        enemy->render(window);
    }
}

void EnemyManager::removeDeadEnemies() {
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(), 
            [](const std::shared_ptr<Enemy>& enemy) {
                return enemy->isDead();
            }),
        enemies.end()
    );
}

const std::vector<std::shared_ptr<Enemy>>& EnemyManager::getEnemyList() const {
    return enemies;
}

std::shared_ptr<Enemy> EnemyManager::findEnemyByName(const std::string& name) {
    auto it = std::find_if(enemies.begin(), enemies.end(),
        [&name](const std::shared_ptr<Enemy>& enemy) {
            return enemy->getName() == name;
        });
    
    if (it != enemies.end()) {
        return *it;
    }
    
    return nullptr;
}

size_t EnemyManager::getEnemyCount() const {
    return enemies.size();
}


// reconsiderar
bool EnemyManager::checkCollisions(Hitbox* hitbox) {
    for (const auto& enemy : enemies) {
        if (enemy->getHurtbox()) {
            return hitbox->getGlobalBounds().intersects(enemy->getHurtbox()->getGlobalBounds());
        }
    }
    return false;
}