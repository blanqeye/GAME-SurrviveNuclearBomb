#ifndef __INVENTARIO_MAPA_H__
#define __INVENTARIO_MAPA_H__

#include <string>
#include <unordered_map>

class InventarioMapa {
public:
    InventarioMapa(int limite = 10); // Constructor con límite de objetos
    bool agregarObjeto(const std::string& objeto);
    void mostrarInventario() const;
    //const std::unordered_map<std::string, int>& mostrarInventarioCompleto() const;

private:
    std::unordered_map<std::string, int> objetos; // Almacena objetos y sus cantidades
    int limite; // Límite máximo de objetos
};

#endif // __INVENTARIO_MAPA_H__

