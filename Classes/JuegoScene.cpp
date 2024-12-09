#include "JuegoScene.h"
#include "BunkerScene.h"
#include "InventarioGlobal.h"
#include "ui/CocosGUI.h"
#include "audio/include/AudioEngine.h"

using namespace cocos2d;

USING_NS_CC;

Scene* JuegoScene::createScene() {
    return JuegoScene::create();
}

bool JuegoScene::init() {
    if (!Scene::init()) {
        return false;
    }

    // Inicializar el mapa antes de cualquier acceso
    inicializarMapa();

    inventario = InventarioMapa(10);
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    // Tamaño de cada celda
    const float anchoCelda = visibleSize.width / mapa[0].size();
    const float altoCelda = visibleSize.height / mapa.size();

    // Encontrar una celda inicial válida (pasillo)
    int startX = -1, startY = -1;
    for (int y = 0; y < mapa.size(); ++y) {
        for (int x = 0; x < mapa[0].size(); ++x) {
            if (mapa[y][x] == 1) { // Si es un pasillo
                startX = x;
                startY = y;
                break;
            }
        }
        if (startX != -1 && startY != -1) break; // Salir del bucle una vez encontrada la celda válida
    }

    // Validar que se haya encontrado una posición inicial válida
    if (startX == -1 || startY == -1) {
        log("Error: No se encontró una celda inicial válida para el personaje.");
        return false;
    }

    // Crear el personaje
    personaje = PersonajeMapa::create();
    personaje->setScale(anchoCelda / personaje->getContentSize().width,
        altoCelda / personaje->getContentSize().height);

    // Centrar el personaje en la celda inicial
    personaje->setPosition(Vec2(startX * anchoCelda + anchoCelda / 2,
        visibleSize.height - (startY * altoCelda + altoCelda / 2)));
    this->addChild(personaje);

    inicializarObjetos();
    manejarTeclas();
    iniciarTemporizador();

    return true;
}


void JuegoScene::inicializarMapa() {

    // Mapa de 20x13 con diferentes tipos de celdas
    mapa = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 2, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 2, 2, 1, 0},
        {0, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 5, 1, 0, 2, 1, 1, 0},
        {0, 0, 0, 0, 1, 1, 0, 3, 3, 3, 0, 1, 0, 5, 1, 0, 0, 1, 0, 0},
        {0, 4, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 4, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 4, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 4, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0},
        {0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0},
        {0, 1, 1, 1, 1, 0, 5, 5, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 3, 0},
        {0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 3, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 3, 3, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    // Tamaño de la pantalla
    const float screenWidth = 1280.0f;
    const float screenHeight = 720.0f;

    // Calcular el tamaño dinámico de cada celda
    const int filas = mapa.size();
    const int columnas = mapa[0].size();
    const float anchoCelda = screenWidth / columnas;
    const float altoCelda = screenHeight / filas;

    // Calcular el offset para centrar el mapa en la pantalla
    const float offsetX = (screenWidth - columnas * anchoCelda) / 2;
    const float offsetY = (screenHeight - filas * altoCelda) / 2;

    // Dibujar las celdas del mapa
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            std::string texture;
            switch (mapa[i][j]) {
            case 0: texture = "pared.png"; break;
            case 1: texture = "pasillo.jpg"; break;
            case 2: texture = "recamara.jpg"; break;
            case 3: texture = "cocina.png"; break;
            case 4: texture = "comedor.jpg"; break;
            case 5: texture = "biblioteca.png"; break;
            case 6: texture = "oculto.png"; break;
            default:
                texture = "";  // Celda inválida
                break;
            }

            if (!texture.empty()) {
                auto celda = Sprite::create(texture);
                if (celda) {
                    // Escalar la celda al tamaño calculado
                    celda->setScale(anchoCelda / celda->getContentSize().width,
                        altoCelda / celda->getContentSize().height);

                    // Posicionar la celda en el mapa
                    celda->setPosition(Vec2(j * anchoCelda + anchoCelda / 2 + offsetX,
                        screenHeight - (i * altoCelda + altoCelda / 2 + offsetY)));
                    this->addChild(celda);
                }
            }
        }
    }
}

