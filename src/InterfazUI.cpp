#include "../include/InterfazUI.h"
#include "../include/Validaciones.h"
#include "../include/Excepciones.h"
#include <iostream>
#include <iomanip>

using namespace Validaciones;

InterfazUI::InterfazUI() {}

void InterfazUI::imprimirBanner(const std::string& titulo) {
    limpiarPantalla();
    std::cout << "========================================================================================\n";
    std::cout << "             SISTEMA DE GESTION DE TURNOS MEDICOS - CLINICA CENTRAL                     \n";
    std::cout << "                      " << titulo << "\n";
    std::cout << "========================================================================================\n\n";
}

void InterfazUI::ejecutar() {
    int opcion = -1;
    do {
        imprimirBanner("MENU PRINCIPAL");
        std::cout << "  [1] Modulo de Medicos (ABML)\n";
        std::cout << "  [2] Modulo de Pacientes (ABML)\n";
        std::cout << "  [3] Asignacion y Control de Turnos\n";
        std::cout << "  [4] Reportes y Estadisticas del Sistema\n";
        std::cout << "  [0] Salir y Guardar Cambios\n";
        std::cout << "----------------------------------------------------------------------------------------\n";
        opcion = leerEntero("Seleccione una opcion [0-4]: ", 0, 4);

        switch (opcion) {
            case 1: menuMedicos(); break;
            case 2: menuPacientes(); break;
            case 3: menuTurnos(); break;
            case 4: menuReportes(); break;
            case 0:
                std::cout << "\nGuardando informacion en disco...\n";
                servicio.guardarDatos();
                std::cout << "Muchas gracias por utilizar el sistema. Hasta luego!\n";
                break;
        }
    } while (opcion != 0);
}

// ==========================================
// SECCION DE MEDICOS
// ==========================================

void InterfazUI::menuMedicos() {
    int opc = -1;
    do {
        imprimirBanner("GESTION DE MEDICOS");
        std::cout << "  [1] Dar de alta nuevo medico\n";
        std::cout << "  [2] Modificar datos de contacto o especialidad\n";
        std::cout << "  [3] Dar de baja logica a un medico\n";
        std::cout << "  [4] Reactivar medico dado de baja\n";
        std::cout << "  [5] Listar medicos registrados\n";
        std::cout << "  [6] Buscar medico por DNI o ID\n";
        std::cout << "  [0] Volver al menu principal\n";
        std::cout << "----------------------------------------------------------------------------------------\n";
        opc = leerEntero("Seleccione una opcion [0-6]: ", 0, 6);

        switch (opc) {
            case 1: vistaAltaMedico(); break;
            case 2: vistaModificarMedico(); break;
            case 3: vistaBajaMedico(); break;
            case 4: vistaReactivarMedico(); break;
            case 5: vistaListarMedicos(); break;
            case 6: vistaBuscarMedico(); break;
        }
    } while (opc != 0);
}

void InterfazUI::vistaAltaMedico() {
    imprimirBanner("ALTA DE MEDICO");
    std::cout << "Complete los siguientes datos del profesional:\n\n";

    std::string dni = leerDNI("Ingrese DNI (sin puntos): ");
    std::string nombre = leerTextoNoVacio("Ingrese Nombre: ");
    std::string apellido = leerTextoNoVacio("Ingrese Apellido: ");
    std::string telefono = leerTextoNoVacio("Ingrese Telefono de contacto: ");
    std::string matricula = leerTextoNoVacio("Ingrese Nro de Matricula: ");
    std::string especialidad = leerTextoNoVacio("Ingrese Especialidad (ej: Cardiologia, Pediatria): ");

    try {
        servicio.agregarMedico(dni, nombre, apellido, telefono, matricula, especialidad);
        std::cout << "\n[OK] El profesional fue registrado exitosamente en el sistema.\n";
    } catch (const ClinicaException& e) {
        std::cout << "\n[ERROR] " << e.what() << "\n";
    }

    pausar();
}

