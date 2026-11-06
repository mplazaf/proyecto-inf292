# Proyecto Optimización INF-292 — Entrega 2

## Descripción

Este proyecto aborda la selección de alternativas constructivas para una vivienda mediante un modelo de **programación lineal entera binaria**. El objetivo es **minimizar el costo total de los materiales**, sujeto a un límite de **transmitancia térmica promedio ponderada por área**.

Se consideran elementos **fijos**, cuyas soluciones ya están definidas, y elementos **optimizables**, para los cuales se selecciona exactamente una alternativa de un catálogo. Las instancias y sus parámetros son sintéticos: no corresponden a cotizaciones comerciales ni a una estimación directa de la demanda energética de la vivienda.

El generador y el validador están implementados en **C++17**. La formulación de optimización utiliza **MiniZinc** y el solver **HiGHS**; los experimentos se ejecutan mediante scripts **PowerShell**.

> Las rutas y comandos de este README corresponden a la **raíz del repositorio de GitHub**, no a la carpeta `codigo/` del paquete local de entrega.

## Estructura del repositorio

```text
proyecto-inf292/
├── main.cpp
├── generador.cpp
├── generador.h
├── validacion.cpp
├── validacion.h
├── Instancia.h
├── Elemento.h
├── Alternativa.h
├── generar_dzn.cpp
├── makefile
├── ejecutar_todas.ps1
├── ejecutar_margenes.ps1
├── resultados.csv
├── resultados_margenes.csv
├── instancia_*.txt
└── minizinc/
    ├── modelo.mzn
    ├── modelo_margenes.mzn
    └── datos/
        ├── pequena_1.dzn
        ├── ...
        ├── mediana_1.dzn
        ├── ...
        └── grande_5.dzn
```

Los archivos principales cumplen las siguientes funciones:

- `main.cpp`: genera y guarda cinco instancias válidas de cada tamaño en archivos `.txt`.
- `generador.cpp` y `generador.h`: definen la construcción de viviendas, elementos y catálogos.
- `validacion.cpp` y `validacion.h`: comprueban la consistencia de las instancias antes de aceptarlas.
- `generar_dzn.cpp`: genera instancias con el mismo procedimiento y las escribe directamente en formato `.dzn`; **no transforma archivos `.txt` existentes**.
- `minizinc/modelo.mzn`: modelo de optimización base.
- `minizinc/modelo_margenes.mzn`: modelo con límites experimentales de transmitancia y costo.
- `ejecutar_todas.ps1`: resuelve las 15 instancias base y escribe `resultados.csv`.
- `ejecutar_margenes.ps1`: resuelve los 375 escenarios de sensibilidad y escribe `resultados_margenes.csv`.

## Requisitos

- **Generación:** compilador `g++` compatible con C++17; `make` es opcional.
- **Resolución:** MiniZinc con el solver HiGHS disponible. Para los experimentos documentados se utilizaron MiniZinc **2.10.1** y HiGHS **1.15.1**.
- **Automatización:** PowerShell en Windows.

Para comprobar la instalación de MiniZinc y la disponibilidad del solver:

```powershell
minizinc --version
minizinc --solvers
```

HiGHS debe aparecer en la lista de solvers. Los archivos `.dzn` ya están incluidos en el repositorio, por lo que **no es necesario volver a generarlos para resolver las instancias**.

## Generación de instancias

Los comandos siguientes se ejecutan **desde la raíz del repositorio**. Para compilar el generador de instancias `.txt` en Linux/WSL:

```bash
g++ -std=c++17 -O2 main.cpp generador.cpp validacion.cpp -o gen
./gen
```

También se puede compilar el ejecutable `gen` con el `makefile` incluido:

```bash
make
./gen
```

En este repositorio, el objetivo definido para eliminar el ejecutable es **`make clear`**, no `make clean`.

Para generar los archivos `.dzn`:

```bash
mkdir -p minizinc/datos
g++ -std=c++17 -O2 generar_dzn.cpp generador.cpp validacion.cpp -o generar_dzn
./generar_dzn
```

El directorio `minizinc/datos/` debe existir antes de ejecutar `generar_dzn`, porque esta versión del programa no lo crea automáticamente.

Los dos programas utilizan el mismo generador, comienzan con la semilla **101** y descartan las instancias que no superan la validación, hasta obtener **5 pequeñas, 5 medianas y 5 grandes**.

**Precaución:** ejecutar los generadores desde el repositorio puede sobrescribir las instancias `.txt` y `.dzn` ya publicadas. Para hacer pruebas sin modificar los datos de referencia, utiliza una copia independiente del proyecto.

## Modelos de optimización

### Modelo base

`minizinc/modelo.mzn` minimiza el costo total de las soluciones constructivas, considerando:

- Una única alternativa seleccionada por cada elemento optimizable.
- El aporte térmico y el costo de los elementos fijos.
- Una transmitancia térmica promedio ponderada por área que no supera el umbral de la instancia.
- Variables binarias de selección.

Las matrices de alternativas se completan con posiciones de relleno cuando los catálogos tienen diferentes tamaños. El modelo fija en cero las variables asociadas a posiciones que no representan alternativas reales.

### Modelo con márgenes

`minizinc/modelo_margenes.mzn` incorpora variaciones relativas sobre el umbral térmico y el presupuesto. Se utilizan los márgenes `-10 %`, `-5 %`, `0 %`, `+5 %` y `+10 %` para cada restricción.

Los límites de cada escenario se definen mediante:

```text
UmaxExperimental = Umax * (1 + margenU)
CostoMaximo = CostoReferencia * (1 + margenCosto) + 0.01
```

