#include "Shop.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

Shop::Shop(sf::RenderWindow& window, KarmaSystem& karmaSystem)
    : window(window), karmaSystem(karmaSystem), openState(false), selectedNodeIndex(-1) {
    if (!font.loadFromFile("./assets/fonts/IMPACT.TTF")) {
        std::cerr << "Error cargando la fuente\n";
        exit(-1);
    }

    // Cargar textura del candado (debes tener este archivo)
    if (!lockTexture.loadFromFile("./resources/lock.png")) {
        std::cerr << "Error cargando la textura del candado\n";
        // No salir en este caso, usaremos una representación alternativa
    }

    // Configurar el fondo
    background.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    background.setFillColor(sf::Color(50, 50, 50, 200));

    // Configurar el título
    titleText.setFont(font);
    titleText.setString("Tienda de Karma");
    titleText.setCharacterSize(36);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(20, 20);

    // Configurar el texto de descripción
    descriptionText.setFont(font);
    descriptionText.setCharacterSize(20);
    descriptionText.setFillColor(sf::Color::White);
    descriptionText.setPosition(20, 80);

    // Configurar el texto de karma
    karmaText.setFont(font);
    karmaText.setCharacterSize(20);
    karmaText.setFillColor(sf::Color::White);
    karmaText.setPosition(20, 50);

    // Configurar títulos de columnas
    pecadoTitleText.setFont(font);
    pecadoTitleText.setString("Pecado");
    pecadoTitleText.setCharacterSize(28);
    pecadoTitleText.setFillColor(sf::Color(200, 50, 50));
    
    absolucionTitleText.setFont(font);
    absolucionTitleText.setString("Absolución");
    absolucionTitleText.setCharacterSize(28);
    absolucionTitleText.setFillColor(sf::Color(50, 150, 200));

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
}

void Shop::render() {
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
                
                // Actualizar texto de descripción
                std::string status = upgrade.isUnlocked ? " [DESBLOQUEADO]" : 
                                    upgrade.isBlocked ? " [BLOQUEADO]" : 
                                    " [DISPONIBLE]";
                                    
                descriptionText.setString(upgrade.name + status + 
                                         "\nCosto: " + std::to_string(upgrade.cost) + 
                                         "\nTipo: " + (upgrade.type == KarmaType::Pecado ? "Pecado" : "Absolución"));
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
                std::cout << "Mejora comprada: " << karmaSystem.getUpgrades()[selectedNodeIndex].name << std::endl;
                updateNodeVisuals();
                // Activar el efecto de la mejora
                //karmaSystem.getUpgrades()[selectedNodeIndex].action();
            } else {
                std::cout << "No puedes comprar esta mejora." << std::endl;
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

    // Escalar otros textos
    titleText.setCharacterSize(static_cast<unsigned int>(windowHeight * 0.06f)); // 6% de la altura
    karmaText.setCharacterSize(static_cast<unsigned int>(windowHeight * 0.033f)); // 3.3% de la altura
    descriptionText.setCharacterSize(static_cast<unsigned int>(windowHeight * 0.033f)); // 3.3% de la altura
    
    // Actualizar posiciones
    titleText.setPosition(windowWidth * 0.025f, windowHeight * 0.033f);
    karmaText.setPosition(windowWidth * 0.025f, windowHeight * 0.083f);
    descriptionText.setPosition(windowWidth * 0.025f, windowHeight * 0.133f);

    // Limpiar arreglos anteriores
    nodes.clear();
    nodeTexts.clear();
    lockIcons.clear();
    
    // Crear los 10 nodos según el patrón de la imagen
    for (size_t i = 0; i < upgrades.size(); ++i) {
        sf::RectangleShape node(sf::Vector2f(nodeSize, nodeSize));
        node.setOutlineColor(sf::Color::Black);
        node.setOutlineThickness(2.0f);
        
        float x, y;
        
        // Determinar la posición basada en el tipo y el nivel
        if (upgrades[i].type == KarmaType::Pecado) {
            x = pecadoX;
        } else {
            x = absolucionX;
        }
        
        // Distribución vertical basada en el índice/2 (para emparejar)
        y = startY + (i / 2) * ySpacing;
        
        node.setPosition(x - nodeSize/2.0f, y);
        nodes.push_back(node);
        
        // Texto del nodo
        sf::Text nodeText;
        nodeText.setFont(font);
        nodeText.setString(std::to_string(i/2 + 1) + (i % 2 == 0 ? " P" : " A"));
        nodeText.setCharacterSize(static_cast<unsigned int>(nodeSize * 0.4f)); // 40% del tamaño del nodo
        nodeText.setFillColor(sf::Color::Black);
        
        // Centrar el texto en el nodo
        float textWidth = nodeText.getGlobalBounds().width;
        float textHeight = nodeText.getGlobalBounds().height;
        nodeText.setPosition(x - textWidth/2.0f, y + nodeSize/2.0f - textHeight/2.0f);
        
        nodeTexts.push_back(nodeText);
        
        // Crear el icono de candado (inicialmente oculto)
        sf::Sprite lockIcon;
        if (lockTexture.getSize().x > 0) {
            lockIcon.setTexture(lockTexture);
            float lockScale = nodeSize/48.0f; // Ajustar según el tamaño de tu textura
            lockIcon.setScale(lockScale, lockScale);
            lockIcon.setPosition(x - nodeSize/4.0f, y + nodeSize/4.0f);
        }
        lockIcons.push_back(lockIcon);
    }
    
    updateNodeVisuals();
}

