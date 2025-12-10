//REPUBLICA BOLIVARIA DE VENEZUELA 
//UNIVERSIDAD RAFAEL URDANETE 
//PROGRAMACION II
//ESTUDIANTE: RUBEN ADARME C.I.: 32.453.470

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include "hospital/Hospital.hpp"
#include "persistencia/GestorArchivos.hpp"
#include "utilidades/Formatos.hpp"
#include "utilidades/Validaciones.hpp"
#include "pacientes/operacionesPacientes.hpp"
#include "doctores/operacionesDoctores.hpp"
#include "citas/operacionesCitas.hpp"
#include "historial/operacionesHistorial.hpp"

using namespace std;

// Declaracion de menus locales
void menuPacientes(Hospital& h);
void menuDoctores(Hospital& h);
void menuCitas(Hospital& h);

int main() {
    // Inicializar sistema
    if (!GestorArchivos::inicializarSistemaArchivos()) {
        cout << "Error critico al inicializar sistema de archivos." << endl;
        return 1;
    }

    // Cargar hospital o crear uno por defecto
    Hospital hospital;
    ifstream fileH(Rutas::HOSPITAL, ios::binary);
    
    if (fileH) {
        fileH.read((char*)&hospital, sizeof(Hospital));
        fileH.close();
    } else {
        // Crear hospital por defecto
        hospital = Hospital("HOSPITAL CENTRAL", "Calle 123, Urb. La Salud, Ciudad: Maracaibo, Edo. Zulia", 
                           "(0261) 123-4567", "ClinicaOdant@gmail.com");
    }

    cout << "Ruta doctor:" << Rutas::DOCTORES << endl;
    cout << "Ruta pacientes:" << Rutas::PACIENTES << endl;
    cout << "Ruta citas:" << Rutas::CITAS << endl;
    cout << "Ruta historiales:" << Rutas::HISTORIAL << endl;   

    int opcion;
    do {
        system("cls");
        cout << "\t\t+----------------------------------------+" << endl;
        cout << "\t\t+            HOSPITAL CENTRAL            +" << endl;
        cout << "\t\t+----------------------------------------+" << endl;
        cout << "\t\t+  RIF: J-12345678-9                     +" << endl;
        cout << "\t\t+  Telefono: (0261) 123-4567             +" << endl;
        cout << "\t\t+  Email: ClinicaOdant@gmail.com         +" << endl;
        cout << "\t\t+----------------------------------------+" << endl;
        cout << "\t\t+  Atencion: Lunes a Viernes             +" << endl;
        cout << "\t\t+  8:00 am a 12:00 m./1:00 pm a 5:00 pm  +" << endl;
        cout << "\t\t+----------------------------------------+" << endl;
        cout << "\t\t+  Direccion: Calle 123, Urb. La Salud   +" << endl;
        cout << "\t\t+  Ciudad: Maracaibo, Edo. Zulia         +" << endl;
        cout << "\t\t+----------------------------------------+" << endl;
        cout << "\t\t+----------------------------------------+" << endl;
        cout << "\t\t+          SELECCIONE UNA OPCION         +" << endl;
        cout << "\t\t+----------------------------------------+" << endl;
        cout << "\t\t+  1. Gestion de Pacientes               +" << endl;
        cout << "\t\t+  2. Gestion de Doctores                +" << endl;
        cout << "\t\t+  3. Gestion de Citas                   +" << endl;
        cout << "\t\t+  4. Realizar Consulta                  +" << endl;
        cout << "\t\t+  5. Ver Estadisticas                   +" << endl;
        cout << "\t\t+  6. Guardar y Salir                    +" << endl;
        cout << "\t\t+----------------------------------------+" << endl;

        cout << "\nOpcion: ";
        opcion = Validaciones::leerEntero("", 1, 6);

        switch (opcion) {
            case 1: 
                menuPacientes(hospital); 
                break;
            case 2: 
                menuDoctores(hospital); 
                break;
            case 3: 
                menuCitas(hospital); 
                break;
            case 4: 
                realizarConsulta(hospital); 
                break;
            case 5:
                hospital.mostrarEstadisticas();
                system("pause");
                break;
            case 6: {
                // Guardar datos antes de salir
                ofstream out(Rutas::HOSPITAL, ios::binary);
                out.write((char*)&hospital, sizeof(Hospital));
                out.close();
                cout << "Saliendo del programa... Vuelva Pronto!" << endl;
                system("pause");
            } break;
            default:
                cout << "ERROR!: Opcion invalida (1-6)" << endl;
                system("pause");
                break;
        }
    } while (opcion != 6);

    return 0;
}

