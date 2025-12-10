#include "GestorArchivos.hpp"
#include <cstdio>
#include <sys/stat.h> // Pa mkdir

#ifdef _WIN32
#include <direct.h>
#define MKDIR(dir) _mkdir(dir)
#else
#define MKDIR(dir) mkdir(dir, 0777)
#endif

using namespace std;

bool GestorArchivos::inicializarSistemaArchivos() {
    // Aqui se ceea la carpeta de datos si no existe
    struct stat info;
    if (stat("datos", &info) != 0) {
        MKDIR("datos");
    }

    bool estado = true;
    estado &= inicializarArchivo(Rutas::PACIENTES);
    estado &= inicializarArchivo(Rutas::DOCTORES);
    estado &= inicializarArchivo(Rutas::CITAS);
    estado &= inicializarArchivo(Rutas::HISTORIAL);
    return estado;
}

bool GestorArchivos::inicializarArchivo(const char* nombreArchivo) {
    ifstream check(nombreArchivo);
    if (check.good()) {
        check.close();
        return true; 
    }
    
    ofstream out(nombreArchivo, ios::binary);
    if (!out) return false;
    
    ArchivoHeader h = {0, 1, 0, 0};
    out.write((char*)&h, sizeof(ArchivoHeader));
    out.close();
    return true;
}

ArchivoHeader GestorArchivos::leerHeader(const char* nombreArchivo) {
    ArchivoHeader h = {0, 0, 0, 0};
    ifstream in(nombreArchivo, ios::binary);
    if (in) {
        in.read((char*)&h, sizeof(ArchivoHeader));
        in.close();
    }
    return h;
}

bool GestorArchivos::actualizarHeader(const char* nombreArchivo, const ArchivoHeader& header) {
    fstream file(nombreArchivo, ios::binary | ios::in | ios::out);
    if (!file) return false;
    file.seekp(0);
    file.write((char*)&header, sizeof(ArchivoHeader));
    return true;
}

std::streamoff GestorArchivos::calcularPosicion(int indice, size_t tamanoRegistro) {
    size_t pos = sizeof(ArchivoHeader) + static_cast<size_t>(indice) * tamanoRegistro;
    return static_cast<std::streamoff>(pos);
}

bool GestorArchivos::guardarRegistro(void* registro, size_t tamano, const char* nombreArchivo) {
    // Abrir el archivo en modo lectura/escritura
    fstream file(nombreArchivo, ios::binary | ios::in | ios::out);
    if (!file) {
        return false;
    }

    // Leer el header actual
    ArchivoHeader h;
    file.read((char*)&h, sizeof(ArchivoHeader));

    file.seekp(0, ios::end);
    file.write((char*)registro, static_cast<std::streamsize>(tamano));

    // Actualizamos el header
    h.cantidadRegistros++;
    h.registrosActivos++;
    h.proximoID = h.proximoID + 1;  // Incrementamos el próximo ID

    // Volvemos al inicio y escribimos el header actualizado
    file.seekp(0);
    file.write((char*)&h, sizeof(ArchivoHeader));

    file.close();
    return true;
}

bool GestorArchivos::leerRegistroPorPosicion(int indice, void* registro, size_t tamano, const char* nombreArchivo) {
    ifstream file(nombreArchivo, ios::binary);
    if (!file) return false;
    
    file.seekg(calcularPosicion(indice, tamano));
    file.read((char*)registro, static_cast<std::streamsize>(tamano));
    
    bool exito = !file.fail();
    file.close();
    return exito;
}

// Busca secuencialmente por ID (asumiendo que el ID es el primer int del struct/class)
bool GestorArchivos::leerRegistroPorID(int id, void* registro, size_t tamano, const char* nombreArchivo, int offsetID) {
    ArchivoHeader h = leerHeader(nombreArchivo);
    ifstream file(nombreArchivo, ios::binary);
    if (!file) return false;
    
    // Buffer temporal para leer y verificar ID
    char* buffer = new char[tamano];
    bool encontrado = false;
    
    for (int i = 0; i < h.cantidadRegistros; i++) {
        file.seekg(calcularPosicion(i, tamano));
        file.read(buffer, static_cast<std::streamsize>(tamano));
        
        // Leemos el ID desde el offset indicado (byte donde empieza el ID)
        int idLeido = *(int*)(buffer + offsetID);
        
        if (idLeido == id) {
            // Copiar al registro destino
            for(size_t k=0; k<tamano; k++) ((char*)registro)[k] = buffer[k];
            encontrado = true;
            break;
        }
    }
    
    delete[] buffer;
    file.close();
    return encontrado;
}

bool GestorArchivos::actualizarRegistro(int id, void* registro, size_t tamano, const char* nombreArchivo, int offsetID) {
    ArchivoHeader h = leerHeader(nombreArchivo);
    fstream file(nombreArchivo, ios::binary | ios::in | ios::out);
    if (!file) return false;
    
    char* buffer = new char[tamano];
    bool encontrado = false;
    
    for (int i = 0; i < h.cantidadRegistros; i++) {
        long pos = calcularPosicion(i, tamano);
        file.seekg(pos);
        file.read(buffer, static_cast<std::streamsize>(tamano));
        
        int idLeido = *(int*)(buffer + offsetID);
        
        if (idLeido == id) {
            file.seekp(pos);
            file.write((char*)registro, static_cast<std::streamsize>(tamano));
            encontrado = true;
            break;
        }
    }
    
    delete[] buffer;
    file.close();
    return encontrado;
}

// offsetEliminado: byte donde esta el bool eliminado
bool GestorArchivos::eliminarRegistroLogico(int id, size_t tamano, const char* nombreArchivo, 
                                            int offsetID, int offsetEliminado) {
    ArchivoHeader h = leerHeader(nombreArchivo);
    fstream file(nombreArchivo, ios::binary | ios::in | ios::out);
    if (!file) return false;
    
    char* buffer = new char[tamano];
    bool encontrado = false;
    
    for (int i = 0; i < h.cantidadRegistros; i++) {
        long pos = calcularPosicion(i, tamano);
        file.seekg(pos);
        file.read(buffer, static_cast<std::streamsize>(tamano));
        
        int idLeido;
        memcpy(&idLeido, buffer + offsetID, sizeof(int));
        
        if (idLeido == id) {
            // Marcar como eliminado
            bool eliminado = true;
            memcpy(buffer + offsetEliminado, &eliminado, sizeof(bool));
            
            file.seekp(pos);
            file.write(buffer, static_cast<std::streamsize>(tamano));
            
            h.registrosActivos--;
            encontrado = true;
            break;
        }
    }
    
    delete[] buffer;
    file.close();
    
    if (encontrado) {
        return actualizarHeader(nombreArchivo, h);
    }
    return false;
}