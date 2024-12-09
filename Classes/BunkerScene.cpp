#include "BunkerScene.h"
#include "InventarioGlobal.h"
#include "ui/CocosGUI.h"
#include "audio/include/AudioEngine.h"

#include <random> // Para el generador aleatorio

// Generador de números aleatorios
int generarNumeroAleatorio(int minimo, int maximo) {
    static std::random_device rd;  // Fuente de entropía
    static std::mt19937 gen(rd()); // Generador Mersenne Twister
    std::uniform_int_distribution<> distribucion(minimo, maximo);
    return distribucion(gen);
}

using namespace cocos2d;

USING_NS_CC;

Scene* BunkerScene::createScene() {
    return BunkerScene::create();
}

bool BunkerScene::init() {
    if (!Scene::init()) {
        return false;
    }

    // Bunker(fondo)
    auto fondo = Sprite::create("bunker.png");
    fondo->setPosition(Director::getInstance()->getVisibleSize() / 2);
    this->addChild(fondo);

    mostrarPantallaInstrucciones();

    // Listener para ESC
    auto listenerTeclado = EventListenerKeyboard::create();
    listenerTeclado->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
        if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
            crearMenuPausa();
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerTeclado, this);

    return true;
}

void BunkerScene::mostrarPantallaInstrucciones() {
    auto CapaInstrucciones = LayerColor::create(Color4B(0, 0, 0, 180));
    this->addChild(CapaInstrucciones, 1);

    // Instrucciones
    auto instrucciones = Sprite::create("instrucciones2.png");
    instrucciones->setPosition(Director::getInstance()->getVisibleSize() / 2);
    instrucciones->setScale(0.9f);
    CapaInstrucciones->addChild(instrucciones);

    // Botón para cerrar instrucciones y comenzar el juego
    auto botonContinuar = ui::Button::create("boton_aceptar.png");
    botonContinuar->setTitleText("Continuar");
    botonContinuar->setTitleFontSize(50);
    botonContinuar->setTitleColor(Color3B::BLACK);
    botonContinuar->setScale(0.4f);
    botonContinuar->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, 100));
    CapaInstrucciones->addChild(botonContinuar);

    botonContinuar->addClickEventListener([=](Ref* sender) {
        CapaInstrucciones->removeFromParent();
        iniciarJuegoBunker(); // Comienza el juego en el búnker
        });
}

void BunkerScene::iniciarJuegoBunker() {
    inicializarPersonajes();
    inicializarInventario();

    mostrarMensajeCambioDiaImagen(1);

    // Botón para pasar día
    auto botonSiguienteDia = ui::Button::create("boton_aceptar.png");
    botonSiguienteDia->setTitleText("Siguiente Dia");
    botonSiguienteDia->setTitleFontSize(60);
    botonSiguienteDia->setScale(0.25f);
    botonSiguienteDia->setPosition(Vec2(Director::getInstance()->getVisibleSize().width - 100, 50));
    this->addChild(botonSiguienteDia, 1);

    botonSiguienteDia->addClickEventListener([=](Ref* sender) {
        avanzarDia();
        });
}


void BunkerScene::inicializarPersonajes() {
    // Valores por personaje
    personajes = {
        {"Papa", 100, 100, 100},
        {"Mama", 100, 100, 100},
        {"Hija", 100, 100, 100},
        {"Hijo", 100, 100, 100}
    };

    float offsetX = 320;
    float startY = Director::getInstance()->getVisibleSize().height / 3;
    
    // Metricas y personaje
    for (int i = 0; i < personajes.size(); i++) {
        auto personajeSprite = Sprite::create(personajes[i].nombre + ".png");
        personajeSprite->setPosition(Vec2(offsetX + (i * 180), startY));
        personajeSprite->setScale(0.5f);
        this->addChild(personajeSprite);

        auto estadoLabel = Label::createWithTTF("Salud: 100\nHambre: 100\nSed: 100", "fonts/Marker Felt.ttf", 20);
        estadoLabel->setAnchorPoint(Vec2(0.5, 1));
        estadoLabel->setPosition(Vec2(offsetX + (i * 180), startY - 120));
        this->addChild(estadoLabel);

        personajes[i].sprite = personajeSprite;
        personajes[i].estadoLabel = estadoLabel;

        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = [=](Touch* touch, Event* event) {
            if (personajeSprite->getBoundingBox().containsPoint(touch->getLocation())) {
                mostrarOpcionesPersonaje(personajes[i]);
                return true;
            }
            return false;
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, personajeSprite);
    }
}

