#ifndef PACIENTE_H
#define PACIENTE_H

#include "Persona.h"
#include <string>

// Forward declarations
class Turno;
class Medico;

// Entidad Paciente: hereda de Persona y contiene los datos de su cobertura medica
class Paciente : public Persona {
private:
    std::string obraSocial;
    std::string numeroAfiliado;

public:
    Paciente();
    Paciente(int id, const std::string& dni, const std::string& nombre, 
             const std::string& apellido, const std::string& telefono, 
             const std::string& obraSocial, const std::string& numeroAfiliado, 
             bool activo = true);
    virtual ~Paciente();

    // Sobrescritura polimorfica para los datos particulares del paciente
    void mostrarDatos() const override;

    std::string getObraSocial() const;
    void setObraSocial(const std::string& nuevaOS);

    std::string getNumeroAfiliado() const;
    void setNumeroAfiliado(const std::string& nuevoNro);

    // Funcion friend que emite el comprobante accediendo a atributos privados
    friend void generarComprobanteTurno(const Turno& t, const Medico& m, const Paciente& p);

    // Helpers para la persistencia en archivo
    std::string toLineaArchivo() const;
    static Paciente fromLineaArchivo(const std::string& linea);
};

#endif // PACIENTE_H
