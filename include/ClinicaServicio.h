#ifndef CLINICA_SERVICIO_H
#define CLINICA_SERVICIO_H

#include <vector>
#include <string>
#include "Medico.h"
#include "Paciente.h"
#include "Turno.h"
#include "ArbolABB.h"

// Clase de servicio que encapsula la logica de negocio, persistencia y colecciones
class ClinicaServicio {
private:
    std::vector<Medico> medicos;
    std::vector<Paciente> pacientes;
    std::vector<Turno> turnos;

    // Estructura de datos propia en template para indexar y ordenar turnos cronologicamente
    ArbolABB<Turno> arbolTurnos;

    // Rutas fijas a los archivos en la carpeta data
    const std::string rutaMedicos = "data/medicos.txt";
    const std::string rutaPacientes = "data/pacientes.txt";
    const std::string rutaTurnos = "data/turnos.txt";

    // Generadores de IDs correlativos
    int obtenerSiguienteIdMedico();
    int obtenerSiguienteIdPaciente();
    int obtenerSiguienteIdTurno();

    // Reconstruye el arbol binario a partir del vector de turnos en memoria
    void sincronizarArbol();

public:
    ClinicaServicio();
    ~ClinicaServicio();

    // Carga inicial y guardado en archivos
    void cargarDatos();
    void guardarDatos();

    // ================= CRUD MEDICOS =================
    void agregarMedico(const std::string& dni, const std::string& nombre, 
                       const std::string& apellido, const std::string& telefono, 
                       const std::string& matricula, const std::string& especialidad);
    void modificarMedico(int id, const std::string& telefono, 
                         const std::string& matricula, const std::string& especialidad);
    void bajaLogicaMedico(int id);
    void reactivarMedico(int id);
    Medico* buscarMedicoPorId(int id);
    Medico* buscarMedicoPorDni(const std::string& dni);
    const std::vector<Medico>& getMedicos() const;

    // ================= CRUD PACIENTES =================
    void agregarPaciente(const std::string& dni, const std::string& nombre, 
                         const std::string& apellido, const std::string& telefono, 
                         const std::string& obraSocial, const std::string& numeroAfiliado);
    void modificarPaciente(int id, const std::string& telefono, 
                           const std::string& obraSocial, const std::string& numeroAfiliado);
    void bajaLogicaPaciente(int id);
    void reactivarPaciente(int id);
    Paciente* buscarPacientePorId(int id);
    Paciente* buscarPacientePorDni(const std::string& dni);
    const std::vector<Paciente>& getPacientes() const;

    // ================= GESTION DE TURNOS =================
    int asignarTurno(int idMedico, int idPaciente, const std::string& fecha, 
                     const std::string& hora, const std::string& motivo);
    void cancelarTurno(int idTurno);
    void reprogramarTurno(int idTurno, const std::string& nuevaFecha, const std::string& nuevaHora);
    void cambiarEstadoTurno(int idTurno, const std::string& nuevoEstado);
    Turno* buscarTurnoPorId(int idTurno);
    
    // Listado cronologico usando el ArbolABB implementado (Recorrido inorden recursivo)
    std::vector<Turno> obtenerTurnosCronologicos() const;

    // Comprobante usando la funcion friend
    void imprimirTicket(int idTurno);

    // ================= REPORTES Y ESTADISTICAS =================
    std::vector<Turno> filtrarTurnosPorFecha(const std::string& fecha) const;
    std::vector<Turno> filtrarTurnosPorMedico(int idMedico) const;
    std::vector<Turno> filtrarTurnosPorPaciente(int idPaciente) const;

    // Estadisticas de uso de la clinica
    void obtenerEstadisticas(int& totalTurnos, int& pendientes, int& atendidos, int& cancelados) const;
};

#endif // CLINICA_SERVICIO_H

