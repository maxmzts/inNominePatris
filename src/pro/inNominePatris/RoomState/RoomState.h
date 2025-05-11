#ifndef ROOM_STATE_H
#define ROOM_STATE_H

#include <vector>
#include <memory>
#include <algorithm>
#include "Enemy.h" // Asumimos que existe esta clase

// Clase base abstracta para los estados (salas)
class RoomState {
protected:
    bool completed;
    std::vector<std::shared_ptr<Enemy>> enemies;
    RoomState* currentRoomState;

public:
    RoomState();
    virtual ~RoomState();

    // Métodos principales del patrón State
    virtual void enter() = 0;    // Cuando entramos a la sala
    virtual void update() = 0;   // Actualización por frame
    virtual void exit() = 0;     // Cuando salimos de la sala
    
    // Métodos adicionales
    bool isCompleted() const;
    void setCompleted(bool value);
    
    // Método para comprobar si quedan enemigos
    bool hasEnemies() const;
};

#endif // ROOM_STATE_H