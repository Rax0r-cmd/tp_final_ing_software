#include "../include/Persona.h"

// Inicializacion de la variable estatica en tiempo de compilacion
int Persona::totalPersonas = 0;

Persona::Persona() 
    : id(0), dni(""), nombre(""), apellido(""), telefono(""), activo(true) {
    totalPersonas++;
}

Persona::Persona(int id, const std::string& dni, const std::string& nombre, 
                 const std::string& apellido, const std::string& telefono, bool activo)
    : id(id), dni(dni), nombre(nombre), apellido(apellido), telefono(telefono), activo(activo) {
    totalPersonas++;
}

// Constructor de copia: fundamental para mantener la cuenta estatica exacta
// cuando se insertan objetos en std::vector
Persona::Persona(const Persona& otra)
    : id(otra.id), dni(otra.dni), nombre(otra.nombre), 
      apellido(otra.apellido), telefono(otra.telefono), activo(otra.activo) {
    totalPersonas++;
}

Persona& Persona::operator=(const Persona& otra) {
    if (this != &otra) {
        id = otra.id;
        dni = otra.dni;
        nombre = otra.nombre;
        apellido = otra.apellido;
        telefono = otra.telefono;
        activo = otra.activo;
    }
    return *this;
}

Persona::~Persona() {
    totalPersonas--;
}

int Persona::getId() const { 
    return id; 
}

void Persona::setId(int nuevoId) { 
    id = nuevoId; 
}

std::string Persona::getDni() const { 
    return dni; 
}

void Persona::setDni(const std::string& nuevoDni) { 
    dni = nuevoDni; 
}

std::string Persona::getNombre() const { 
    return nombre; 
}

void Persona::setNombre(const std::string& nuevoNombre) { 
    nombre = nuevoNombre; 
}

std::string Persona::getApellido() const { 
    return apellido; 
}

void Persona::setApellido(const std::string& nuevoApellido) { 
    apellido = nuevoApellido; 
}

std::string Persona::getTelefono() const { 
    return telefono; 
}

void Persona::setTelefono(const std::string& nuevoTel) { 
    telefono = nuevoTel; 
}

bool Persona::isActivo() const { 
    return activo; 
}

void Persona::setActivo(bool estado) { 
    activo = estado; 
}

int Persona::getTotalPersonas() { 
    return totalPersonas; 
}
