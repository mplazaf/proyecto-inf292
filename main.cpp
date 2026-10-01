#include <bits/stdc++.h>
#include "Instancia.h"
#include "generador.h"

using namespace std;
#include <iostream>
#include <random>
#include <string>

using namespace std;



int main() {

    int semilla = 42;
    mt19937 gen(semilla);

    cout << "Pequena: "
         << generarCantidadAlternativas("pequena", gen)
         << endl;

    cout << "Mediana: "
         << generarCantidadAlternativas("mediana", gen)
         << endl;

    cout << "Grande: "
         << generarCantidadAlternativas("grande", gen)
         << endl;

    return 0;
}
