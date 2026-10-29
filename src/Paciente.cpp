#include "../include/Paciente.h"
#include <iostream>
#include <sstream>
#include <vector>

Paciente::Paciente() 
    : Persona(), obraSocial(""), numeroAfiliado("") {}

Paciente::Paciente(int id, const std::string& dni, const std::string& nombre, 
                   const std::string& apellido, const std::string& telefono, 
                   const std::string& obraSocial, const std::string& numeroAfiliado, 
                   bool activo)
    : Persona(id, dni, nombre, apellido, telefono, activo), 
      obraSocial(obraSocial), numeroAfiliado(numeroAfiliado) {}

Paciente::~Paciente() {}

void Paciente::mostrarDatos() const {
    std::cout << "[ID: " << id << "] " << apellido << ", " << nombre 
              << " | DNI: " << dni 
              << " | OS: " << obraSocial 
              << " | Nro Afiliado: " << numeroAfiliado
              << " | Tel: " << telefono
              << " | Estado: " << (activo ? "Activo" : "Baja logica") 
              << std::endl;
}

std::string Paciente::getObraSocial() const { 
    return obraSocial; 
}

void Paciente::setObraSocial(const std::string& nuevaOS) { 
    obraSocial = nuevaOS; 
}

std::string Paciente::getNumeroAfiliado() const { 
    return numeroAfiliado; 
}

void Paciente::setNumeroAfiliado(const std::string& nuevoNro) { 
    numeroAfiliado = nuevoNro; 
}

std::string Paciente::toLineaArchivo() const {
    // Formato CSV delimitado por pipes: id|dni|nombre|apellido|telefono|obraSocial|numeroAfiliado|activo
    std::stringstream ss;
    ss << id << "|" << dni << "|" << nombre << "|" << apellido << "|" 
       << telefono << "|" << obraSocial << "|" << numeroAfiliado << "|" << (activo ? "1" : "0");
    return ss.str();
}

Paciente Paciente::fromLineaArchivo(const std::string& linea) {
    std::stringstream ss(linea);
    std::string item;
    std::vector<std::string> campos;
    while (std::getline(ss, item, '|')) {
        campos.push_back(item);
    }
    if (campos.size() < 8) {
        return Paciente();
    }
    int id = std::stoi(campos[0]);
    bool act = (campos[7] == "1");
    return Paciente(id, campos[1], campos[2], campos[3], campos[4], campos[5], campos[6], act);
}

