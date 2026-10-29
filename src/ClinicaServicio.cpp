#include "../include/ClinicaServicio.h"
#include "../include/GestorArchivos.h"
#include "../include/Excepciones.h"
#include <algorithm>
#include <iostream>
#include <fstream>

ClinicaServicio::ClinicaServicio() {
    cargarDatos();
}

ClinicaServicio::~ClinicaServicio() {
    try {
        guardarDatos();
    } catch (...) {
        // Evitamos propagar excepciones en destructores
    }
}

void ClinicaServicio::sincronizarArbol() {
    arbolTurnos.vaciar();
    for (const auto& t : turnos) {
        arbolTurnos.insertar(t);
    }
    // Guardamos la estructura del arbol persistida en disco
    arbolTurnos.guardarEstructura(rutaArbol);
}

void ClinicaServicio::cargarDatos() {
    try {
        GestorArchivos<Medico>::cargar(rutaMedicos, medicos);
        GestorArchivos<Paciente>::cargar(rutaPacientes, pacientes);
        GestorArchivos<Turno>::cargar(rutaTurnos, turnos);
        
        // Intentamos cargar la estructura persistida del arbol directamente
        std::ifstream fArbol(rutaArbol);
        if (fArbol.is_open() && fArbol.peek() != std::ifstream::traits_type::eof()) {
            fArbol.close();
            arbolTurnos.cargarEstructura(rutaArbol);
        } else {
            if (fArbol.is_open()) fArbol.close();
            sincronizarArbol();
        }
    } catch (const ArchivoException& e) {
        std::cerr << "[Aviso] " << e.what() << "\n";
    }
}

void ClinicaServicio::guardarDatos() {
    GestorArchivos<Medico>::guardar(rutaMedicos, medicos);
    GestorArchivos<Paciente>::guardar(rutaPacientes, pacientes);
    GestorArchivos<Turno>::guardar(rutaTurnos, turnos);
    // Persistencia explicita de la estructura de datos
    arbolTurnos.guardarEstructura(rutaArbol);
}

int ClinicaServicio::obtenerSiguienteIdMedico() {
    int maxId = 0;
    for (const auto& m : medicos) {
        if (m.getId() > maxId) maxId = m.getId();
    }
    return maxId + 1;
}

int ClinicaServicio::obtenerSiguienteIdPaciente() {
    int maxId = 0;
    for (const auto& p : pacientes) {
        if (p.getId() > maxId) maxId = p.getId();
    }
    return maxId + 1;
}

int ClinicaServicio::obtenerSiguienteIdTurno() {
    int maxId = 0;
    for (const auto& t : turnos) {
        if (t.getIdTurno() > maxId) maxId = t.getIdTurno();
    }
    return maxId + 1;
}

// ================= CRUD MEDICOS =================

void ClinicaServicio::agregarMedico(const std::string& dni, const std::string& nombre, 
                                   const std::string& apellido, const std::string& telefono, 
                                   const std::string& matricula, const std::string& especialidad) {
    if (buscarMedicoPorDni(dni) != nullptr) {
        throw ReglaNegocioException("Ya existe un medico registrado con el DNI " + dni);
    }

    for (const auto& m : medicos) {
        if (m.getMatricula() == matricula) {
            throw ReglaNegocioException("Ya existe un medico registrado con la matricula " + matricula);
        }
    }

    int nuevoId = obtenerSiguienteIdMedico();
    Medico nuevo(nuevoId, dni, nombre, apellido, telefono, matricula, especialidad, true);
    medicos.push_back(nuevo);
    guardarDatos();
}

void ClinicaServicio::modificarMedico(int id, const std::string& telefono, 
                                     const std::string& matricula, const std::string& especialidad) {
    Medico* m = buscarMedicoPorId(id);
    if (!m) {
        throw EntidadNoEncontradaException("No se encontro un medico con el ID indicado.");
    }

    for (const auto& otro : medicos) {
        if (otro.getId() != id && otro.getMatricula() == matricula) {
            throw ReglaNegocioException("La matricula ya pertenece a otro profesional.");
        }
    }

    m->setTelefono(telefono);
    m->setMatricula(matricula);
    m->setEspecialidad(especialidad);
    guardarDatos();
}

void ClinicaServicio::bajaLogicaMedico(int id) {
    Medico* m = buscarMedicoPorId(id);
    if (!m) {
        throw EntidadNoEncontradaException("No se encontro el medico para dar de baja.");
    }
    if (!m->isActivo()) {
        throw ReglaNegocioException("El medico ya se encuentra dado de baja.");
    }
    m->setActivo(false);
    guardarDatos();
}

void ClinicaServicio::reactivarMedico(int id) {
    Medico* m = buscarMedicoPorId(id);
    if (!m) {
        throw EntidadNoEncontradaException("No se encontro el medico.");
    }
    m->setActivo(true);
    guardarDatos();
}

Medico* ClinicaServicio::buscarMedicoPorId(int id) {
    for (auto& m : medicos) {
        if (m.getId() == id) return &m;
    }
    return nullptr;
}

