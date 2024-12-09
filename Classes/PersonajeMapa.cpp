#include "PersonajeMapa.h"

USING_NS_CC;

PersonajeMapa* PersonajeMapa::create() {
    auto personaje = new PersonajeMapa();
    if (personaje && personaje->initWithFile("personajeMapa.png")) {
        personaje->autorelease();
        return personaje;
    }
    delete personaje;
    return nullptr;
}
