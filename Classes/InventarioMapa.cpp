#include "InventarioMapa.h"
#include "cocos2d.h"

USING_NS_CC;

InventarioMapa::InventarioMapa(int limite) : limite(limite) {}

bool InventarioMapa::agregarObjeto(const std::string& objeto) {
    int totalObjetos = 0;
    for (const auto& it : objetos) {
        totalObjetos += it.second;
    }

    if (totalObjetos < limite) {
        objetos[objeto]++;
        log("Has recolectado: %s. Total: %d", objeto.c_str(), objetos[objeto]);
        return true;
    }

    log("Inventario lleno. No puedes recolectar más objetos.");
    return false;
}

void InventarioMapa::mostrarInventario() const {
    log("Inventario actual:");
    for (const auto& it : objetos) {
        log("%s: %d", it.first.c_str(), it.second);
    }
}