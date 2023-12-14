# Proyecto de Algoritmos de Ordenación


Este proyecto aborda la implementación y análisis de seis algoritmos de ordenación:

- Selection Sort.
- Bubble Sort.
- Insertion Sort.
- Shell Sort.
- Merge Sort.
- Quick Sort.

## Estructura del Proyecto.

La solución se organiza en seis partes claramente definidas:

### Parte 1: Definición de las Estructuras de Datos.

En el archivo `datos.h`, se encuentran las definiciones de las estructuras de datos utilizadas por los algoritmos de ordenación. Las estructuras son:
- `VectorDeDatos`: Representa un vector de datos.
- `ResultadosCarrera`: Estructura para representar los resultados de una carrera.

### Parte 2: Implementación de los Algoritmos de Ordenación

Los algoritmos se implementan en el archivo `main.cpp` utilizando un estilo de programación modular. Cada algoritmo tiene su propia función independiente, utilizando las estructuras de datos definidas en `datos.h`.

### Parte 3: Generación de Datos de Entrada

En `main.cpp`, la función `generarDatos()` se encarga de generar datos de entrada para las carreras. Pueden ser ordenados, desordenados o con repeticiones.

### Parte 4: Medición del Tiempo de Ejecución de los Algoritmos

La medición del tiempo de ejecución se realiza en `main.cpp` utilizando la función `medirTiempo()`. Esta función mide el tiempo que tarda en ejecutarse una función.

### Parte 5: Visualización de los Resultados

La visualización de los resultados se encuentra en `main.cpp`, utilizando la función `visualizarResultados()`. Esta función imprime una tabla con los resultados de las carreras.

### Parte 6: Análisis de los Resultados

El análisis de los resultados se realiza en `main.cpp` mediante la función `analizarResultados()`, que imprime un resumen de los resultados de las carreras.

## Conclusiones

Tras analizar los resultados, se concluye que, en general, los algoritmos Merge Sort y Quick Sort son los más eficientes. Su eficiencia radica en su capacidad para dividir el problema en subproblemas más pequeños y resolverlos recursivamente. En contraste, el algoritmo Selection Sort es el menos eficiente debido a su enfoque comparativo intensivo.

En términos generales, se recomienda el uso de Merge Sort o Quick Sort para ordenar datos, ya que son los algoritmos más eficientes en la mayoría de los casos.
