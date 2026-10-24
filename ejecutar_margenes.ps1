$modelo = "minizinc\modelo_margenes.mzn"
$carpetaDatos = "minizinc\datos"
$archivoBase = "resultados.csv"
$archivoSalida = "resultados_margenes.csv"

# Márgenes a estudiar
$margenes = @(-0.10, -0.05, 0.00, 0.05, 0.10)

# Leer resultados base
$base = Import-Csv $archivoBase

$resultados = @()

foreach ($filaBase in $base) {

    $instancia = $filaBase.Instancia
    $costoReferencia =
        [double]::Parse(
            $filaBase.CostoOptimo,
            [System.Globalization.CultureInfo]::CurrentCulture
        )

    $archivoDzn =
        Join-Path $carpetaDatos ($instancia + ".dzn")

    foreach ($margenU in $margenes) {
        foreach ($margenCosto in $margenes) {

            Write-Host ""
            Write-Host "=============================================="
            Write-Host "$instancia | U: $margenU | Costo: $margenCosto"
            Write-Host "=============================================="
            # MiniZinc necesita punto decimal
            $mU = $margenU.ToString(
                [System.Globalization.CultureInfo]::InvariantCulture
            )
            $mC = $margenCosto.ToString(
                [System.Globalization.CultureInfo]::InvariantCulture
            )
            $cRef = $costoReferencia.ToString(
                [System.Globalization.CultureInfo]::InvariantCulture
            )
            $salida = minizinc `
                --solver HiGHS `
                --statistics `
                -D "margenU=$mU" `
                -D "margenCosto=$mC" `
                -D "costoReferencia=$cRef" `
                $modelo `
                $archivoDzn 2>&1

            $texto = $salida -join "`n"

            # ==========================================
            # ESTADO
            # ==========================================

            if ($texto -match "UNSATISFIABLE") {
                $estado = "INFACTIBLE"
            }
            elseif ($texto -match "==========") {
                $estado = "OPTIMO"
            }
            else {
                $estado = "DESCONOCIDO"
            }

            # ==========================================
            # COSTO TOTAL
            # ==========================================

            $matchCosto = [regex]::Match(
                $texto,
                'Costo total = \$([0-9\.]+)'
            )
            if ($matchCosto.Success) {
                $costo =
                    [double]::Parse(
                        $matchCosto.Groups[1].Value,
                        [System.Globalization.CultureInfo]::InvariantCulture
                    )
            }
            else {
                $costo = $null
            }

            # ==========================================
            # U OBTENIDO
            # ==========================================

            $matchU = [regex]::Match(
                $texto,
                'U obtenido = ([0-9\.]+)'
            )
            if ($matchU.Success) {
                $uObtenido =
                    [double]::Parse(
                        $matchU.Groups[1].Value,
                        [System.Globalization.CultureInfo]::InvariantCulture
                    )
            }
            else {
                $uObtenido = $null
            }

            # ==========================================
            # UMAX EXPERIMENTAL
            # ==========================================
            $matchUmax = [regex]::Match(
                $texto,
                'Umax experimental = ([0-9\.]+)'
            )
            if ($matchUmax.Success) {
                $uMaxExperimental =
                    [double]::Parse(
                        $matchUmax.Groups[1].Value,
                        [System.Globalization.CultureInfo]::InvariantCulture
                    )
            }
            else {

                $uMaxExperimental = $null
            }

            # ==========================================
            # COSTO MAXIMO
            # ==========================================

            $matchCostoMax = [regex]::Match(
                $texto,
                'Costo maximo = \$([0-9\.]+)'
            )
            if ($matchCostoMax.Success) {
                $costoMax =
                    [double]::Parse(
                        $matchCostoMax.Groups[1].Value,
                        [System.Globalization.CultureInfo]::InvariantCulture
                    )
            }
            else {
                $costoMax =
                    $costoReferencia * (1 + $margenCosto)
            }

            # ==========================================
            # NODOS
            # ==========================================

            $matchesNodes = [regex]::Matches(
                $texto,
                'nodes=([0-9]+)'
            )
            if ($matchesNodes.Count -gt 0) {
                $nodes =
                    [int]$matchesNodes[
                        $matchesNodes.Count - 1
                    ].Groups[1].Value
            }
            else {
                $nodes = $null
            }

            # ==========================================
            # SOLVE TIME
            # ==========================================

            $matchesTime = [regex]::Matches(
                $texto,
                'solveTime=([0-9\.]+)'
            )
            if ($matchesTime.Count -gt 0) {
                $solveTime =
                    [double]::Parse(
                        $matchesTime[
                            $matchesTime.Count - 1
                        ].Groups[1].Value,
                        [System.Globalization.CultureInfo]::InvariantCulture
                    )
            }
            else {
                $solveTime = $null
            }

            # ==========================================
            # OBJECTIVE Y BOUND
            # ==========================================

            $matchesObjective = [regex]::Matches(
                $texto,
                'objective=([0-9\.]+)'
            )
            if ($matchesObjective.Count -gt 0) {
                $objective =
                    [double]::Parse(
                        $matchesObjective[
                            $matchesObjective.Count - 1
                        ].Groups[1].Value,
                        [System.Globalization.CultureInfo]::InvariantCulture
                    )
            }
            else {
                $objective = $null
            }

            $matchesBound = [regex]::Matches(
                $texto,
                'objectiveBound=([0-9\.]+)'
            )
            if ($matchesBound.Count -gt 0) {
                $objectiveBound =
                    [double]::Parse(
                        $matchesBound[
                            $matchesBound.Count - 1
                        ].Groups[1].Value,
                        [System.Globalization.CultureInfo]::InvariantCulture
                    )
            }
            else {

                $objectiveBound = $null
            }

            # ==========================================
            # MIP GAP
            # ==========================================

            if (
                $null -ne $objective -and
                $null -ne $objectiveBound -and
                $objective -ne 0
            ) {
                $gap =
                    [math]::Abs(
                        $objective - $objectiveBound
                    ) /
                    [math]::Abs($objective) * 100
            }
            else {

                $gap = $null
            }

            # ==========================================
            # GUARDAR RESULTADO
            # ==========================================

            $resultados += [PSCustomObject]@{
                Instancia = $instancia
                MargenU = $margenU
                MargenCosto = $margenCosto
                Estado = $estado
                CostoReferencia = $costoReferencia
                CostoMaximo = $costoMax
                CostoOptimo = $costo
                UObtenido = $uObtenido
                UmaxExperimental = $uMaxExperimental
                SolveTime = $solveTime
                Nodos = $nodes
                ObjectiveBound = $objectiveBound
                MIPGapPorcentaje = $gap
            }
        }
    }
}


$resultados |
    Export-Csv `
    -Path $archivoSalida `
    -NoTypeInformation `
    -Encoding UTF8

Write-Host ""
Write-Host "=============================================="
Write-Host "EXPERIMENTOS DE MARGEN TERMINADOS"
Write-Host "Resultados: resultados_margenes.csv"
Write-Host "Cantidad esperada: 375"
Write-Host "Cantidad obtenida: $($resultados.Count)"
Write-Host "=============================================="