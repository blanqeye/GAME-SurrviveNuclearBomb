#include "PantallaCarga.h"
#include "MenuPrincipal.h"

USING_NS_CC;

Scene* PantallaCarga::createScene() {
    return PantallaCarga::create();
}

bool PantallaCarga::init() {
    if (!Scene::init()) {
        return false;
    }

    // Fondo de la pantalla de carga, centrado y cubriendo toda la pantalla
    auto fondo = Sprite::create("carga_fondo.jpg");
    fondo->setPosition(Director::getInstance()->getVisibleSize() / 2);
    fondo->setScale(Director::getInstance()->getVisibleSize().width / fondo->getContentSize().width,
                    Director::getInstance()->getVisibleSize().height / fondo->getContentSize().height);
    this->addChild(fondo, -1);

    // Texto de "Cargando...", con opacidad para un efecto suave
    auto cargandoLabel = Label::createWithTTF("Cargando Juego...", "fonts/Marker Felt.ttf", 32);
    cargandoLabel->setPosition(Director::getInstance()->getVisibleSize() / 2);
    cargandoLabel->setColor(Color3B::WHITE);
    cargandoLabel->setOpacity(200); // Suavidad visual
    this->addChild(cargandoLabel);

    // Cambiar al menú principal después de 2 segundos
    this->scheduleOnce(CC_SCHEDULE_SELECTOR(PantallaCarga::cambiarAMenu), 2.0f);

    return true;
}

void PantallaCarga::cambiarAMenu(float dt) {
    auto menuScene = MenuPrincipal::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, menuScene));
}
