#include "Paciente.hpp"
#include <cstring>
#include <iostream>
#include <iomanip>

using namespace std;

Paciente::Paciente() {
    id = 0;
    nombre[0] = '\0';
    apellido[0] = '\0';
    cedula[0] = '\0';
    edad = 0;
    sexo = 'M';
    telefono[0] = '\0';
    email[0] = '\0';
    activo = true;
    eliminado = false;
    cantidadConsultas = 0;
    primerConsultaID = -1;
    cantidadCitas = 0;
    fechaRegistro = time(0);
    
    // Inicializar array de citas
    for (int i = 0; i < 20; i++) {
        citasIDs[i] = -1;
    }
}

// Setters
void Paciente::setId(int i) { 
    id = i; 
}

void Paciente::setDatosPersonales(const char* n, const char* a, const char* c, int e, char s) {
    strncpy(nombre, n, 49);
    nombre[49] = '\0';
    strncpy(apellido, a, 49);
    apellido[49] = '\0';
    strncpy(cedula, c, 19);
    cedula[19] = '\0';
    edad = e;
    sexo = s;
}

void Paciente::setContacto(const char* t, const char* mail) {
    strncpy(telefono, t, 14);
    telefono[14] = '\0';
    if (mail != nullptr) {
        strncpy(email, mail, 49);
        email[49] = '\0';
    } else {
        email[0] = '\0';
    }
}

void Paciente::setEliminado(bool estado) { 
    eliminado = estado; 
}

// Getters
int Paciente::getId() const { 
    return id; 
}

const char* Paciente::getNombre() const { 
    return nombre; 
}

const char* Paciente::getApellido() const { 
    return apellido; 
}

const char* Paciente::getCedula() const { 
    return cedula; 
}

const char* Paciente::getTelefono() const {  // CORREGIDO
    return telefono;
}

const char* Paciente::getEmail() const {  
    return email;
}

bool Paciente::isEliminado() const {  // CORREGIDO (era "isFininado")
    return eliminado;
}

int Paciente::getEdad() const { 
    return edad; 
}

char Paciente::getSexo() const { 
    return sexo; 
}

// Métodos para relaciones
bool Paciente::agregarCita(int citaID) {  // CORREGIDO
    if (cantidadCitas >= 20) {
        return false; // Array lleno
    }
    citasIDs[cantidadCitas] = citaID;
    cantidadCitas++;
    return true;
}

void Paciente::setPrimerConsulta(int consultaID) { 
    primerConsultaID = consultaID; 
}

int Paciente::getPrimerConsulta() const { 
    return primerConsultaID; 
}

int Paciente::getCantidadConsultas() const { 
    return cantidadConsultas; 
}

void Paciente::incrementarConsultas() { 
    cantidadConsultas++; 
}

int Paciente::getCantidadCitas() const { 
    return cantidadCitas; 
}

int Paciente::getCitaID(int index) const { 
    if(index >= 0 && index < cantidadCitas) {
        return citasIDs[index];
    }
    return -1;
}

// Validación
bool Paciente::validarDatos() const {
    return (strlen(nombre) > 0 && 
            strlen(apellido) > 0 && 
            strlen(cedula) > 0 && 
            edad >= 0 && 
            edad <= 120);
}

// Métodos de visualización
void Paciente::mostrarFila() const {
    // Formato: ID | Nombre Apellido | Cedula | Edad | Sexo | Telefono | Estado
    cout << "| " << setw(4) << id << " | "
         << left << setw(20) << (string(nombre) + " " + string(apellido)) << " | "
         << setw(12) << cedula << " | "
         << right << setw(3) << edad << " | "
         << setw(4) << sexo << " | "
         << left << setw(12) << telefono << " | "
         << (eliminado ? "ELIM" : "ACT")
         << "\n";
}

void Paciente::mostrarDetalle() const {
    cout << "\n--- DATOS DEL PACIENTE ---\n";
    cout << "ID: " << id << "\nNombre: " << nombre << " " << apellido;
    cout << "\nCedula: " << cedula << "\nEdad: " << edad << "\nSexo: " << sexo;
    cout << "\nTelefono: " << telefono << "\nEmail: " << email << endl;
    cout << "Consultas realizadas: " << cantidadConsultas << endl;
    cout << "Citas agendadas: " << cantidadCitas << endl;
}