void BunkerScene::inicializarInventario() {
    float offsetX = 100;
    float startY = Director::getInstance()->getVisibleSize().height - 50;

    const float tamanoObjetivo = 50.0f; // Tamaño objetivo para todos los sprites (en píxeles)
    const auto& inventario = InventarioGlobal::getInstance().obtenerInventario();

    for (const auto& item : inventario) {
        const std::string& nombre = item.first;  // Nombre del recurso
        int cantidad = item.second;             // Cantidad del recurso
        // Crear un sprite para cada recurso
        auto objeto = Sprite::create(nombre + ".png"); // Asegúrate de que los nombres coincidan con las imágenes
        if (!objeto) {
            log("Error al cargar el sprite para el recurso: %s", nombre.c_str());
            continue;
        }
        // Calcular escala para que todos sean iguales
        float escalaX = tamanoObjetivo / objeto->getContentSize().width;
        float escalaY = tamanoObjetivo / objeto->getContentSize().height;
        float escalaFinal = std::min(escalaX, escalaY);
        objeto->setScale(escalaFinal);

        // Posición
        objeto->setPosition(Vec2(offsetX, startY));
        this->addChild(objeto);

        // Cantidad de cada recurso
        auto etiquetaCantidad = Label::createWithTTF(std::to_string(cantidad), "fonts/Marker Felt.ttf", 20);
        etiquetaCantidad->setPosition(Vec2(offsetX, startY - 30));
        etiquetaCantidad->setTextColor(Color4B::WHITE);
        this->addChild(etiquetaCantidad);

        nombresRecursos.push_back(nombre);
        spritesRecursos.push_back(objeto);
        etiquetasRecursos.push_back(etiquetaCantidad);

        offsetX += 100;
    }
}

void BunkerScene::mostrarOpcionesPersonaje(Personaje& personaje) {
    auto capaOpciones = LayerColor::create(Color4B(0, 0, 0, 180));
    this->addChild(capaOpciones, 10);

    // Imagen Recurso
    auto imagenRecursoUsar = Sprite::create("Recurso_Usar.png");
    if (imagenRecursoUsar) {
        imagenRecursoUsar->setScale(0.32f); // Ajustar tamaño al 50%
        imagenRecursoUsar->setPosition(Vec2(
            Director::getInstance()->getVisibleSize().width / 2,
            Director::getInstance()->getVisibleSize().height / 2 + 200
        ));
        capaOpciones->addChild(imagenRecursoUsar);
    }
    else {
        log("Error: No se pudo cargar la imagen Recurso_Usar.png");
    }

    // Posicionamiento inicial para los botones
    float startX = Director::getInstance()->getVisibleSize().width / 2 - 300; // Posición inicial en X
    float startY = Director::getInstance()->getVisibleSize().height / 2 + 150;     // Posición inicial en Y
    float offsetY = -75; // Espaciado vertical entre botones


    // Obtener el inventario global
    auto& inventarioGlobal = InventarioGlobal::getInstance();
    auto inventario = inventarioGlobal.obtenerInventario();

    int index = 0;
    for (const auto& recurso : inventario) {
        const std::string& nombreRecurso = recurso.first;
        int cantidadTotal = recurso.second;

        // Botón para usar el recurso
        auto botonUsar = ui::Button::create("boton_aceptar.png");
        botonUsar->setTitleText(nombreRecurso + " (" + std::to_string(cantidadTotal) + ")");
        botonUsar->setTitleFontSize(60);
        botonUsar->setScale(0.25f);
        botonUsar->setPosition(Vec2(startX, startY + (offsetY * index)));// Ajustar posición según el índice
        capaOpciones->addChild(botonUsar);

        // Cambiar referencia `personaje` a puntero
        botonUsar->addClickEventListener([=, this, &inventarioGlobal, personajePtr = &personaje](Ref* sender) {
            mostrarSubmenuRecurso(*personajePtr, nombreRecurso, cantidadTotal);// Submenú de administración
            capaOpciones->removeFromParent();
            });
        index++; // Incrementar índice para el siguiente botón
    }

    // Botón para cancelar
    auto botonCancelar = ui::Button::create("boton_rechazar.png");
    botonCancelar->setTitleText("Cancelar");
    botonCancelar->setTitleFontSize(65);
    botonCancelar->setScale(0.3f);
    botonCancelar->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, 100));
    capaOpciones->addChild(botonCancelar);

    botonCancelar->addClickEventListener([=](Ref* sender) {
        capaOpciones->removeFromParent();
        });
}