void InterfazUI::vistaModificarMedico() {
    imprimirBanner("MODIFICACION DE MEDICO");
    int id = leerEntero("Ingrese el ID del medico a modificar: ", 1, 99999);

    Medico* m = servicio.buscarMedicoPorId(id);
    if (!m) {
        std::cout << "\n[ERROR] No existe ningun medico con ese ID.\n";
        pausar();
        return;
    }

    std::cout << "\nDatos actuales:\n";
    m->mostrarDatos();
    std::cout << "\nIngrese los nuevos datos (o repita los actuales si no desea cambiarlos):\n";

    std::string nuevoTel = leerTextoNoVacio("Nuevo Telefono: ");
    std::string nuevaMat = leerTextoNoVacio("Nueva Matricula: ");
    std::string nuevaEsp = leerTextoNoVacio("Nueva Especialidad: ");

    try {
        servicio.modificarMedico(id, nuevoTel, nuevaMat, nuevaEsp);
        std::cout << "\n[OK] Los datos del medico se actualizaron correctamente.\n";
    } catch (const ClinicaException& e) {
        std::cout << "\n[ERROR] " << e.what() << "\n";
    }

    pausar();
}

void InterfazUI::vistaBajaMedico() {
    imprimirBanner("BAJA LOGICA DE MEDICO");
    int id = leerEntero("Ingrese el ID del medico a dar de baja: ", 1, 99999);

    Medico* m = servicio.buscarMedicoPorId(id);
    if (!m) {
        std::cout << "\n[ERROR] Medico no encontrado.\n";
        pausar();
        return;
    }

    m->mostrarDatos();
    if (confirmarAccion("\nEsta seguro que desea dar de baja logica a este profesional?")) {
        try {
            servicio.bajaLogicaMedico(id);
            std::cout << "\n[OK] Se aplico la baja logica. El medico ya no podra recibir nuevos turnos.\n";
        } catch (const ClinicaException& e) {
            std::cout << "\n[ERROR] " << e.what() << "\n";
        }
    } else {
        std::cout << "\nOperacion cancelada.\n";
    }

    pausar();
}

void InterfazUI::vistaReactivarMedico() {
    imprimirBanner("REACTIVACION DE MEDICO");
    int id = leerEntero("Ingrese el ID del medico a reactivar: ", 1, 99999);

    try {
        servicio.reactivarMedico(id);
        std::cout << "\n[OK] El profesional ha sido reactivado y vuelve a estar disponible para turnos.\n";
    } catch (const ClinicaException& e) {
        std::cout << "\n[ERROR] " << e.what() << "\n";
    }

    pausar();
}

void InterfazUI::vistaListarMedicos() {
    imprimirBanner("LISTADO DE MEDICOS");
    const auto& lista = servicio.getMedicos();

    if (lista.empty()) {
        std::cout << "No hay medicos registrados en el sistema.\n";
    } else {
        std::cout << std::left 
                  << std::setw(5)  << "ID" 
                  << std::setw(12) << "DNI" 
                  << std::setw(24) << "Apellido y Nombre" 
                  << std::setw(12) << "Matricula" 
                  << std::setw(20) << "Especialidad" 
                  << std::setw(14) << "Telefono" 
                  << std::setw(10) << "Estado" << "\n";
        std::cout << std::string(97, '-') << "\n";

        for (const auto& m : lista) {
            std::string nombreCompleto = m.getApellido() + ", " + m.getNombre();
            std::cout << std::left 
                      << std::setw(5)  << m.getId() 
                      << std::setw(12) << m.getDni() 
                      << std::setw(24) << (nombreCompleto.length() > 22 ? nombreCompleto.substr(0, 21) + "." : nombreCompleto)
                      << std::setw(12) << m.getMatricula() 
                      << std::setw(20) << (m.getEspecialidad().length() > 18 ? m.getEspecialidad().substr(0, 17) + "." : m.getEspecialidad())
                      << std::setw(14) << m.getTelefono() 
                      << std::setw(10) << (m.isActivo() ? "Activo" : "Baja") 
                      << "\n";
        }
    }

    pausar();
}

