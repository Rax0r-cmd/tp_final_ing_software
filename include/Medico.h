#ifndef MEDICO_H
#define MEDICO_H

#include "Persona.h"
#include <string>

// Forward declarations
class Turno;
class Paciente;

// Entidad Medico: hereda de Persona y le suma sus campos profesionales
class Medico : public Persona {
private:
    std::string matricula;
    std::string especialidad;

public:
    Medico();
    Medico(int id, const std::string& dni, const std::string& nombre, 
           const std::string& apellido, const std::string& telefono, 
           const std::string& matricula, const std::string& especialidad, 
           bool activo = true);
    virtual ~Medico();

    // Sobrescritura polimorfica para imprimir los datos especificos del profesional
    void mostrarDatos() const override;

    std::string getMatricula() const;
    void setMatricula(const std::string& nuevaMatricula);

    std::string getEspecialidad() const;
    void setEspecialidad(const std::string& nuevaEsp);

    // Funcion friend que emite el comprobante accediendo a atributos privados
    friend void generarComprobanteTurno(const Turno& t, const Medico& m, const Paciente& p);

    // Helpers para convertir de/hacia linea de texto para el archivo
    std::string toLineaArchivo() const;
    static Medico fromLineaArchivo(const std::string& linea);
};

#endif // MEDICO_H
