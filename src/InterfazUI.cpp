#include "../include/InterfazUI.h"
#include "../include/Validaciones.h"
#include "../include/Excepciones.h"
#include "../include/Colores.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace Validaciones;
using namespace Colores;

InterfazUI::InterfazUI() {
    activarColoresWindows();
}

void InterfazUI::imprimirBanner(const std::string& titulo) {
    limpiarPantalla();
    std::cout << CIAN << "========================================================================================\n"
              << NEGRITA << "             SISTEMA DE GESTION DE TURNOS MEDICOS - CLINICA CENTRAL                     \n"
              << "                      " << titulo << "\n"
              << RESET << CIAN 
              << "========================================================================================\n\n" << RESET;
}

// ==========================================
// METODOS AUXILIARES: TABLAS E HISTORIALES
// ==========================================

void InterfazUI::mostrarTablaMedicos(const std::string& titulo, bool soloActivos) {
    const auto& lista = servicio.getMedicos();
    std::cout << AMARILLO << "--- " << titulo << " ---" << RESET << "\n";

    int visibles = 0;
    for (const auto& m : lista) {
        if (!soloActivos || m.isActivo()) visibles++;
    }

    if (visibles == 0) {
        std::cout << GRIS << "  (No hay medicos registrados para mostrar)\n" << RESET;
        std::cout << CIAN << std::string(97, '-') << RESET << "\n";
        return;
    }

    std::cout << BLANCO << NEGRITA << std::left 
              << std::setw(6)  << "ID" 
              << std::setw(12) << "DNI" 
              << std::setw(24) << "Apellido y Nombre" 
              << std::setw(12) << "Matricula" 
              << std::setw(20) << "Especialidad" 
              << std::setw(13) << "Telefono" 
              << std::setw(10) << "Estado" << RESET << "\n";
    std::cout << CIAN << std::string(97, '-') << RESET << "\n";

    for (const auto& m : lista) {
        if (soloActivos && !m.isActivo()) continue;
        std::string nombreCompleto = m.getApellido() + ", " + m.getNombre();
        std::cout << std::left 
                  << std::setw(6)  << m.getId() 
                  << std::setw(12) << m.getDni() 
                  << std::setw(24) << (nombreCompleto.length() > 22 ? nombreCompleto.substr(0, 21) + "." : nombreCompleto)
                  << std::setw(12) << m.getMatricula() 
                  << std::setw(20) << (m.getEspecialidad().length() > 18 ? m.getEspecialidad().substr(0, 17) + "." : m.getEspecialidad())
                  << std::setw(13) << m.getTelefono() 
                  << (m.isActivo() ? VERDE + "Activo" : ROJO + "Baja") << RESET 
                  << "\n";
    }
    std::cout << CIAN << std::string(97, '-') << RESET << "\n";
}

void InterfazUI::mostrarTablaPacientes(const std::string& titulo, bool soloActivos) {
    const auto& lista = servicio.getPacientes();
    std::cout << AMARILLO << "--- " << titulo << " ---" << RESET << "\n";

    int visibles = 0;
    for (const auto& p : lista) {
        if (!soloActivos || p.isActivo()) visibles++;
    }

    if (visibles == 0) {
        std::cout << GRIS << "  (No hay pacientes registrados para mostrar)\n" << RESET;
        std::cout << CIAN << std::string(96, '-') << RESET << "\n";
        return;
    }

    std::cout << BLANCO << NEGRITA << std::left 
              << std::setw(6)  << "ID" 
              << std::setw(12) << "DNI" 
              << std::setw(24) << "Apellido y Nombre" 
              << std::setw(15) << "Obra Social" 
              << std::setw(16) << "Nro Afiliado" 
              << std::setw(13) << "Telefono" 
              << std::setw(10) << "Estado" << RESET << "\n";
    std::cout << CIAN << std::string(96, '-') << RESET << "\n";

    for (const auto& p : lista) {
        if (soloActivos && !p.isActivo()) continue;
        std::string nombreCompleto = p.getApellido() + ", " + p.getNombre();
        std::cout << std::left 
                  << std::setw(6)  << p.getId() 
                  << std::setw(12) << p.getDni() 
                  << std::setw(24) << (nombreCompleto.length() > 22 ? nombreCompleto.substr(0, 21) + "." : nombreCompleto)
                  << std::setw(15) << (p.getObraSocial().length() > 13 ? p.getObraSocial().substr(0, 12) + "." : p.getObraSocial())
                  << std::setw(16) << p.getNumeroAfiliado() 
                  << std::setw(13) << p.getTelefono() 
                  << (p.isActivo() ? VERDE + "Activo" : ROJO + "Baja") << RESET 
                  << "\n";
    }
    std::cout << CIAN << std::string(96, '-') << RESET << "\n";
}