void Shop::createConnectionLines() {
    connectionLines.clear();
    
    // Solo crear las líneas si hay suficientes nodos
    if (nodes.size() < 10) return;
    
    // Crea las conexiones según el patrón de la imagen
    // Nodo 1 (Pecado) a Nodo 2 (Absolución)
    sf::VertexArray line1(sf::Lines, 2);
    line1[0].position = sf::Vector2f(nodes[0].getPosition().x + nodes[0].getSize().x, 
                                    nodes[0].getPosition().y + nodes[0].getSize().y/2.0f);
    line1[1].position = sf::Vector2f(nodes[1].getPosition().x, 
                                    nodes[1].getPosition().y + nodes[1].getSize().y/2.0f);
    line1[0].color = sf::Color::White;
    line1[1].color = sf::Color::White;
    connectionLines.push_back(line1);
    
    // Nodo 2 (Absolución) a Nodo 3 (Absolución)
    sf::VertexArray line2(sf::Lines, 2);
    line2[0].position = sf::Vector2f(nodes[1].getPosition().x + nodes[1].getSize().x/2.0f, 
                                    nodes[1].getPosition().y + nodes[1].getSize().y);
    line2[1].position = sf::Vector2f(nodes[3].getPosition().x + nodes[3].getSize().x/2.0f, 
                                    nodes[3].getPosition().y);
    line2[0].color = sf::Color::White;
    line2[1].color = sf::Color::White;
    connectionLines.push_back(line2);
    
    // Nodo 3 (Absolución) a Nodo 4 (Pecado)
    sf::VertexArray line3(sf::Lines, 2);
    line3[0].position = sf::Vector2f(nodes[3].getPosition().x, 
                                    nodes[3].getPosition().y + nodes[3].getSize().y/2.0f);
    line3[1].position = sf::Vector2f(nodes[4].getPosition().x + nodes[4].getSize().x, 
                                    nodes[4].getPosition().y + nodes[4].getSize().y/2.0f);
    line3[0].color = sf::Color::White;
    line3[1].color = sf::Color::White;
    connectionLines.push_back(line3);
    
    // Nodo 4 (Pecado) a Nodo 5 (líneas punteadas a ambos nodos del nivel 5)
    // Como SFML no tiene líneas punteadas integradas, usamos múltiples segmentos cortos
    float dotSpacing = window.getSize().x * 0.006f; // Escalar el espaciado de puntos
    
    // Línea a Nodo 5P
    sf::Vector2f start = sf::Vector2f(nodes[4].getPosition().x + nodes[4].getSize().x/2.0f, 
                                    nodes[4].getPosition().y + nodes[4].getSize().y);
    sf::Vector2f end = sf::Vector2f(nodes[8].getPosition().x + nodes[8].getSize().x/2.0f, 
                                   nodes[8].getPosition().y);
    
    float distance = std::sqrt(std::pow(end.x - start.x, 2) + std::pow(end.y - start.y, 2));
    int dotCount = static_cast<int>(distance / (dotSpacing * 2));
    
    sf::VertexArray dotLine1(sf::Lines, dotCount * 2);
    for (int i = 0; i < dotCount; ++i) {
        float t = static_cast<float>(i) / static_cast<float>(dotCount);
        sf::Vector2f pos = start + (end - start) * t;
        dotLine1[i*2].position = pos;
        dotLine1[i*2].color = sf::Color::White;
        dotLine1[i*2+1].position = sf::Vector2f(pos.x + dotSpacing, pos.y + (end.y - start.y) * dotSpacing / distance);
        dotLine1[i*2+1].color = sf::Color::White;
    }
    connectionLines.push_back(dotLine1);
    
    // Línea a Nodo 5A
    end = sf::Vector2f(nodes[9].getPosition().x + nodes[9].getSize().x/2.0f, 
                      nodes[9].getPosition().y);
    
    distance = std::sqrt(std::pow(end.x - start.x, 2) + std::pow(end.y - start.y, 2));
    dotCount = static_cast<int>(distance / (dotSpacing * 2));
    
    sf::VertexArray dotLine2(sf::Lines, dotCount * 2);
    for (int i = 0; i < dotCount; ++i) {
        float t = static_cast<float>(i) / static_cast<float>(dotCount);
        sf::Vector2f pos = start + (end - start) * t;
        dotLine2[i*2].position = pos;
        dotLine2[i*2].color = sf::Color::White;
        dotLine2[i*2+1].position = sf::Vector2f(pos.x + dotSpacing, pos.y + (end.y - start.y) * dotSpacing / distance);
        dotLine2[i*2+1].color = sf::Color::White;
    }
    connectionLines.push_back(dotLine2);
}

