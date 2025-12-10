#ifndef PACIENTE_HPP
#define PACIENTE_HPP

#include <ctime>

class Paciente {
private:
    int id;
    char nombre[50];
    char apellido[50];
    char cedula[20];
    int edad;
    char sexo;
    char telefono[15];
    char email[50];
    
    // Relaciones
    int cantidadConsultas;
    int primerConsultaID;
    int cantidadCitas;
    int citasIDs[20];
    
    // Metadata
    bool activo;
    bool eliminado;
    time_t fechaRegistro;

public:
    Paciente();
    
    // Setters
    void setId(int i);
    void setDatosPersonales(const char* n, const char* a, const char* c, int e, char s);
    void setContacto(const char* t, const char* mail);
    void setEliminado(bool estado);
    
    // Getters
    int getId() const;
    const char* getNombre() const;
    const char* getApellido() const;
    const char* getCedula() const;
    const char* getTelefono() const;
    const char* getEmail() const;
    bool isEliminado() const;
    int getEdad() const;
    char getSexo() const;
    
    // Métodos para relaciones
    bool agregarCita(int citaID);
    void setPrimerConsulta(int consultaID);
    int getPrimerConsulta() const;
    int getCantidadConsultas() const;
    void incrementarConsultas();
    int getCantidadCitas() const;
    int getCitaID(int index) const;

    // Validación
    bool validarDatos() const;
    
    // Visualización
    void mostrarFila() const;
    void mostrarDetalle() const;
};

#endif