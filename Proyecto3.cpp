#include <iostream>
#include <string>
#include <random>
#include <set>
#include <algorithm>
#include <iterator>
#include <chrono>
#include <stack>
#include <utility>
#include <vector>
#include <numeric>
#include <unordered_set>

using namespace std;

//funcion para medir el tiempo de ejecucion
template<typename Func>
double medirTiempo(Func&& func, std::vector<int>& datos, bool orden) 
{
    auto inicio = std::chrono::steady_clock::now();
    //ejecuta el algoritmo de busqueda con el booleano para el orden
    func(datos, orden); 
    auto fin = std::chrono::steady_clock::now();

    return std::chrono::duration<double, std::milli>(fin - inicio).count();
}



void heapify(vector<int>& datos, int tamano, int indice, bool orden)
{
    int mayor = indice;
    int izquierda = 2 * indice + 1;
    int derecha = 2 * indice + 2;

    if (orden)
    {
        if (izquierda < tamano && datos[izquierda] > datos[mayor])
        {
            mayor = izquierda;
        }

        if (derecha < tamano && datos[derecha] > datos[mayor])
        {
            mayor = derecha;
        }
    }
    else
    {
        if (izquierda < tamano && datos[izquierda] < datos[mayor])
        {
            mayor = izquierda;
        }

        if (derecha < tamano && datos[derecha] < datos[mayor])
        {
            mayor = derecha;
        }
    }

    if (mayor != indice)
    {
        swap(datos[indice], datos[mayor]);
        heapify(datos, tamano, mayor, orden);
    }
}

void heapSort(vector<int>& datos, bool orden)
{
    int tamano = datos.size();

    for (int i = tamano / 2 - 1; i >= 0; --i)
    {
        heapify(datos, tamano, i, orden);
    }

    for (int i = tamano - 1; i >= 0; --i)
    {
        swap(datos[0], datos[i]);
        heapify(datos, i, 0, orden);
    }
}

void quickSort(vector<int>& datos, int inicio, int fin, bool orden)
{
    stack<pair<int, int>> pila;
    pila.push(make_pair(inicio, fin));

    while (!pila.empty())
    {
        int inicioActual = pila.top().first;
        int finActual = pila.top().second;
        pila.pop();

        if (inicioActual >= finActual)
            continue;

        int pivote = datos[finActual];
        int i = inicioActual - 1;

        for (int j = inicioActual; j <= finActual - 1; ++j)
        {
            if (orden)
            {
                if (datos[j] < pivote)
                {
                    ++i;
                    swap(datos[i], datos[j]);
                }
            }
            else
            {
                if (datos[j] > pivote)
                {
                    ++i;
                    swap(datos[i], datos[j]);
                }
            }
        }

        swap(datos[i + 1], datos[finActual]);
        int indicePivote = i + 1;

        pila.push(make_pair(inicioActual, indicePivote - 1));
        pila.push(make_pair(indicePivote + 1, finActual));
    }
}


void merge(vector<int>& datos, int inicio, int medio, int fin, bool orden)
{
    int tamanoIzquierda = medio - inicio + 1;
    int tamanoDerecha = fin - medio;

    vector<int> izquierda(datos.begin() + inicio, datos.begin() + medio + 1);
    vector<int> derecha(datos.begin() + medio + 1, datos.begin() + fin + 1);

    int indiceIzquierda = 0;
    int indiceDerecha = 0;
    int indiceMerge = inicio;

    while (indiceIzquierda < tamanoIzquierda && indiceDerecha < tamanoDerecha)
    {
        if (orden)
        {
            if (izquierda[indiceIzquierda] <= derecha[indiceDerecha])
            {
                datos[indiceMerge] = izquierda[indiceIzquierda];
                ++indiceIzquierda;
            }
            else
            {
                datos[indiceMerge] = derecha[indiceDerecha];
                ++indiceDerecha;
            }
        }
        else
        {
            if (izquierda[indiceIzquierda] >= derecha[indiceDerecha])
            {
                datos[indiceMerge] = izquierda[indiceIzquierda];
                ++indiceIzquierda;
            }
            else
            {
                datos[indiceMerge] = derecha[indiceDerecha];
                ++indiceDerecha;
            }
        }
        ++indiceMerge;
    }

    while (indiceIzquierda < tamanoIzquierda)
    {
        datos[indiceMerge] = izquierda[indiceIzquierda];
        ++indiceIzquierda;
        ++indiceMerge;
    }

    while (indiceDerecha < tamanoDerecha)
    {
        datos[indiceMerge] = derecha[indiceDerecha];
        ++indiceDerecha;
        ++indiceMerge;
    }
}

void mergeSort(vector<int>& datos, int inicio, int fin, bool orden)
{
    if (inicio < fin)
    {
        int medio = inicio + (fin - inicio) / 2;
        mergeSort(datos, inicio, medio, orden);
        mergeSort(datos, medio + 1, fin, orden);
        merge(datos, inicio, medio, fin, orden);
    }
}