Medico* ClinicaServicio::buscarMedicoPorDni(const std::string& dni) {
    for (auto& m : medicos) {
        if (m.getDni() == dni) return &m;
    }
    return nullptr;
}

const std::vector<Medico>& ClinicaServicio::getMedicos() const {
    return medicos;
}

// ================= CRUD PACIENTES =================

void ClinicaServicio::agregarPaciente(const std::string& dni, const std::string& nombre, 
                                     const std::string& apellido, const std::string& telefono, 
                                     const std::string& obraSocial, const std::string& numeroAfiliado) {
    if (buscarPacientePorDni(dni) != nullptr) {
        throw ReglaNegocioException("Ya existe un paciente registrado con el DNI " + dni);
    }

    int nuevoId = obtenerSiguienteIdPaciente();
    Paciente nuevo(nuevoId, dni, nombre, apellido, telefono, obraSocial, numeroAfiliado, true);
    pacientes.push_back(nuevo);
    guardarDatos();
}

void ClinicaServicio::modificarPaciente(int id, const std::string& telefono, 
                                       const std::string& obraSocial, const std::string& numeroAfiliado) {
    Paciente* p = buscarPacientePorId(id);
    if (!p) {
        throw EntidadNoEncontradaException("No se encontro el paciente especificado.");
    }

    p->setTelefono(telefono);
    p->setObraSocial(obraSocial);
    p->setNumeroAfiliado(numeroAfiliado);
    guardarDatos();
}

void ClinicaServicio::bajaLogicaPaciente(int id) {
    Paciente* p = buscarPacientePorId(id);
    if (!p) {
        throw EntidadNoEncontradaException("No se encontro el paciente para dar de baja.");
    }
    if (!p->isActivo()) {
        throw ReglaNegocioException("El paciente ya se encuentra dado de baja.");
    }
    p->setActivo(false);
    guardarDatos();
}

void ClinicaServicio::reactivarPaciente(int id) {
    Paciente* p = buscarPacientePorId(id);
    if (!p) {
        throw EntidadNoEncontradaException("No se encontro el paciente.");
    }
    p->setActivo(true);
    guardarDatos();
}

Paciente* ClinicaServicio::buscarPacientePorId(int id) {
    for (auto& p : pacientes) {
        if (p.getId() == id) return &p;
    }
    return nullptr;
}

Paciente* ClinicaServicio::buscarPacientePorDni(const std::string& dni) {
    for (auto& p : pacientes) {
        if (p.getDni() == dni) return &p;
    }
    return nullptr;
}

const std::vector<Paciente>& ClinicaServicio::getPacientes() const {
    return pacientes;
}

// ================= GESTION DE TURNOS =================

int ClinicaServicio::asignarTurno(int idMedico, int idPaciente, const std::string& fecha, 
                                 const std::string& hora, const std::string& motivo) {
    Medico* med = buscarMedicoPorId(idMedico);
    if (!med) {
        throw EntidadNoEncontradaException("El medico con ID " + std::to_string(idMedico) + " no existe.");
    }
    if (!med->isActivo()) {
        throw ReglaNegocioException("El medico seleccionado esta de baja logica. No puede recibir turnos.");
    }

    Paciente* pac = buscarPacientePorId(idPaciente);
    if (!pac) {
        throw EntidadNoEncontradaException("El paciente con ID " + std::to_string(idPaciente) + " no existe.");
    }
    if (!pac->isActivo()) {
        throw ReglaNegocioException("El paciente seleccionado esta de baja logica.");
    }

    for (const auto& t : turnos) {
        if (t.getIdMedico() == idMedico && t.getFecha() == fecha && t.getHora() == hora && t.getEstado() != "CANCELADO") {
            throw TurnoOcupadoException("El Dr/a. " + med->getApellido() + " ya tiene un turno reservado el " + fecha + " a las " + hora);
        }
    }

    for (const auto& t : turnos) {
        if (t.getIdPaciente() == idPaciente && t.getFecha() == fecha && t.getHora() == hora && t.getEstado() != "CANCELADO") {
            throw TurnoOcupadoException("El paciente " + pac->getApellido() + " ya tiene un turno programado en esa misma fecha y hora.");
        }
    }

    int idNuevoTurno = obtenerSiguienteIdTurno();
    Turno nuevo(idNuevoTurno, idMedico, idPaciente, fecha, hora, motivo, "PENDIENTE");
    turnos.push_back(nuevo);
    sincronizarArbol();
    guardarDatos();

    return idNuevoTurno;
}

void ClinicaServicio::cancelarTurno(int idTurno) {
    Turno* t = buscarTurnoPorId(idTurno);
    if (!t) {
        throw EntidadNoEncontradaException("No se encontro un turno con el ID " + std::to_string(idTurno));
    }
    if (t->getEstado() == "CANCELADO") {
        throw ReglaNegocioException("El turno ya se encuentra cancelado.");
    }

    t->setEstado("CANCELADO");
    sincronizarArbol();
    guardarDatos();
}

