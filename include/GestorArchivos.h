#ifndef GESTOR_ARCHIVOS_H
#define GESTOR_ARCHIVOS_H

#include <string>
#include <vector>
#include <fstream>
#include "Excepciones.h"

// Clase template reutilizable para persistir cualquier entidad que implemente
// toLineaArchivo() y fromLineaArchivo(). Aplica el concepto de templates de clases.
template <typename T>
class GestorArchivos {
public:
    // Guarda una lista completa en el archivo especificado
    static void guardar(const std::string& ruta, const std::vector<T>& items) {
        std::ofstream archivo(ruta, std::ios::out | std::ios::trunc);
        if (!archivo.is_open()) {
            throw ArchivoException("No se pudo abrir el archivo para escritura: " + ruta);
        }

        for (const auto& item : items) {
            archivo << item.toLineaArchivo() << "\n";
        }

        archivo.close();
    }

    // Carga los elementos desde el archivo. Si no existe, lanza ArchivoException
    // pero si esta vacio simplemente devuelve la lista vacia sin fallar.
    static void cargar(const std::string& ruta, std::vector<T>& items) {
        items.clear();
        std::ifstream archivo(ruta, std::ios::in);
        if (!archivo.is_open()) {
            // Si el archivo no existe todavia, intentamos crearlo vacio
            std::ofstream nuevo(ruta);
            if (!nuevo.is_open()) {
                throw ArchivoException("No se pudo crear ni leer el archivo: " + ruta);
            }
            nuevo.close();
            return;
        }

        std::string linea;
        while (std::getline(archivo, linea)) {
            // Ignoramos lineas vacias
            if (linea.empty()) continue;
            T item = T::fromLineaArchivo(linea);
            items.push_back(item);
        }

        archivo.close();
    }
};

#endif // GESTOR_ARCHIVOS_H

