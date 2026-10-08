# Laboratorio 03

Programas para Linux. Los ejercicios de terminal se ejecutan en una misma terminal interactiva cuando comparten trabajos.

Se mantienen la lógica y los comentarios de los resueltos. Ajustes accidentales: quitar los punto y coma después de #include, agregar <sys/wait.h> en 13 y 14, eliminar el punto suelto de 22, corregir «terminadod» en 16 y hacer coincidir nombres al compilar/ejecutar. En 14 se asigna el nombre jerarquia.c porque la guía no especifica uno. Las llaves de 4 y 16 se transcriben desde la imagen del PDF.

## Resueltos

Ejecutar desde `lab03/resueltos`.

### Ejercicio 01

Archivo: resueltos/holamundo.c

```bash
gcc -Wall holamundo.c -o holamundo
./holamundo
```

### Ejercicio 02

Archivo: resueltos/hola.c

```bash
gcc -Wall hola.c -o hola
./hola
```

### Ejercicio 03

Archivo: resueltos/ejemplofork.c

```bash
gcc -Wall ejemplofork.c -o ejemplofork
./ejemplofork
```

### Ejercicio 04

Archivo: resueltos/ejemplofork3en1.c

```bash
gcc -Wall ejemplofork3en1.c -o ejemplofork3en1
./ejemplofork3en1
```

### Ejercicio 05

```bash
sleep 300
```

Esperar cinco minutos.

### Ejercicio 06

```bash
sleep 1000
```

Interrumpir con Ctrl+C.

### Ejercicio 07

```bash
sleep 1000
```

Suspender con Ctrl+Z.

### Ejercicio 08

```bash
sleep 1000 &
sleep 200 &
jobs
```

### Ejercicio 09

```bash
fg %1
```

Usar el número indicado por jobs; %1 identifica el trabajo 1. Suspender nuevamente con Ctrl+Z si se continuará con el ejercicio 10.

### Ejercicio 10

```bash
bg %1
jobs
```

Usar el mismo número de trabajo suspendido.

### Ejercicio 11

Archivo: resueltos/primero.c

```bash
gcc -Wall primero.c -o primero
./primero
```

### Ejercicio 12

Archivo: resueltos/segundo.c

```bash
gcc -Wall segundo.c -o segundo
./segundo
```

### Ejercicio 13

Archivo: resueltos/sexto.c

```bash
gcc -Wall sexto.c -o sexto
./sexto
```

### Ejercicio 14

Archivo: resueltos/jerarquia.c

```bash
gcc -Wall jerarquia.c -o jerarquia
./jerarquia
```

### Ejercicio 15

Archivo: resueltos/usandowait.c

```bash
gcc -Wall usandowait.c -o usandowait
./usandowait
```

### Ejercicio 16

Archivo: resueltos/ejemplowait.c

```bash
gcc -Wall ejemplowait.c -o ejemplowait
./ejemplowait
```

### Ejercicio 17

```bash
ps -A
```

### Ejercicio 18

```bash
ps -u "$(whoami)"
```

### Ejercicio 19

```bash
ps -ef
```

### Ejercicio 20

```bash
ps -ef | wc -l
```

Conserva el comando de la guía: también cuenta la línea de encabezado.

### Ejercicio 21

Archivo: resueltos/procesozombi.c

```bash
gcc -Wall procesozombi.c -o procesozombi
./procesozombi
```

La creación del zombi es intencional. No se añade wait().

### Ejercicio 22

Archivo: resueltos/procesohuerfano.c

```bash
gcc -Wall procesohuerfano.c -o procesohuerfano
./procesohuerfano
```

La creación del huérfano es intencional. No se añade wait().

## Propuestos

Ejecutar desde `lab03/propuestos`.

### Ejercicio 01

Archivo: propuestos/propuesto1.c

```bash
gcc -Wall propuesto1.c -o propuesto1
./propuesto1
```

### Ejercicio 02

Archivo: propuestos/propuesto2.c

```bash
gcc -Wall propuesto2.c -o propuesto2
./propuesto2
```

### Ejercicio 03

Archivo: propuestos/propuesto3.c

```bash
gcc -Wall propuesto3.c -o propuesto3
./propuesto3
```