void ClinicaServicio::reprogramarTurno(int idTurno, const std::string& nuevaFecha, const std::string& nuevaHora) {
    Turno* t = buscarTurnoPorId(idTurno);
    if (!t) {
        throw EntidadNoEncontradaException("No se encontro el turno a reprogramar.");
    }
    if (t->getEstado() == "CANCELADO") {
        throw ReglaNegocioException("No se puede reprogramar un turno cancelado. Asigne un nuevo turno.");
    }

    for (const auto& otro : turnos) {
        if (otro.getIdTurno() != idTurno && 
            otro.getIdMedico() == t->getIdMedico() && 
            otro.getFecha() == nuevaFecha && 
            otro.getHora() == nuevaHora && 
            otro.getEstado() != "CANCELADO") {
            throw TurnoOcupadoException("El medico ya tiene un turno asignado para la nueva fecha y hora.");
        }
    }

    for (const auto& otro : turnos) {
        if (otro.getIdTurno() != idTurno && 
            otro.getIdPaciente() == t->getIdPaciente() && 
            otro.getFecha() == nuevaFecha && 
            otro.getHora() == nuevaHora && 
            otro.getEstado() != "CANCELADO") {
            throw TurnoOcupadoException("El paciente ya posee otro turno en ese horario.");
        }
    }

    t->setFecha(nuevaFecha);
    t->setHora(nuevaHora);
    t->setEstado("PENDIENTE");

    sincronizarArbol();
    guardarDatos();
}

void ClinicaServicio::cambiarEstadoTurno(int idTurno, const std::string& nuevoEstado) {
    Turno* t = buscarTurnoPorId(idTurno);
    if (!t) {
        throw EntidadNoEncontradaException("No se encontro el turno solicitado.");
    }
    t->setEstado(nuevoEstado);
    sincronizarArbol();
    guardarDatos();
}

Turno* ClinicaServicio::buscarTurnoPorId(int idTurno) {
    for (auto& t : turnos) {
        if (t.getIdTurno() == idTurno) return &t;
    }
    return nullptr;
}

std::vector<Turno> ClinicaServicio::obtenerTurnosCronologicos() const {
    return arbolTurnos.obtenerInorden();
}

void ClinicaServicio::mostrarEstructuraArbol() const {
    arbolTurnos.imprimirGrafico();
}

int ClinicaServicio::getAlturaArbol() const {
    return arbolTurnos.getAltura();
}

void ClinicaServicio::imprimirTicket(int idTurno) {
    Turno* t = buscarTurnoPorId(idTurno);
    if (!t) {
        throw EntidadNoEncontradaException("Turno no encontrado para emitir comprobante.");
    }
    Medico* m = buscarMedicoPorId(t->getIdMedico());
    Paciente* p = buscarPacientePorId(t->getIdPaciente());

    if (!m || !p) {
        throw ReglaNegocioException("No se pueden recuperar los datos del medico o paciente asociados al turno.");
    }

    generarComprobanteTurno(*t, *m, *p);
}

// ================= REPORTES Y ESTADISTICAS =================

std::vector<Turno> ClinicaServicio::filtrarTurnosPorFecha(const std::string& fecha) const {
    std::vector<Turno> lista;
    auto ordenados = arbolTurnos.obtenerInorden();
    for (const auto& t : ordenados) {
        if (t.getFecha() == fecha) {
            lista.push_back(t);
        }
    }
    return lista;
}

std::vector<Turno> ClinicaServicio::filtrarTurnosPorMedico(int idMedico) const {
    std::vector<Turno> lista;
    auto ordenados = arbolTurnos.obtenerInorden();
    for (const auto& t : ordenados) {
        if (t.getIdMedico() == idMedico) {
            lista.push_back(t);
        }
    }
    return lista;
}

std::vector<Turno> ClinicaServicio::filtrarTurnosPorPaciente(int idPaciente) const {
    std::vector<Turno> lista;
    auto ordenados = arbolTurnos.obtenerInorden();
    for (const auto& t : ordenados) {
        if (t.getIdPaciente() == idPaciente) {
            lista.push_back(t);
        }
    }
    return lista;
}

void ClinicaServicio::obtenerEstadisticas(int& totalTurnos, int& pendientes, int& atendidos, int& cancelados) const {
    totalTurnos = turnos.size();
    pendientes = 0;
    atendidos = 0;
    cancelados = 0;

    for (const auto& t : turnos) {
        if (t.getEstado() == "PENDIENTE") pendientes++;
        else if (t.getEstado() == "ATENDIDO") atendidos++;
        else if (t.getEstado() == "CANCELADO") cancelados++;
    }
}

void ClinicaServicio::exportarReporteTexto(const std::string& nombreArchivo, const std::string& contenido) const {
    std::string ruta = "data/" + nombreArchivo;
    std::ofstream salida(ruta);
    if (!salida.is_open()) {
        throw ArchivoException("No se pudo exportar el reporte en " + ruta);
    }
    salida << contenido;
    salida.close();
}
