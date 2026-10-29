#include "../include/Validaciones.h"
#include <iostream>
#include <limits>
#include <cctype>
#include <sstream>
#include <cstdlib>

namespace Validaciones {

bool esBisiesto(int anio) {
    return (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
}

bool esFechaValida(int dia, int mes, int anio) {
    if (anio < 2024 || anio > 2035) return false;
    if (mes < 1 || mes > 12) return false;

    int diasPorMes[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (mes == 2 && esBisiesto(anio)) {
        diasPorMes[2] = 29;
    }

    return (dia >= 1 && dia <= diasPorMes[mes]);
}

int leerEntero(const std::string& mensaje, int min, int max) {
    int valor;
    while (true) {
        std::cout << mensaje;
        if (std::cin >> valor) {
            // Verificamos que este dentro del rango permitido
            if (valor >= min && valor <= max) {
                // Limpiamos los restos de la linea
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return valor;
            } else {
                std::cout << " [!] El valor debe estar entre " << min << " y " << max << ". Intente nuevamente.\n";
            }
        } else {
            // Si el usuario metio letras o simbolos, cin entra en estado de falla
            std::cout << " [!] Entrada invalida. Por favor ingrese un numero entero.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

std::string leerTextoNoVacio(const std::string& mensaje) {
    std::string texto;
    while (true) {
        std::cout << mensaje;
        std::getline(std::cin, texto);

        // Eliminamos espacios al inicio y al final
        size_t inicio = texto.find_first_not_of(" \t\r\n");
        if (inicio != std::string::npos) {
            size_t fin = texto.find_last_not_of(" \t\r\n");
            std::string limpio = texto.substr(inicio, fin - inicio + 1);
            if (!limpio.empty()) {
                return limpio;
            }
        }
        std::cout << " [!] Este campo no puede quedar vacio. Ingrese un texto valido.\n";
    }
}

std::string leerDNI(const std::string& mensaje) {
    while (true) {
        std::string dni = leerTextoNoVacio(mensaje);
        bool soloNumeros = true;
        for (char c : dni) {
            if (!std::isdigit(static_cast<unsigned char>(c))) {
                soloNumeros = false;
                break;
            }
        }

        if (soloNumeros && dni.length() >= 7 && dni.length() <= 8) {
            return dni;
        }

        std::cout << " [!] DNI invalido. Debe contener entre 7 y 8 digitos numericos sin puntos.\n";
    }
}

std::string leerFecha(const std::string& mensaje) {
    while (true) {
        std::string entrada = leerTextoNoVacio(mensaje + " (DD/MM/AAAA): ");
        if (entrada.length() == 10 && entrada[2] == '/' && entrada[5] == '/') {
            try {
                int d = std::stoi(entrada.substr(0, 2));
                int m = std::stoi(entrada.substr(3, 2));
                int a = std::stoi(entrada.substr(6, 4));

                if (esFechaValida(d, m, a)) {
                    return entrada;
                }
            } catch (...) {
                // Si la conversion falla, cae en el mensaje de error de abajo
            }
        }
        std::cout << " [!] Fecha invalida o imposible. Use el formato DD/MM/AAAA (ej: 15/11/2026).\n";
    }
}

std::string leerHora(const std::string& mensaje) {
    while (true) {
        std::string entrada = leerTextoNoVacio(mensaje + " (HH:MM entre 08:00 y 20:00): ");
        if (entrada.length() == 5 && entrada[2] == ':') {
            try {
                int h = std::stoi(entrada.substr(0, 2));
                int m = std::stoi(entrada.substr(3, 2));

                if (h >= 8 && h <= 20 && m >= 0 && m <= 59) {
                    return entrada;
                }
            } catch (...) {
                // Captura error de parseo numerico
            }
        }
        std::cout << " [!] Horario invalido. Debe ser entre las 08:00 y las 20:00 hs con formato HH:MM.\n";
    }
}

bool confirmarAccion(const std::string& mensaje) {
    while (true) {
        std::cout << mensaje << " (S/N): ";
        std::string opc;
        std::getline(std::cin, opc);
        if (!opc.empty()) {
            char c = std::toupper(opc[0]);
            if (c == 'S') return true;
            if (c == 'N') return false;
        }
        std::cout << " [!] Opcion invalida. Ingrese 'S' para Si o 'N' para No.\n";
    }
}

void pausar() {
    std::cout << "\nPresione [Enter] para continuar...";
    std::cin.get();
}

void limpiarPantalla() {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
}

} // namespace Validaciones

