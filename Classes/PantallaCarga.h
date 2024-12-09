#ifndef __PANTALLA_CARGA_H__
#define __PANTALLA_CARGA_H__

#include "cocos2d.h"

class PantallaCarga : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    CREATE_FUNC(PantallaCarga);

private:
    void cambiarAMenu(float dt);
};

#endif // __PANTALLA_CARGA_H__
