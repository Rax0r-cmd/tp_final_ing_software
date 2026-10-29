#ifndef TURNO_H
#define TURNO_H

#include <string>
#include <iostream>

// Forward declarations para la funcion amiga
class Medico;
class Paciente;

// Clase Turno: representa la relacion entre un Medico, un Paciente y una Fecha/Hora
class Turno {
private:
    int idTurno;
    int idMedico;
    int idPaciente;
    std::string fecha;   // Formato DD/MM/AAAA
    std::string hora;    // Formato HH:MM
    std::string motivo;
    std::string estado;  // "PENDIENTE", "CANCELADO", "ATENDIDO"

public:
    Turno();
    Turno(int idTurno, int idMedico, int idPaciente, const std::string& fecha, 
          const std::string& hora, const std::string& motivo, 
          const std::string& estado = "PENDIENTE");
    ~Turno();

    // Getters y Setters
    int getIdTurno() const;
    void setIdTurno(int nuevoId);

    int getIdMedico() const;
    void setIdMedico(int nuevoIdMed);

    int getIdPaciente() const;
    void setIdPaciente(int nuevoIdPac);

    std::string getFecha() const;
    void setFecha(const std::string& nuevaFecha);

    std::string getHora() const;
    void setHora(const std::string& nuevaHora);

    std::string getMotivo() const;
    void setMotivo(const std::string& nuevoMotivo);

    std::string getEstado() const;
    void setEstado(const std::string& nuevoEstado);

    // Metodo helper para obtener clave cronologica YYYYMMDDHHMM para ordenamiento
    std::string getClaveCronologica() const;

    // Sobrecarga de operadores (Requisito obligatorio)
    // 1) operator== : detecta colision de turnos (mismo medico y misma fecha/hora, o mismo id)
    bool operator==(const Turno& otro) const;

    // 2) operator< : permite ordenar cronologicamente los turnos en el Arbol Binario de Busqueda (ABB)
    bool operator<(const Turno& otro) const;

    // 3) Sobrecarga del operador de insercion de flujo (cout << turno)
    friend std::ostream& operator<<(std::ostream& os, const Turno& t);

    // Funcion friend justificada: genera un comprobante oficial de turno accediendo
    // de manera directa a los miembros privados de Turno, Medico y Paciente
    // sin romper el encapsulamiento para el resto del sistema.
    friend void generarComprobanteTurno(const Turno& t, const Medico& m, const Paciente& p);

    // Persistencia
    std::string toLineaArchivo() const;
    static Turno fromLineaArchivo(const std::string& linea);
};

#endif // TURNO_H

