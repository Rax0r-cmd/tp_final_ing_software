#ifndef EXCEPCIONES_H
#define EXCEPCIONES_H

#include <exception>
#include <string>

// Clase base para las excepciones del sistema
// Hereda de std::exception para mantener compatibilidad con el manejo estandar de C++
class ClinicaException : public std::exception {
protected:
    std::string mensaje;
public:
    explicit ClinicaException(const std::string& msg) : mensaje(msg) {}
    virtual const char* what() const noexcept override {
        return mensaje.c_str();
    }
};

// Se lanza cuando se intenta asignar un turno en un horario ya reservado
class TurnoOcupadoException : public ClinicaException {
public:
    explicit TurnoOcupadoException(const std::string& msg = "El profesional o paciente ya cuenta con un turno asignado en ese horario.")
        : ClinicaException(msg) {}
};

// Se lanza cuando no existe el medico, paciente o turno solicitado
class EntidadNoEncontradaException : public ClinicaException {
public:
    explicit EntidadNoEncontradaException(const std::string& msg = "La entidad buscada no existe en el sistema.")
        : ClinicaException(msg) {}
};

// Se lanza cuando el usuario ingresa un dato que no cumple el formato esperado
class DatoInvalidoException : public ClinicaException {
public:
    explicit DatoInvalidoException(const std::string& msg = "El dato ingresado es invalido.")
        : ClinicaException(msg) {}
};

// Se lanza ante problemas de lectura o escritura en disco
class ArchivoException : public ClinicaException {
public:
    explicit ArchivoException(const std::string& msg = "Ocurrio un error al operar con el archivo de persistencia.")
        : ClinicaException(msg) {}
};

// Se lanza cuando se viola una regla del negocio (ej: dar turno a alguien dado de baja)
class ReglaNegocioException : public ClinicaException {
public:
    explicit ReglaNegocioException(const std::string& msg = "Operacion rechazada por regla de negocio.")
        : ClinicaException(msg) {}
};

#endif // EXCEPCIONES_H