void InterfazUI::vistaBuscarMedico() {
    imprimirBanner("BUSQUEDA DE MEDICO");
    std::cout << "  [1] Buscar por ID interno\n";
    std::cout << "  [2] Buscar por numero de DNI\n";
    int opc = leerEntero("Opcion [1-2]: ", 1, 2);

    Medico* m = nullptr;
    if (opc == 1) {
        int id = leerEntero("Ingrese ID: ", 1, 99999);
        m = servicio.buscarMedicoPorId(id);
    } else {
        std::string dni = leerDNI("Ingrese DNI: ");
        m = servicio.buscarMedicoPorDni(dni);
    }

    if (m) {
        std::cout << "\nResultado de la busqueda:\n";
        m->mostrarDatos();
    } else {
        std::cout << "\nNo se encontro ningun profesional con el criterio indicado.\n";
    }

    pausar();
}

// ==========================================
// SECCION DE PACIENTES
// ==========================================

void InterfazUI::menuPacientes() {
    int opc = -1;
    do {
        imprimirBanner("GESTION DE PACIENTES");
        std::cout << "  [1] Dar de alta nuevo paciente\n";
        std::cout << "  [2] Modificar datos del paciente\n";
        std::cout << "  [3] Dar de baja logica a un paciente\n";
        std::cout << "  [4] Reactivar paciente\n";
        std::cout << "  [5] Listar pacientes registrados\n";
        std::cout << "  [6] Buscar paciente por DNI o ID\n";
        std::cout << "  [0] Volver al menu principal\n";
        std::cout << "----------------------------------------------------------------------------------------\n";
        opc = leerEntero("Seleccione una opcion [0-6]: ", 0, 6);

        switch (opc) {
            case 1: vistaAltaPaciente(); break;
            case 2: vistaModificarPaciente(); break;
            case 3: vistaBajaPaciente(); break;
            case 4: vistaReactivarPaciente(); break;
            case 5: vistaListarPacientes(); break;
            case 6: vistaBuscarPaciente(); break;
        }
    } while (opc != 0);
}

void InterfazUI::vistaAltaPaciente() {
    imprimirBanner("ALTA DE PACIENTE");
    std::cout << "Ingrese los datos del nuevo paciente:\n\n";

    std::string dni = leerDNI("Ingrese DNI (sin puntos): ");
    std::string nombre = leerTextoNoVacio("Ingrese Nombre: ");
    std::string apellido = leerTextoNoVacio("Ingrese Apellido: ");
    std::string telefono = leerTextoNoVacio("Ingrese Telefono: ");
    std::string obraSocial = leerTextoNoVacio("Ingrese Obra Social / Prepaga (ej: OSDE, PAMI, Particular): ");
    std::string nroAfiliado = leerTextoNoVacio("Ingrese Nro de Afiliado (o 'S/N' si es particular): ");

    try {
        servicio.agregarPaciente(dni, nombre, apellido, telefono, obraSocial, nroAfiliado);
        std::cout << "\n[OK] El paciente fue registrado con exito.\n";
    } catch (const ClinicaException& e) {
        std::cout << "\n[ERROR] " << e.what() << "\n";
    }

    pausar();
}

void InterfazUI::vistaModificarPaciente() {
    imprimirBanner("MODIFICACION DE PACIENTE");
    int id = leerEntero("Ingrese ID del paciente: ", 1, 99999);

    Paciente* p = servicio.buscarPacientePorId(id);
    if (!p) {
        std::cout << "\n[ERROR] Paciente no encontrado.\n";
        pausar();
        return;
    }

    std::cout << "\nDatos actuales:\n";
    p->mostrarDatos();

    std::cout << "\nNuevos datos:\n";
    std::string tel = leerTextoNoVacio("Nuevo Telefono: ");
    std::string os = leerTextoNoVacio("Nueva Obra Social: ");
    std::string af = leerTextoNoVacio("Nuevo Nro de Afiliado: ");

    try {
        servicio.modificarPaciente(id, tel, os, af);
        std::cout << "\n[OK] Paciente actualizado correctamente.\n";
    } catch (const ClinicaException& e) {
        std::cout << "\n[ERROR] " << e.what() << "\n";
    }

    pausar();
}