void Shop::updateNodeVisuals() {
    const std::vector<Upgrade>& upgrades = karmaSystem.getUpgrades();

    for (size_t i = 0; i < nodes.size(); ++i) {
        // Color y estado visual del nodo
        if (upgrades[i].isUnlocked) {
            // Nodo desbloqueado
            nodes[i].setFillColor(sf::Color::Black);
            nodeTexts[i].setFillColor(sf::Color::White);
            // Ocultar el candado
            lockIcons[i].setColor(sf::Color(255, 255, 255, 0)); // Transparente
        } else if (upgrades[i].isBlocked) {
            // Nodo bloqueado
            nodes[i].setFillColor(sf::Color::White);
            nodeTexts[i].setFillColor(sf::Color(150, 150, 150)); // Gris
            // Mostrar el candado
            lockIcons[i].setColor(sf::Color(255, 255, 255, 255)); // Visible
        } else {
            // Nodo disponible pero no comprado
            nodes[i].setFillColor(sf::Color::White);
            nodeTexts[i].setFillColor(sf::Color::Black);
            // Ocultar el candado
            lockIcons[i].setColor(sf::Color(255, 255, 255, 0)); // Transparente
        }
        
        // Resaltar el nodo seleccionado
        if (static_cast<int>(i) == selectedNodeIndex) {
            nodes[i].setOutlineColor(sf::Color::Yellow);
            nodes[i].setOutlineThickness(3.0f);
        } else {
            nodes[i].setOutlineColor(sf::Color::Black);
            nodes[i].setOutlineThickness(2.0f);
        }
    }
}