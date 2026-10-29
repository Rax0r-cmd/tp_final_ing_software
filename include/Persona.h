#ifndef PERSONA_H
#define PERSONA_H

#include <string>
#include <iostream>

// Clase base abstracta. Aplicamos abstraccion para modelar los atributos
// y comportamientos comunes entre medicos y pacientes sin repetir codigo.
class Persona {
protected:
    int id;
    std::string dni;
    std::string nombre;
    std::string apellido;
    std::string telefono;
    bool activo; // Para la baja logica

    // Atributo estatico para llevar la cuenta global de personas cargadas en memoria (visto en tema Estaticos)
    static int totalPersonas;

public:
    Persona();
    Persona(int id, const std::string& dni, const std::string& nombre, 
            const std::string& apellido, const std::string& telefono, bool activo = true);
    Persona(const Persona& otra);
    Persona& operator=(const Persona& otra);
    virtual ~Persona();

    // Metodo virtual puro: convierte a Persona en clase abstracta.
    // Obliga a que cada subclase defina como mostrar su propia informacion.
    virtual void mostrarDatos() const = 0;

    // Getters y Setters
    int getId() const;
    void setId(int nuevoId);

    std::string getDni() const;
    void setDni(const std::string& nuevoDni);

    std::string getNombre() const;
    void setNombre(const std::string& nuevoNombre);

    std::string getApellido() const;
    void setApellido(const std::string& nuevoApellido);

    std::string getTelefono() const;
    void setTelefono(const std::string& nuevoTel);

    bool isActivo() const;
    void setActivo(bool estado);

    // Metodo estatico para consultar el contador
    static int getTotalPersonas();
};

#endif // PERSONA_H
