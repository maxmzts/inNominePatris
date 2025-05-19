#pragma once

#include <string>
#include <vector>
#include <fstream>
#include "Character.h"
#include "LobbyState.h"
#include "KarmaSystem.h"

class SaveSystem {
public:
    // Obtiene la instancia única del sistema de guardado
    static SaveSystem& getInstance();

    // Guarda el estado actual del juego en el archivo
    bool saveGameState(KarmaSystem& karmaSystem);
    
    // Carga el estado del juego desde el archivo
    bool loadGameState(KarmaSystem& karmaSystem);
    
    // Establece la ruta del archivo de guardado
    void setSaveFilePath(const std::string& path);
    
    // Obtiene la ruta actual del archivo de guardado
    std::string getSaveFilePath() const;

private:
    // Constructor privado (patrón singleton)
    SaveSystem();
    
    // Destructor privado
    ~SaveSystem();
    
    // Constructor de copia y operador de asignación eliminados
    SaveSystem(const SaveSystem&) = delete;
    SaveSystem& operator=(const SaveSystem&) = delete;
    
    // Comprueba si el archivo de guardado existe
    bool saveFileExists() const;
    
    // Funciones para escribir valores específicos en el archivo
    void writeIntValue(std::ofstream& file, const std::string& key, int value);
    void writeBoolValue(std::ofstream& file, const std::string& key, bool value);
    void writeStringValue(std::ofstream& file, const std::string& key, const std::string& value);
    
    // Funciones para leer valores específicos del archivo
    bool readIntValue(const std::string& fileContent, const std::string& key, int& value);
    bool readBoolValue(const std::string& fileContent, const std::string& key, bool& value);
    bool readStringValue(const std::string& fileContent, const std::string& key, std::string& value);

    // Función auxiliar para encontrar la línea con una clave
    std::string findLine(const std::string& fileContent, const std::string& key);
    
    // Ruta del archivo de guardado
    std::string saveFilePath;
    
    // Nombre del archivo de guardado por defecto
    static const std::string DEFAULT_SAVE_FILE;
};