void InterfazUI::vistaBajaPaciente() {
    imprimirBanner("BAJA LOGICA DE PACIENTE");
    int id = leerEntero("Ingrese ID del paciente a dar de baja: ", 1, 99999);

    Paciente* p = servicio.buscarPacientePorId(id);
    if (!p) {
        std::cout << "\n[ERROR] Paciente inexistente.\n";
        pausar();
        return;
    }

    p->mostrarDatos();
    if (confirmarAccion("\nConfirma la baja logica de este paciente?")) {
        try {
            servicio.bajaLogicaPaciente(id);
            std::cout << "\n[OK] El paciente fue dado de baja logica.\n";
        } catch (const ClinicaException& e) {
            std::cout << "\n[ERROR] " << e.what() << "\n";
        }
    } else {
        std::cout << "\nOperacion cancelada.\n";
    }

    pausar();
}

void InterfazUI::vistaReactivarPaciente() {
    imprimirBanner("REACTIVACION DE PACIENTE");
    int id = leerEntero("Ingrese ID del paciente a reactivar: ", 1, 99999);

    try {
        servicio.reactivarPaciente(id);
        std::cout << "\n[OK] El paciente fue reactivado satisfactoriamente.\n";
    } catch (const ClinicaException& e) {
        std::cout << "\n[ERROR] " << e.what() << "\n";
    }

    pausar();
}

void InterfazUI::vistaListarPacientes() {
    imprimirBanner("LISTADO DE PACIENTES");
    const auto& lista = servicio.getPacientes();

    if (lista.empty()) {
        std::cout << "No hay pacientes registrados en el sistema.\n";
    } else {
        std::cout << std::left 
                  << std::setw(5)  << "ID" 
                  << std::setw(12) << "DNI" 
                  << std::setw(24) << "Apellido y Nombre" 
                  << std::setw(15) << "Obra Social" 
                  << std::setw(16) << "Nro Afiliado" 
                  << std::setw(14) << "Telefono" 
                  << std::setw(10) << "Estado" << "\n";
        std::cout << std::string(96, '-') << "\n";

        for (const auto& p : lista) {
            std::string nombreCompleto = p.getApellido() + ", " + p.getNombre();
            std::cout << std::left 
                      << std::setw(5)  << p.getId() 
                      << std::setw(12) << p.getDni() 
                      << std::setw(24) << (nombreCompleto.length() > 22 ? nombreCompleto.substr(0, 21) + "." : nombreCompleto)
                      << std::setw(15) << (p.getObraSocial().length() > 13 ? p.getObraSocial().substr(0, 12) + "." : p.getObraSocial())
                      << std::setw(16) << p.getNumeroAfiliado() 
                      << std::setw(14) << p.getTelefono() 
                      << std::setw(10) << (p.isActivo() ? "Activo" : "Baja") 
                      << "\n";
        }
    }

    pausar();
}

void InterfazUI::vistaBuscarPaciente() {
    imprimirBanner("BUSQUEDA DE PACIENTE");
    std::cout << "  [1] Buscar por ID interno\n";
    std::cout << "  [2] Buscar por numero de DNI\n";
    int opc = leerEntero("Opcion [1-2]: ", 1, 2);

    Paciente* p = nullptr;
    if (opc == 1) {
        int id = leerEntero("Ingrese ID: ", 1, 99999);
        p = servicio.buscarPacientePorId(id);
    } else {
        std::string dni = leerDNI("Ingrese DNI: ");
        p = servicio.buscarPacientePorDni(dni);
    }

    if (p) {
        std::cout << "\nResultado de la busqueda:\n";
        p->mostrarDatos();
    } else {
        std::cout << "\nNo se encontro ningun paciente con los datos provistos.\n";
    }

    pausar();
}

