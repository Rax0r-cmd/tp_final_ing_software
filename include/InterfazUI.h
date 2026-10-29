#ifndef INTERFAZ_UI_H
#define INTERFAZ_UI_H

#include "ClinicaServicio.h"

// Clase encargada de la vista e interaccion por consola (separacion de responsabilidades)
class InterfazUI {
private:
    ClinicaServicio servicio;

    // Submenus
    void menuMedicos();
    void menuPacientes();
    void menuTurnos();
    void menuReportes();

    // Vistas Medicos
    void vistaAltaMedico();
    void vistaModificarMedico();
    void vistaBajaMedico();
    void vistaReactivarMedico();
    void vistaListarMedicos();
    void vistaBuscarMedico();

    // Vistas Pacientes
    void vistaAltaPaciente();
    void vistaModificarPaciente();
    void vistaBajaPaciente();
    void vistaReactivarPaciente();
    void vistaListarPacientes();
    void vistaBuscarPaciente();

    // Vistas Turnos
    void vistaAsignarTurno();
    void vistaCancelarTurno();
    void vistaReprogramarTurno();
    void vistaCambiarEstadoTurno();
    void vistaListarTurnos();
    void vistaBuscarTurno();
    void vistaImprimirComprobante();

    // Vistas Reportes
    void vistaReportePorFecha();
    void vistaReportePorMedico();
    void vistaReportePorPaciente();
    void vistaEstadisticas();

    // Encabezados visuales
    void imprimirBanner(const std::string& titulo);

public:
    InterfazUI();
    void ejecutar();
};

#endif // INTERFAZ_UI_H

