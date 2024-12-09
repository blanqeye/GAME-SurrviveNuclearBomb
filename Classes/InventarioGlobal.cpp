#include "InventarioGlobal.h"
#include <algorithm>
#include <iostream>

void InventarioGlobal::agregarObjeto(const std::string& nombre, int cantidad) {
    inventario[nombre] += cantidad; // Incrementa la cantidad del objeto
    std::cout << "Se agregó " << cantidad << " de " << nombre << " al inventario." << std::endl;
}

void InventarioGlobal::eliminarObjeto(const std::string& nombre, int cantidad) {
    if (inventario.find(nombre) != inventario.end()) {
        inventario[nombre] = std::max(0, inventario[nombre] - cantidad);
        if (inventario[nombre] == 0) {
            inventario.erase(nombre); // Elimina el objeto si la cantidad llega a 0
        }
        std::cout << "Se eliminó " << cantidad << " de " << nombre << " del inventario." << std::endl;
    }
    else {
        std::cout << "El objeto " << nombre << " no existe en el inventario." << std::endl;
    }
}

const std::unordered_map<std::string, int>& InventarioGlobal::obtenerInventario() const {
    return inventario; // Devuelve una referencia constante al inventario
}

bool InventarioGlobal::usarObjeto(const std::string& nombre, int cantidad) {
    auto it = inventario.find(nombre);
    if (it != inventario.end() && it->second >= cantidad) {
        it->second -= cantidad;
        if (it->second == 0) {
            inventario.erase(it); // Elimina el objeto si la cantidad llega a 0
        }
        return true;
    }
    return false; // No hay suficiente cantidad o el objeto no existe
}