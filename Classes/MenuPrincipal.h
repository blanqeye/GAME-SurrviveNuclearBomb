#ifndef __MENU_PRINCIPAL_H__
#define __MENU_PRINCIPAL_H__

#include "cocos2d.h"

class MenuPrincipal : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void mostrarPantallaInstrucciones();

    CREATE_FUNC(MenuPrincipal);

private:
    void iniciarJuego(cocos2d::Ref* sender);
    void salirJuego(cocos2d::Ref* sender);
};

#endif // __MENU_PRINCIPAL_H__
