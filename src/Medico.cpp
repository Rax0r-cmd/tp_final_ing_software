#include "../include/Medico.h"
#include <iostream>
#include <sstream>
#include <vector>

Medico::Medico() 
    : Persona(), matricula(""), especialidad("") {}

Medico::Medico(int id, const std::string& dni, const std::string& nombre, 
               const std::string& apellido, const std::string& telefono, 
               const std::string& matricula, const std::string& especialidad, 
               bool activo)
    : Persona(id, dni, nombre, apellido, telefono, activo), 
      matricula(matricula), especialidad(especialidad) {}

Medico::~Medico() {}

void Medico::mostrarDatos() const {
    std::cout << "[ID: " << id << "] Dr/a. " << apellido << ", " << nombre 
              << " | DNI: " << dni 
              << " | Mat: " << matricula 
              << " | Especialidad: " << especialidad
              << " | Tel: " << telefono
              << " | Estado: " << (activo ? "Activo" : "Baja logica") 
              << std::endl;
}

std::string Medico::getMatricula() const { 
    return matricula; 
}

void Medico::setMatricula(const std::string& nuevaMatricula) { 
    matricula = nuevaMatricula; 
}

std::string Medico::getEspecialidad() const { 
    return especialidad; 
}

void Medico::setEspecialidad(const std::string& nuevaEsp) { 
    especialidad = nuevaEsp; 
}

std::string Medico::toLineaArchivo() const {
    // Formato CSV delimitado por pipes: id|dni|nombre|apellido|telefono|matricula|especialidad|activo
    std::stringstream ss;
    ss << id << "|" << dni << "|" << nombre << "|" << apellido << "|" 
       << telefono << "|" << matricula << "|" << especialidad << "|" << (activo ? "1" : "0");
    return ss.str();
}

Medico Medico::fromLineaArchivo(const std::string& linea) {
    std::stringstream ss(linea);
    std::string item;
    std::vector<std::string> campos;
    while (std::getline(ss, item, '|')) {
        campos.push_back(item);
    }
    if (campos.size() < 8) {
        return Medico();
    }
    int id = std::stoi(campos[0]);
    bool act = (campos[7] == "1");
    return Medico(id, campos[1], campos[2], campos[3], campos[4], campos[5], campos[6], act);
}

