#include "MenuPrincipal.h"
#include "JuegoScene.h" // La escena del juego
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* MenuPrincipal::createScene() {
    return MenuPrincipal::create();
}

bool MenuPrincipal::init() {
    if (!Scene::init()) {
        return false;
    }

    // Fondo del menú principal, ajustado a la pantalla
    auto fondo = Sprite::create("menu_fondo.png");
    fondo->setPosition(Director::getInstance()->getVisibleSize() / 2);
    fondo->setScale(Director::getInstance()->getVisibleSize().width / fondo->getContentSize().width,
        Director::getInstance()->getVisibleSize().height / fondo->getContentSize().height);
    this->addChild(fondo, -1);

    // Botón "Iniciar Juego" más pequeño
    auto botonIniciar = ui::Button::create("boton_iniciar.png", "boton_iniciar_presionado.png");
    botonIniciar->setPosition(Vec2(640, 210));  // Ajustado al centro de la pantalla
    botonIniciar->setScale(0.35f);               // Tamaño reducido (50% de escala)
    botonIniciar->addClickEventListener([=](Ref* sender) {
        botonIniciar->setScale(0.4f); // Efecto de escala al hacer clic
        mostrarPantallaInstrucciones();
        //iniciarJuego(sender);
        });
    this->addChild(botonIniciar);

    // Botón "Salir" más pequeño
    auto botonSalir = ui::Button::create("boton_salir.png", "boton_salir_presionado.png");
    botonSalir->setPosition(Vec2(640, 100));    // Debajo del botón de inicio
    botonSalir->setScale(0.25f);                 // Tamaño reducido (50% de escala)
    botonSalir->addClickEventListener([=](Ref* sender) {
        botonSalir->setScale(0.3f); // Efecto de escala al hacer clic
        salirJuego(sender);
        });
    this->addChild(botonSalir);

    return true;
}

void MenuPrincipal::mostrarPantallaInstrucciones() {
    // Crear una capa para mostrar las instrucciones
    auto capaInstrucciones = LayerColor::create(Color4B(0, 0, 0, 180));
    this->addChild(capaInstrucciones, 10);

    // Imagen de las instrucciones
    auto imagenInstrucciones = Sprite::create("instrucciones.png"); // Asegúrate de tener esta imagen
    imagenInstrucciones->setPosition(Director::getInstance()->getVisibleSize() / 2);
    imagenInstrucciones->setScale(0.9f); // Ajustar escala si es necesario
    capaInstrucciones->addChild(imagenInstrucciones);

    // Botón "Continuar" para cerrar las instrucciones y comenzar el juego
    auto botonContinuar = ui::Button::create("boton_aceptar.png");
    botonContinuar->setTitleText("Continuar");
    botonContinuar->setTitleFontSize(50);
    botonContinuar->setTitleColor(Color3B::BLACK);
    botonContinuar->setScale(0.4f);
    botonContinuar->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, 100));
    capaInstrucciones->addChild(botonContinuar);

    botonContinuar->addClickEventListener([=](Ref* sender) {
        capaInstrucciones->removeFromParent(); // Eliminar la capa de instrucciones
        iniciarJuego(sender);                 // Llamar a la función para iniciar el juego
    });
}

void MenuPrincipal::iniciarJuego(Ref* sender) {
    auto scene = JuegoScene::createScene(); // Cambia a la escena de juego
    Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0f, scene));
}

void MenuPrincipal::salirJuego(Ref* sender) {
    Director::getInstance()->end();
}