void BunkerScene::mostrarSubmenuRecurso(Personaje& personaje, const std::string& nombreRecurso, int cantidadTotal) {
    auto capaSubmenu = LayerColor::create(Color4B(0, 0, 0, 180));
    this->addChild(capaSubmenu, 11);

    auto textoRecurso = Label::createWithTTF("Administrar: " + nombreRecurso, "fonts/Marker Felt.ttf", 28);
    textoRecurso->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
        Director::getInstance()->getVisibleSize().height / 2 + 150));
    capaSubmenu->addChild(textoRecurso);

    float offsetX = 100;
    auto& inventarioGlobal = InventarioGlobal::getInstance();

    // Botón para aceptar uso
    auto botonUsarPorcion = ui::Button::create("boton_aceptar.png");
    botonUsarPorcion->setTitleText("Usar 1");
    botonUsarPorcion->setTitleFontSize(40);
    botonUsarPorcion->setScale(0.28f);
    botonUsarPorcion->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 - offsetX, Director::getInstance()->getVisibleSize().height / 2));
    capaSubmenu->addChild(botonUsarPorcion);

    botonUsarPorcion->addClickEventListener([=, this, &personaje](Ref* sender) {
        usarRecursoEnPersonaje(nombreRecurso, personaje);
        capaSubmenu->removeFromParent();
        });

    // Botón para cancelar
    auto botonCancelar = ui::Button::create("boton_rechazar.png");
    botonCancelar->setTitleText("Cancelar");
    botonCancelar->setTitleFontSize(40);
    botonCancelar->setScale(0.28f);
    botonCancelar->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 + offsetX, Director::getInstance()->getVisibleSize().height / 2));
    capaSubmenu->addChild(botonCancelar);

    botonCancelar->addClickEventListener([=](Ref* sender) {
        capaSubmenu->removeFromParent();
        });
}

void BunkerScene::usarRecursoEnPersonaje(const std::string& recursoNombre, Personaje& personaje) {
    auto& inventario = InventarioGlobal::getInstance();

    // Verificar si el recurso existe y hay al menos 1 disponible
    const auto& inventarioActual = inventario.obtenerInventario();
    if (inventarioActual.find(recursoNombre) == inventarioActual.end() || inventarioActual.at(recursoNombre) <= 0) {
        log("No hay suficiente %s en el inventario.", recursoNombre.c_str());
        return;
    }

    // Usar el recurso
    if (inventario.usarObjeto(recursoNombre, 1)) {
        // Efectos al personaje
        if (recursoNombre == "comida") {
            personaje.hambre = std::min(personaje.hambre + 25, 100);
            log("Usando comida. Nueva hambre: %d", personaje.hambre);
        }
        else if (recursoNombre == "agua") {
            personaje.sed = std::min(personaje.sed + 25, 100);
            log("Usando agua. Nueva sed: %d", personaje.sed);
        }
        else if (recursoNombre == "medicina") {
            personaje.salud = std::min(personaje.salud + 80, 100);
            log("Usando medicina. Nueva salud: %d", personaje.salud);
        }

        actualizarEstadoPersonaje(personaje);

        // Contador visual
        for (size_t i = 0; i < spritesRecursos.size(); ++i) {
            if (nombresRecursos[i] == recursoNombre) {
                auto it = inventario.obtenerInventario().find(recursoNombre);
                if (it != inventario.obtenerInventario().end()) {
                    int nuevaCantidad = it->second;
                    etiquetasRecursos[i]->setString(std::to_string(nuevaCantidad));
                }
                else {
                    etiquetasRecursos[i]->setString("0");
                }
                break;
            }
        }
    } else {
        log("Error al intentar usar %s. Verifica el inventario.", recursoNombre.c_str());
    }
}

