# Laboratorio 05: Colas de mensajes

Programas de la guía de Sistemas Operativos sobre colas de mensajes System V en C para Linux. La organización mantiene las carpetas `resueltos` y `propuestos` de los laboratorios anteriores.

## Requisitos y compilación

Usar Linux, o una distribución Linux funcional en WSL, con GCC y las utilidades `ipcs` e `ipcrm`. Estas llamadas System V no se ejecutan directamente con el compilador nativo de Windows.

Desde la carpeta `lab05`:

```sh
gcc -std=c11 -Wall -Wextra resueltos/primero.c -o resueltos/primero
gcc -std=c11 -Wall -Wextra propuestos/propuesto1.c -o propuestos/propuesto1
gcc -std=c11 -Wall -Wextra propuestos/propuesto2.c -o propuestos/propuesto2
gcc -std=c11 -Wall -Wextra propuestos/propuesto3.c -o propuestos/propuesto3
```

Cada fuente es un programa independiente. No se compilan todos los archivos en un solo ejecutable. Los resultados siguientes son los esperados para realizar las pruebas y tomar las capturas de ejecución.

## Resuelto 01: persistencia de una cola

```sh
./resueltos/primero
```

1. Elegir `1`: crea o abre la cola con clave decimal `2000`, envía un mensaje de tipo `7` con información `150` y termina. La cola y el mensaje permanecen después de finalizar el emisor.
2. Consultar `ipcs -q` antes de ejecutar el programa de nuevo. La clave 2000 aparece en hexadecimal como `0x000007d0`.
3. Ejecutarlo otra vez y elegir `2`. La recepción muestra:

```text
Mensaje recibido de tipo = 7 con info = 150
Cola de mensajes eliminada
```

Prueba equivalente con entrada redirigida, empezando sin mensajes anteriores en esa cola:

```sh
printf '1\n' | ./resueltos/primero
ipcs -q
printf '2\n' | ./resueltos/primero
ipcs -q
```

### Comportamientos originales que se conservan

Se mantienen las instrucciones del ejemplo de las páginas 4 y 5 del enunciado. Se omiten únicamente sus comentarios para cumplir el formato solicitado; no se añadieron correcciones.

- Elegir `2` con una cola vacía bloquea `msgrcv` hasta que llegue un mensaje. Se puede enviar desde otra terminal con la opción `1`.
- Si la creación falla por una causa diferente de `EEXIST`, el ejemplo no termina en ese punto: continúa y puede mostrar «Cola de mensajes creada...» aunque `qid` valga `-1`.
- `scanf` no valida la entrada. Una opción numérica distinta de 1 o 2 conserva la cola.
- El lector recibe el primer mensaje, sin filtrar por tipo, y después elimina toda la cola. Si se enviaron varios mensajes, los restantes se descartan al eliminarla.
- Se conservan `SHM_R`, `SHM_W` y `sizeof(MENSAJE) - sizeof(long)`. Este último puede incluir bytes de relleno de la estructura según la arquitectura.

Estas observaciones describen el ejemplo de la guía; no representan modificaciones del archivo.

## Propuesto 01: dos procesos

```sh
./propuestos/propuesto1
```

`fork()` crea un padre emisor y un hijo receptor. Ambos heredan el identificador de una cola nueva creada con `IPC_PRIVATE`. El padre envía el valor `150` con tipo `7`; el hijo lo recibe. Al terminar el hijo, el padre elimina la cola.

La salida incluye el identificador de la cola, los PID del padre y del hijo, el tipo y la información, y termina con:

```text
Cola eliminada; ambos procesos finalizaron.
```

Los PID y los identificadores IPC cambian entre ejecuciones.

## Propuesto 02: IPC_NOWAIT

```sh
./propuestos/propuesto2
```

El programa utiliza una cola nueva para comprobar tres situaciones. Primero intenta recibir de una cola vacía. Después envía y recibe un mensaje disponible. Finalmente reduce `msg_qbytes` a 64 bytes, ocupa esa capacidad con un mensaje e intenta enviar otro.

Resultado esperado:

```text
Cola vacia: ENOMSG; el proceso no se bloquea.
Lectura disponible: Hola desde IPC_NOWAIT
Cola llena: EAGAIN; el proceso no se bloquea.
Cola eliminada.
```

Con `IPC_NOWAIT`, la falta de un mensaje compatible produce `ENOMSG` al recibir; la falta de espacio produce `EAGAIN` al enviar. La llamada retorna inmediatamente. La capacidad se modifica solo para la cola privada de esta prueba.

## Propuesto 03: tres en raya y rendezvous

```sh
./propuestos/propuesto3
```

Un solo ejecutable crea exactamente dos procesos mediante `fork`: el padre juega con X y el hijo con O. Una matriz `char tablero[3][3]` reside en memoria compartida System V. En cada turno se ingresan fila y columna entre 1 y 3, separadas por un espacio.

La cola transporta turnos y confirmaciones. Los tipos `1` y `2` entregan el turno a X y O; los tipos `11` y `12` confirman la recepción. El emisor espera esa confirmación antes de continuar. No basta con usar `msgsnd` bloqueante para conseguir el rendezvous: por eso existe el intercambio explícito de confirmación.

Las casillas ocupadas, las coordenadas fuera del tablero y las entradas con formato incorrecto se rechazan sin cambiar el turno. Una victoria, un empate o el fin de entrada finalizan el intercambio. El padre espera al hijo y libera la cola y la memoria compartida.

### Victoria de X

Introducir estas cinco jugadas, una por línea:

```text
1 1
2 1
1 2
2 2
1 3
```

Resultado final esperado:

```text
 X | X | X
---+---+---
 O | O |
---+---+---
   |   |
Gana X.
```

Prueba reproducible desde `lab05`:

```sh
printf '1 1\n2 1\n1 2\n2 2\n1 3\n' | ./propuestos/propuesto3
```

### Victoria de O y empate

```sh
printf '1 1\n2 1\n1 2\n2 2\n3 3\n2 3\n' | ./propuestos/propuesto3
printf '1 1\n1 2\n1 3\n2 2\n2 1\n2 3\n3 2\n3 1\n3 3\n' | ./propuestos/propuesto3
```

La primera prueba debe terminar con `Gana O.` y la segunda con `Empate.`.

Para comprobar una entrada inválida, ingresar `0 1` o repetir una casilla ocupada. El mismo jugador debe volver a introducir su movimiento.

## Recursos IPC y limpieza

Los propuestos usan identificadores privados para cada ejecución y limpian sus recursos al terminar normalmente. Si se interrumpen con Ctrl+C, pueden quedar recursos pendientes. Consultarlos antes de eliminar alguno:

```sh
ipcs -q
ipcs -m
```

Para eliminar exclusivamente los recursos de una ejecución interrumpida, sustituir los identificadores que el programa mostró:

```sh
ipcrm -q ID_COLA
ipcrm -m ID_MEMORIA
```

No eliminar recursos de otros programas. Para consultar los límites de mensajes del entorno Linux:

```sh
ipcs -l
cat /proc/sys/kernel/msgmax
cat /proc/sys/kernel/msgmnb
```

## Referencias

- [msgget(2)](https://man7.org/linux/man-pages/man2/msgget.2.html)
- [msgsnd(2) y msgrcv(2)](https://man7.org/linux/man-pages/man2/msgop.2.html)
- [msgctl(2)](https://man7.org/linux/man-pages/man2/msgctl.2.html)
- [System V IPC](https://man7.org/linux/man-pages/man7/sysvipc.7.html)

Las capturas de código y el informe se entregan aparte. Este directorio contiene únicamente fuentes e instrucciones.