//funcion envoltorio para el MergeSort
void mergeSortWrapper(vector<int>& datos, bool orden) {
    int inicio = 0;
    int fin = datos.size() - 1;
    mergeSort(datos, inicio, fin, orden);
}

void selectionSort(vector<int>& datos, bool orden)
{
    int tamano = datos.size();

    for (int i = 0; i < tamano - 1; ++i)
    {
        int indiceExtremo = i;
        for (int j = i + 1; j < tamano; ++j)
        {
            if (orden)
            {
                if (datos[j] < datos[indiceExtremo])
                {
                    indiceExtremo = j;
                }
            }
            else
            {
                if (datos[j] > datos[indiceExtremo])
                {
                    indiceExtremo = j;
                }
            }
        }
        swap(datos[i], datos[indiceExtremo]);
    }
}

void bubbleSort(vector<int>& datos, bool orden)
{
    int tamano = datos.size();

    for (int i = 0; i < tamano - 1; ++i)
    {
        for (int j = 0; j < tamano - i - 1; ++j)
        {
            if (orden)
            {
                if (datos[j] > datos[j + 1])
                {
                    swap(datos[j], datos[j + 1]);
                }
            }
            else
            {
                if (datos[j] < datos[j + 1])
                {
                    swap(datos[j], datos[j + 1]);
                }
            }
        }
    }
}

void insertionSort(vector<int>& datos, bool orden)
{
    int tamano = datos.size();

    for (int i = 1; i < tamano; ++i)
    {
        int valorActual = datos[i];
        int j = i - 1;
        if (orden)
        {
            while (j >= 0 && datos[j] > valorActual)
            {
                datos[j + 1] = datos[j];
                --j;
            }
        }
        else
        {
            while (j >= 0 && datos[j] < valorActual)
            {
                datos[j + 1] = datos[j];
                --j;
            }
        }
        datos[j + 1] = valorActual;
    }
}

void shellSort(vector<int>& datos, bool orden)
{
    int tamano = datos.size();

    for (int brecha = tamano / 2; brecha > 0; brecha /= 2)
    {
        for (int i = brecha; i < tamano; ++i)
        {
            int valorActual = datos[i];
            int j = i;

            if (orden)
            {
                while (j >= brecha && datos[j - brecha] > valorActual)
                {
                    datos[j] = datos[j - brecha];
                    j -= brecha;
                }
            }
            else
            {
                while (j >= brecha && datos[j - brecha] < valorActual)
                {
                    datos[j] = datos[j - brecha];
                    j -= brecha;
                }
            }
            datos[j] = valorActual;
        }
    }
}

//funcion para generar aleatoriamente el total de elementos para las carreras segun se requiera 
int generarTotal(int min, int max, int ext) 
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distribucion(min, max);
    return distribucion(gen) * ext;
}

//funcion para generar elementos en orden ascendente o descendente
vector<int> generarOrden(int totalElementos, bool ascendente) 
{
    vector<int> datos(totalElementos);
    if (ascendente) 
    {   
        for (int i = 0; i < totalElementos; ++i) 
        {
            //generar elementos en orden ascendente
            datos[i] = i; 
        }
    } 
    else 
    {
        for (int i = 0; i < totalElementos; ++i)
        {
            //generar elementos en orden descendente
            datos[i] = totalElementos - i - 1; 
        }
    }
    return datos;
}

//funcion para generar elementos aleatorios
vector<int> generarAleatorioConRepeticion(int totalElementos) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distribucion(1, totalElementos);

    vector<int> datos;
    for (int i = 0; i < totalElementos; ++i) {
        datos.push_back(distribucion(gen)); // Agregar elementos aleatorios al vector
    }
    return datos;
}

//funcion para generar elementos aleatorios únicos
unordered_set<int> generarAleatorioUnicos(int totalElementos) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distribucion(1, totalElementos);

    unordered_set<int> datos;
    while (datos.size() < totalElementos) {
        int valor = distribucion(gen);
        // insertar el valor solo si no esta presente en el conjunto
        if (datos.find(valor) == datos.end()) {
            datos.insert(valor);
        }
    }
    return datos;
}

//funcion para imprimir el menu
void mostrarMenu() 
{
    cout << "******************* Menu *******************" << endl;
    cout << "*          1. Carrera de Colas de Espera   *" << endl;
    cout << "*  2. Carrera de Trazabilidad de Objetos   *" << endl;
    cout << "*    3. Carrera de Eventos de Escenario    *" << endl;
    cout << "*                4. Salir                  *" << endl;
    cout << "*              Ingrese su opcion           *";
}

// Convertir unordered_set a vector
vector<int> convertirUnorderedSetAVector(const unordered_set<int>& datosSinRepeticion) 
{
    vector<int> vectorDatos(datosSinRepeticion.begin(), datosSinRepeticion.end());
    return vectorDatos;
}

