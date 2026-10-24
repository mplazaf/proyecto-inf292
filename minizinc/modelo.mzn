% ==========================================
% MODELO DE OPTIMIZACION - ENTREGA 2
% ==========================================

% Cantidad de elementos optimizables
int: nOpt;

% Maximo numero de alternativas entre los elementos
int: maxAlt;

set of int: E = 1..nOpt;
set of int: A = 1..maxAlt;

% Cantidad real de alternativas de cada elemento
array[E] of int: nAlt;

% Nombre de cada elemento optimizable
array[E] of string: nombreElemento;

% Area de cada elemento optimizable
array[E] of float: area;

% Valor U de cada alternativa
array[E, A] of float: U;

% Costo por m2 de cada alternativa
array[E, A] of float: costo;

% Limite termico maximo
float: Umax;

% Suma de las areas de TODOS los elementos
float: areaTotal;

% Contribucion de los elementos fijos: suma(Area_e * U_e)
float: aporteUFijo;

% Costo total de los elementos fijos: suma(Area_e * Costo_e)
float: costoFijo;


% ==========================================
% VARIABLES DE DECISION
% ==========================================

% x[e,a] = 1 si para el elemento e se selecciona la alternativa a
array[E, A] of var 0..1: x;


% ==========================================
% RESTRICCION 1: UNA ALTERNATIVA POR ELEMENTO
% ==========================================

constraint
    forall(e in E)(
        sum(a in A where a <= nAlt[e])
        (x[e,a]) = 1
    );


% Las posiciones que no representan alternativas reales deben quedar en cero
constraint
    forall(e in E, a in A)(
        if a > nAlt[e] then
            x[e,a] = 0
        else
            true
        endif
    );


% ==========================================
% RESTRICCION TERMICA
% ==========================================

constraint
    sum(e in E, a in A where a <= nAlt[e])
    (
        area[e] * U[e,a] * x[e,a]
    )
    + aporteUFijo <= Umax * areaTotal;

% ==========================================
% U OBTENIDO
% ==========================================

var float: Uobtenido =
    (
        sum(e in E, a in A where a <= nAlt[e])
        (
            area[e] * U[e,a] * x[e,a]
        )
        + aporteUFijo
    )
    / areaTotal;
    
% ==========================================
% FUNCION OBJETIVO
% ==========================================

solve minimize
    sum(e in E, a in A where a <= nAlt[e])
    (
        area[e] * costo[e,a] * x[e,a]
    )
    + costoFijo;


% ==========================================
% SALIDA
% ==========================================

output [
    "==================================\n",
    "RESULTADO DE OPTIMIZACION\n",
    "==================================\n\n",

    "Alternativas seleccionadas:\n",

    concat([
        if fix(x[e,a]) = 1 then
            nombreElemento[e]
            ++ " -> A"
            ++ show(a)
            ++ "\n"
        else
            ""
        endif

        | e in E, a in A where a <= nAlt[e]
    ]),

    "\nCosto total = $",
    show(
        sum(e in E, a in A where a <= nAlt[e])
        (
            area[e] * costo[e,a] * x[e,a]
        )
        + costoFijo
    ),

    "\nU obtenido = ",
    show(Uobtenido),

    "\nUmax = ",
    show(Umax),

    "\n"
];