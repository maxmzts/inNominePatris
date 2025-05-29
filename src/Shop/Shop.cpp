#include "Shop.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <SaveSystem.h>

Shop::Shop(sf::RenderWindow& window, KarmaSystem& karmaSystem)
    : window(window), karmaSystem(karmaSystem), openState(false), selectedNodeIndex(-1) {
    if (!font.loadFromFile("./src/assets/fonts/IMPACT.TTF")) {
        std::cerr << "Error cargando la fuente\n";
        exit(-1);
    }

    // Cargar textura del candado (debes tener este archivo)
    if (!lockTexture.loadFromFile("./src/resources/lock.png")) {
        std::cerr << "Error cargando la textura del candado\n";
        // No salir en este caso, usaremos una representación alternativa
    }

    // Configurar el fondo con un color más oscuro y temático
    background.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    background.setFillColor(sf::Color(10, 5, 15, 245)); // Negro azulado muy oscuro con alta opacidad

    // Configurar el título con estilo más dramático
    titleText.setFont(font);
    titleText.setString("Tienda de Karma");
    titleText.setCharacterSize(36);
    titleText.setFillColor(sf::Color(220, 0, 0)); // Rojo más intenso
    titleText.setOutlineColor(sf::Color(50, 0, 0)); // Contorno rojo oscuro
    titleText.setOutlineThickness(1.5f);
    titleText.setPosition(20, 20);

    // Configurar el texto de descripción
    descriptionText.setFont(font);
    descriptionText.setCharacterSize(20);
    descriptionText.setFillColor(sf::Color(180, 180, 180)); // Gris claro
    descriptionText.setPosition(20, 80);

    // Configurar el texto de karma con efecto más dramático
    karmaText.setFont(font);
    karmaText.setCharacterSize(20);
    karmaText.setFillColor(sf::Color(240, 200, 0)); // Dorado para el karma
    karmaText.setPosition(20, 50);

    // Configurar títulos de columnas con colores más contrastantes
    pecadoTitleText.setFont(font);
    pecadoTitleText.setString("Pecado");
    pecadoTitleText.setCharacterSize(28);
    pecadoTitleText.setFillColor(sf::Color(200, 0, 0)); // Rojo intenso para "Pecado"
    pecadoTitleText.setOutlineColor(sf::Color(100, 0, 0));
    pecadoTitleText.setOutlineThickness(1.0f);
    
    absolucionTitleText.setFont(font);
    absolucionTitleText.setString("Absolucion");
    absolucionTitleText.setCharacterSize(28);
    absolucionTitleText.setFillColor(sf::Color(100, 180, 255)); // Azul celeste para "Absolución"
    absolucionTitleText.setOutlineColor(sf::Color(0, 50, 120));
    absolucionTitleText.setOutlineThickness(1.0f);

    initializeTree();
    createConnectionLines();
}