void BunkerScene::actualizarEstadoPersonaje(Personaje& personaje) {
    personaje.estadoLabel->setString("Salud: " + std::to_string(personaje.salud) +
        "\nHambre: " + std::to_string(personaje.hambre) +
        "\nSed: " + std::to_string(personaje.sed));
}

void BunkerScene::avanzarDia() {
    static int diaActual = 1; // Cuenta de los días

    if (diaActual >= 10) {
        if (!personajes.empty()) {
            mostrarPantallaVictoria();
        } else {
            mostrarPantallaDerrota();
        }
        return;
    }

    diaActual++;

    mostrarMensajeCambioDiaImagen(diaActual);
    gestionarSucesoDelDia(diaActual);

    // Reducir recursos y actualizar personajes
    for (auto it = personajes.begin(); it != personajes.end();) {
        it->hambre -= rand() % 25 + 10;
        it->sed -= rand() % 25 + 10;

        if (it->hambre <= 0 || it->sed <= 0) {
            it->salud -= 30;
        }

        if (it->salud <= 0) {
            log("%s ha muerto.", it->nombre.c_str());

            // Mostrar mensaje de muerte
            auto mensajeMuerte = Label::createWithTTF(it->nombre + " ha muerto.", "fonts/Marker Felt.ttf", 30);
            mensajeMuerte->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2 + 150));
            mensajeMuerte->setTextColor(Color4B::BLACK);
            this->addChild(mensajeMuerte, 10);

            // Eliminar el mensaje después de 2 segundos
            mensajeMuerte->runAction(Sequence::create(
                DelayTime::create(1.5f),
                RemoveSelf::create(),
                nullptr
            ));

            it->sprite->removeFromParent();
            it->estadoLabel->removeFromParent();
            it = personajes.erase(it);
        }
        else {
            actualizarEstadoPersonaje(*it);
            ++it;
        }
    }
    // Revisar si todos los personajes han muerto
    if (personajes.empty()) {
        mostrarPantallaDerrota();
    }
}

void BunkerScene::mostrarMensajeCambioDiaImagen(int dia) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Crear un sprite para el mensaje de cambio de día
    auto imagenCambioDia = Sprite::create("dia_" + std::to_string(dia) + ".png");
    if (!imagenCambioDia) {
        log("Error: No se encontró la imagen para el día %d", dia);
        return;
    }

    // Ubicación
    imagenCambioDia->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    this->addChild(imagenCambioDia, 101);

    // Eliminar el mensaje después de 2 segundos
    imagenCambioDia->runAction(Sequence::create(
        DelayTime::create(2.0f),
        RemoveSelf::create(),
        nullptr
    ));
}

void BunkerScene::mostrarPantallaVictoria() {
    auto capaVictoria = LayerColor::create(Color4B(0, 0, 0, 180));
    this->addChild(capaVictoria, 10);

    // Imagen de victoria
    auto imagenVictoria = Sprite::create("victoria.png");
    imagenVictoria->setPosition(Director::getInstance()->getVisibleSize() / 2);
    capaVictoria->addChild(imagenVictoria);

    // Botón para finalizar el juego
    auto botonSalir = ui::Button::create("boton_aceptar.png");
    botonSalir->setTitleText("Finalizar");
    botonSalir->setTitleFontSize(45);
    botonSalir->setScale(0.4f);
    botonSalir->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, 100));
    capaVictoria->addChild(botonSalir);

    botonSalir->addClickEventListener([=](Ref* sender) {
        Director::getInstance()->end(); // Finaliza el juego
        });
}

void BunkerScene::mostrarPantallaDerrota() {
    auto capaDerrota = LayerColor::create(Color4B(0, 0, 0, 180));
    this->addChild(capaDerrota, 10);

    // Imagen de derrota
    auto imagenDerrota = Sprite::create("derrota.png");
    imagenDerrota->setPosition(Director::getInstance()->getVisibleSize() / 2);
    capaDerrota->addChild(imagenDerrota);

    // Botón para finalizar el juego
    auto botonSalir = ui::Button::create("boton_aceptar.png");
    botonSalir->setTitleText("Finalizar");
    botonSalir->setTitleFontSize(45);
    botonSalir->setScale(0.4f);
    botonSalir->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, 100));
    capaDerrota->addChild(botonSalir);

    botonSalir->addClickEventListener([=](Ref* sender) {
        Director::getInstance()->end(); // Finaliza el juego
        });
}

