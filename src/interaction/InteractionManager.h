#pragma once

#include <map>
#include <vector>
#include <set>
#include <memory>
#include "Interaction.h"
#include "DoorInteraction.h"
#include "ButtonInteraction.h"

class InteractionManager {
private:
    // Singleton
    static InteractionManager* instance;
    
    // Mapa que registra qué botones (por ID) han sido presionados
    std::map<int, bool> m_activatedButtons;
    
    // Mapa que asocia cada puerta con los botones requeridos para abrirla
    // La clave es el ID de la puerta, el valor es un conjunto de IDs de botones
    std::map<int, std::set<int>> m_doorRequirements;
    
    // Constructor privado (singleton)
    InteractionManager();
    
public:
    // Obtener instancia única
    static InteractionManager* getInstance();
    
    // Registrar un botón como presionado
    void registerButtonPress(int buttonId);
    
    // Verificar si todos los botones requeridos para una puerta están presionados
    bool checkDoorRequirements(int doorId);
    
    // Añadir un requisito para una puerta (asociar un botón a una puerta)
    void addDoorRequirement(int doorId, int buttonId);
    
    // Configurar los requisitos para una puerta desde una lista de botones
    void setDoorRequirements(int doorId, const std::vector<int>& buttonIds);
    
    // Reiniciar todos los botones a no presionados
    void resetButtons();
    
    // Destructor
    ~InteractionManager();
};