void Shop::open() {
    openState = true;
    
    // Guardar la vista actual para restaurarla al cerrar
    previousView = window.getView();
    
    // Establecer una vista fija centrada en la pantalla
    sf::View shopView(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
    window.setView(shopView);
    
    // Actualizar el tamaño del fondo con las dimensiones actuales de la ventana
    background.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    
    // Recalcular el árbol y las líneas de conexión para que se ajusten al nuevo tamaño
    initializeTree();
    createConnectionLines();
}

void Shop::close() {
    openState = false;
    
    // Restaurar la vista anterior
    window.setView(previousView);
}

bool Shop::isOpen() const {
    return openState;
}

void Shop::update(int playerKarma) {
    if (!openState) return;

    // Verificar si el tamaño de la ventana ha cambiado
    sf::Vector2u currentSize = window.getSize();
    if (lastWindowSize != currentSize) {
        // Actualizar el tamaño del fondo
        background.setSize(sf::Vector2f(currentSize.x, currentSize.y));
        
        // Recalcular el árbol y las líneas de conexión
        initializeTree();
        createConnectionLines();
        
        // Guardar el nuevo tamaño de ventana
        lastWindowSize = currentSize;
    }

    // Actualizar el texto del karma actual
    karmaText.setString("Karma actual: " + std::to_string(playerKarma));

    // Actualizar los nodos visuales
    updateNodeVisuals();
    
    // Actualizar las líneas de conexión para reflejar los cambios en los nodos
    createConnectionLines();
}

void Shop::render(sf::RenderWindow& window) {
    if (!openState) return;

    // Guardamos la vista actual temporalmente
    sf::View currentView = window.getView();
    
    // Establecemos una vista fija para la tienda
    sf::View shopView(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
    window.setView(shopView);

    // Dibujar fondo
    window.draw(background);

    // Dibujar líneas de conexión
    for (auto& line : connectionLines) {
        window.draw(line);
    }

    // Dibujar nodos y textos
    for (const auto& node : nodes) {
        window.draw(node);
    }

    for (const auto& text : nodeTexts) {
        window.draw(text);
    }

    // Dibujar iconos de candado para nodos bloqueados
    for (const auto& lock : lockIcons) {
        window.draw(lock);
    }

    // Dibujar títulos y textos informativos
    window.draw(titleText);
    window.draw(karmaText);
    window.draw(descriptionText);
    window.draw(pecadoTitleText);
    window.draw(absolucionTitleText);
    
    // Restauramos la vista original
    window.setView(currentView);
}

void Shop::handleInput(const sf::Event& event) {
    if (!openState) return;

    if (event.type == sf::Event::MouseMoved) {
        // Convertir las coordenadas del ratón al sistema de coordenadas de la tienda
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        
        // Usar la vista actual de la tienda para mapear las coordenadas
        sf::View shopView(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos, shopView);
        
        selectedNodeIndex = -1;

        for (size_t i = 0; i < nodes.size(); ++i) {
            if (nodes[i].getGlobalBounds().contains(worldPos)) {
                selectedNodeIndex = i;
                const Upgrade& upgrade = karmaSystem.getUpgrades()[i];
                
                // Actualizar texto de descripción con formato mejorado
                std::string estado;
                if (upgrade.isUnlocked) {
                    estado = "[DESBLOQUEADO]";
                } else if (upgrade.isBlocked) {
                    estado = "[BLOQUEADO]";
                } else {
                    estado = "[DISPONIBLE]";
                }
                
                std::string tipoTexto = upgrade.type == KarmaType::Pecado ? "Pecado" : "Absolucion";
                sf::Color tipoColor = upgrade.type == KarmaType::Pecado ? sf::Color(220, 0, 0) : sf::Color(100, 180, 255);
                
                descriptionText.setString(upgrade.name + " " + estado + 
                                         "\nCosto: " + std::to_string(upgrade.cost) + 
                                         "\nTipo: " + tipoTexto);
                
                // Color basado en el tipo
                descriptionText.setFillColor(tipoColor);
                break;
            }
        }
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        // Convertir coordenadas del ratón para la interacción con los nodos
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        
        // Usar la vista actual de la tienda para mapear las coordenadas
        sf::View shopView(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos, shopView);
        
        selectedNodeIndex = -1;
        
        for (size_t i = 0; i < nodes.size(); ++i) {
            if (nodes[i].getGlobalBounds().contains(worldPos)) {
                selectedNodeIndex = i;
                break;
            }
        }
        
        if (selectedNodeIndex != -1) {
            if (karmaSystem.purchaseUpgrade(selectedNodeIndex)) {
                 // std::cout << "Mejora comprada: " << karmaSystem.getUpgrades()[selectedNodeIndex].name << std::endl;
                updateNodeVisuals();
                // Activar el efecto de la mejora
                createConnectionLines();
                //karmaSystem.getUpgrades()[selectedNodeIndex].action();
                SaveSystem::getInstance().saveGameState(karmaSystem);
            } else {
                 // std::cout << "No puedes comprar esta mejora." << std::endl;
            }
        }
    }
    
    // Detectar cambios de tamaño de ventana
    if (event.type == sf::Event::Resized) {
        // Actualizar el tamaño del fondo
        background.setSize(sf::Vector2f(event.size.width, event.size.height));
        
        // Recalcular el árbol y las líneas de conexión
        initializeTree();
        createConnectionLines();
        
        // Actualizar la vista
        sf::View shopView(sf::FloatRect(0, 0, event.size.width, event.size.height));
        window.setView(shopView);
        
        // Guardar el nuevo tamaño de ventana
        lastWindowSize = sf::Vector2u(event.size.width, event.size.height);
    }
}

void Shop::initializeTree() {
    const std::vector<Upgrade>& upgrades = karmaSystem.getUpgrades();
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;
    
    // Calcular el tamaño de los nodos y espaciado basado en el tamaño de la ventana
    float nodeSize = windowHeight * 0.07f; // 7% de la altura de la ventana
    float ySpacing = windowHeight * 0.12f; // 12% de la altura de la ventana
    
    // Posición central del árbol
    float centerX = windowWidth / 2.0f;
    
    // Posiciones para las columnas (escalar basado en el ancho de ventana)
    float columnSpacing = windowWidth * 0.15f; // 15% del ancho de la ventana
    float pecadoX = centerX - columnSpacing;
    float absolucionX = centerX + columnSpacing;
    
    // Posición inicial en Y (más arriba para dar más espacio)
    float startY = windowHeight * 0.15f; // 15% de la altura de la ventana
    
    // Configurar títulos de columnas
    pecadoTitleText.setCharacterSize(static_cast<unsigned int>(windowHeight * 0.047f)); // 4.7% de la altura
    absolucionTitleText.setCharacterSize(static_cast<unsigned int>(windowHeight * 0.047f)); // 4.7% de la altura
    
    pecadoTitleText.setPosition(pecadoX - pecadoTitleText.getGlobalBounds().width/2.0f, startY - windowHeight * 0.1f);
    absolucionTitleText.setPosition(absolucionX - absolucionTitleText.getGlobalBounds().width/2.0f, startY - windowHeight * 0.1f);

    // Limpiar arreglos anteriores
    nodes.clear();
    nodeTexts.clear();
    lockIcons.clear();
    
    // Crear los nodos con formas básicas para asegurar que aparezcan
    for (size_t i = 0; i < upgrades.size(); ++i) {
        float x, y;
        
        // Determinar la posición basada en el tipo
        if (upgrades[i].type == KarmaType::Pecado) {
            x = pecadoX;
        } else {
            x = absolucionX;
        }
        
        // Distribución vertical basada en el índice/2 (para emparejar)
        y = startY + (i / 2) * ySpacing;
        
        // Crear nodo como un rectángulo simple
        sf::RectangleShape node(sf::Vector2f(nodeSize, nodeSize));
        
        // Aplicar color base según el tipo
        if (upgrades[i].type == KarmaType::Pecado) {
            node.setFillColor(sf::Color(120, 0, 0)); // Rojo oscuro para pecado
        } else {
            node.setFillColor(sf::Color(0, 60, 120)); // Azul oscuro para absolución
        }
        
        node.setOutlineColor(sf::Color::White);
        node.setOutlineThickness(2.0f);
        
        // Posicionar el nodo (centrado en x)
        node.setPosition(x - nodeSize/2.0f, y);
        
        nodes.push_back(node);
        
        // Texto del nodo con alto contraste
        sf::Text nodeText;
        nodeText.setFont(font);
        nodeText.setString(std::to_string(i/2 + 1) + (i % 2 == 0 ? " P" : " A"));
        nodeText.setCharacterSize(static_cast<unsigned int>(nodeSize * 0.4f)); // 40% del tamaño del nodo
        nodeText.setFillColor(sf::Color::White); // Texto blanco para mejor contraste
        
        // Centrar el texto en el nodo
        float textWidth = nodeText.getGlobalBounds().width;
        float textHeight = nodeText.getGlobalBounds().height;
        nodeText.setPosition(x - textWidth/2.0f, y + nodeSize/2.0f - textHeight/2.0f);
        
        nodeTexts.push_back(nodeText);
        
        // Crear el icono de candado
        sf::Sprite lockIcon;
        if (lockTexture.getSize().x > 0) {
            lockIcon.setTexture(lockTexture);
            float lockScale = nodeSize/48.0f; // Ajustar según el tamaño de tu textura
            lockIcon.setPosition(x - nodeSize/4.0f, y + nodeSize/4.0f);
        }
        lockIcons.push_back(lockIcon);
    }
    
    updateNodeVisuals();
    createConnectionLines(); // Asegurar que las líneas de conexión se actualicen
}

void Shop::createConnectionLines() {
    connectionLines.clear();
    
    // Solo crear las líneas si hay suficientes nodos
    if (nodes.size() < 2) return;
    
    const std::vector<Upgrade>& upgrades = karmaSystem.getUpgrades();
    
    // Recorrer todos los pares de nodos para verificar si debe existir una conexión
    for (size_t i = 0; i < nodes.size(); ++i) {
        for (size_t j = i + 1; j < nodes.size(); ++j) {
            // Verificar si la conexión debe existir basada en el estado de desbloqueo
            if (shouldConnectNodes(i, j)) {
                sf::VertexArray line(sf::Lines, 2);
                
                // Si es una conexión de "requisito" (línea punteada)
                bool isDottedLine = (i == 4 && (j == 8 || j == 9));
                
                if (!isDottedLine) {
                    // Línea normal
                    line[0].position = sf::Vector2f(nodes[i].getPosition().x + nodes[i].getSize().x/2.0f, 
                                                  nodes[i].getPosition().y + nodes[i].getSize().y/2.0f);
                    line[1].position = sf::Vector2f(nodes[j].getPosition().x + nodes[j].getSize().x/2.0f, 
                                                  nodes[j].getPosition().y + nodes[j].getSize().y/2.0f);
                    
                    // Color de la línea basado en los tipos de nodos
                    if (upgrades[i].type == upgrades[j].type) {
                        // Mismo tipo - usar el color de ese tipo
                        if (upgrades[i].type == KarmaType::Pecado) {
                            line[0].color = sf::Color(180, 0, 0);
                            line[1].color = sf::Color(180, 0, 0);
                        } else {
                            line[0].color = sf::Color(80, 160, 235);
                            line[1].color = sf::Color(80, 160, 235);
                        }
                    } else {
                        // Tipos diferentes - gradiente entre los dos colores
                        if (upgrades[i].type == KarmaType::Pecado) {
                            line[0].color = sf::Color(180, 0, 0);
                            line[1].color = sf::Color(80, 160, 235);
                        } else {
                            line[0].color = sf::Color(80, 160, 235);
                            line[1].color = sf::Color(180, 0, 0);
                        }
                    }
                    
                    connectionLines.push_back(line);
                } else {
                    // Línea punteada para conexiones especiales (requisitos)
                    float dotSpacing = window.getSize().x * 0.006f;
                    
                    sf::Vector2f start = sf::Vector2f(nodes[i].getPosition().x + nodes[i].getSize().x/2.0f, 
                                                    nodes[i].getPosition().y + nodes[i].getSize().y);
                    sf::Vector2f end = sf::Vector2f(nodes[j].getPosition().x + nodes[j].getSize().x/2.0f, 
                                                   nodes[j].getPosition().y);
                    
                    float distance = std::sqrt(std::pow(end.x - start.x, 2) + std::pow(end.y - start.y, 2));
                    int dotCount = static_cast<int>(distance / (dotSpacing * 2));
                    
                    sf::VertexArray dotLine(sf::Lines, dotCount * 2);
                    sf::Color lineColor;
                    
                    // Color basado en el tipo del nodo destino
                    if (upgrades[j].type == KarmaType::Pecado) {
                        lineColor = sf::Color(150, 0, 0, 200);
                    } else {
                        lineColor = sf::Color(80, 160, 235, 200);
                    }
                    
                    for (int k = 0; k < dotCount; ++k) {
                        float t = static_cast<float>(k) / static_cast<float>(dotCount);
                        sf::Vector2f pos = start + (end - start) * t;
                        dotLine[k*2].position = pos;
                        dotLine[k*2].color = lineColor;
                        dotLine[k*2+1].position = sf::Vector2f(pos.x + dotSpacing, pos.y + (end.y - start.y) * dotSpacing / distance);
                        dotLine[k*2+1].color = lineColor;
                    }
                    connectionLines.push_back(dotLine);
                }
            }
        }
    }
}

bool Shop::shouldConnectNodes(size_t index1, size_t index2) {
    const std::vector<Upgrade>& upgrades = karmaSystem.getUpgrades();
    
    // No mostrar conexiones entre nodos bloqueados
    if (upgrades[index1].isBlocked || upgrades[index2].isBlocked) {
        return false;
    }
    
    // Solo conectar nodos si al menos uno está desbloqueado
    bool anyUnlocked = upgrades[index1].isUnlocked || upgrades[index2].isUnlocked;
    if (!anyUnlocked) {
        return false;
    }
    
    // Conexiones específicas según la estructura del árbol
    
    // Nivel 1: Conexión entre nodos 0 y 1 (siempre que no estén bloqueados)
    if ((index1 == 0 && index2 == 1) || (index1 == 1 && index2 == 0)) {
        return true;
    }
    
    // Nivel 2: Conexión desde nodo 1 al nodo 3
    if ((index1 == 1 && index2 == 3) || (index1 == 3 && index2 == 1)) {
        return upgrades[1].isUnlocked;
    }
    
    // Nivel 2: Conexión desde nodo 3 al nodo 4
    if ((index1 == 3 && index2 == 4) || (index1 == 4 && index2 == 3)) {
        return upgrades[3].isUnlocked;
    }
    
    // Nivel 3: Conexión desde nodo 4 a los nodos 8 y 9 (líneas punteadas)
    if ((index1 == 4 && index2 == 8) || (index1 == 8 && index2 == 4)) {
        return upgrades[4].isUnlocked;
    }
    
    if ((index1 == 4 && index2 == 9) || (index1 == 9 && index2 == 4)) {
        return upgrades[4].isUnlocked;
    }
    
    // Si no es ninguna de las conexiones predefinidas, no conectar
    return false;
}

// En el método updateNodeVisuals(), modificar colores de nodos según su estado
void Shop::updateNodeVisuals() {
    const std::vector<Upgrade>& upgrades = karmaSystem.getUpgrades();

    for (size_t i = 0; i < nodes.size(); ++i) {
        // Colores base según el tipo (pecado o absolución)
        sf::Color baseColor;
        if (upgrades[i].type == KarmaType::Pecado) {
            baseColor = sf::Color(120, 0, 0); // Rojo oscuro para pecado
        } else {
            baseColor = sf::Color(0, 60, 120); // Azul oscuro para absolución
        }

        if (upgrades[i].isUnlocked) {
            // Nodo desbloqueado - versión más brillante del color base
            if (upgrades[i].type == KarmaType::Pecado) {
                nodes[i].setFillColor(sf::Color(180, 0, 0)); // Rojo más brillante
            } else {
                nodes[i].setFillColor(sf::Color(0, 100, 180)); // Azul más brillante
            }
            nodeTexts[i].setFillColor(sf::Color::White);
            lockIcons[i].setColor(sf::Color(255, 255, 255, 0)); // Ocultar candado
        } else if (upgrades[i].isBlocked) {
            // Nodo bloqueado - versión más oscura y grisácea
            if (upgrades[i].type == KarmaType::Pecado) {
                nodes[i].setFillColor(sf::Color(60, 0, 0)); // Rojo muy oscuro
            } else {
                nodes[i].setFillColor(sf::Color(0, 30, 60)); // Azul muy oscuro
            }
            nodeTexts[i].setFillColor(sf::Color(100, 100, 100));
            lockIcons[i].setColor(sf::Color(200, 0, 0, 255)); // Rojo para el candado
        } else {
            // Nodo disponible - color base
            nodes[i].setFillColor(baseColor);
            nodeTexts[i].setFillColor(sf::Color::White);
            lockIcons[i].setColor(sf::Color(255, 255, 255, 0)); // Ocultar candado
        }

        // Resaltar el nodo seleccionado con un brillo más temático
        if (static_cast<int>(i) == selectedNodeIndex) {
            if (upgrades[i].type == KarmaType::Pecado) {
                nodes[i].setOutlineColor(sf::Color(255, 120, 0)); // Naranja ardiente
            } else {
                nodes[i].setOutlineColor(sf::Color(120, 220, 255)); // Azul celestial
            }
            nodes[i].setOutlineThickness(3.5f);
        } else {
            nodes[i].setOutlineColor(sf::Color(30, 30, 30)); // Casi negro
            nodes[i].setOutlineThickness(2.0f);
        }
    }
}