void BunkerScene::crearMenuPausa() {
    auto capaPausa = LayerColor::create(Color4B(0, 0, 0, 180));
    this->addChild(capaPausa, 20);

    // Imagen del menú de pausa
    auto imagenPausa = Sprite::create("Menu_Pausa.png");
    if (imagenPausa) {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto originalSize = imagenPausa->getContentSize();

        float scaleX = visibleSize.width / originalSize.width;
        float scaleY = visibleSize.height / originalSize.height;

        imagenPausa->setScale(std::min(scaleX, scaleY) * 0.95f); // Ajustar el tamaño al 95% de la pantalla
        imagenPausa->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        capaPausa->addChild(imagenPausa);
    }
    else {
        log("Error: No se pudo cargar la imagen Menu_Pausa.png");
    }

    // Botón para reanudar el juego
    auto botonReanudar = ui::Button::create("boton_aceptar.png");
    botonReanudar->setTitleText("Reanudar");
    botonReanudar->setTitleFontSize(60);
    botonReanudar->setScale(0.25f);
    botonReanudar->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
        Director::getInstance()->getVisibleSize().height / 2 - 150));
    capaPausa->addChild(botonReanudar);

    botonReanudar->addClickEventListener([=](Ref* sender) {
        capaPausa->removeFromParent(); // Eliminar
        });

    // Botón para salir del juego
    auto botonSalir = ui::Button::create("boton_rechazar.png");
    botonSalir->setTitleText("Salir");
    botonSalir->setTitleFontSize(60);
    botonSalir->setScale(0.2f);
    botonSalir->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
        Director::getInstance()->getVisibleSize().height / 2 - 220));
    capaPausa->addChild(botonSalir);

    botonSalir->addClickEventListener([=](Ref* sender) {
        Director::getInstance()->end(); // Finalizar el juego
        });
}

void BunkerScene::mostrarOpcionUsoRecurso(const std::string& tipoRecurso, const std::function<void(bool)>& callback) {
    auto capaOpcion = LayerColor::create(Color4B(0, 0, 0, 180));
    this->addChild(capaOpcion, 20);

    // Crear la imagen de la opción
    auto imagenOpcion = Sprite::create("prevenir_Danos.png");
    if (imagenOpcion) {
        imagenOpcion->setScale(0.8f);
        imagenOpcion->setPosition(Vec2(
            Director::getInstance()->getVisibleSize().width / 2,
            Director::getInstance()->getVisibleSize().height / 2 + 50
        ));
        capaOpcion->addChild(imagenOpcion);
        log("Imagen prevenir_Danos.png cargada correctamente.");
    }
    else {
        log("Error: No se pudo cargar la imagen prevenir_Danos.png. Verifica el nombre o la ruta.");
    }

    // Crear botones
    capaOpcion->runAction(Sequence::create(
        DelayTime::create(3.0f), // Esperar 3 segundos
        CallFunc::create([=]() {
            // Botón de confirmar uso del recurso
            auto botonConfirmar = ui::Button::create("boton_aceptar.png");
            botonConfirmar->setTitleText("Usar");
            botonConfirmar->setTitleFontSize(40);
            botonConfirmar->setScale(0.3f);
            botonConfirmar->setPosition(Vec2(
                Director::getInstance()->getVisibleSize().width / 2 - 150,
                Director::getInstance()->getVisibleSize().height / 2 - 50
            ));
            capaOpcion->addChild(botonConfirmar);

            botonConfirmar->addClickEventListener([=](Ref* sender) {
                capaOpcion->removeFromParent();
                callback(true); 
                });

            // Botón de cancelar
            auto botonCancelar = ui::Button::create("boton_rechazar.png");
            botonCancelar->setTitleText("Ignorar");
            botonCancelar->setTitleFontSize(40);
            botonCancelar->setScale(0.3f);
            botonCancelar->setPosition(Vec2(
                Director::getInstance()->getVisibleSize().width / 2 + 150,
                Director::getInstance()->getVisibleSize().height / 2 - 50
            ));
            capaOpcion->addChild(botonCancelar);

            botonCancelar->addClickEventListener([=](Ref* sender) {
                capaOpcion->removeFromParent();
                callback(false);
                });
            }),
        nullptr
                ));
}

