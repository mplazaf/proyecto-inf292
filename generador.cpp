#include <iostream>
#include <vector>
#include <string>
#include <random>
using namespace std;

class Alternativa {
public:
    double U;
    double costoM2;
};

class Elemento {
public:
    string nombre;
    double area;
    bool fijo;
    vector<Alternativa> alternativas;
    //si es fijo
    double Ufijo;
    double costoFijoM2;
};

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
};



Instancia generarInstancia(string tipo, int semilla) {
    mt19937 gen(semilla);

    uniform_real_distribution<double> distLargo(8.0, 15.0);
    uniform_real_distribution<double> distAncho(6.0, 12.0);
    uniform_real_distribution<double> distAltura(2.4, 2.8);

    double largo = distLargo(gen);
    double ancho = distAncho(gen);
    double altura = distAltura(gen);

    double superficie = largo * ancho;
    double volumen = superficie * altura;

    uniform_real_distribution<double> distUmax(0.4, 1.0);
    double Umax = distUmax(gen);

    Instancia instancia(
        tipo,
        superficie,
        altura,
        Umax,
        semilla
    );

    double areaPiso = superficie;
    double areaTecho = superficie;
    double perimetro = 2.0 * (largo + ancho);
    double areaMurosBruta = perimetro * altura;
    uniform_real_distribution<double> distPorcentajeVentanas(0.10, 0.25);
    double porcentajeVentanas = distPorcentajeVentanas(gen);
    double areaVentanas = areaMurosBruta * porcentajeVentanas;
    double areaMuros = areaMurosBruta - areaVentanas;

    Elemento muro(
        "Muro",
        areaMuros,
        false
    );
    Elemento techo(
        "Techo",
        areaTecho,
        false
    );
    Elemento piso(
        "Piso",
        areaPiso,
        false
    );
    Elemento ventana(
        "Ventana",
        areaVentanas,
        false
    );
    instancia.elementos.push_back(muro);
    instancia.elementos.push_back(techo);
    instancia.elementos.push_back(piso);
    instancia.elementos.push_back(ventana);
    return instancia;
}

int main() {

    Instancia casa =
        generarInstancia("pequena", 42);

    cout << "Tipo: "
         << casa.tipo << endl;

    cout << "Superficie: "
         << casa.superficie << endl;

    cout << "Altura: "
         << casa.altura << endl;

    cout << "Volumen: "
         << casa.volumen << endl;

    cout << "Umax: "
         << casa.Umax << endl;

    cout << "Semilla: "
         << casa.semilla << endl;

    cout << endl;

    for (const Elemento& e : casa.elementos) {

        cout << e.nombre << endl;

        cout << "Area: "
             << e.area << endl;
        cout << endl;
    }
    return 0;
}
