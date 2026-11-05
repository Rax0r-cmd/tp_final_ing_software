#ifndef VALIDACIONES_H
#define VALIDACIONES_H

#include <string>

// Modulo de validacion y captura robusta de datos por consola.
// Evita que el programa se rompa si el usuario ingresa letras en vez de numeros,
// fechas inexistentes o cadenas vacias.
namespace Validaciones {
    // Lee un entero dentro de un rango cerrado [min, max]
    int leerEntero(const std::string& mensaje, int min, int max);

    // Lee un texto que no puede quedar vacio ni tener solo espacios
    std::string leerTextoNoVacio(const std::string& mensaje);

    // Lee un DNI valido (solo digitos, entre 7 y 8 numeros). Si permitirCancelar es true, '0' cancela.
    std::string leerDNI(const std::string& mensaje, bool permitirCancelar = false);

    // Lee un telefono valido (solo digitos, entre 8 y 15 numeros). Si permitirCancelar es true, '0' cancela.
    std::string leerTelefono(const std::string& mensaje, bool permitirCancelar = false);

    // Lee una matricula medica valida con formato (ej: MN-45120 o MP-38902). Si permitirCancelar es true, '0' cancela.
    std::string leerMatricula(const std::string& mensaje, bool permitirCancelar = false);

    // Lee una fecha con formato DD/MM/AAAA y valida que sea una fecha real del calendario. Si permitirCancelar es true, '0' cancela.
    std::string leerFecha(const std::string& mensaje, bool permitirCancelar = false);

    // Lee una hora con formato HH:MM en horario de atencion clinica (08:00 a 20:00)
    std::string leerHora(const std::string& mensaje);

    // Lee una confirmacion S o N
    bool confirmarAccion(const std::string& mensaje);

    // Pausa hasta presionar Enter
    void pausar();

    // Limpia la pantalla de consola de forma multiplataforma
    void limpiarPantalla();

    // Valida si un anio es bisiesto
    bool esBisiesto(int anio);

    // Valida si una fecha dada en dia, mes y anio existe en el calendario
    bool esFechaValida(int dia, int mes, int anio);
}

#endif // VALIDACIONES_H

