#ifndef ALTERNATIVA_H
#define ALTERNATIVA_H

#include <bits/stdc++.h>
using namespace std;


class Alternativa {
  public:
    string nombre;

    double U;          
    double costoM2;   

    Alternativa(
        string nombre,
        double U,
        double costoM2
    ) {
        this->nombre = nombre;
        this->U = U;
        this->costoM2 = costoM2;
    }
};
#endif
