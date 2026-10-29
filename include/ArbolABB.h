#ifndef ARBOL_ABB_H
#define ARBOL_ABB_H

#include <iostream>
#include <vector>

// Estructura nodo para el Arbol Binario de Busqueda (ABB)
// Aplicamos Templates para que pueda almacenar cualquier tipo de dato (Turno, int, etc.)
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
            // Si es mayor o igual, se inserta en el subarbol derecho
            insertarRec(nodo->der, valor);
        }
    }

    // Recorrido Inorden recursivo: Subarbol Izquierdo -> Raiz -> Subarbol Derecho
    // Para turnos, al ordenarse por operator<, nos devuelve la lista ordenada cronologicamente
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

    // Destruccion recursiva en postorden para liberar la memoria dinamica y evitar leaks
    void destruirRec(NodoABB<T>* nodo) {
        if (nodo != nullptr) {
            destruirRec(nodo->izq);
            destruirRec(nodo->der);
            delete nodo;
        }
    }

public:
    ArbolABB() : raiz(nullptr), cantidadElementos(0) {}

    ~ArbolABB() {
        vaciar();
    }

    // Evitamos copia superficial accidental
    ArbolABB(const ArbolABB&) = delete;
    ArbolABB& operator=(const ArbolABB&) = delete;

    void insertar(const T& valor) {
        insertarRec(raiz, valor);
    }

    // Devuelve los elementos ordenados mediante inorden
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

    bool estaVacio() const {
        return raiz == nullptr;
    }
};

#endif // ARBOL_ABB_H

