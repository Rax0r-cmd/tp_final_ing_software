#ifndef ARBOL_ABB_H
#define ARBOL_ABB_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

// Estructura nodo para el Arbol Binario de Busqueda (ABB)
// Aplicamos Templates para que pueda almacenar cualquier tipo de dato
template <typename T>
struct NodoABB {
    T dato;
    NodoABB<T>* izq;
    NodoABB<T>* der;

    NodoABB(const T& valor) : dato(valor), izq(nullptr), der(nullptr) {}
};

// Clase Template ArbolABB implementada desde cero (Estructura de datos propia + Recursion)
template <typename T>
class ArbolABB {
private:
    NodoABB<T>* raiz;
    int cantidadElementos;

    // Insercion recursiva respetando la propiedad del ABB: menores a la izquierda, mayores a la derecha
    void insertarRec(NodoABB<T>*& nodo, const T& valor) {
        if (nodo == nullptr) {
            nodo = new NodoABB<T>(valor);
            cantidadElementos++;
            return;
        }

        if (valor < nodo->dato) {
            insertarRec(nodo->izq, valor);
        } else {
            insertarRec(nodo->der, valor);
        }
    }

    // Recorrido Inorden recursivo: Subarbol Izquierdo -> Raiz -> Subarbol Derecho
    // Devuelve los elementos ordenados cronologicamente segun operator<
    void inordenRec(NodoABB<T>* nodo, std::vector<T>& resultado) const {
        if (nodo != nullptr) {
            inordenRec(nodo->izq, resultado);
            resultado.push_back(nodo->dato);
            inordenRec(nodo->der, resultado);
        }
    }

    // Busqueda recursiva por valor / coincidencia (usa operator==)
    NodoABB<T>* buscarRec(NodoABB<T>* nodo, const T& valor) const {
        if (nodo == nullptr) {
            return nullptr;
        }
        if (nodo->dato == valor) {
            return nodo;
        }
        if (valor < nodo->dato) {
            return buscarRec(nodo->izq, valor);
        } else {
            return buscarRec(nodo->der, valor);
        }
    }

    // Destruccion recursiva en postorden para liberar la memoria dinamica
    void destruirRec(NodoABB<T>* nodo) {
        if (nodo != nullptr) {
            destruirRec(nodo->izq);
            destruirRec(nodo->der);
            delete nodo;
        }
    }

    // Recorrido Preorden recursivo para persistir la forma exacta de la estructura
    void preordenGuardarRec(NodoABB<T>* nodo, std::ofstream& arch) const {
        if (nodo == nullptr) {
            arch << "#\n"; // Indicador de rama nula para preservar la topologia
            return;
        }
        arch << nodo->dato.toLineaArchivo() << "\n";
        preordenGuardarRec(nodo->izq, arch);
        preordenGuardarRec(nodo->der, arch);
    }

    // Reconstruccion recursiva del arbol desde el archivo persistido
    void preordenCargarRec(NodoABB<T>*& nodo, std::ifstream& arch) {
        std::string linea;
        if (!std::getline(arch, linea) || linea.empty() || linea == "#") {
            nodo = nullptr;
            return;
        }
        T valor = T::fromLineaArchivo(linea);
        nodo = new NodoABB<T>(valor);
        cantidadElementos++;
        preordenCargarRec(nodo->izq, arch);
        preordenCargarRec(nodo->der, arch);
    }

    // Visualizacion recursiva de la jerarquia del arbol en consola
    void mostrarGraficoRec(NodoABB<T>* nodo, int nivel) const {
        if (nodo != nullptr) {
            mostrarGraficoRec(nodo->der, nivel + 1);
            for (int i = 0; i < nivel; i++) {
                std::cout << "         ";
            }
            std::cout << "-> [#" << nodo->dato.getIdTurno() << " " << nodo->dato.getFecha() << " " << nodo->dato.getHora() << "]\n";
            mostrarGraficoRec(nodo->izq, nivel + 1);
        }
    }

    // Calculo recursivo de la altura / profundidad maxima del arbol
    int calcularAlturaRec(NodoABB<T>* nodo) const {
        if (nodo == nullptr) return 0;
        int hIzq = calcularAlturaRec(nodo->izq);
        int hDer = calcularAlturaRec(nodo->der);
        return 1 + (hIzq > hDer ? hIzq : hDer);
    }

public:
    ArbolABB() : raiz(nullptr), cantidadElementos(0) {}

    ~ArbolABB() {
        vaciar();
    }

    ArbolABB(const ArbolABB&) = delete;
    ArbolABB& operator=(const ArbolABB&) = delete;

    void insertar(const T& valor) {
        insertarRec(raiz, valor);
    }

    std::vector<T> obtenerInorden() const {
        std::vector<T> lista;
        inordenRec(raiz, lista);
        return lista;
    }

    bool buscar(const T& buscado, T& encontrado) const {
        NodoABB<T>* res = buscarRec(raiz, buscado);
        if (res != nullptr) {
            encontrado = res->dato;
            return true;
        }
        return false;
    }

    void vaciar() {
        destruirRec(raiz);
        raiz = nullptr;
        cantidadElementos = 0;
    }

    int getCantidad() const {
        return cantidadElementos;
    }

    int getAltura() const {
        return calcularAlturaRec(raiz);
    }

    bool estaVacio() const {
        return raiz == nullptr;
    }

    // Metodo opcional cumplido: Visualizacion jerarquica del arbol
    void imprimirGrafico() const {
        if (raiz == nullptr) {
            std::cout << " (El arbol binario se encuentra vacio)\n";
            return;
        }
        mostrarGraficoRec(raiz, 0);
    }

    // Metodo opcional cumplido: Persistencia directa de la estructura de datos
    void guardarEstructura(const std::string& ruta) const {
        std::ofstream arch(ruta, std::ios::out | std::ios::trunc);
        if (arch.is_open()) {
            preordenGuardarRec(raiz, arch);
            arch.close();
        }
    }

    void cargarEstructura(const std::string& ruta) {
        vaciar();
        std::ifstream arch(ruta, std::ios::in);
        if (arch.is_open()) {
            preordenCargarRec(raiz, arch);
            arch.close();
        }
    }
};

#endif // ARBOL_ABB_H
