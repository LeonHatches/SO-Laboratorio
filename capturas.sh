#!/usr/bin/env bash
set -euo pipefail
export LC_ALL=C
cd -- "$(dirname -- "${BASH_SOURCE[0]}")"
temp=$(mktemp -d)
trap 'rm -f -- "$temp/programa" "$temp/con_join" "$temp/salida"; rmdir -- "$temp"' EXIT

compilar() {
    local fuente=$1
    local destino=$2
    shift 2
    printf '$ gcc -std=gnu11 %s %s -o %s\n' "$*" "$fuente" "$destino"
    gcc -std=gnu11 "$@" "$fuente" -o "$destino"
}

case "${1:-}" in
    tiempos)
        printf '%s\n' '--- Resuelto 06: procesos ---'
        compilar lab06/resuelto/ejercicio6_procesos.c "$temp/programa"
        procesos100=$("$temp/programa" 100)
        procesos200=$("$temp/programa" 200)
        printf '%s\n' "$procesos100" "$procesos200"

        printf '\n%s\n' '--- Resuelto 06: hilos y comparacion ---'
        compilar lab06/resuelto/ejercicio6_hilos.c "$temp/programa" -pthread
        hilos100=$("$temp/programa" 100)
        hilos200=$("$temp/programa" 200)
        printf '%s\n' "$hilos100" "$hilos200"
        printf '%s\n' "$procesos100" "$hilos100" "$procesos200" "$hilos200" |
            awk 'NR % 2 == 1 {proceso=$3; next}
                 {menor=(proceso+0 < $3+0 ? "procesos" : (proceso+0 > $3+0 ? "hilos" : "empate"));
                  printf "Con %d de cada tipo, el menor promedio fue: %s.\n", $1, menor}'
        ;;
    join)
        compilar lab06/resuelto/ejercicio4.c "$temp/con_join" -pthread
        compilar lab06/resuelto/ejercicio5.c "$temp/programa" -pthread
        "$temp/con_join" > "$temp/salida"
        printf 'Con pthread_join: %s lineas de Hijo\n' "$(grep -c '^Hijo:' "$temp/salida" || true)"
        for n in 1 2 3 4 5; do
            "$temp/programa" > "$temp/salida"
            printf 'Sin pthread_join, prueba %s: %s de 300 lineas de Hijo\n' \
                "$n" "$(grep -c '^Hijo:' "$temp/salida" || true)"
        done
        ;;
    cuestionario)
        compilar lab06/propuesto/cuestionario3.c "$temp/programa" -O0 -pthread
        "$temp/programa" > lab06/salida_cuestionario3.txt
        printf '\nPrimeras operaciones mostradas por cada hilo:\n'
        awk '/^Suma / && ++s<=3 {print} /^Resta / && ++r<=3 {print}' lab06/salida_cuestionario3.txt
        printf '\nResultados finales de las cinco pruebas:\n'
        grep '^Resultado de la prueba ' lab06/salida_cuestionario3.txt
        ;;
    mutex)
        compilar lab07/resuelto/resuelto2.c "$temp/programa" -pthread
        "$temp/programa" > "$temp/salida"
        printf '\nPrimeras lineas completas de cada hilo:\n'
        awk '/^Inicio proceso Padre:/ && ++p<=4 {print} /^Inicio Proceso Hijo:/ && ++h<=4 {print}' "$temp/salida"
        ;;
    *)
        printf 'Uso: bash capturas.sh {join|tiempos|cuestionario|mutex}\n' >&2
        exit 2
        ;;
esac
