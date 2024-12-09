#ifndef INVENTARIO_GLOBAL_H
#define INVENTARIO_GLOBAL_H

#include <unordered_map>
#include <string>

class InventarioGlobal {
public:
    static InventarioGlobal& getInstance() {
        static InventarioGlobal instance;
        return instance;
    }

    void agregarObjeto(const std::string& nombre, int cantidad);
    void eliminarObjeto(const std::string& nombre, int cantidad);
    const std::unordered_map<std::string, int>& obtenerInventario() const;
    bool usarObjeto(const std::string& nombre, int cantidad);

private:
    std::unordered_map<std::string, int> inventario;

    InventarioGlobal() = default;
    InventarioGlobal(const InventarioGlobal&) = delete;
    InventarioGlobal& operator=(const InventarioGlobal&) = delete;
};

#endif // INVENTARIO_GLOBAL_H