#include <cstdlib> // Para rand() y srand()
#include <ctime>   // Para time()

void JuegoScene::inicializarObjetos() {
    srand(static_cast<unsigned>(time(nullptr)));

    std::vector<std::pair<std::string, int>> objetos = {
        {"comida.png", 7},
        {"agua.png", 7},
        {"medicina.png", 3},
        {"herramienta.png", 2},
        {"mascara.png", 1},
    };

    const float anchoCelda = visibleSize.width / mapa[0].size();
    const float altoCelda = visibleSize.height / mapa.size();

    std::set<std::pair<int, int>> celdasOcupadas;

    for (auto& objeto : objetos) {
        for (int i = 0; i < objeto.second; ++i) {
            int x, y;
            do {
                x = rand() % mapa[0].size();
                y = rand() % mapa.size();
                if (y < 0 || y >= mapa.size() || x < 0 || x >= mapa[0].size()) {
                    log("Error: Índice fuera de rango en inicializarObjetos");
                    continue;
                }
            } while (mapa[y][x] != 1 || celdasOcupadas.count({ x, y }) > 0);

            celdasOcupadas.insert({ x, y });

            auto sprite = Sprite::create(objeto.first);
            sprite->setScale(anchoCelda / sprite->getContentSize().width,
                altoCelda / sprite->getContentSize().height);
            sprite->setPosition(Vec2(x * anchoCelda + anchoCelda / 2,
                visibleSize.height - (y * altoCelda + altoCelda / 2)));
            sprite->setName(objeto.first); // Asignar nombre del objeto
            this->addChild(sprite);
            objetosSprites.push_back(sprite);
        }
    }
}



void JuegoScene::manejarTeclas() {
    const float anchoCelda = visibleSize.width / mapa[0].size();
    const float altoCelda = visibleSize.height / mapa.size();

    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
        Vec2 nuevaPosicion = personaje->getPosition();
        Vec2 posicionActual = nuevaPosicion;

        switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_W: 
            nuevaPosicion.y += altoCelda; 
            personaje->setRotation(0);  // Orientar hacia arriba
            break;
        case EventKeyboard::KeyCode::KEY_S: 
            nuevaPosicion.y -= altoCelda; 
            personaje->setRotation(180);  // Orientar hacia abajo
            break;
        case EventKeyboard::KeyCode::KEY_A: 
            nuevaPosicion.x -= anchoCelda; 
            personaje->setRotation(270);  // Orientar hacia la izquierda
            break;
        case EventKeyboard::KeyCode::KEY_D: 
            nuevaPosicion.x += anchoCelda; 
            personaje->setRotation(90);  // Orientar hacia la derecha
            break;
        default: 
            break;
        }

        int x = nuevaPosicion.x / anchoCelda;
        int y = (visibleSize.height - nuevaPosicion.y) / altoCelda;

        // Validar índices
        if (y < 0 || y >= mapa.size() || x < 0 || x >= mapa[0].size()) {
            log("Movimiento inválido hacia [%d, %d]: fuera de rango", x, y);
            return;
        }

        if (mapa[y][x] != 0) {
            personaje->setPosition(nuevaPosicion);
            verificarColisionObjetos();
        }
        else {
            personaje->setPosition(posicionActual); // Volver a la posición previa
            log("Movimiento inválido hacia [%d, %d]", x, y);
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}


