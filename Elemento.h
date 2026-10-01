#ifndef ELEMENTO_H
#define ELEMENTO_H
#include <string>
#include <vector>
#include "Alternativa.h"
using namespace std;

class Elemento {
public:
    string nombre;
    string categoria;
    double area;
    bool fijo;
    vector<Alternativa> alternativas;
    double Ufijo;
    double costoFijoM2;

    Elemento(
        string nombre,
        string categoria,
        double area,
        bool fijo
    ) {
        this->nombre = nombre;
        this->categoria = categoria;
        this->area = area;
        this->fijo = fijo;

        Ufijo = 0.0;
        costoFijoM2 = 0.0;
    }
};
#endif
