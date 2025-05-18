#include "SaveSystem.h"
#include "../Game.h"
#include <sstream>
#include <fstream>
#include <iostream>

// Definición de la constante estática
const std::string SaveSystem::DEFAULT_SAVE_FILE = "savefile.txt";

// Implementación del patrón singleton para obtener la instancia única
SaveSystem& SaveSystem::getInstance() {
    static SaveSystem instance;
    return instance;
}

// Constructor: inicializa la ruta del archivo de guardado con el valor por defecto
SaveSystem::SaveSystem() : saveFilePath(DEFAULT_SAVE_FILE) {
}

// Destructor
SaveSystem::~SaveSystem() {
}

// Establece la ruta del archivo de guardado
void SaveSystem::setSaveFilePath(const std::string& path) {
    saveFilePath = path;
}

// Obtiene la ruta actual del archivo de guardado
std::string SaveSystem::getSaveFilePath() const {
    return saveFilePath;
}

// Comprueba si el archivo de guardado existe
bool SaveSystem::saveFileExists() const {
    std::ifstream file(saveFilePath);
    return file.good();
}

// Busca una línea que comience con la clave especificada
std::string SaveSystem::findLine(const std::string& fileContent, const std::string& key) {
    std::string searchKey = key + "=";
    size_t pos = fileContent.find(searchKey);
    
    if (pos == std::string::npos) {
        return "";
    }
    
    size_t lineStart = pos;
    size_t lineEnd = fileContent.find('\n', pos);
    
    if (lineEnd == std::string::npos) {
        return fileContent.substr(lineStart);
    }
    
    return fileContent.substr(lineStart, lineEnd - lineStart);
}

// WRITERS

// Escribe un valor entero en el archivo
void SaveSystem::writeIntValue(std::ofstream& file, const std::string& key, int value) {
    file << key << "=" << value << std::endl;
}

// Escribe un valor booleano en el archivo
void SaveSystem::writeBoolValue(std::ofstream& file, const std::string& key, bool value) {
    file << key << "=" << (value ? "true" : "false") << std::endl;
}

// Escribe un valor string en el archivo
void SaveSystem::writeStringValue(std::ofstream& file, const std::string& key, const std::string& value) {
    file << key << "=" << value << std::endl;
}

// READERS

// Lee un valor entero del archivo
bool SaveSystem::readIntValue(const std::string& fileContent, const std::string& key, int& value) {
    std::string line = findLine(fileContent, key);
    if (line.empty()) {
        return false;
    }
    
    size_t equalsPos = line.find('=');
    if (equalsPos == std::string::npos || equalsPos + 1 >= line.length()) {
        return false;
    }
    
    std::string valueStr = line.substr(equalsPos + 1);
    try {
        value = std::stoi(valueStr);
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

// Lee un valor booleano del archivo
bool SaveSystem::readBoolValue(const std::string& fileContent, const std::string& key, bool& value) {
    std::string line = findLine(fileContent, key);
    if (line.empty()) {
        return false;
    }
    
    size_t equalsPos = line.find('=');
    if (equalsPos == std::string::npos || equalsPos + 1 >= line.length()) {
        return false;
    }
    
    std::string valueStr = line.substr(equalsPos + 1);
    
    // Convierte a minúsculas para comparar
    std::transform(valueStr.begin(), valueStr.end(), valueStr.begin(), 
                   [](unsigned char c){ return std::tolower(c); });
    
    if (valueStr == "true" || valueStr == "1") {
        value = true;
        return true;
    } else if (valueStr == "false" || valueStr == "0") {
        value = false;
        return true;
    }
    
    return false;
}

// Lee un valor string del archivo
bool SaveSystem::readStringValue(const std::string& fileContent, const std::string& key, std::string& value) {
    std::string line = findLine(fileContent, key);
    if (line.empty()) {
        return false;
    }
    
    size_t equalsPos = line.find('=');
    if (equalsPos == std::string::npos || equalsPos + 1 >= line.length()) {
        return false;
    }
    
    value = line.substr(equalsPos + 1);
    return true;
}

// Guarda el estado actual del juego en el archivo
bool SaveSystem::saveGameState(KarmaSystem& karmaSystem) {
    try {
        // Abre el archivo para escribir (sobreescribe si existe)
        std::ofstream saveFile(saveFilePath);
        if (!saveFile.is_open()) {
            std::cerr << "Error: No se pudo abrir el archivo para guardar: " << saveFilePath << std::endl;
            return false;
        }

        // Obtiene el karma del jugador
        int karma = Character::getInstance()->getKarma();
        
        // Obtiene el estado de los mundos completados
        bool world1Completed = LobbyState::isWorld1Completed();
        bool world2Completed = LobbyState::isWorld2Completed();
        
        // Obtiene las mejoras permanentes
        std::vector<int> upgrades = karmaSystem.getPurchasedUpgrades();
        
        // Escribe el karma
        writeIntValue(saveFile, "karma", karma);
        
        // Escribe los estados de los mundos
        writeBoolValue(saveFile, "world1_completed", world1Completed);
        writeBoolValue(saveFile, "world2_completed", world2Completed);
        
        // Escribe las mejoras (IDs separados por comas)
        std::stringstream upgradesStr;
        for (size_t i = 0; i < upgrades.size(); ++i) {
            upgradesStr << upgrades[i];
            if (i < upgrades.size() - 1) {
                upgradesStr << ",";
            }
        }
        writeStringValue(saveFile, "upgrades", upgradesStr.str());
        
        saveFile.close();
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error al guardar el estado del juego: " << e.what() << std::endl;
        return false;
    }
}

// Carga el estado del juego desde el archivo
bool SaveSystem::loadGameState(KarmaSystem& karmaSystem) {
    try {
        // Verifica si el archivo existe
        if (!saveFileExists()) {
            std::cerr << "Archivo de guardado no encontrado: " << saveFilePath << std::endl;
            return false;
        }
        
        // Lee todo el contenido del archivo
        std::ifstream saveFile(saveFilePath);
        std::stringstream buffer;
        buffer << saveFile.rdbuf();
        std::string fileContent = buffer.str();
        saveFile.close();
        
        // Variables para almacenar los valores leídos
        int karma = 0;
        bool world1Completed = false;
        bool world2Completed = false;
        std::string upgradesStr;
        
        // Lee los valores del archivo
        bool karmaRead = readIntValue(fileContent, "karma", karma);
        bool world1Read = readBoolValue(fileContent, "world1_completed", world1Completed);
        bool world2Read = readBoolValue(fileContent, "world2_completed", world2Completed);
        bool upgradesRead = readStringValue(fileContent, "upgrades", upgradesStr);
        
        // Verifica si se pudieron leer todos los valores
        if (!karmaRead || !world1Read || !world2Read || !upgradesRead) {
            std::cerr << "Archivo de guardado inválido o corrupto: " << saveFilePath << std::endl;
            return false;
        }
        
        // Establece el karma
        Character::getInstance()->addKarma(karma);
        
        // Establece el estado de los mundos
        LobbyState::setWorld1completed();
        LobbyState::setWorld2completed();
        
        // Procesa y establece las mejoras
        std::vector<Upgrade> upgrades;
        std::stringstream ss(upgradesStr);
        std::string item;
        
        // Divide la cadena por comas y convierte cada parte a un ID de mejora
        while (std::getline(ss, item, ',')) {
            if (!item.empty()) {
                int id = std::stoi(item);
                karmaSystem.setUpgrade(id);
            }
        }
        
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error al cargar el estado del juego: " << e.what() << std::endl;
        return false;
    }
}