// ==========================================
// SECCION DE TURNOS
// ==========================================

void InterfazUI::menuTurnos() {
    int opc = -1;
    do {
        imprimirBanner("GESTION DE TURNOS");
        std::cout << "  [1] Asignar nuevo turno (Validacion de disponibilidad)\n";
        std::cout << "  [2] Cancelar turno existente\n";
        std::cout << "  [3] Reprogramar fecha/hora de turno\n";
        std::cout << "  [4] Cambiar estado de turno (Atendido / Pendiente)\n";
        std::cout << "  [5] Listado cronologico general (Arbol ABB Inorden)\n";
        std::cout << "  [6] Consultar turno por ID\n";
        std::cout << "  [7] Imprimir Comprobante Oficial (Funcion Friend)\n";
        std::cout << "  [0] Volver al menu principal\n";
        std::cout << "----------------------------------------------------------------------------------------\n";
        opc = leerEntero("Seleccione una opcion [0-7]: ", 0, 7);

        switch (opc) {
            case 1: vistaAsignarTurno(); break;
            case 2: vistaCancelarTurno(); break;
            case 3: vistaReprogramarTurno(); break;
            case 4: vistaCambiarEstadoTurno(); break;
            case 5: vistaListarTurnos(); break;
            case 6: vistaBuscarTurno(); break;
            case 7: vistaImprimirComprobante(); break;
        }
    } while (opc != 0);
}

void InterfazUI::vistaAsignarTurno() {
    imprimirBanner("ASIGNACION DE NUEVO TURNO");

    // Mostramos resumen rapido de medicos activos disponibles
    std::cout << "Medicos activos disponibles:\n";
    for (const auto& m : servicio.getMedicos()) {
        if (m.isActivo()) {
            std::cout << "  ID [" << m.getId() << "] Dr/a. " << m.getApellido() << " (" << m.getEspecialidad() << ")\n";
        }
    }
    std::cout << "\n";

    int idMed = leerEntero("Ingrese ID del Medico: ", 1, 99999);
    int idPac = leerEntero("Ingrese ID del Paciente: ", 1, 99999);
    std::string fecha = leerFecha("Fecha del turno");
    std::string hora = leerHora("Hora del turno");
    std::string motivo = leerTextoNoVacio("Motivo de consulta: ");

    try {
        int idGenerado = servicio.asignarTurno(idMed, idPac, fecha, hora, motivo);
        std::cout << "\n[OK] Turno #" << idGenerado << " registrado exitosamente!\n";

        if (confirmarAccion("Desea ver o imprimir el comprobante oficial en este momento?")) {
            servicio.imprimirTicket(idGenerado);
        }
    } catch (const ClinicaException& e) {
        std::cout << "\n[ERROR DE REGLA DE NEGOCIO] " << e.what() << "\n";
    }

    pausar();
}

void InterfazUI::vistaCancelarTurno() {
    imprimirBanner("CANCELACION DE TURNO");
    int idTurno = leerEntero("Ingrese ID del turno a cancelar: ", 1, 99999);

    Turno* t = servicio.buscarTurnoPorId(idTurno);
    if (!t) {
        std::cout << "\n[ERROR] Turno no encontrado.\n";
        pausar();
        return;
    }

    std::cout << "\nDatos del turno:\n" << *t << "\n";

    if (confirmarAccion("Esta seguro que desea cancelar este turno?")) {
        try {
            servicio.cancelarTurno(idTurno);
            std::cout << "\n[OK] El turno ha sido cancelado exitosamente.\n";
        } catch (const ClinicaException& e) {
            std::cout << "\n[ERROR] " << e.what() << "\n";
        }
    } else {
        std::cout << "\nOperacion cancelada.\n";
    }

    pausar();
}

