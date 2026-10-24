$modelo = "minizinc\modelo.mzn"
$carpetaDatos = "minizinc\datos"
$archivoSalida = "resultados.csv"

$resultados = @()

$archivos = Get-ChildItem "$carpetaDatos\*.dzn" | Sort-Object Name

foreach ($archivo in $archivos) {

    Write-Host ""
    Write-Host "======================================"
    Write-Host "Resolviendo $($archivo.Name)"
    Write-Host "======================================"

    $salida = minizinc --solver HiGHS --statistics `
        $modelo `
        $archivo.FullName 2>&1

    $texto = $salida -join "`n"

    # ----------------------------------
    # Costo optimo
    # ----------------------------------

    $matchCosto = [regex]::Match(
        $texto,
        'Costo total = \$([0-9\.]+)'
    )

    $costo = if ($matchCosto.Success) {
        [double]$matchCosto.Groups[1].Value
    } else {
        ""
    }

    # ----------------------------------
    # U obtenido
    # ----------------------------------

    $matchU = [regex]::Match(
        $texto,
        'U obtenido = ([0-9\.]+)'
    )

    $uObtenido = if ($matchU.Success) {
        [double]$matchU.Groups[1].Value
    } else {
        ""
    }

    # ----------------------------------
    # Umax
    # ----------------------------------

    $matchUmax = [regex]::Match(
        $texto,
        'Umax = ([0-9\.]+)'
    )

    $uMax = if ($matchUmax.Success) {
        [double]$matchUmax.Groups[1].Value
    } else {
        ""
    }

    # ----------------------------------
    # Objective
    # ----------------------------------

    $matchesObjective = [regex]::Matches(
        $texto,
        'objective=([0-9\.]+)'
    )

    $objective = if ($matchesObjective.Count -gt 0) {
        [double]$matchesObjective[
            $matchesObjective.Count - 1
        ].Groups[1].Value
    } else {
        ""
    }

    # ----------------------------------
    # Objective bound
    # ----------------------------------

    $matchesBound = [regex]::Matches(
        $texto,
        'objectiveBound=([0-9\.]+)'
    )

    $objectiveBound = if ($matchesBound.Count -gt 0) {
        [double]$matchesBound[
            $matchesBound.Count - 1
        ].Groups[1].Value
    } else {
        ""
    }

    # ----------------------------------
    # Nodos
    # ----------------------------------

    $matchesNodes = [regex]::Matches(
        $texto,
        'nodes=([0-9]+)'
    )

    $nodes = if ($matchesNodes.Count -gt 0) {
        [int]$matchesNodes[
            $matchesNodes.Count - 1
        ].Groups[1].Value
    } else {
        ""
    }

    # ----------------------------------
    # Tiempo solver
    # ----------------------------------

    $matchesTime = [regex]::Matches(
        $texto,
        'solveTime=([0-9\.]+)'
    )

    $solveTime = if ($matchesTime.Count -gt 0) {
        [double]$matchesTime[
            $matchesTime.Count - 1
        ].Groups[1].Value
    } else {
        ""
    }

    # ----------------------------------
    # Flat time
    # ----------------------------------

    $matchFlat = [regex]::Match(
        $texto,
        'flatTime=([0-9\.]+)'
    )

    $flatTime = if ($matchFlat.Success) {
        [double]$matchFlat.Groups[1].Value
    } else {
        ""
    }

    # ----------------------------------
    # Leer nOpt y nAlt desde .dzn
    # ----------------------------------

    $contenidoDzn = Get-Content $archivo.FullName -Raw

    $matchNOpt = [regex]::Match(
        $contenidoDzn,
        'nOpt\s*=\s*(\d+)'
    )

    $nOpt = [int]$matchNOpt.Groups[1].Value

    $matchNAlt = [regex]::Match(
        $contenidoDzn,
        'nAlt\s*=\s*\[([^\]]+)\]'
    )

    $nAltTexto = $matchNAlt.Groups[1].Value

    $alternativas = $nAltTexto -split "," |
        ForEach-Object {
            [int]$_.Trim()
        }

    $variablesBinarias =
        ($alternativas | Measure-Object -Sum).Sum

    # Restricciones originales:
    # una por elemento + restriccion termica

    $restricciones = $nOpt + 1

    # ----------------------------------
    # MIP GAP
    # ----------------------------------

    if (
        $objective -ne "" -and
        $objectiveBound -ne "" -and
        $objective -ne 0
    ) {

        $gap =
            [math]::Abs(
                $objective - $objectiveBound
            ) / [math]::Abs($objective) * 100
    }
    else {

        $gap = ""
    }

    # ----------------------------------
    # Estado
    # ----------------------------------

    if ($texto -match '==========') {
        $estado = "OPTIMO"
    }
    elseif ($texto -match 'UNSATISFIABLE') {
        $estado = "INFACTIBLE"
    }
    else {
        $estado = "DESCONOCIDO"
    }

    # ----------------------------------
    # Guardar fila
    # ----------------------------------

    $resultados += [PSCustomObject]@{

        Instancia = $archivo.BaseName
        Estado = $estado

        VariablesBinarias = $variablesBinarias
        Restricciones = $restricciones

        CostoOptimo = $costo

        UObtenido = $uObtenido
        Umax = $uMax

        SolveTime = $solveTime
        FlatTime = $flatTime

        Nodos = $nodes

        ObjectiveBound = $objectiveBound
        MIPGapPorcentaje = $gap
    }
}

# ----------------------------------
# Exportar CSV
# ----------------------------------

$resultados |
    Export-Csv `
    -Path $archivoSalida `
    -NoTypeInformation `
    -Encoding UTF8

Write-Host ""
Write-Host "======================================"
Write-Host "15 INSTANCIAS TERMINADAS"
Write-Host "Resultados guardados en resultados.csv"
Write-Host "======================================"