void menuPacientes(Hospital& h) {
    int op;
    do {
        system("cls");
        cout << "\t\t+----------------------------------------+" << endl;
        cout << "\t\t+           GESTION DE PACIENTES         +" << endl;
        cout << "\t\t+----------------------------------------+" << endl;
        cout << "\t\t+         SELECCIONE UNA OPCION          +" << endl;
        cout << "\t\t+----------------------------------------+" << endl;
        cout << "\t\t+  1. Registrar Paciente                 +" << endl;
        cout << "\t\t+  2. Listar Todos                       +" << endl;
        cout << "\t\t+  3. Buscar por ID                      +" << endl;
        cout << "\t\t+  4. Buscar por Cedula                  +" << endl;
        cout << "\t\t+  5. Ver Historial Medico               +" << endl;
        cout << "\t\t+  6. Eliminar Paciente                  +" << endl;
        cout << "\t\t+  7. Modificar Datos                    +" << endl;
        cout << "\t\t+  8. Volver al menu principal           +" << endl;
        cout << "\t\t+----------------------------------------+" << endl;

        cout << "\nOpcion: ";
        op = Validaciones::leerEntero("", 1, 8);
        
        switch (op) {
            case 1: 
                registrarPaciente(h); 
                break;
            case 2: 
                listarPacientes(); 
                break;
            case 3: 
                buscarPacientePorID(); 
                break;
            case 4: 
                buscarPacientePorCedula(); 
                break;
            case 5: 
                verHistorialPaciente(); 
                break;
            case 6: 
                eliminarPaciente(h); 
                break;
            case 7: 
                modificarPaciente(); 
                break;
            case 8: 
                break;
            default: 
                cout << "ERROR!: Opcion invalida (1-8)" << endl;
                system("pause");
                break;
        }
    } while(op != 8);
}

void menuDoctores(Hospital& h) {
    int op;
    do {
        system("cls");
        cout << "\t\t+----------------------------------------+" << endl;
        cout << "\t\t+           GESTION DE DOCTORES          +" << endl;
        cout << "\t\t+----------------------------------------+" << endl;
        cout << "\t\t+         SELECCIONE UNA OPCION          +" << endl;
        cout << "\t\t+----------------------------------------+" << endl;
        cout << "\t\t+  1. Registrar Doctor                   +" << endl;
        cout << "\t\t+  2. Listar Doctores                    +" << endl;
        cout << "\t\t+  3. Buscar por ID                      +" << endl;
        cout << "\t\t+  4. Asignar Paciente                   +" << endl;
        cout << "\t\t+  5. Ver Pacientes Asignados            +" << endl;
        cout << "\t\t+  6. Modificar Doctor                   +" << endl;
        cout << "\t\t+  7. Volver al menu principal           +" << endl;
        cout << "\t\t+----------------------------------------+" << endl;
        
        cout << "\nOpcion: ";
        op = Validaciones::leerEntero("", 1, 7);
        
        switch(op) {
            case 1: 
                registrarDoctor(h); 
                break;
            case 2: 
                listarDoctores(); 
                break;
            case 3: 
                buscarDoctorPorID(); 
                break;
            case 4: 
                asignarPacienteDoctor(); 
                break;
            case 5: 
                verPacientesDeDoctor(); 
                break;
            case 6: 
                modificarDoctor(); 
                break;
            case 7: 
                break;
            default: 
                cout << "ERROR!: Opcion invalida (1-7)" << endl;
                system("pause");
                break;
        }
    } while(op != 7);
}

void menuCitas(Hospital& h) {
    int op;
    do {
        system("cls");
        cout << "\t\t+----------------------------------------+" << endl;
        cout << "\t\t+            GESTION DE CITAS            +" << endl;
        cout << "\t\t+----------------------------------------+" << endl;
        cout << "\t\t+         SELECCIONE UNA OPCION          +" << endl;
        cout << "\t\t+----------------------------------------+" << endl;
        cout << "\t\t+  1. Agendar Cita                       +" << endl;
        cout << "\t\t+  2. Listar Citas                       +" << endl;
        cout << "\t\t+  3. Modificar/Cancelar Cita            +" << endl;
        cout << "\t\t+  4. Volver al menu principal           +" << endl;
        cout << "\t\t+----------------------------------------+" << endl;
        
        cout << "\nOpcion: ";
        op = Validaciones::leerEntero("", 1, 4);
        
        switch(op) {
            case 1: 
                agendarCita(h); 
                break;
            case 2: 
                listarCitas(); 
                break;
            case 3: 
                modificarCita(); 
                break;
            case 4: 
                break;
            default: 
                cout << "ERROR!: Opcion invalida (1-4)" << endl;
                system("pause");
                break;
        }
    } while(op != 4);
}