void BunkerScene::gestionarSucesoDelDia(int dia) {
    int eventoId;

    // Determinar el evento según el día
    if (dia == 3) {
        eventoId = generarNumeroAleatorio(1, 3); // 1 (bueno), 2 (normal), 3 (malo)
    }
    else if (dia == 6) {
        eventoId = generarNumeroAleatorio(4, 6); // 4 (bueno), 5 (malo), 6 (malo)
    }
    else if (dia == 9) {
        eventoId = generarNumeroAleatorio(7, 9); // 7 (bueno), 8 (normal), 9 (malo)
    }
    else {
        return; // No hay evento para este día
    }

    // Ejecutar el evento correspondiente
    switch (eventoId) {
    case 1: // Suceso bueno
        log("Evento bueno 1: Llegada de recursos inesperados");
        agregarRecursos(1, 3, 2); // 1 máscara, 3 comidas, 2 aguas
        mostrarImagenEvento("evento_bueno_suministros.png");
        break;

    case 2: // Suceso normal
        log("Evento normal 3: Ruido extraño afuera");
        mostrarImagenEvento("evento_normal_ruido.png");
        break;

    case 3: // Suceso malo: Equipo dañado
        log("Evento malo 5: Equipo dañado");
        AudioEngine::play2d("alarma_malo.mp3");
        mostrarOpcionUsoRecurso("herramienta", [=](bool usarRecurso) {
            if (usarRecurso) {
                manejarEquipoDañado();
            }
            else {
                log("No se usó la herramienta. Procediendo con los daños.");
                mostrarImagenEvento("evento_malo_insalubridad.png");
                auto& personajeAfectado = personajes[generarNumeroAleatorio(0, personajes.size() - 1)];
                personajeAfectado.salud -= 20;
                actualizarEstadoPersonaje(personajeAfectado);
            }
            });
        break;

    case 4: // Suceso bueno
        log("Evento bueno 2: Recuperación general");
        recuperarVidaPersonajes(50); // Recupera 50% de la vida perdida
        mostrarImagenEvento("evento_bueno_descanso.png");
        break;

    case 5: // Suceso malo: Ataque tóxico
        log("Evento malo 6: Ataque tóxico");
        AudioEngine::play2d("alarma_malo.mp3");
        mostrarOpcionUsoRecurso("máscara", [=](bool usarRecurso) {
            if (usarRecurso) {
                manejarAtaqueToxico();
            }
            else {
                log("No se usó la máscara. Procediendo con los daños.");
                mostrarImagenEvento("evento_malo_toxico.png");
                auto& personajeAfectado = personajes[generarNumeroAleatorio(0, personajes.size() - 1)];
                personajeAfectado.salud -= 40;
                actualizarEstadoPersonaje(personajeAfectado);
            }
            });
        break;

    case 6: // Suceso malo
        log("Evento malo 5: Equipo dañado");
        AudioEngine::play2d("alarma_malo.mp3");
        manejarEquipoDañado();
        mostrarImagenEvento("evento_malo_insalubridad.png");
        break;

    case 7: // Suceso bueno
        log("Evento bueno 2: Recuperación general");
        recuperarVidaPersonajes(50);
        mostrarImagenEvento("evento_bueno_descanso.png");
        break;

    case 8: // Suceso normal
        log("Evento normal 4: Día tranquilo");
        mostrarImagenEvento("evento_normal_tranquilo.png");
        break;

    case 9: // Suceso malo
        log("Evento malo 7: Muerte aleatoria");
        AudioEngine::play2d("alarma_malo.mp3");
        manejarMuerteAleatoria();
        mostrarImagenEvento("evento_malo_muerte.png");
        break;
    }
}

