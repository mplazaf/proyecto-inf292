#include <bits/stdc++.h>
#include "Instancia.h"
#include "generador.h"
#include "validacion.h"

using namespace std;

int main() {

    string tipo = "mediana";
    int semilla = 42;

    Instancia casa = generarInstancia(
        tipo,
        semilla
    );

    // ========================================================
    // VALIDACION DE LA INSTANCIA
    // ========================================================

    cout << "==============================" << endl;
    cout << "VALIDACION" << endl;
    cout << "==============================" << endl;

    if (validarInstancia(casa)) {
        cout << "Instancia valida" << endl;
    }
    else {
        cout << "Instancia invalida" << endl;
        return 1;
    }


    cout << "\n==============================" << endl;
    cout << "DATOS DE LA INSTANCIA" << endl;
    cout << "==============================" << endl;

    cout << "Tipo: " << casa.tipo << endl;
    cout << "Semilla: " << casa.semilla << endl;

    cout << "\nDIMENSIONES" << endl;

    cout << "Largo: "
         << casa.largo
         << " m" << endl;

    cout << "Ancho: "
         << casa.ancho
         << " m" << endl;

    cout << "Altura: "
         << casa.altura
         << " m" << endl;

    cout << "Superficie: "
         << casa.superficie
         << " m2" << endl;

    cout << "Volumen: "
         << casa.volumen
         << " m3" << endl;

    cout << "\nUmax: "
         << casa.Umax
         << endl;

    cout << "\nCantidad total de elementos: "
         << casa.elementos.size()
         << endl;


    cout << "\n==============================" << endl;
    cout << "ELEMENTOS" << endl;
    cout << "==============================" << endl;

    int cantidadFijos = 0;
    int cantidadOptimizables = 0;

    for (const Elemento& elemento : casa.elementos) {

        cout << "\n------------------------------" << endl;

        cout << "Nombre: "
             << elemento.nombre
             << endl;

        cout << "Categoria: "
             << elemento.categoria
             << endl;

        cout << "Area: "
             << elemento.area
             << " m2" << endl;


        // ELEMENTO FIJO
        if (elemento.fijo) {

            cantidadFijos++;

            cout << "Tipo: FIJO" << endl;

            cout << "U fijo: "
                 << elemento.Ufijo
                 << endl;

            cout << "Costo fijo/m2: $"
                 << elemento.costoFijoM2
                 << endl;
        }

        // ELEMENTO OPTIMIZABLE
        else {

            cantidadOptimizables++;

            cout << "Tipo: OPTIMIZABLE" << endl;

            cout << "Cantidad de alternativas: "
                 << elemento.alternativas.size()
                 << endl;

            cout << "\nCatalogo:" << endl;

            for (const Alternativa& alternativa :
                 elemento.alternativas) {

                cout << "  "
                     << alternativa.nombre
                     << " | U: "
                     << alternativa.U
                     << " | Costo/m2: $"
                     << alternativa.costoM2
                     << endl;
            }
        }
    }


    cout << "\n==============================" << endl;
    cout << "RESUMEN" << endl;
    cout << "==============================" << endl;

    cout << "Elementos totales: "
         << casa.elementos.size()
         << endl;

    cout << "Elementos fijos: "
         << cantidadFijos
         << endl;

    cout << "Elementos optimizables: "
         << cantidadOptimizables
         << endl;

    return 0;
}