void InterfazUI::vistaReprogramarTurno() {
    imprimirBanner("REPROGRAMACION DE TURNO");
    int idTurno = leerEntero("Ingrese ID del turno a reprogramar: ", 1, 99999);

    Turno* t = servicio.buscarTurnoPorId(idTurno);
    if (!t) {
        std::cout << "\n[ERROR] Turno no encontrado.\n";
        pausar();
        return;
    }

    std::cout << "\nTurno actual:\n" << *t << "\n\n";
    std::cout << "Ingrese la nueva fecha y hora deseadas:\n";
    std::string nuevaFecha = leerFecha("Nueva fecha");
    std::string nuevaHora = leerHora("Nueva hora");

    try {
        servicio.reprogramarTurno(idTurno, nuevaFecha, nuevaHora);
        std::cout << "\n[OK] El turno fue reprogramado correctamente para el " 
                  << nuevaFecha << " a las " << nuevaHora << " hs.\n";
    } catch (const ClinicaException& e) {
        std::cout << "\n[ERROR] " << e.what() << "\n";
    }

    pausar();
}

void InterfazUI::vistaCambiarEstadoTurno() {
    imprimirBanner("CAMBIO DE ESTADO DE TURNO");
    int idTurno = leerEntero("Ingrese ID del turno: ", 1, 99999);

    Turno* t = servicio.buscarTurnoPorId(idTurno);
    if (!t) {
        std::cout << "\n[ERROR] Turno no encontrado.\n";
        pausar();
        return;
    }

    std::cout << "\nTurno actual:\n" << *t << "\n\n";
    std::cout << "Seleccione nuevo estado:\n";
    std::cout << "  [1] PENDIENTE\n";
    std::cout << "  [2] ATENDIDO\n";
    std::cout << "  [3] CANCELADO\n";
    int opc = leerEntero("Opcion [1-3]: ", 1, 3);

    std::string nuevoEst = (opc == 1 ? "PENDIENTE" : (opc == 2 ? "ATENDIDO" : "CANCELADO"));

    try {
        servicio.cambiarEstadoTurno(idTurno, nuevoEst);
        std::cout << "\n[OK] Estado del turno actualizado a: " << nuevoEst << "\n";
    } catch (const ClinicaException& e) {
        std::cout << "\n[ERROR] " << e.what() << "\n";
    }

    pausar();
}

void InterfazUI::vistaListarTurnos() {
    imprimirBanner("LISTADO CRONOLOGICO DE TURNOS (ARBOL ABB)");
    // Aqui se utiliza el Arbol Binario de Busqueda propio en template
    auto lista = servicio.obtenerTurnosCronologicos();

    if (lista.empty()) {
        std::cout << "No hay turnos registrados en la clinica.\n";
    } else {
        std::cout << std::left 
                  << std::setw(6)  << "ID" 
                  << std::setw(12) << "Fecha" 
                  << std::setw(8)  << "Hora" 
                  << std::setw(24) << "Medico" 
                  << std::setw(24) << "Paciente" 
                  << std::setw(12) << "Estado" 
                  << "Motivo\n";
        std::cout << std::string(98, '-') << "\n";

        for (const auto& t : lista) {
            Medico* m = servicio.buscarMedicoPorId(t.getIdMedico());
            Paciente* p = servicio.buscarPacientePorId(t.getIdPaciente());

            std::string nombreMed = (m ? "Dr. " + m->getApellido() : "ID " + std::to_string(t.getIdMedico()));
            std::string nombrePac = (p ? p->getApellido() + ", " + p->getNombre() : "ID " + std::to_string(t.getIdPaciente()));

            std::cout << std::left 
                      << std::setw(6)  << t.getIdTurno() 
                      << std::setw(12) << t.getFecha() 
                      << std::setw(8)  << t.getHora() 
                      << std::setw(24) << (nombreMed.length() > 22 ? nombreMed.substr(0, 21) + "." : nombreMed)
                      << std::setw(24) << (nombrePac.length() > 22 ? nombrePac.substr(0, 21) + "." : nombrePac)
                      << std::setw(12) << t.getEstado() 
                      << t.getMotivo() << "\n";
        }
    }

    pausar();
}