void InterfazUI::mostrarTablaTurnos(const std::string& titulo) {
    auto lista = servicio.obtenerTurnosCronologicos();
    std::cout << AMARILLO << "--- " << titulo << " ---" << RESET << "\n";

    if (lista.empty()) {
        std::cout << GRIS << "  (No hay turnos registrados en el sistema)\n" << RESET;
        std::cout << CIAN << std::string(98, '-') << RESET << "\n";
        return;
    }

    std::cout << BLANCO << NEGRITA << std::left 
              << std::setw(6)  << "ID" 
              << std::setw(12) << "Fecha" 
              << std::setw(8)  << "Hora" 
              << std::setw(22) << "Medico" 
              << std::setw(24) << "Paciente" 
              << std::setw(13) << "Estado" 
              << "Motivo" << RESET << "\n";
    std::cout << CIAN << std::string(98, '-') << RESET << "\n";

    for (const auto& t : lista) {
        Medico* m = servicio.buscarMedicoPorId(t.getIdMedico());
        Paciente* p = servicio.buscarPacientePorId(t.getIdPaciente());

        std::string nombreMed = (m ? "Dr. " + m->getApellido() : "ID " + std::to_string(t.getIdMedico()));
        std::string nombrePac = (p ? p->getApellido() + ", " + p->getNombre() : "ID " + std::to_string(t.getIdPaciente()));

        std::string colorEst = RESET;
        if (t.getEstado() == "PENDIENTE") colorEst = AMARILLO;
        else if (t.getEstado() == "ATENDIDO") colorEst = VERDE;
        else if (t.getEstado() == "CANCELADO") colorEst = ROJO;

        std::cout << std::left 
                  << std::setw(6)  << t.getIdTurno() 
                  << std::setw(12) << t.getFecha() 
                  << std::setw(8)  << t.getHora() 
                  << std::setw(22) << (nombreMed.length() > 20 ? nombreMed.substr(0, 19) + "." : nombreMed)
                  << std::setw(24) << (nombrePac.length() > 22 ? nombrePac.substr(0, 21) + "." : nombrePac)
                  << colorEst << std::setw(13) << t.getEstado() << RESET
                  << (t.getMotivo().length() > 20 ? t.getMotivo().substr(0, 19) + "." : t.getMotivo()) << "\n";
    }
    std::cout << CIAN << std::string(98, '-') << RESET << "\n";
}

// ==========================================
// MENU PRINCIPAL
// ==========================================

