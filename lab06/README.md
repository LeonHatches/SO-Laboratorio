# Laboratorio 06 - Hilos POSIX

Desde la carpeta que contiene `capturas.sh`, ejecuta estos comandos en Linux:

```bash
bash capturas.sh join
bash capturas.sh tiempos
bash capturas.sh cuestionario
```

El primero muestra cuántas líneas del hilo secundario aparecen con y sin
`pthread_join`. El segundo mide 100 y 200 ciclos de crear y esperar a un
proceso o un hilo, e imprime el promedio en microsegundos por ciclo. El
tercero imprime una muestra y los cinco resultados del cuestionario.
La salida completa se guarda en `lab06/salida_cuestionario3.txt`. La carrera
de datos en ese ejemplo se conserva a propósito para observarla.

Los demás resueltos se ejecutan desde esta carpeta. `code1.c` conserva el
ejemplo de la guía. En `Ejercicio_Matriz.c`, el hilo principal imprime la
matriz antes y después de multiplicarla por 5.

```bash
gcc -std=gnu11 -pthread resuelto/code1.c -o /tmp/lab06-code1
/tmp/lab06-code1
gcc -std=gnu11 -pthread resuelto/Ejercicio_Matriz.c -o /tmp/lab06-matriz
/tmp/lab06-matriz
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
En Linux con glibc, ambos programas producen 300188. Los tiempos dependen del entorno. En las capturas del informe se obtuvieron 213 µs en paralelo y 5 µs en secuencial.
