# Laboratorio 08 — Semáforos en Linux

Se usa GCC en Linux, con `-pthread` para enlazar PThreads y los semáforos POSIX. Ejecuta los comandos desde la carpeta `lab08` del repositorio.

## Resueltos

`resueltos/` contiene los doce ejemplos transcritos de la guía. Cada ejercicio se compila por separado. En los ejercicios 02 y 03, `-include stdio.h` proporciona la declaración de `printf` al compilador sin editar el archivo original.

| Ejercicio | Compilación | Ejecución | Salida habitual o esperada |
| --- | --- | --- | --- |
| 01 | `gcc -pthread resueltos/ejercicio01.c -o /tmp/lab08-01` | `timeout 2s /tmp/lab08-01`, luego `echo $?` | Sin texto del programa. `timeout` devuelve `124`. |
| 02 | `gcc -include stdio.h -pthread resueltos/ejercicio02.c -o /tmp/lab08-02` | `timeout 2s /tmp/lab08-02 \| head -n 8` | Ocho líneas `sección critica` en el recorte. |
| 03 | `gcc -include stdio.h -pthread resueltos/ejercicio03.c -o /tmp/lab08-03` | `timeout 2s /tmp/lab08-03 \| head -n 8` | Mensajes `Entra` y `Sale` por turnos. |
| 04 | `gcc -pthread resueltos/ejercicio04.c -o /tmp/lab08-04` | `timeout 10s stdbuf -oL /tmp/lab08-04` | Aproximadamente cuatro segundos entre `Entra` y `Sale`. |
| 05 | `gcc -pthread resueltos/ejercicio05.c -o /tmp/lab08-05` | `timeout 10s stdbuf -oL /tmp/lab08-05` | Se retrasa dos segundos la creación del segundo hilo. La consola conserva `Entra` y `Sale`. |
| 06 | `gcc -pthread resueltos/ejercicio06.c -o /tmp/lab08-06` | `timeout 10s stdbuf -oL /tmp/lab08-06` | Los hilos siguen trabajando. El primer `pthread_join` continúa esperando. |
| 07 | `gcc -pthread resueltos/ejercicio07.c -o /tmp/lab08-07` | `timeout 10s stdbuf -oL /tmp/lab08-07` | Tras `Sale` pasan unos dos segundos antes de permitir otra entrada. |
| 08 | `gcc -pthread resueltos/ejercicio08.c -o /tmp/lab08-08` | `/tmp/lab08-08` | Dos hilos incrementan el contador. Se espera que pase de `0` a `1` y de `1` a `2`. |
| 09 | `gcc -pthread resueltos/ejercicio09.c -o /tmp/lab08-09` | `/tmp/lab08-09 10000` | `shared = 40000`. |
| 10 | `gcc -pthread resueltos/ejercicio10.c -o /tmp/lab08-10` | `/tmp/lab08-10 10000` | `shared = 80000`. |
| 11 | `gcc -pthread resueltos/ejercicio11.c -o /tmp/lab08-11` | `stdbuf -oL /tmp/lab08-11` | Mensajes `Thread n: k` de diez productores. |
| 12 | `gcc -pthread resueltos/ejercicio12.c -o /tmp/lab08-12` | `stdbuf -oL /tmp/lab08-12` | Mensajes `Thread n: k` con semáforos sin nombre. |

Los ejercicios 01 a 07 contienen bucles infinitos. En las filas 04 a 07, `stdbuf -oL` asegura que veas cada línea en la consola antes de que termine `timeout`. La guía llama `message_send` al binario del ejercicio 09. El nombre del ejecutable puede elegirse con `-o`.

El ejercicio 08 conserva la firma `void handler(void *)` de la guía, que no es la firma que espera `pthread_create`. Los ejercicios 11 y 12 conservan la publicación del mensaje fuera del bloqueo que reserva el índice. Que se vea una secuencia de mensajes en una corrida no garantiza que todos se consuman correctamente. En el 11, ejecutar solo una instancia permite usar los nombres de semáforo de la guía sin compartirlos con otra ejecución. En el 12, la guía invoca `sem_destroy` tres veces sobre `mutex_sem`.

## Propuestos

| Ejercicio | Compilación | Ejecución | Qué comprobar |
| --- | --- | --- | --- |
| 01 · Lectores y escritores | `gcc -Wall -Wextra -pthread propuestos/propuesto01.c -o /tmp/lab08-p01` | `/tmp/lab08-p01` | Hay lecturas concurrentes y cada escritor tiene acceso exclusivo. Dos incrementos de 10 dejan `Dato final: 20`. |
| 02 · Semáforos anidados | `gcc -Wall -Wextra -pthread propuestos/propuesto02.c -o /tmp/lab08-p02` | `/tmp/lab08-p02` | Hasta dos hilos en la sala y uno solo usando el equipo. Los cuatro hilos terminan. |
| 03 · Contador sin argumentos | `gcc -Wall -Wextra -pthread propuestos/propuesto03.c -o /tmp/lab08-p03` | `/tmp/lab08-p03` | `shared = 40000` al ejecutar sin argumentos. |

El orden de los mensajes que imprimen los hilos depende del planificador. Los valores finales de los contadores sí son reproducibles con la entrada indicada.