void InterfazUI::vistaBuscarTurno() {
    imprimirBanner("CONSULTA DE TURNO POR ID");
    int idTurno = leerEntero("Ingrese ID de Turno: ", 1, 99999);

    Turno* t = servicio.buscarTurnoPorId(idTurno);
    if (t) {
        std::cout << "\nDetalles del turno encontrado:\n";
        std::cout << *t << "\n";

        Medico* m = servicio.buscarMedicoPorId(t->getIdMedico());
        Paciente* p = servicio.buscarPacientePorId(t->getIdPaciente());
        if (m) std::cout << "  Medico: Dr/a. " << m->getApellido() << ", " << m->getNombre() << " (" << m->getEspecialidad() << ")\n";
        if (p) std::cout << "  Paciente: " << p->getApellido() << ", " << p->getNombre() << " (OS: " << p->getObraSocial() << ")\n";
    } else {
        std::cout << "\nNo existe ningun turno con el ID " << idTurno << "\n";
    }

    pausar();
}

void InterfazUI::vistaImprimirComprobante() {
    imprimirBanner("IMPRESION DE COMPROBANTE OFICIAL");
    int idTurno = leerEntero("Ingrese ID de Turno: ", 1, 99999);

    try {
        servicio.imprimirTicket(idTurno);
    } catch (const ClinicaException& e) {
        std::cout << "\n[ERROR] " << e.what() << "\n";
    }

    pausar();
}

// ==========================================
// SECCION DE REPORTES Y ESTADISTICAS
// ==========================================

void InterfazUI::menuReportes() {
    int opc = -1;
    do {
        imprimirBanner("REPORTES Y ESTADISTICAS");
        std::cout << "  [1] Reporte de turnos por fecha especifica\n";
        std::cout << "  [2] Reporte de turnos por medico\n";
        std::cout << "  [3] Reporte de historial por paciente\n";
        std::cout << "  [4] Estadisticas generales del consultorio\n";
        std::cout << "  [0] Volver al menu principal\n";
        std::cout << "----------------------------------------------------------------------------------------\n";
        opc = leerEntero("Seleccione una opcion [0-4]: ", 0, 4);

        switch (opc) {
            case 1: vistaReportePorFecha(); break;
            case 2: vistaReportePorMedico(); break;
            case 3: vistaReportePorPaciente(); break;
            case 4: vistaEstadisticas(); break;
        }
    } while (opc != 0);
}

void InterfazUI::vistaReportePorFecha() {
    imprimirBanner("REPORTE DE TURNOS POR FECHA");
    std::string fecha = leerFecha("Fecha a consultar");

    auto lista = servicio.filtrarTurnosPorFecha(fecha);
    std::cout << "\nTurnos programados para el dia " << fecha << ":\n\n";

    if (lista.empty()) {
        std::cout << "No hay turnos registrados en esa fecha.\n";
    } else {
        for (const auto& t : lista) {
            Medico* m = servicio.buscarMedicoPorId(t.getIdMedico());
            Paciente* p = servicio.buscarPacientePorId(t.getIdPaciente());
            std::cout << "  Hora: " << t.getHora() 
                      << " | Dr. " << (m ? m->getApellido() : "?") 
                      << " | Paciente: " << (p ? p->getApellido() : "?") 
                      << " | Estado: " << t.getEstado() 
                      << " | Motivo: " << t.getMotivo() << "\n";
        }
    }

    pausar();
}