void JuegoScene::verificarColisionObjetos() {
    const float anchoCelda = visibleSize.width / mapa[0].size();
    const float altoCelda = visibleSize.height / mapa.size();

    // Mapa de traducción para nombres
    std::unordered_map<std::string, std::string> nombresRecursos = {
        {"comida.png", "comida"},
        {"agua.png", "agua"},
        {"medicina.png", "medicina"},
        {"herramienta.png", "herramienta"},
        {"mascara.png", "mascara"}
    };

    for (int i = 0; i < objetosSprites.size(); ++i) {
        Vec2 posicionObjeto = objetosSprites[i]->getPosition();
        Vec2 posicionPersonaje = personaje->getPosition();

        float distanciaX = fabs(posicionObjeto.x - posicionPersonaje.x);
        float distanciaY = fabs(posicionObjeto.y - posicionPersonaje.y);

        if (distanciaX < anchoCelda / 2 && distanciaY < altoCelda / 2) {
            std::string nombreArchivo = objetosSprites[i]->getName(); // Obtener nombre
            if (nombresRecursos.find(nombreArchivo) != nombresRecursos.end()) {
                std::string nombreRecurso = nombresRecursos[nombreArchivo];

                mostrarCuadroDecision(nombreRecurso, [this, i, nombreRecurso]() {
                    auto& inventarioGlobal = InventarioGlobal::getInstance();
                    inventarioGlobal.agregarObjeto(nombreRecurso, 1); // Incrementar recurso
                    objetosSprites[i]->removeFromParent(); // Eliminar sprite
                    objetosSprites.erase(objetosSprites.begin() + i); // Quitar del vector
                    actualizarContadorObjetos(); // Reflejar cambios
                    });
            }
            else {
                log("Objeto desconocido: %s", nombreArchivo.c_str());
            }
            return; // Salir del bucle después de manejar la interacción
        }
    }
}

void JuegoScene::iniciarTemporizador() {
    int tiempoRestante = 60; // Segundos

    // Reproducir la alarma al inicio del tiempo de recolección
    int alarmaInicioID = AudioEngine::play2d("alarmaInicio.mp3", false, 1.0f);

    // Crear una etiqueta para el temporizador si no existe
    etiquetaTiempo = Label::createWithTTF("Tiempo: 60", "fonts/Marker Felt.ttf", 24);
    etiquetaTiempo->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 42));
    this->addChild(etiquetaTiempo);

    // Programar el temporizador para que se actualice cada segundo
    schedule([=](float) mutable {
        tiempoRestante--;
        etiquetaTiempo->setString("Tiempo: " + std::to_string(tiempoRestante));

        if (tiempoRestante <= 0) {
            unschedule("temporizador"); // Detener temporizador
            log("¡Tiempo agotado!");
            _eventDispatcher->removeAllEventListeners(); // Deshabilitar controles
            
            // Mostrar aviso y reproducir alarma
            mostrarAvisoCam("¡Tiempo agotado! Cambiando de escena...");

            // Esperar 6 segundos antes de cambiar de escena
            this->scheduleOnce([this](float) {
                pasarABunker(); // Cambiar a la escena del búnker
                }, 6.0f, "cambiarEscenaPorTiempo");
        }
    }, 1.0f, "temporizador");
}


void JuegoScene::actualizarContadorObjetos() {
    if (!etiquetaContador) {
        etiquetaContador = Label::createWithTTF("Objetos: 0/10", "fonts/Marker Felt.ttf", 24);
        etiquetaContador->setPosition(Vec2(visibleSize.width - 100, visibleSize.height - 42));
        this->addChild(etiquetaContador);
    }

    objetosRecolectados++;
    etiquetaContador->setString("Objetos: " + std::to_string(objetosRecolectados) + "/10");

    if (objetosRecolectados >= 10) {
        log("Inventario lleno. No puedes recolectar más objetos.");

        mostrarAvisoCam("Inventario lleno. camino al BUNKER...");
        _eventDispatcher->removeAllEventListeners(); // Deshabilitar controles

        // Esperar 6 segundos y luego pasar a la escena del búnker
        this->scheduleOnce([this](float) {
            pasarABunker();
            }, 6.0f, "esperaInventarioLleno");
    }
}

