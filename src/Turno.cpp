#include "../include/Turno.h"
#include "../include/Medico.h"
#include "../include/Paciente.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

Turno::Turno() 
    : idTurno(0), idMedico(0), idPaciente(0), fecha(""), hora(""), motivo(""), estado("PENDIENTE") {}

Turno::Turno(int idTurno, int idMedico, int idPaciente, const std::string& fecha, 
             const std::string& hora, const std::string& motivo, const std::string& estado)
    : idTurno(idTurno), idMedico(idMedico), idPaciente(idPaciente), 
      fecha(fecha), hora(hora), motivo(motivo), estado(estado) {}

Turno::~Turno() {}

int Turno::getIdTurno() const { 
    return idTurno; 
}

void Turno::setIdTurno(int nuevoId) { 
    idTurno = nuevoId; 
}

int Turno::getIdMedico() const { 
    return idMedico; 
}

void Turno::setIdMedico(int nuevoIdMed) { 
    idMedico = nuevoIdMed; 
}

int Turno::getIdPaciente() const { 
    return idPaciente; 
}

void Turno::setIdPaciente(int nuevoIdPac) { 
    idPaciente = nuevoIdPac; 
}

std::string Turno::getFecha() const { 
    return fecha; 
}

void Turno::setFecha(const std::string& nuevaFecha) { 
    fecha = nuevaFecha; 
}

std::string Turno::getHora() const { 
    return hora; 
}

void Turno::setHora(const std::string& nuevaHora) { 
    hora = nuevaHora; 
}

std::string Turno::getMotivo() const { 
    return motivo; 
}

void Turno::setMotivo(const std::string& nuevoMotivo) { 
    motivo = nuevoMotivo; 
}

std::string Turno::getEstado() const { 
    return estado; 
}

void Turno::setEstado(const std::string& nuevoEstado) { 
    estado = nuevoEstado; 
}

// Convierte la fecha DD/MM/AAAA y hora HH:MM a una clave comparable "AAAAMMDD_HHMM_ID"
std::string Turno::getClaveCronologica() const {
    if (fecha.length() >= 10 && hora.length() >= 5) {
        std::string dia = fecha.substr(0, 2);
        std::string mes = fecha.substr(3, 2);
        std::string anio = fecha.substr(6, 4);
        std::string hh = hora.substr(0, 2);
        std::string mm = hora.substr(3, 2);
        return anio + mes + dia + "_" + hh + mm + "_" + std::to_string(idTurno);
    }
    return fecha + "_" + hora + "_" + std::to_string(idTurno);
}

// Sobrecarga del operador == para detectar solapamiento de horario o coincidencia de turno
bool Turno::operator==(const Turno& otro) const {
    // Si tienen el mismo ID, representan el mismo turno
    if (this->idTurno != 0 && otro.idTurno != 0 && this->idTurno == otro.idTurno) {
        return true;
    }
    // Si comparten el mismo medico, fecha y hora (y ninguno esta cancelado), hay solapamiento
    if (this->idMedico == otro.idMedico && 
        this->fecha == otro.fecha && 
        this->hora == otro.hora && 
        this->estado != "CANCELADO" && otro.estado != "CANCELADO") {
        return true;
    }
    return false;
}

// Sobrecarga del operador < para permitir ordenar cronologicamente en el ABB
bool Turno::operator<(const Turno& otro) const {
    return this->getClaveCronologica() < otro.getClaveCronologica();
}

// Sobrecarga de << para imprimir una linea resumen del turno
std::ostream& operator<<(std::ostream& os, const Turno& t) {
    os << "[Turno #" << t.idTurno << "] "
       << t.fecha << " " << t.hora 
       << " | Medico ID: " << t.idMedico 
       << " | Paciente ID: " << t.idPaciente 
       << " | Estado: " << t.estado 
       << " | Motivo: " << t.motivo;
    return os;
}

// Funcion friend: accede a los miembros privados de las tres clases colaboradoras
// para generar un ticket de turno profesional sin exponer datos con getters publicos adicionales.
void generarComprobanteTurno(const Turno& t, const Medico& m, const Paciente& p) {
    std::cout << "\n============================================================\n";
    std::cout << "                 COMPROBANTE OFICIAL DE TURNO               \n";
    std::cout << "============================================================\n";
    std::cout << " Turno Nro:       #" << t.idTurno << "\n";
    std::cout << " Fecha y Hora:    " << t.fecha << " a las " << t.hora << " hs\n";
    std::cout << " Estado actual:   " << t.estado << "\n";
    std::cout << " Motivo consulta: " << t.motivo << "\n";
    std::cout << "------------------------------------------------------------\n";
    std::cout << " Profesional:     Dr/a. " << m.apellido << ", " << m.nombre << "\n";
    std::cout << " Especialidad:    " << m.especialidad << "\n";
    std::cout << " Matricula:       " << m.matricula << "\n";
    std::cout << "------------------------------------------------------------\n";
    std::cout << " Paciente:        " << p.apellido << ", " << p.nombre << "\n";
    std::cout << " DNI:             " << p.dni << "\n";
    std::cout << " Obra Social:     " << p.obraSocial << " (Afiliado: " << p.numeroAfiliado << ")\n";
    std::cout << "============================================================\n\n";
}

std::string Turno::toLineaArchivo() const {
    // idTurno|idMedico|idPaciente|fecha|hora|motivo|estado
    std::stringstream ss;
    ss << idTurno << "|" << idMedico << "|" << idPaciente << "|" 
       << fecha << "|" << hora << "|" << motivo << "|" << estado;
    return ss.str();
}

Turno Turno::fromLineaArchivo(const std::string& linea) {
    std::stringstream ss(linea);
    std::string item;
    std::vector<std::string> campos;
    while (std::getline(ss, item, '|')) {
        campos.push_back(item);
    }
    if (campos.size() < 7) {
        return Turno();
    }
    int idT = std::stoi(campos[0]);
    int idM = std::stoi(campos[1]);
    int idP = std::stoi(campos[2]);
    return Turno(idT, idM, idP, campos[3], campos[4], campos[5], campos[6]);
}