void InterfazUI::ejecutar() {
    int opcion = -1;
    do {
        imprimirBanner("MENU PRINCIPAL");
        std::cout << "  " << AMARILLO << "[1]" << RESET << " Modulo de Medicos\n";
        std::cout << "  " << AMARILLO << "[2]" << RESET << " Modulo de Pacientes\n";
        std::cout << "  " << AMARILLO << "[3]" << RESET << " Asignacion y Control de Turnos\n";
        std::cout << "  " << AMARILLO << "[4]" << RESET << " Reportes y Estadisticas del Sistema\n";
        std::cout << "  " << ROJO << "[0]" << RESET << " Salir y Guardar Cambios\n";
        std::cout << CIAN << "----------------------------------------------------------------------------------------\n" << RESET;
        opcion = leerEntero("Seleccione una opcion [0-4]: ", 0, 4);

        switch (opcion) {
            case 1: menuMedicos(); break;
            case 2: menuPacientes(); break;
            case 3: menuTurnos(); break;
            case 4: menuReportes(); break;
            case 0:
                std::cout << "\n" << VERDE << "[+] Guardando informacion y sincronizando registros en disco..." << RESET << "\n";
                servicio.guardarDatos();
                std::cout << VERDE << "[+] Datos guardados exitosamente. Hasta luego!\n" << RESET;
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
        std::cout << "  " << AMARILLO << "[1]" << RESET << " Dar de alta nuevo medico\n";
        std::cout << "  " << AMARILLO << "[2]" << RESET << " Modificar datos de contacto o especialidad\n";
        std::cout << "  " << AMARILLO << "[3]" << RESET << " Dar de baja logica a un medico\n";
        std::cout << "  " << AMARILLO << "[4]" << RESET << " Reactivar medico dado de baja\n";
        std::cout << "  " << AMARILLO << "[5]" << RESET << " Listar medicos registrados\n";
        std::cout << "  " << AMARILLO << "[6]" << RESET << " Buscar medico por DNI o ID\n";
        std::cout << "  " << ROJO << "[0]" << RESET << " Volver al menu principal\n";
        std::cout << CIAN << "----------------------------------------------------------------------------------------\n" << RESET;
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
    std::string telefono = leerTelefono("Ingrese Telefono de contacto (ej: 1144556677 o 2234388758): ");
    std::string matricula = leerMatricula("Ingrese Nro de Matricula (ej: MN-45120 o MP-38902): ");
    std::string especialidad = leerTextoNoVacio("Ingrese Especialidad (ej: Cardiologia, Pediatria): ");

    try {
        servicio.agregarMedico(dni, nombre, apellido, telefono, matricula, especialidad);
        std::cout << "\n" << VERDE << "[OK] El profesional fue registrado exitosamente en el sistema." << RESET << "\n";
    } catch (const ClinicaException& e) {
        std::cout << "\n" << ROJO << "[ERROR] " << e.what() << RESET << "\n";
    }

    pausar();
}

void InterfazUI::vistaModificarMedico() {
    imprimirBanner("MODIFICACION DE MEDICO");
    mostrarTablaMedicos("HISTORIAL DE MEDICOS REGISTRADOS");

    int id = leerEntero("\nIngrese el ID del medico a modificar: ", 1, 99999);

    Medico* m = servicio.buscarMedicoPorId(id);
    if (!m) {
        std::cout << "\n" << ROJO << "[ERROR] No existe ningun medico con el ID " << id << "." << RESET << "\n";
        pausar();
        return;
    }

    std::cout << "\nDatos actuales del profesional:\n";
    m->mostrarDatos();
    std::cout << "\nIngrese los nuevos datos a actualizar:\n";

    std::string nuevoTel = leerTelefono("Nuevo Telefono (ej: 1144556677 o 2234388758): ");
    std::string nuevaMat = leerMatricula("Nueva Matricula (ej: MN-45120 o MP-38902): ");
    std::string nuevaEsp = leerTextoNoVacio("Nueva Especialidad: ");

    try {
        servicio.modificarMedico(id, nuevoTel, nuevaMat, nuevaEsp);
        std::cout << "\n" << VERDE << "[OK] Los datos del medico se actualizaron correctamente." << RESET << "\n";
    } catch (const ClinicaException& e) {
        std::cout << "\n" << ROJO << "[ERROR] " << e.what() << RESET << "\n";
    }

    pausar();
}

void InterfazUI::vistaBajaMedico() {
    imprimirBanner("BAJA LOGICA DE MEDICO");
    mostrarTablaMedicos("HISTORIAL DE MEDICOS REGISTRADOS");

    int id = leerEntero("\nIngrese el ID del medico a dar de baja: ", 1, 99999);

    Medico* m = servicio.buscarMedicoPorId(id);
    if (!m) {
        std::cout << "\n" << ROJO << "[ERROR] No existe ningun medico con el ID " << id << "." << RESET << "\n";
        pausar();
        return;
    }

    std::cout << "\nDatos del profesional seleccionado:\n";
    m->mostrarDatos();

    // Comprobacion explicita si ya se encuentra de baja
    if (!m->isActivo()) {
        std::cout << "\n" << AMARILLO << NEGRITA 
                  << "[!] AVISO: El Dr/a. " << m->getApellido() 
                  << " YA SE ENCUENTRA DADO DE BAJA LOGICA en el sistema." << RESET << "\n";
        std::cout << "    Actualmente su estado es inactivo y no puede recibir turnos.\n";
        std::cout << "    Si desea volver a habilitarlo, utilice la opcion 'Reactivar medico'.\n";
        pausar();
        return;
    }

    if (confirmarAccion("\nEsta seguro que desea dar de baja logica a este profesional?")) {
        try {
            servicio.bajaLogicaMedico(id);
            std::cout << "\n" << VERDE << "[OK] Se aplico la baja logica exitosamente al Dr/a. " 
                      << m->getApellido() << ". El profesional ya no recibira nuevos turnos." << RESET << "\n";
        } catch (const ClinicaException& e) {
            std::cout << "\n" << ROJO << "[ERROR] " << e.what() << RESET << "\n";
        }
    } else {
        std::cout << "\nOperacion cancelada por el usuario.\n";
    }

    pausar();
}

void InterfazUI::vistaReactivarMedico() {
    imprimirBanner("REACTIVACION DE MEDICO");
    mostrarTablaMedicos("HISTORIAL DE MEDICOS REGISTRADOS");

    int id = leerEntero("\nIngrese el ID del medico a reactivar: ", 1, 99999);

    Medico* m = servicio.buscarMedicoPorId(id);
    if (!m) {
        std::cout << "\n" << ROJO << "[ERROR] No existe ningun medico con el ID " << id << "." << RESET << "\n";
        pausar();
        return;
    }

    std::cout << "\nDatos del profesional seleccionado:\n";
    m->mostrarDatos();

    // Comprobacion explicita si ya se encuentra activo
    if (m->isActivo()) {
        std::cout << "\n" << AMARILLO << NEGRITA 
                  << "[!] AVISO: El Dr/a. " << m->getApellido() 
                  << " YA SE ENCUENTRA ACTIVO en el sistema." << RESET << "\n";
        std::cout << "    No requiere reactivacion porque ya esta habilitado para atender turnos.\n";
        pausar();
        return;
    }

    if (confirmarAccion("\nConfirma que desea reactivar a este profesional?")) {
        try {
            servicio.reactivarMedico(id);
            std::cout << "\n" << VERDE << "[OK] El profesional Dr/a. " << m->getApellido() 
                      << " ha sido reactivado y vuelve a estar disponible para turnos." << RESET << "\n";
        } catch (const ClinicaException& e) {
            std::cout << "\n" << ROJO << "[ERROR] " << e.what() << RESET << "\n";
        }
    } else {
        std::cout << "\nOperacion cancelada por el usuario.\n";
    }

    pausar();
}

void InterfazUI::vistaListarMedicos() {
    imprimirBanner("LISTADO DE MEDICOS REGISTRADOS");
    mostrarTablaMedicos("HISTORIAL COMPLETO DE MEDICOS", false);
    pausar();
}

void InterfazUI::vistaBuscarMedico() {
    imprimirBanner("BUSQUEDA DE MEDICO");
    std::cout << "  [1] Buscar por ID interno\n";
    std::cout << "  [2] Buscar por numero de DNI\n";
    int opc = leerEntero("Opcion [1-2]: ", 1, 2);

    Medico* m = nullptr;
    if (opc == 1) {
        mostrarTablaMedicos("HISTORIAL DISPONIBLE", false);
        int id = leerEntero("\nIngrese ID: ", 1, 99999);
        m = servicio.buscarMedicoPorId(id);
    } else {
        std::string dni = leerDNI("Ingrese DNI: ");
        m = servicio.buscarMedicoPorDni(dni);
    }

    if (m) {
        std::cout << "\n" << VERDE << "Resultado de la busqueda:" << RESET << "\n";
        m->mostrarDatos();
    } else {
        std::cout << "\n" << ROJO << "No se encontro ningun profesional con el criterio indicado." << RESET << "\n";
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
        std::cout << "  " << AMARILLO << "[1]" << RESET << " Dar de alta nuevo paciente\n";
        std::cout << "  " << AMARILLO << "[2]" << RESET << " Modificar datos del paciente\n";
        std::cout << "  " << AMARILLO << "[3]" << RESET << " Dar de baja logica a un paciente\n";
        std::cout << "  " << AMARILLO << "[4]" << RESET << " Reactivar paciente\n";
        std::cout << "  " << AMARILLO << "[5]" << RESET << " Listar pacientes registrados\n";
        std::cout << "  " << AMARILLO << "[6]" << RESET << " Buscar paciente por DNI o ID\n";
        std::cout << "  " << ROJO << "[0]" << RESET << " Volver al menu principal\n";
        std::cout << CIAN << "----------------------------------------------------------------------------------------\n" << RESET;
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
    std::string telefono = leerTelefono("Ingrese Telefono de contacto (ej: 1144556677 o 2234388758): ");
    std::string obraSocial = leerTextoNoVacio("Ingrese Obra Social / Prepaga (ej: OSDE, PAMI, Particular): ");
    std::string nroAfiliado = leerTextoNoVacio("Ingrese Nro de Afiliado (o 'S/N' si es particular): ");

    try {
        servicio.agregarPaciente(dni, nombre, apellido, telefono, obraSocial, nroAfiliado);
        std::cout << "\n" << VERDE << "[OK] El paciente fue registrado con exito." << RESET << "\n";
    } catch (const ClinicaException& e) {
        std::cout << "\n" << ROJO << "[ERROR] " << e.what() << RESET << "\n";
    }

    pausar();
}

void InterfazUI::vistaModificarPaciente() {
    imprimirBanner("MODIFICACION DE PACIENTE");
    mostrarTablaPacientes("HISTORIAL DE PACIENTES REGISTRADOS");

    int id = leerEntero("\nIngrese ID del paciente a modificar: ", 1, 99999);

    Paciente* p = servicio.buscarPacientePorId(id);
    if (!p) {
        std::cout << "\n" << ROJO << "[ERROR] No existe ningun paciente con el ID " << id << "." << RESET << "\n";
        pausar();
        return;
    }

    std::cout << "\nDatos actuales del paciente:\n";
    p->mostrarDatos();

    std::cout << "\nIngrese los nuevos datos a actualizar:\n";
    std::string tel = leerTelefono("Nuevo Telefono (ej: 1144556677 o 2234388758): ");
    std::string os = leerTextoNoVacio("Nueva Obra Social: ");
    std::string af = leerTextoNoVacio("Nuevo Nro de Afiliado: ");

    try {
        servicio.modificarPaciente(id, tel, os, af);
        std::cout << "\n" << VERDE << "[OK] Paciente actualizado correctamente." << RESET << "\n";
    } catch (const ClinicaException& e) {
        std::cout << "\n" << ROJO << "[ERROR] " << e.what() << RESET << "\n";
    }

    pausar();
}

void InterfazUI::vistaBajaPaciente() {
    imprimirBanner("BAJA LOGICA DE PACIENTE");
    mostrarTablaPacientes("HISTORIAL DE PACIENTES REGISTRADOS");

    int id = leerEntero("\nIngrese ID del paciente a dar de baja: ", 1, 99999);

    Paciente* p = servicio.buscarPacientePorId(id);
    if (!p) {
        std::cout << "\n" << ROJO << "[ERROR] No existe ningun paciente con el ID " << id << "." << RESET << "\n";
        pausar();
        return;
    }

    std::cout << "\nDatos del paciente seleccionado:\n";
    p->mostrarDatos();

    // Comprobacion explicita si ya se encuentra de baja
    if (!p->isActivo()) {
        std::cout << "\n" << AMARILLO << NEGRITA 
                  << "[!] AVISO: El paciente " << p->getApellido() << ", " << p->getNombre()
                  << " YA SE ENCUENTRA DADO DE BAJA LOGICA en el padron." << RESET << "\n";
        std::cout << "    Actualmente su estado es inactivo y no puede solicitar nuevos turnos.\n";
        std::cout << "    Si desea volver a habilitarlo, utilice la opcion 'Reactivar paciente'.\n";
        pausar();
        return;
    }

    if (confirmarAccion("\nConfirma la baja logica de este paciente?")) {
        try {
            servicio.bajaLogicaPaciente(id);
            std::cout << "\n" << VERDE << "[OK] El paciente fue dado de baja logica exitosamente." << RESET << "\n";
        } catch (const ClinicaException& e) {
            std::cout << "\n" << ROJO << "[ERROR] " << e.what() << RESET << "\n";
        }
    } else {
        std::cout << "\nOperacion cancelada por el usuario.\n";
    }

    pausar();
}

void InterfazUI::vistaReactivarPaciente() {
    imprimirBanner("REACTIVACION DE PACIENTE");
    mostrarTablaPacientes("HISTORIAL DE PACIENTES REGISTRADOS");

    int id = leerEntero("\nIngrese ID del paciente a reactivar: ", 1, 99999);

    Paciente* p = servicio.buscarPacientePorId(id);
    if (!p) {
        std::cout << "\n" << ROJO << "[ERROR] No existe ningun paciente con el ID " << id << "." << RESET << "\n";
        pausar();
        return;
    }

    std::cout << "\nDatos del paciente seleccionado:\n";
    p->mostrarDatos();

    // Comprobacion explicita si ya se encuentra activo
    if (p->isActivo()) {
        std::cout << "\n" << AMARILLO << NEGRITA 
                  << "[!] AVISO: El paciente " << p->getApellido() << ", " << p->getNombre()
                  << " YA SE ENCUENTRA ACTIVO en el padron." << RESET << "\n";
        std::cout << "    No requiere reactivacion.\n";
        pausar();
        return;
    }

    if (confirmarAccion("\nConfirma que desea reactivar a este paciente?")) {
        try {
            servicio.reactivarPaciente(id);
            std::cout << "\n" << VERDE << "[OK] El paciente ha sido reactivado satisfactoriamente." << RESET << "\n";
        } catch (const ClinicaException& e) {
            std::cout << "\n" << ROJO << "[ERROR] " << e.what() << RESET << "\n";
        }
    } else {
        std::cout << "\nOperacion cancelada por el usuario.\n";
    }

    pausar();
}

void InterfazUI::vistaListarPacientes() {
    imprimirBanner("LISTADO DE PACIENTES REGISTRADOS");
    mostrarTablaPacientes("HISTORIAL COMPLETO DE PACIENTES", false);
    pausar();
}

void InterfazUI::vistaBuscarPaciente() {
    imprimirBanner("BUSQUEDA DE PACIENTE");
    std::cout << "  [1] Buscar por ID interno\n";
    std::cout << "  [2] Buscar por numero de DNI\n";
    int opc = leerEntero("Opcion [1-2]: ", 1, 2);

    Paciente* p = nullptr;
    if (opc == 1) {
        mostrarTablaPacientes("HISTORIAL DISPONIBLE", false);
        int id = leerEntero("\nIngrese ID: ", 1, 99999);
        p = servicio.buscarPacientePorId(id);
    } else {
        std::string dni = leerDNI("Ingrese DNI: ");
        p = servicio.buscarPacientePorDni(dni);
    }

    if (p) {
        std::cout << "\n" << VERDE << "Resultado de la busqueda:" << RESET << "\n";
        p->mostrarDatos();
    } else {
        std::cout << "\n" << ROJO << "No se encontro ningun paciente con los datos provistos." << RESET << "\n";
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
        std::cout << "  " << AMARILLO << "[1]" << RESET << " Asignar nuevo turno\n";
        std::cout << "  " << AMARILLO << "[2]" << RESET << " Cancelar turno existente\n";
        std::cout << "  " << AMARILLO << "[3]" << RESET << " Reprogramar turno\n";
        std::cout << "  " << AMARILLO << "[4]" << RESET << " Cambiar estado de turno\n";
        std::cout << "  " << AMARILLO << "[5]" << RESET << " Listado cronologico general de turnos\n";
        std::cout << "  " << AMARILLO << "[6]" << RESET << " Consultar turno por ID\n";
        std::cout << "  " << AMARILLO << "[7]" << RESET << " Imprimir comprobante oficial de turno\n";
        std::cout << "  " << AMARILLO << "[8]" << RESET << " Visualizar esquema de turnos en arbol\n";
        std::cout << "  " << ROJO << "[0]" << RESET << " Volver al menu principal\n";
        std::cout << CIAN << "----------------------------------------------------------------------------------------\n" << RESET;
        opc = leerEntero("Seleccione una opcion [0-8]: ", 0, 8);

        switch (opc) {
            case 1: vistaAsignarTurno(); break;
            case 2: vistaCancelarTurno(); break;
            case 3: vistaReprogramarTurno(); break;
            case 4: vistaCambiarEstadoTurno(); break;
            case 5: vistaListarTurnos(); break;
            case 6: vistaBuscarTurno(); break;
            case 7: vistaImprimirComprobante(); break;
            case 8: vistaVisualizarArbol(); break;
        }
    } while (opc != 0);
}

void InterfazUI::vistaAsignarTurno() {
    imprimirBanner("ASIGNACION DE NUEVO TURNO");

    // Mostramos las tablas de medicos y pacientes activos disponibles
    mostrarTablaMedicos("1. MEDICOS ACTIVOS DISPONIBLES", true);
    std::cout << "\n";
    mostrarTablaPacientes("2. PACIENTES ACTIVOS DISPONIBLES", true);
    std::cout << "\n" << BLANCO << NEGRITA << "Complete los datos del turno a continuacion:\n" << RESET;

    int idMed = leerEntero("  -> Ingrese ID del Medico: ", 1, 99999);
    int idPac = leerEntero("  -> Ingrese ID del Paciente: ", 1, 99999);
    std::string fecha = leerFecha("  -> Fecha del turno");
    std::string hora = leerHora("  -> Hora del turno");
    std::string motivo = leerTextoNoVacio("  -> Motivo de consulta: ");

    try {
        int idGenerado = servicio.asignarTurno(idMed, idPac, fecha, hora, motivo);
        std::cout << "\n" << VERDE << "[OK] Turno #" << idGenerado << " registrado exitosamente!" << RESET << "\n";

        if (confirmarAccion("\nDesea ver o imprimir el comprobante oficial en este momento?")) {
            servicio.imprimirTicket(idGenerado);
        }
    } catch (const ClinicaException& e) {
        std::cout << "\n" << ROJO << "[ERROR DE REGLA DE NEGOCIO] " << e.what() << RESET << "\n";
    }

    pausar();
}

void InterfazUI::vistaCancelarTurno() {
    imprimirBanner("CANCELACION DE TURNO");
    mostrarTablaTurnos("HISTORIAL DE TURNOS REGISTRADOS");

    int idTurno = leerEntero("\nIngrese ID del turno a cancelar: ", 1, 99999);

    Turno* t = servicio.buscarTurnoPorId(idTurno);
    if (!t) {
        std::cout << "\n" << ROJO << "[ERROR] No existe ningun turno con el ID " << idTurno << "." << RESET << "\n";
        pausar();
        return;
    }

    std::cout << "\nDatos del turno seleccionado:\n" << *t << "\n";

    // Comprobacion explicita si ya esta cancelado
    if (t->getEstado() == "CANCELADO") {
        std::cout << "\n" << AMARILLO << NEGRITA 
                  << "[!] AVISO: El turno #" << idTurno << " YA SE ENCUENTRA CANCELADO." << RESET << "\n";
        std::cout << "    No es posible cancelarlo nuevamente.\n";
        pausar();
        return;
    }

    if (confirmarAccion("\nEsta seguro que desea cancelar este turno?")) {
        try {
            servicio.cancelarTurno(idTurno);
            std::cout << "\n" << VERDE << "[OK] El turno ha sido cancelado exitosamente." << RESET << "\n";
        } catch (const ClinicaException& e) {
            std::cout << "\n" << ROJO << "[ERROR] " << e.what() << RESET << "\n";
        }
    } else {
        std::cout << "\nOperacion cancelada por el usuario.\n";
    }

    pausar();
}

void InterfazUI::vistaReprogramarTurno() {
    imprimirBanner("REPROGRAMACION DE TURNO");
    mostrarTablaTurnos("HISTORIAL DE TURNOS REGISTRADOS");

    int idTurno = leerEntero("\nIngrese ID del turno a reprogramar: ", 1, 99999);

    Turno* t = servicio.buscarTurnoPorId(idTurno);
    if (!t) {
        std::cout << "\n" << ROJO << "[ERROR] No existe ningun turno con el ID " << idTurno << "." << RESET << "\n";
        pausar();
        return;
    }

    std::cout << "\nTurno seleccionado:\n" << *t << "\n\n";

    if (t->getEstado() == "CANCELADO") {
        std::cout << AMARILLO << "[!] AVISO: No es posible reprogramar un turno cancelado. Asigne un nuevo turno desde el menu.\n" << RESET;
        pausar();
        return;
    }

    std::cout << "Ingrese la nueva fecha y hora deseadas:\n";
    std::string nuevaFecha = leerFecha("Nueva fecha");
    std::string nuevaHora = leerHora("Nueva hora");

    try {
        servicio.reprogramarTurno(idTurno, nuevaFecha, nuevaHora);
        std::cout << "\n" << VERDE << "[OK] El turno fue reprogramado correctamente para el " 
                  << nuevaFecha << " a las " << nuevaHora << " hs." << RESET << "\n";
    } catch (const ClinicaException& e) {
        std::cout << "\n" << ROJO << "[ERROR] " << e.what() << RESET << "\n";
    }

    pausar();
}

void InterfazUI::vistaCambiarEstadoTurno() {
    imprimirBanner("CAMBIO DE ESTADO DE TURNO");
    mostrarTablaTurnos("HISTORIAL DE TURNOS REGISTRADOS");

    int idTurno = leerEntero("\nIngrese ID del turno: ", 1, 99999);

    Turno* t = servicio.buscarTurnoPorId(idTurno);
    if (!t) {
        std::cout << "\n" << ROJO << "[ERROR] No existe ningun turno con el ID " << idTurno << "." << RESET << "\n";
        pausar();
        return;
    }

    std::cout << "\nTurno actual:\n" << *t << "\n\n";
    std::cout << "Seleccione el nuevo estado:\n";
    std::cout << "  [1] PENDIENTE\n";
    std::cout << "  [2] ATENDIDO\n";
    std::cout << "  [3] CANCELADO\n";
    int opc = leerEntero("Opcion [1-3]: ", 1, 3);

    std::string nuevoEst = (opc == 1 ? "PENDIENTE" : (opc == 2 ? "ATENDIDO" : "CANCELADO"));

    try {
        servicio.cambiarEstadoTurno(idTurno, nuevoEst);
        std::cout << "\n" << VERDE << "[OK] Estado del turno actualizado a: " << nuevoEst << RESET << "\n";
    } catch (const ClinicaException& e) {
        std::cout << "\n" << ROJO << "[ERROR] " << e.what() << RESET << "\n";
    }

    pausar();
}

void InterfazUI::vistaListarTurnos() {
    imprimirBanner("LISTADO CRONOLOGICO DE TURNOS");
    mostrarTablaTurnos("AGENDA CRONOLOGICA COMPLETA");
    pausar();
}

void InterfazUI::vistaBuscarTurno() {
    imprimirBanner("CONSULTA DE TURNO POR ID");
    mostrarTablaTurnos("HISTORIAL DISPONIBLE");

    int idTurno = leerEntero("\nIngrese ID de Turno: ", 1, 99999);

    Turno* t = servicio.buscarTurnoPorId(idTurno);
    if (t) {
        std::cout << "\n" << VERDE << "Detalles del turno encontrado:" << RESET << "\n";
        std::cout << *t << "\n";

        Medico* m = servicio.buscarMedicoPorId(t->getIdMedico());
        Paciente* p = servicio.buscarPacientePorId(t->getIdPaciente());
        if (m) std::cout << "  Medico: Dr/a. " << m->getApellido() << ", " << m->getNombre() << " (" << m->getEspecialidad() << ")\n";
        if (p) std::cout << "  Paciente: " << p->getApellido() << ", " << p->getNombre() << " (OS: " << p->getObraSocial() << ")\n";
    } else {
        std::cout << "\n" << ROJO << "No existe ningun turno con el ID " << idTurno << RESET << "\n";
    }

    pausar();
}

void InterfazUI::vistaImprimirComprobante() {
    imprimirBanner("IMPRESION DE COMPROBANTE OFICIAL");
    mostrarTablaTurnos("HISTORIAL DE TURNOS REGISTRADOS");

    int idTurno = leerEntero("\nIngrese ID de Turno: ", 1, 99999);

    try {
        servicio.imprimirTicket(idTurno);
    } catch (const ClinicaException& e) {
        std::cout << "\n" << ROJO << "[ERROR] " << e.what() << RESET << "\n";
    }

    pausar();
}

void InterfazUI::vistaVisualizarArbol() {
    imprimirBanner("ESQUEMA DE TURNOS EN ARBOL");
    std::cout << "Visualizacion de los nodos del Arbol Binario generados por fecha y hora:\n";
    std::cout << GRIS << "(Ramas derechas hacia arriba/derecha, ramas izquierdas hacia abajo/izquierda)\n\n" << RESET;

    servicio.mostrarEstructuraArbol();

    std::cout << "\n" << CIAN << "Altura / Profundidad del Arbol calculada recursivamente: " 
              << servicio.getAlturaArbol() << " niveles\n" << RESET;
    pausar();
}

// ==========================================
// SECCION DE REPORTES Y ESTADISTICAS
// ==========================================

void InterfazUI::menuReportes() {
    int opc = -1;
    do {
        imprimirBanner("REPORTES Y ESTADISTICAS");
        std::cout << "  " << AMARILLO << "[1]" << RESET << " Reporte de turnos por fecha especifica\n";
        std::cout << "  " << AMARILLO << "[2]" << RESET << " Reporte de turnos por medico\n";
        std::cout << "  " << AMARILLO << "[3]" << RESET << " Reporte de historial por paciente\n";
        std::cout << "  " << AMARILLO << "[4]" << RESET << " Estadisticas generales del consultorio\n";
        std::cout << "  " << ROJO << "[0]" << RESET << " Volver al menu principal\n";
        std::cout << CIAN << "----------------------------------------------------------------------------------------\n" << RESET;
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
    std::stringstream buffer;
    buffer << "Turnos programados para el dia " << fecha << ":\n\n";

    if (lista.empty()) {
        std::cout << "\nNo hay turnos registrados en la fecha " << fecha << ".\n";
    } else {
        for (const auto& t : lista) {
            Medico* m = servicio.buscarMedicoPorId(t.getIdMedico());
            Paciente* p = servicio.buscarPacientePorId(t.getIdPaciente());
            buffer << "  Hora: " << t.getHora() 
                   << " | Dr. " << (m ? m->getApellido() : "?") 
                   << " | Paciente: " << (p ? p->getApellido() : "?") 
                   << " | Estado: " << t.getEstado() 
                   << " | Motivo: " << t.getMotivo() << "\n";
        }
        std::cout << buffer.str();

        if (confirmarAccion("\nDesea exportar este reporte a un archivo de texto?")) {
            std::string nom = "reporte_fecha_" + fecha.substr(0, 2) + "_" + fecha.substr(3, 2) + ".txt";
            servicio.exportarReporteTexto(nom, buffer.str());
            std::cout << VERDE << "[OK] Reporte exportado en data/" << nom << RESET << "\n";
        }
    }

    pausar();
}

void InterfazUI::vistaReportePorMedico() {
    imprimirBanner("REPORTE DE TURNOS POR MEDICO");
    mostrarTablaMedicos("HISTORIAL DE MEDICOS REGISTRADOS");

    int idMed = leerEntero("\nIngrese ID del Medico: ", 1, 99999);

    Medico* m = servicio.buscarMedicoPorId(idMed);
    if (!m) {
        std::cout << "\n" << ROJO << "[ERROR] Medico no encontrado." << RESET << "\n";
        pausar();
        return;
    }

    std::stringstream buffer;
    buffer << "Agenda de turnos para Dr/a. " << m->getApellido() << ", " << m->getNombre() 
           << " (" << m->getEspecialidad() << "):\n\n";

    auto lista = servicio.filtrarTurnosPorMedico(idMed);
    if (lista.empty()) {
        std::cout << "\nEl profesional no tiene turnos registrados.\n";
    } else {
        for (const auto& t : lista) {
            Paciente* p = servicio.buscarPacientePorId(t.getIdPaciente());
            buffer << "  Turno #" << t.getIdTurno() 
                   << " | " << t.getFecha() << " " << t.getHora() 
                   << " | Paciente: " << (p ? p->getApellido() + " (DNI " + p->getDni() + ")" : "?")
                   << " | Estado: " << t.getEstado() 
                   << " | Motivo: " << t.getMotivo() << "\n";
        }
        std::cout << buffer.str();

        if (confirmarAccion("\nDesea exportar este reporte a archivo?")) {
            std::string nom = "reporte_medico_" + std::to_string(idMed) + ".txt";
            servicio.exportarReporteTexto(nom, buffer.str());
            std::cout << VERDE << "[OK] Reporte exportado en data/" << nom << RESET << "\n";
        }
    }

    pausar();
}

void InterfazUI::vistaReportePorPaciente() {
    imprimirBanner("HISTORIAL DE TURNOS POR PACIENTE");
    mostrarTablaPacientes("HISTORIAL DE PACIENTES REGISTRADOS");

    int idPac = leerEntero("\nIngrese ID del Paciente: ", 1, 99999);

    Paciente* p = servicio.buscarPacientePorId(idPac);
    if (!p) {
        std::cout << "\n" << ROJO << "[ERROR] Paciente no encontrado." << RESET << "\n";
        pausar();
        return;
    }

    std::stringstream buffer;
    buffer << "Historial clinico de turnos para: " << p->getApellido() << ", " << p->getNombre() 
           << " (DNI: " << p->getDni() << "):\n\n";

    auto lista = servicio.filtrarTurnosPorPaciente(idPac);
    if (lista.empty()) {
        std::cout << "\nEl paciente no tiene turnos registrados.\n";
    } else {
        for (const auto& t : lista) {
            Medico* m = servicio.buscarMedicoPorId(t.getIdMedico());
            buffer << "  Turno #" << t.getIdTurno() 
                   << " | " << t.getFecha() << " " << t.getHora() 
                   << " | Dr/a: " << (m ? m->getApellido() + " (" + m->getEspecialidad() + ")" : "?")
                   << " | Estado: " << t.getEstado() 
                   << " | Motivo: " << t.getMotivo() << "\n";
        }
        std::cout << buffer.str();

        if (confirmarAccion("\nDesea exportar este historial a archivo?")) {
            std::string nom = "reporte_paciente_" + std::to_string(idPac) + ".txt";
            servicio.exportarReporteTexto(nom, buffer.str());
            std::cout << VERDE << "[OK] Historial exportado en data/" << nom << RESET << "\n";
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

    std::cout << BLANCO << NEGRITA << "RESUMEN DE PROFESIONALES Y PACIENTES:\n" << RESET;
    std::cout << "  Total de medicos registrados: " << totalMedicos << " (" << VERDE << medicosActivos << " activos" << RESET << ")\n";
    std::cout << "  Total de pacientes en padron: " << totalPacientes << " (" << VERDE << pacientesActivos << " activos" << RESET << ")\n";
    std::cout << "  Total de personas en memoria (static): " << CIAN << Persona::getTotalPersonas() << RESET << "\n\n";

    std::cout << BLANCO << NEGRITA << "RESUMEN DE GESTION DE TURNOS:\n" << RESET;
    std::cout << "  Total historico de turnos:    " << totalTurnos << "\n";
    std::cout << "  Turnos pendientes:            " << AMARILLO << pendientes << RESET << "\n";
    std::cout << "  Turnos atendidos:             " << VERDE << atendidos << RESET << "\n";
    std::cout << "  Turnos cancelados:            " << ROJO << cancelados << RESET << "\n\n";

    if (totalTurnos > 0) {
        double porcAtendidos = (atendidos * 100.0) / totalTurnos;
        double porcCancelados = (cancelados * 100.0) / totalTurnos;
        std::cout << std::fixed << std::setprecision(1);
        std::cout << "  Tasa de atencion efectiva:    " << VERDE << porcAtendidos << "%" << RESET << "\n";
        std::cout << "  Tasa de cancelacion:          " << ROJO << porcCancelados << "%" << RESET << "\n";
    }

    pausar();
}
