#ifndef COLORES_H
#define COLORES_H

#include <iostream>
#include <string>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

// Definicion de codigos de escape ANSI para colores en la consola
namespace Colores {
    const std::string RESET       = "\033[0m";
    const std::string NEGRITA     = "\033[1m";
    const std::string SUBRAYADO   = "\033[4m";

    // Colores de texto
    const std::string ROJO        = "\033[1;31m";
    const std::string VERDE       = "\033[1;32m";
    const std::string AMARILLO    = "\033[1;33m";
    const std::string AZUL        = "\033[1;34m";
    const std::string MAGENTA     = "\033[1;35m";
    const std::string CIAN        = "\033[1;36m";
    const std::string BLANCO      = "\033[1;37m";
    const std::string GRIS        = "\033[0;90m";

    // Habilita el soporte de secuencias ANSI en la consola de Windows 10/11
    inline void activarColoresWindows() {
#if defined(_WIN32) || defined(_WIN64)
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut != INVALID_HANDLE_VALUE) {
            DWORD dwMode = 0;
            if (GetConsoleMode(hOut, &dwMode)) {
                dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(hOut, dwMode);
            }
        }
#endif
    }
}

#endif // COLORES_H