int main() 
{
    int opcion;
    bool salir = false;


    //genera un total de jugadores para cola de espera (100,000 a 110,000)
    int totalColasEspera = generarTotal(100000, 110000, 1);
    cout << "Total de jugadores en Colas de Espera: " << totalColasEspera << endl;


    //generar vectores ascendente y descendente a partir del total de jugadores
    vector<int> datosAscendentesColaEspera = generarOrden(totalColasEspera, true);
    vector<int> datosDescendentesColaEspera = generarOrden(totalColasEspera, false);
    vector<int> datosConRepeticionColaEspera = generarAleatorioConRepeticion(totalColasEspera);
    unordered_set<int> datosSinRepeticionColaEspera = generarAleatorioUnicos(totalColasEspera);
    vector<int> vectorDatosSinRepeticionColaEspera = convertirUnorderedSetAVector(datosSinRepeticionColaEspera);


    //generar un total de objetos para la trazabilidad de objetos (1000 a 1500 por cada categoria, 15 categorias)
    int totalTrazabilidadObjetos = generarTotal(1000, 1500, 15);
    cout << "Total de objetos en trazabilidad de objetos: " << totalTrazabilidadObjetos << endl;

    //generar vectores ascendente y descendente a partir del total de objetos
    vector<int> datosAscendentesObjetos = generarOrden(totalTrazabilidadObjetos, true);
    vector<int> datosDescendentesObjetos = generarOrden(totalTrazabilidadObjetos, false);
    vector<int> datosConRepeticionObjetos = generarAleatorioConRepeticion(totalTrazabilidadObjetos);
    unordered_set<int> datosSinRepeticionObjetos = generarAleatorioUnicos(totalTrazabilidadObjetos);
    vector<int> vectorDatosSinrepeticionObjetos = convertirUnorderedSetAVector(datosSinRepeticionObjetos);

    //generar total de posibles combinaciones de eventos para eventos de escenario (60,000 a 80,000)
    int totalEventosEscenario = generarTotal(60000, 80000, 1);
    cout << "Total de posibles combinaciones de eventos en escenario: " << totalEventosEscenario << endl;

    vector<int> datosAscendentesEventos = generarOrden(totalEventosEscenario, true);
    vector<int> datosDescendentesEventos = generarOrden(totalEventosEscenario, false);
    vector<int> datosConRepeticionEventos = generarAleatorioConRepeticion(totalEventosEscenario);
    unordered_set<int> datosSinRepeticionEventos = generarAleatorioUnicos(totalEventosEscenario);
    vector<int> vectorDatosSinrepeticionEventos = convertirUnorderedSetAVector(datosSinRepeticionEventos);



    while (!salir) 
    {
        mostrarMenu();
        cin >> opcion;

        switch (opcion) 
        {
            case 1:
                
                //logica para la carrera de colas de espera
                cout << "Realizando Carrera de colas de espera..." << endl;

                break;
                
            case 2:

                //logica para la carrera de trazabilidad de objetos
                cout << "Realizando Carrera de Trazabilidad de objetos..." << endl;
                //implementa la logica para esta carrera
                break;

            case 3:

                //logica para la carrera de eventos de escenario
                cout << "Realizando carrera de eventos de escenario..." << endl;
                double tiempoAscendente, tiempoDescendente, tiempoConRepeticion, tiempoSinRepeticion;

                //medicion del tiempo para el insertionSort con diferentes conjuntos de datos de eventos
                tiempoAscendente = medirTiempo(mergeSortWrapper, datosAscendentesEventos, true);
                tiempoDescendente = medirTiempo(mergeSortWrapper, datosDescendentesEventos, true);
                tiempoConRepeticion = medirTiempo(mergeSortWrapper, datosConRepeticionEventos, true);
                tiempoSinRepeticion = medirTiempo(mergeSortWrapper, vectorDatosSinrepeticionEventos, true);

                //imprimir tiempos de ejecucion
                cout << "Tiempo de ejecucion de mergeSortWrapper con datos ascendentes: " << tiempoAscendente << " milisegundos" << std::endl;
                cout << "Tiempo de ejecucion de mergeSortWrapper con datos descendentes: " << tiempoDescendente << " milisegundos" << std::endl;
                cout << "Tiempo de ejecucion de mergeSortWrapper con datos con repeticion: " << tiempoConRepeticion << " milisegundos" << std::endl;
                cout << "Tiempo de ejecucion de mergeSortWrapper con datos sin repeticion: " << tiempoSinRepeticion << " milisegundos" << std::endl;
                

                break;

            case 4:
                salir = true;
                cout << "Gracias por usar el programa, saliendo..." << endl;
                break;
            default:
                cout << "Opcion no valida. Por favor, ingrese una opcion valida." << endl;
                break;
        }
    }

    return 0;
}
