#include "../include/InterfazUI.h"
#include <iostream>

int main() {
    try {
        // Punto de entrada principal del sistema de turnos
        InterfazUI aplicacion;
        aplicacion.ejecutar();
    } catch (const std::exception& e) {
        // Red de seguridad final para atrapar cualquier excepcion no controlada
        std::cerr << "\n[ERROR CRITICO NO CONTROLADO] " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "\n[ERROR DESCONOCIDO] El programa debio cerrarse de emergencia." << std::endl;
        return 2;
    }

    return 0;
}