void InterfazUI::vistaReportePorMedico() {
    imprimirBanner("REPORTE DE TURNOS POR MEDICO");
    int idMed = leerEntero("Ingrese ID del Medico: ", 1, 99999);

    Medico* m = servicio.buscarMedicoPorId(idMed);
    if (!m) {
        std::cout << "\n[ERROR] Medico no encontrado.\n";
        pausar();
        return;
    }

    std::cout << "\nAgenda para Dr/a. " << m->getApellido() << ", " << m->getNombre() 
              << " (" << m->getEspecialidad() << "):\n\n";

    auto lista = servicio.filtrarTurnosPorMedico(idMed);
    if (lista.empty()) {
        std::cout << "El profesional no tiene turnos registrados.\n";
    } else {
        for (const auto& t : lista) {
            Paciente* p = servicio.buscarPacientePorId(t.getIdPaciente());
            std::cout << "  Turno #" << t.getIdTurno() 
                      << " | " << t.getFecha() << " " << t.getHora() 
                      << " | Paciente: " << (p ? p->getApellido() + " (DNI " + p->getDni() + ")" : "?")
                      << " | Estado: " << t.getEstado() 
                      << " | Motivo: " << t.getMotivo() << "\n";
        }
    }

    pausar();
}

void InterfazUI::vistaReportePorPaciente() {
    imprimirBanner("HISTORIAL DE TURNOS POR PACIENTE");
    int idPac = leerEntero("Ingrese ID del Paciente: ", 1, 99999);

    Paciente* p = servicio.buscarPacientePorId(idPac);
    if (!p) {
        std::cout << "\n[ERROR] Paciente no encontrado.\n";
        pausar();
        return;
    }

    std::cout << "\nHistorial para: " << p->getApellido() << ", " << p->getNombre() 
              << " (DNI: " << p->getDni() << "):\n\n";

    auto lista = servicio.filtrarTurnosPorPaciente(idPac);
    if (lista.empty()) {
        std::cout << "El paciente no tiene turnos registrados.\n";
    } else {
        for (const auto& t : lista) {
            Medico* m = servicio.buscarMedicoPorId(t.getIdMedico());
            std::cout << "  Turno #" << t.getIdTurno() 
                      << " | " << t.getFecha() << " " << t.getHora() 
                      << " | Dr/a: " << (m ? m->getApellido() + " (" + m->getEspecialidad() + ")" : "?")
                      << " | Estado: " << t.getEstado() 
                      << " | Motivo: " << t.getMotivo() << "\n";
        }
    }

    pausar();
}

void InterfazUI::vistaEstadisticas() {
    imprimirBanner("ESTADISTICAS GENERALES");

    int totalTurnos = 0, pendientes = 0, atendidos = 0, cancelados = 0;
    servicio.obtenerEstadisticas(totalTurnos, pendientes, atendidos, cancelados);

    int totalMedicos = servicio.getMedicos().size();
    int medicosActivos = 0;
    for (const auto& m : servicio.getMedicos()) if (m.isActivo()) medicosActivos++;

    int totalPacientes = servicio.getPacientes().size();
    int pacientesActivos = 0;
    for (const auto& p : servicio.getPacientes()) if (p.isActivo()) pacientesActivos++;

    std::cout << "RESUMEN DE PROFESIONALES Y PACIENTES:\n";
    std::cout << "  Total de medicos registrados: " << totalMedicos << " (" << medicosActivos << " activos)\n";
    std::cout << "  Total de pacientes en padron: " << totalPacientes << " (" << pacientesActivos << " activos)\n";
    std::cout << "  Total de personas en memoria (static): " << Persona::getTotalPersonas() << "\n\n";

    std::cout << "RESUMEN DE GESTION DE TURNOS:\n";
    std::cout << "  Total historico de turnos:    " << totalTurnos << "\n";
    std::cout << "  Turnos pendientes:            " << pendientes << "\n";
    std::cout << "  Turnos atendidos:             " << atendidos << "\n";
    std::cout << "  Turnos cancelados:            " << cancelados << "\n\n";

    if (totalTurnos > 0) {
        double porcAtendidos = (atendidos * 100.0) / totalTurnos;
        double porcCancelados = (cancelados * 100.0) / totalTurnos;
        std::cout << std::fixed << std::setprecision(1);
        std::cout << "  Tasa de atencion efectiva:    " << porcAtendidos << "%\n";
        std::cout << "  Tasa de cancelacion:          " << porcCancelados << "%\n";
    }

    pausar();
}

