#ifndef ROOM_MANAGER_H
#define ROOM_MANAGER_H

#include <unordered_map>
#include <memory>
#include <string>
#include "RoomState.h"

// Gestor de estados (salas)
class RoomManager {
private:
    std::shared_ptr<RoomState> currentState;
    std::unordered_map<std::string, std::shared_ptr<RoomState>> states;
    
    // Singleton
    static RoomManager* instance;
    
    // Constructor privado (patrón Singleton)
    RoomManager();

public:
    // Obtener instancia del Singleton
    static RoomManager* getInstance();
    
    // Destructor
    ~RoomManager();
    
    // Registrar un nuevo estado (sala)
    void registerState(const std::string& stateId, std::shared_ptr<RoomState> state);
    
    // Cambiar al estado (sala) especificado
    void changeState(const std::string& stateId);
    
    // Actualizar el estado actual
    void update(TileMap& tileMap);
    
    // Obtener el estado actual
    std::shared_ptr<RoomState> getCurrentState() const;

    void clearRooms(){ states.clear(); currentState.reset(); }

    void resetAllRooms();

    void resetInstance() {
        if (instance) {
            delete instance;
            instance = nullptr;
        }
    }
};

#endif // ROOM_MANAGER_H