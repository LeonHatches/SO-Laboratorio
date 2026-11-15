# Laboratorio 06 - Threads en C (PThreads)

## Resuelto

### code1.c
```bash
gcc resuelto/code1.c -lpthread -o resuelto/code1
./resuelto/code1
```
Resultado esperado:
```text
Padre: 1
Padre: 2
...
Hijo: 1
Hijo: 0
Hijo: -1
...
```

### ejercicio6_procesos.c
```bash
gcc resuelto/ejercicio6_procesos.c -o resuelto/ejercicio6_procesos
./resuelto/ejercicio6_procesos
```
Resultado esperado:
```text
377.260000
```

### ejercicio6_hilos.c
```bash
gcc resuelto/ejercicio6_hilos.c -lpthread -o resuelto/ejercicio6_hilos
./resuelto/ejercicio6_hilos
```
Resultado esperado:
```text
58.750000
```

### Ejercicio_Matriz.c
```bash
gcc resuelto/Ejercicio_Matriz.c -lpthread -o resuelto/Ejercicio_Matriz
./resuelto/Ejercicio_Matriz
```
Resultado esperado:
```text
 Hilo 1 multiplicando fila 0
 Hilo 1 multiplicando fila 1
 Hilo 1 multiplicando fila 2
Fin 
```

## Propuesto

### propuesto1.c
```bash
gcc propuesto/propuesto1.c -lpthread -o propuesto/propuesto1
./propuesto/propuesto1
```
Resultado esperado (ingresando N = 2):
```text
Ingrese escalar N: 2
Hilo 1 multiplicando fila 0
Hilo 2 multiplicando fila 1
Hilo 3 multiplicando fila 2
8.00	14.00	16.00	
12.00	8.00	12.00	
14.00	6.00	20.00	
```

### propuesto2.c
```bash
gcc propuesto/propuesto2.c -lpthread -o propuesto/propuesto2
./propuesto/propuesto2
```
Resultado esperado:
```text
Hilo 1 multiplicando fila 0
Hilo 1 multiplicando fila 1
Hilo 1 multiplicando fila 2
20.00	35.00	40.00	
30.00	20.00	30.00	
35.00	15.00	50.00	
```

### propuesto3.c y propuesto3_secuencial.c
```bash
gcc propuesto/propuesto3.c -lpthread -o propuesto/propuesto3
./propuesto/propuesto3

gcc propuesto/propuesto3_secuencial.c -o propuesto/propuesto3_secuencial
./propuesto/propuesto3_secuencial
```
Resultado esperado:
```text
Suma total (4 hilos): 300188
Tiempo paralelo: 783 microsegundos

Suma total (secuencial): 300188
Tiempo secuencial: 16 microsegundos
```
