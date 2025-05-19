// Ejemplo de otra sala concreta
class BossRoom : public RoomState {
private:
    std::string roomId;
    std::shared_ptr<Enemy> bossEnemy;
    
public:
    BossRoom(const std::string& id) : roomId(id) {}
    
    void enter() override {
        if (!completed) {
            // Spawnear el jefe y otros enemigos
            EnemyManager::getInstance()->addEnemies(enemies);
        }
        // Lógica específica de inicialización de la sala del jefe
    }
    
    void update() override {
        removeDeadEnemies();
        
        if (!hasEnemies() && !completed) {
            completed = true;
            // Recompensas especiales por derrotar al jefe
        }
        
        // Lógica específica de la sala del jefe
    }
    
    void exit() override {
        // Lógica al salir de la sala del jefe
    }
    
    // Método específico para establecer el enemigo jefe
    void setBossEnemy(std::shared_ptr<Enemy> boss) {
        bossEnemy = boss;
        addEnemy(bossEnemy); // Lo añadimos también a la lista general
    }
};