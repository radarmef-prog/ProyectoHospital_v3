#ifndef FORMATOS_HPP
#define FORMATOS_HPP

#include <string>

class Formatos {
public:
    static void limpiarPantalla();
    static void pausar();
    static void limpiarBuffer();
    static void toLowerCopy(const char* src, char* dst, int dstSize);

    static void imprimirLineaSeparadora();
    static void imprimirEncabezado(const char* titulo);
    static void printCentrado(const char* texto);
    static void printError(const char* mensaje);
    static void printExito(const char* mensaje);
};

#endif