void BunkerScene::actualizarInventarioVisual() {
    auto& inventario = InventarioGlobal::getInstance();
    const auto& recursos = inventario.obtenerInventario();

    for (const auto& recurso : recursos) {
        const std::string& nombre = recurso.first;
        int cantidad = recurso.second;

        for (size_t i = 0; i < nombresRecursos.size(); ++i) {
            if (nombresRecursos[i] == nombre) {
                etiquetasRecursos[i]->setString(std::to_string(cantidad));
                break;
            }
        }
    }
}

void BunkerScene::agregarRecursos(int mascaras, int comidas, int aguas) {
    auto& inventario = InventarioGlobal::getInstance();
    inventario.agregarObjeto("mascara", mascaras);
    inventario.agregarObjeto("comida", comidas);
    inventario.agregarObjeto("agua", aguas);

    actualizarInventarioVisual();
    log("Recursos añadidos: Mascaras=%d, Comidas=%d, Aguas=%d", mascaras, comidas, aguas);
}

void BunkerScene::recuperarVidaPersonajes(int porcentaje) {
    for (auto& personaje : personajes) {
        int saludRecuperada = (100 - personaje.salud) * porcentaje / 100;
        personaje.salud = std::min(personaje.salud + saludRecuperada, 100);
        actualizarEstadoPersonaje(personaje);
    }
    log("Todos los personajes recuperaron %d%% de salud.", porcentaje);
}

void BunkerScene::manejarEquipoDañado() {
    auto& inventario = InventarioGlobal::getInstance();
    if (inventario.usarObjeto("herramienta", 1)) {
        mostrarImagenEvento("evento_bueno_fuga_reparada.png");
    }
    else {
        if (!personajes.empty()) {
            auto& personajeAfectado = personajes[generarNumeroAleatorio(0, personajes.size() - 1)];
            personajeAfectado.salud -= 20;
            actualizarEstadoPersonaje(personajeAfectado);

            mostrarImagenEvento("evento_malo_insalubridad.png");
        }
        else {
            log("Error: No hay personajes disponibles para afectar.");
        }
    }
}

void BunkerScene::manejarAtaqueToxico() {
    auto& inventario = InventarioGlobal::getInstance();
    if (inventario.usarObjeto("mascara", 1)) {
        mostrarImagenEvento("evento_bueno_mascara.png"); 
    }
    else {
        if (!personajes.empty()) {
            auto& personajeAfectado = personajes[generarNumeroAleatorio(0, personajes.size() - 1)];
            personajeAfectado.salud -= 40;
            actualizarEstadoPersonaje(personajeAfectado);

            mostrarImagenEvento("evento_malo_toxico.png");
        }
        else {
            log("Error: No hay personajes disponibles para afectar.");
        }
    }
}

void BunkerScene::manejarMuerteAleatoria() {
    if (!personajes.empty()) {
        int personajeMuerto = generarNumeroAleatorio(0, personajes.size() - 1);
        auto& personaje = personajes[personajeMuerto];

        personaje.sprite->removeFromParent();
        personaje.estadoLabel->removeFromParent();
        personajes.erase(personajes.begin() + personajeMuerto);

        mostrarImagenEvento("evento_malo_muerte.png");
    }
    else {
        log("Error: No hay personajes para seleccionar al azar.");
    }
}

void BunkerScene::mostrarImagenEvento(const std::string& nombreImagen) {
    auto capaEvento = LayerColor::create(Color4B(0, 0, 0, 180));
    this->addChild(capaEvento, 20);

    // Crear imagen del evento
    auto imagenEvento = Sprite::create(nombreImagen);
    if (imagenEvento) {
        imagenEvento->setScale(0.8f);
        imagenEvento->setPosition(Vec2(
            Director::getInstance()->getVisibleSize().width / 2,
            Director::getInstance()->getVisibleSize().height / 2
        ));
        imagenEvento->setOpacity(0);
        capaEvento->addChild(imagenEvento);

        imagenEvento->runAction(Sequence::create(
            DelayTime::create(2.0f),
            FadeIn::create(0.5f),       
            DelayTime::create(4.0f),
            RemoveSelf::create(),
            nullptr
        ));

        // Eliminar la capa completa después del tiempo total
        capaEvento->runAction(Sequence::create(
            DelayTime::create(6.5f),
            RemoveSelf::create(),
            nullptr
        ));
    }
    else {
        log("Error: No se pudo cargar la imagen %s", nombreImagen.c_str());
    }
}