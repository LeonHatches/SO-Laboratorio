# Laboratorio 07 - Exclusion Mutua con Mutex (PThreads)

## Resuelto

### resuelto1.c
```bash
gcc resuelto/resuelto1.c -lpthread -o resuelto/resuelto1
./resuelto/resuelto1
```
Resultado esperado:
```text
Inicio Inicio Proceso Hijo:: 0 proceso Padre: 0 Finalizacion 
Finalizacion 
Inicio proceso Padre: 1 Finalizacion 
...
```

### resuelto2.c
```bash
gcc resuelto/resuelto2.c -lpthread -o resuelto/resuelto2
./resuelto/resuelto2
```
Resultado esperado:
```text
Inicio proceso Padre: 1 Finalizacion 
Inicio Proceso Hijo:: 0 Finalizacion 
Inicio proceso Padre: 1 Finalizacion 
...
```

## Propuesto

### propuesto1.c
```bash
gcc propuesto/propuesto1.c -lpthread -o propuesto/propuesto1
./propuesto/propuesto1
```
Resultado esperado:
```text
Suma total con mutex: 300188
```

### propuesto2.c
```bash
gcc propuesto/propuesto2.c -lpthread -o propuesto/propuesto2
./propuesto/propuesto2
```
Resultado esperado:
```text
Productor escribio: 10
Consumidor leyo: 10
Productor escribio: 20
Consumidor leyo: 20
Productor escribio: 30
Consumidor leyo: 30
Productor escribio: 40
Consumidor leyo: 40
Productor escribio: 50
Consumidor leyo: 50
```

### propuesto3.c
```bash
gcc propuesto/propuesto3.c -lpthread -o propuesto/propuesto3
./propuesto/propuesto3
```
Resultado esperado:
```text
Hilo [0][0] calculo: 8
...
Hilo [3][3] calculo: 37
8	7	11	16	
24	19	27	44	
22	4	16	36	
20	16	23	37	
```
