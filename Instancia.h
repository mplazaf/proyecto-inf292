#ifndef INSTANCIA_H
#define INSTANCIA_H
#include <string>
#include <vector>
#include "Elemento.h"
#include "Alternativa.h"
using namespace std;

class Instancia {
public:
    string tipo;
    int semilla;
    double largo;
    double ancho;
    double altura;
    double superficie;
    double volumen;
    double Umax;
    vector<Elemento> elementos;

    Instancia(
        string tipo,
        int semilla,
        double largo,
        double ancho,
        double altura,
        double Umax
    ) {
        this->tipo = tipo;
        this->semilla = semilla;
        this->largo = largo;
        this->ancho = ancho;
        this->altura = altura;
        superficie = largo * ancho;
        volumen = superficie * altura;
        this->Umax = Umax;
    }
};

#endif