void JuegoScene::mostrarCuadroDecision(const std::string& objeto, const std::function<void()>& onAceptar) {

    // Crear fondo de la nota con el texto integrado
    auto fondoNota = Sprite::create("nota.png");
    fondoNota->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 50));
    fondoNota->setScale(0.3f);  // Escalar la imagen
    this->addChild(fondoNota, 1);

    // Botón "Aceptar"
    auto botonAceptar = ui::Button::create("boton_aceptar.png");
    botonAceptar->setTitleText("Aceptar");
    botonAceptar->setTitleFontSize(45);
    botonAceptar->setTitleColor(Color3B::BLACK); // Texto del botón en negro
    botonAceptar->setScale(0.28f);  // Escalar el botón (0.6 es el 60% del tamaño original)
    botonAceptar->setPosition(Vec2(fondoNota->getPositionX() - 100, fondoNota->getPositionY() - fondoNota->getContentSize().height * 0.3f)); // Ajustar posición
    this->addChild(botonAceptar);

    // Botón "Rechazar"
    auto botonRechazar = ui::Button::create("boton_rechazar.png");
    botonRechazar->setTitleText("Rechazar");
    botonRechazar->setTitleFontSize(45);  // Reducir aún más el tamaño de la fuente
    botonRechazar->setTitleColor(Color3B::BLACK); // Texto del botón en negro
    botonRechazar->setPosition(Vec2(fondoNota->getPositionX() + 100, fondoNota->getPositionY() - fondoNota->getContentSize().height * 0.3f)); // Ajustar posición
    botonRechazar->setScale(0.28f);
    this->addChild(botonRechazar);

    // Acción para aceptar
    botonAceptar->addClickEventListener([=](Ref* sender) {  
        onAceptar(); // Ejecutar la acción de aceptar
        fondoNota->removeFromParent(); // Eliminar la nota
        botonAceptar->removeFromParent(); // Eliminar botón Aceptar
        botonRechazar->removeFromParent(); // Eliminar botón Rechazar
        });

    // Acción para rechazar
    botonRechazar->addClickEventListener([=](Ref* sender) {
        fondoNota->removeFromParent(); // Eliminar la nota
        botonAceptar->removeFromParent(); // Eliminar botón Aceptar
        botonRechazar->removeFromParent(); // Eliminar botón Rechazar
        });
}

void JuegoScene::pasarABunker() {
    // Mostrar la pantalla de carga
    auto pantallaCarga = LayerColor::create(Color4B::BLACK);
    this->addChild(pantallaCarga, 10); // Colocar por encima de todo

    auto textoCarga = Label::createWithTTF("Cargando...", "fonts/Marker Felt.ttf", 36);
    textoCarga->setTextColor(Color4B::WHITE);
    textoCarga->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    pantallaCarga->addChild(textoCarga);

    // Esperar un tiempo antes de cambiar la escena
    this->scheduleOnce([](float) {
        Director::getInstance()->replaceScene(TransitionFade::create(1.0f, BunkerScene::createScene()));
        }, 2.0f, "cambiarEscena");
}

void JuegoScene::mostrarAvisoCam(const std::string& mensaje) {
    // Mostrar mensaje en el centro de la pantalla
    auto etiquetaAviso = Label::createWithTTF(mensaje, "fonts/Marker Felt.ttf", 32);
    etiquetaAviso->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    etiquetaAviso->setTextColor(Color4B::RED); // Color rojo para destacar
    this->addChild(etiquetaAviso, 107);

    // Reproducir la alarma (asegúrate de que "alarma.mp3" esté en los recursos del proyecto)
    int alarmaID = AudioEngine::play2d("alarma.mp3", false, 1.0f);

    // Eliminar el mensaje después de 6 segundos
    auto fadeOut = FadeOut::create(6.0f);
    auto remove = RemoveSelf::create();
    auto sequence = Sequence::create(fadeOut, remove, nullptr);
    etiquetaAviso->runAction(sequence);
}
