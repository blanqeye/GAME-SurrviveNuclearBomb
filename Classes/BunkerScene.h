#ifndef BUNKER_SCENE_H
#define BUNKER_SCENE_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class BunkerScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    CREATE_FUNC(BunkerScene);

private:

    struct Personaje {
        std::string nombre;
        int salud;
        int hambre;
        int sed;
        cocos2d::Sprite* sprite;
        cocos2d::Label* estadoLabel;
    };

    std::vector<Personaje> personajes;
    // Vectores paralelos para gestionar los recursos visuales
    std::vector<std::string> nombresRecursos;        // Nombres de los recursos
    std::vector<cocos2d::Sprite*> spritesRecursos;   // Sprites de los recursos
    std::vector<cocos2d::Label*> etiquetasRecursos;  // Etiquetas de los recursos

    void mostrarPantallaInstrucciones();
    void iniciarJuegoBunker();
    void inicializarPersonajes();
    void inicializarInventario();
    void mostrarOpcionesPersonaje(Personaje& personaje);
    void mostrarSubmenuRecurso(Personaje& personaje, const std::string& nombreRecurso, int cantidadTotal);
    void usarRecursoEnPersonaje(const std::string& recursoNombre, Personaje& personaje);
    void actualizarEstadoPersonaje(Personaje& personaje);
    void avanzarDia();
    void mostrarMensajeCambioDiaImagen(int dia);
    void mostrarPantallaDerrota();
    void mostrarPantallaVictoria();
    void crearMenuPausa();
    void mostrarOpcionUsoRecurso(const std::string& tipoRecurso, const std::function<void(bool)>& callback);
    void gestionarSucesoDelDia(int dia);
    void actualizarInventarioVisual();
    void agregarRecursos(int mascaras, int comidas, int aguas);
    void recuperarVidaPersonajes(int porcentaje);
    void manejarEquipoDañado();
    void manejarAtaqueToxico();
    void manejarMuerteAleatoria();
    void mostrarImagenEvento(const std::string& nombreImagen);
};

#endif // BUNKER_SCENE_H
