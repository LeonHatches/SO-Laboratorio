# Laboratorio 04: Comunicación de procesos

Ejecutar cada comando en Linux desde la carpeta indicada. Los números corresponden a la guía.

## Resueltos

Desde `lab04/resueltos`, compilar individualmente:

```sh
g++ -Wall primero.cpp -o primero
gcc -Wall segundo.c -o segundo
gcc -Wall tercero.c -o tercero
gcc -Wall cuarto.c -o cuarto
gcc -Wall quinto.c -o quinto
gcc -Wall sexto.c -o sexto
gcc -Wall setimo.c -o setimo
gcc -Wall octavo.c -o octavo
gcc -Wall setimo_bidireccional.c -o setimo_bidireccional
gcc -Wall octavo_fifo.c -o octavo_fifo
```

| Ejercicio | Archivo | Ejecución y observación |
| --- | --- | --- |
| 1 | `primero.cpp` | `./primero`: Ctrl+C ejecuta el manejador y continúa el ciclo. Para terminarlo, enviar SIGTERM a su PID desde otra terminal. |
| 2 | `segundo.c` | `./segundo`: pulsar Ctrl+C varias veces durante los 15 segundos de la alarma. |
| 3 | `tercero.c` | `./tercero`: desde otra terminal consultar `pgrep -x tercero` y enviar `kill -USR1 PID`; repetir el envío para observar el contador. Finalizar con `kill -TERM PID`. |
| 4 | `cuarto.c` | `./cuarto`: muestra su PID. Enviar `kill -USR1 PID` desde otra terminal. SIGKILL y SIGSTOP no pueden capturarse. |
| 5 | `quinto.c` | `./quinto`: el padre avisa al hijo cada segundo. Ctrl+C termina el grupo en primer plano. |
| 6 | `sexto.c` | `./sexto`: envía SIGHUP, SIGINT y SIGQUIT. Se conserva la carrera del ejemplo: SIGHUP puede llegar antes de que el hijo registre su manejador. |
| 7 | `setimo.c` | `./setimo`: ejemplo original, el padre envía «Sistemas». |
| 7, ampliación | `setimo_bidireccional.c` | `./setimo_bidireccional`: dos pipes; el hijo responde «Operativos», como pide la guía. |
| 8 | `octavo.c` | `./octavo ls wc`: ejemplo original; conecta dos comandos mediante un pipe anónimo. |
| 8, FIFO | `octavo_fifo.c` | `./octavo_fifo`: utiliza una tubería con nombre creada con `mkfifo`, como solicita el enunciado. Elimina la FIFO al terminar normalmente. |

En los comandos con `PID`, sustituir ese texto por el identificador real del programa del ejercicio.

### Correcciones de transcripción

- Se cambiaron las comillas tipográficas por comillas válidas en C.
- Se añadió `<unistd.h>` al segundo y `<stdio.h>` al cuarto y al octavo.
- Los manejadores del segundo y sexto reciben el argumento `int` exigido por `signal()`.
- Se cambió `void main()` por `int main(void)` en quinto y sexto.
- Se mantuvieron los bucles, mensajes y comportamiento del código original. Por eso los resueltos no constituyen ejemplos de manejo robusto de señales: conservan, entre otras limitaciones, llamadas a `printf` desde manejadores y la carrera del sexto.
- La versión bidireccional y la FIFO se incluyen en archivos separados para conservar los originales.

## Propuestos

Desde `lab04/propuestos`:

```sh
gcc -Wall propuesto1.c -o propuesto1
./propuesto1

gcc -Wall propuesto2.c -o propuesto2
./propuesto2

gcc -Wall propuesto3.c -o propuesto3
./propuesto3
```

1. El padre envía SIGINT al hijo y muestra la señal que causó su terminación. Se conserva el programa de la guía, se agregan las cabeceras que faltan y se comprueba el fallo de `fork()`.
2. El valor esperado del padre es 15: el hijo modifica una copia independiente y SIGCHLD incrementa la copia del padre. Se conserva el programa de la guía, se agregan las cabeceras que faltan y se comprueba el fallo de `fork()`.
3. P crea H1 y H2; H1 crea H3. H3 ejecuta `bienvenida.sh` con `/bin/sh`. H1 envía SIGUSR1 a H2 cada 20 segundos y H2 muestra su PID cuando la recibe. La señal se bloquea antes de crear los hijos y H2 espera con `sigsuspend`. P espera a sus hijos; H1 recoge a H3.

El propuesto 3 envía tres señales por defecto y termina en aproximadamente 60 segundos. Para cambiar la cantidad manteniendo el intervalo de 20 segundos:

```sh
./propuesto3 5
```

Ejecutarlo desde la carpeta que contiene `bienvenida.sh`, o pasar su ruta como segundo argumento:

```sh
./propuesto3 3 ./bienvenida.sh
```

El script saluda al usuario que ejecuta el programa según la hora local: mañana de 06:00 a 11:59, tarde de 12:00 a 18:59 y noche el resto del día.
