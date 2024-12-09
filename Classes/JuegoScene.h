#ifndef __JUEGO_SCENE_H__
#define __JUEGO_SCENE_H__

#include "cocos2d.h"
#include "PersonajeMapa.h"
#include "InventarioMapa.h"

class JuegoScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    void inicializarMapa();
    void inicializarObjetos();
    void manejarTeclas();
    void verificarColisionObjetos();
    void iniciarTemporizador(); // Declarar iniciarTemporizador
    void actualizarContadorObjetos(); // Declarar actualizarContadorObjetos
    void mostrarCuadroDecision(const std::string& objeto, const std::function<void()>& onAceptar);
    void pasarABunker();
    void mostrarAvisoCam(const std::string& mensaje);


    CREATE_FUNC(JuegoScene);

private:
    std::vector<std::vector<int>> mapa; // Matriz del mapa
    std::vector<cocos2d::Sprite*> objetosSprites; // Sprites de objetos recolectables
    cocos2d::Sprite* personaje; // Referencia al personaje
    cocos2d::Size visibleSize; // Tamaño visible de la pantalla
    cocos2d::Vec2 origin;
    int objetosRecolectados = 0; // Contador de objetos recolectados
    cocos2d::Label* etiquetaContador; // Etiqueta para el contador
    cocos2d::Label* etiquetaTiempo; // Etiqueta para el temporizador
    InventarioMapa inventario; // Miembro del inventario
};

#endif // __JUEGO_SCENE_H__