El término `0.01` es una tolerancia numérica incorporada en el modelo para la restricción de costo; no representa una variación presupuestaria adicional.

Se evalúan **15 × 5 × 5 = 375 escenarios** de márgenes.

## Resolución automática

Los scripts deben ejecutarse **desde la raíz del repositorio**, en PowerShell de Windows, con `minizinc` disponible en `PATH`.

**1. Resolver las 15 instancias base:**

```powershell
powershell -ExecutionPolicy Bypass -File .\ejecutar_todas.ps1
```

El script procesa los archivos `.dzn` de `minizinc/datos/` y escribe `resultados.csv`.

**2. Resolver los 375 escenarios con márgenes:**

```powershell
powershell -ExecutionPolicy Bypass -File .\ejecutar_margenes.ps1
```

Este segundo script lee `resultados.csv` para obtener los costos óptimos de referencia y escribe `resultados_margenes.csv`. Por ello, debe ejecutarse después de disponer del CSV base.

**Importante:** los scripts de esta versión **no reciben** parámetros `-CarpetaSalida` ni `-ArchivoBase`: utilizan directamente los nombres de archivo establecidos en su código. Una nueva ejecución puede **sobrescribir los CSV publicados**. Si deseas conservarlos, realiza la ejecución en una copia del repositorio o respáldalos primero.

También es posible resolver una instancia individual:

```powershell
minizinc --solver HiGHS --statistics minizinc/modelo.mzn minizinc/datos/pequena_1.dzn
```

## Resultados incluidos

Los CSV almacenados en este repositorio registran los siguientes estados:

| Experimento | Óptimos | Infactibles | Total |
|---|---:|---:|---:|
| Instancias base | 15 | 0 | 15 |
| Escenarios con márgenes | 191 | 184 | 375 |

Los scripts registran métricas como el costo óptimo, la transmitancia obtenida, el estado de resolución, el tiempo del solver, los nodos explorados, la cota del objetivo y el MIP Gap cuando esas estadísticas están disponibles. `ejecutar_todas.ps1` también registra la dimensionalidad del modelo y el tiempo de transformación.

**Alcance de los resultados publicados:** los CSV presentes en GitHub son los archivos de esta versión del repositorio. Sus **tiempos de ejecución no deben identificarse automáticamente** con los del informe final corregido, que fue preparado con un conjunto de registros de benchmark distinto. Tampoco deben confundirse las verificaciones complementarias del paquete local con archivos disponibles en este repositorio.

## Validación de instancias

La función `validarInstancia()` rechaza los casos que incumplen las condiciones establecidas. En la versión actual comprueba:

- Tipo de instancia: `pequena`, `mediana` o `grande`.
- Rangos de dimensiones: largo de 8 a 15 m, ancho de 6 a 12 m y altura de 2,3 a 2,8 m.
- Superficie y volumen positivos y coherentes con las dimensiones generadas.
- Áreas de elementos estrictamente positivas.
- Cantidad de categorías: entre 3 y 4 en instancias pequeñas; 5 en medianas y grandes.
- Cantidad de elementos optimizables: 2–4, 5–8 y 9–15, respectivamente.
- Cantidad de alternativas por elemento optimizable: 5–10, 11–25 y 26–50, según el tamaño.
- Rangos de transmitancia térmica correspondientes a cada categoría.
- Coherencia del costo con la fórmula por categoría `C = base + factor / U`.
- Relación inversa entre transmitancia y costo dentro de cada catálogo.
- Factibilidad térmica global, comprobando que el promedio ponderado mínimo alcanzable no supere `Umax`.

Las comparaciones geométricas y de costos utilizan tolerancias numéricas para evitar rechazos por pequeñas diferencias de punto flotante.

## Estructuras y funciones principales

- `Alternativa`: identificador de alternativa, transmitancia `U` y costo por metro cuadrado.
- `Elemento`: nombre, categoría, área, carácter fijo u optimizable y datos de solución o catálogo.
- `Instancia`: tipo, semilla, dimensiones, superficie, volumen, elementos y `Umax`.
- `generarDimensiones()`: produce dimensiones aleatorias dentro de los rangos definidos.
- `elegirCategorias()`: determina las categorías presentes según el tamaño de instancia.
- `generarCantidadAlternativas()`: establece el tamaño de los catálogos.
- `generarU()` y `generarCosto()`: generan los parámetros térmicos y económicos.
- `generarCatalogo()`: construye las alternativas de una categoría.
- `generarElementoFijo()` y `generarElementoOptimizable()`: asignan la información correspondiente a cada tipo de elemento.
- `generarUmax()`: calcula el mínimo promedio térmico alcanzable y suma un margen positivo que garantiza factibilidad en la generación.
- `validarInstancia()`: aplica las comprobaciones anteriores.

## Consideraciones metodológicas y reproducibilidad

- Los parámetros térmicos y económicos corresponden a **datos sintéticos**, no a valores comerciales ni a exigencias normativas.
- La transmitancia térmica promedio ponderada por área es un **indicador simplificado**: no representa por sí sola la demanda energética real de la vivienda.
- Para reproducir las instancias deben conservarse el código, las semillas y una implementación compatible de la biblioteca estándar de C++.
- Los tiempos de resolución dependen del equipo y del entorno de ejecución. Los scripts del repositorio permiten repetir los experimentos, pero sus nuevas mediciones no tienen por qué coincidir con las originales.
- Las carpetas `benchmark_original/` y `verificacion/`, así como `experimentos_comun.ps1`, **no forman parte de esta versión publicada en GitHub**; corresponden al paquete local de trabajo y verificación.
