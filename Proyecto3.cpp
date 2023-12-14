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
#include <map>


using namespace std;

//funcion para medir el tiempo de ejecucion
template<typename Func>
double medirTiempo(Func&& func, vector<int>& datos, bool orden) 
{
    auto inicio = chrono::steady_clock::now();
    //ejecuta el algoritmo de busqueda con el booleano para el orden
    func(datos, orden);
    auto fin = chrono::steady_clock::now();

    return chrono::duration<double>(fin - inicio).count();
}


//estructura para almacenar los resultados de los algoritmos
struct ResultadosCarrera 
{
    // tipo de carrera: cola de espera, trazabilidad de objetos, eventos de escenario, etc
    string tipoCarrera; 
    //mapa para almacenar tiempos de ejecucion por nombre del algoritmo
    map<string, double> tiempos; 
    //constructor que recibe el tipo de carrera
    ResultadosCarrera(const string& tipo) : tipoCarrera(tipo) {}

    //metodo para agregar un resultado a la estructura
    void agregarResultado(const string& nombreAlgoritmo, double tiempo) 
    {
        //almacenar el tiempo asociado al nombre del algoritmo
        tiempos[nombreAlgoritmo] = tiempo;     
    }

    //metodo para mostrar el resumen de la carrera
    void mostrarResumen() 
    {
        cout << "Carrera " << tipoCarrera << ":\n";
        int posicion = 1;
        double mejorTiempo = -1;
        string nombreGanador;

        //iterar sobre los resultados para imprimir la posicion y tiempo de ejecucion
        for (const auto& par : tiempos) 
        {
            cout << posicion << ". " << par.first << ", " << par.second << " segundos\n";
            if (mejorTiempo < 0 || par.second < mejorTiempo) 
            {
                mejorTiempo = par.second;
                nombreGanador = par.first;
            }
            posicion++;
        }

        if (!tiempos.empty()) 
        {
            cout << "El ganador es: " << nombreGanador << " con un tiempo de " << mejorTiempo << " segundos\n";
        } 
        else 
        {
            cout << "No hay resultados para mostrar.\n";
        }
    }
};


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

//funcion envoltorio para el QuickSort
void quickSortWrapper(vector<int>& datos, bool orden) 
{
    int inicio = 0;
    int fin = datos.size() - 1;
    quickSort(datos, inicio, fin, orden);
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
void mergeSortWrapper(vector<int>& datos, bool orden) 
{
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
            // Generar elementos en orden ascendente
            datos[i] = i;
        }
    } 
    else 
    {
        for (int i = 0; i < totalElementos; ++i)
         {
            // Generar elementos en orden descendente
            datos[i] = totalElementos - i - 1; 
        }
    }
    return datos;
}

//funcion para generar elementos aleatorios
vector<int> generarAleatorioConRepeticion(int totalElementos) 
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distribucion(1, totalElementos);

    vector<int> datos;
    for (int i = 0; i < totalElementos; ++i) 
    {
        //agregar elementos aleatorios al vector
        datos.push_back(distribucion(gen)); 
    }
    return datos;
}

//funcion para generar elementos aleatorios unicos
unordered_set<int> generarAleatorioUnicos(int totalElementos) 
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distribucion(1, totalElementos);

    unordered_set<int> datos;
    while (datos.size() < totalElementos) 
    {
        int valor = distribucion(gen);
        //insertar el valor solo si no esta presente en el conjunto
        if (datos.find(valor) == datos.end()) 
        {
            datos.insert(valor);
        }
    }
    return datos;
}

//Funcion para imprimir el menu
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
    cout << "Total de objetos en Trazabilidad de Objetos: " << totalTrazabilidadObjetos << endl;

    //generar vectores ascendente y descendente a partir del total de objetos
    vector<int> datosAscendentesObjetos = generarOrden(totalTrazabilidadObjetos, true);
    vector<int> datosDescendentesObjetos = generarOrden(totalTrazabilidadObjetos, false);
    vector<int> datosConRepeticionObjetos = generarAleatorioConRepeticion(totalTrazabilidadObjetos);
    unordered_set<int> datosSinRepeticionObjetos = generarAleatorioUnicos(totalTrazabilidadObjetos);
    vector<int> vectorDatosSinrepeticionObjetos = convertirUnorderedSetAVector(datosSinRepeticionObjetos);

    //generar total de posibles combinaciones de eventos para eventos de escenario (60,000 a 80,000)
    int totalEventosEscenario = generarTotal(60000, 80000, 1);
    cout << "Total de posibles combinaciones de eventos en Escenario: " << totalEventosEscenario << endl;

    vector<int> datosAscendentesEventos = generarOrden(totalEventosEscenario, true);
    vector<int> datosDescendentesEventos = generarOrden(totalEventosEscenario, false);
    vector<int> datosConRepeticionEventos = generarAleatorioConRepeticion(totalEventosEscenario);
    unordered_set<int> datosSinRepeticionEventos = generarAleatorioUnicos(totalEventosEscenario);
    vector<int> vectorDatosSinrepeticionEventos = convertirUnorderedSetAVector(datosSinRepeticionEventos);

    ResultadosCarrera resultadosColaEsperaOrdenada("Carrera Cola de espera, Modo Ordenado");
    ResultadosCarrera resultadosColaEsperaInversa("Carrera Cola de espera, Modo Inverso");
    ResultadosCarrera resultadosColaEsperaConRepeticion("Carrera Cola de espera, Modo con repetición");
    ResultadosCarrera resultadosColaEsperaSinRepeticion("Carrera Cola de espera, Modo sin repetición");
    ResultadosCarrera resultadosObjetosOrdenada("Carrera Trazabilidad de Objetos, Modo Ordenado");
    ResultadosCarrera resultadosObjetosInversa("Carrera Trazabilidad de Objetos, Modo Inverso");
    ResultadosCarrera resultadosObjetosConRepeticion("Carrera Trazabilidad de Objetos, Modo con repetición");
    ResultadosCarrera resultadosObjetosSinRepeticion("Carrera Trazabilidad de Objetos, Modo sin repetición");
    ResultadosCarrera resultadosEventosOrdenada("Eventos de Escenario, Modo Ordenado");
    ResultadosCarrera resultadosEventosInversa("Eventos de Escenario, Modo Inverso");
    ResultadosCarrera resultadosEventosConRepeticion("Eventos de Escenario, Modo con repetición");
    ResultadosCarrera resultadosEventosSinRepeticion("Eventos de Escenario, Modo sin repetición");

    vector<int> copiadatosAscendentesColaEspera = datosAscendentesColaEspera;
    vector<int> copiadatosDescendentesColaEspera = datosDescendentesColaEspera;
    vector<int> copiadatosConRepeticionColaEspera = datosConRepeticionColaEspera;
    vector<int> copiadatosSinRepeticionColaEspera = vectorDatosSinRepeticionColaEspera;

    vector<int> copiadatosAscendentesObjetos = datosAscendentesObjetos;
    vector<int> copiadatosDescendentesObjetos = datosDescendentesObjetos;
    vector<int> copiadatosConRepeticionObjetos = datosConRepeticionObjetos;
    vector<int> copiadatosSinRepeticionObjetos = vectorDatosSinrepeticionObjetos;

    vector<int> copiadatosAscendentesEventos = datosAscendentesEventos;
    vector<int> copiadatosDescendentesEventos = datosDescendentesEventos;
    vector<int> copiadatosConRepeticionEventos = datosConRepeticionEventos;
    vector<int> copiadatosSinRepeticionEventos = vectorDatosSinrepeticionEventos;

    while (!salir) 
    {
        mostrarMenu();
        cin >> opcion;

        switch (opcion) 
        {
            case 1:
                cout << "Realizando Carrera de Colas de Espera..." << endl;
                double tiempoAscendente, tiempoDescendente, tiempoConRepeticion, tiempoSinRepeticion;

                
                tiempoAscendente = medirTiempo(selectionSort, copiadatosAscendentesColaEspera, true);
                resultadosColaEsperaOrdenada.agregarResultado("Selection Sort", tiempoAscendente);

                copiadatosAscendentesColaEspera = datosAscendentesColaEspera;
                tiempoAscendente = medirTiempo(bubbleSort,copiadatosAscendentesColaEspera, true);
                resultadosColaEsperaOrdenada.agregarResultado("Bubble Sort", tiempoAscendente);

                copiadatosAscendentesColaEspera = datosAscendentesColaEspera;
                tiempoAscendente = medirTiempo(insertionSort,copiadatosAscendentesColaEspera, true);
                resultadosColaEsperaOrdenada.agregarResultado("Insertion Sort", tiempoAscendente);

                copiadatosAscendentesColaEspera = datosAscendentesColaEspera;
                tiempoAscendente = medirTiempo(shellSort,copiadatosAscendentesColaEspera, true);
                resultadosColaEsperaOrdenada.agregarResultado("Shell Sort", tiempoAscendente);

                copiadatosAscendentesColaEspera = datosAscendentesColaEspera;
                tiempoAscendente = medirTiempo(mergeSortWrapper,copiadatosAscendentesColaEspera, true);
                resultadosColaEsperaOrdenada.agregarResultado("Merge Sort", tiempoAscendente);

                copiadatosAscendentesColaEspera = datosAscendentesColaEspera;
                tiempoAscendente = medirTiempo(quickSortWrapper,copiadatosAscendentesColaEspera, true);
                resultadosColaEsperaOrdenada.agregarResultado("Quick Sort", tiempoAscendente);

                copiadatosAscendentesColaEspera = datosAscendentesColaEspera;
                tiempoAscendente = medirTiempo(heapSort,copiadatosAscendentesColaEspera, true);
                resultadosColaEsperaOrdenada.agregarResultado("Heap Sort", tiempoAscendente);

                resultadosColaEsperaOrdenada.mostrarResumen();

                tiempoDescendente = medirTiempo(selectionSort, copiadatosDescendentesColaEspera, true);
                resultadosColaEsperaInversa.agregarResultado("Selection Sort", tiempoDescendente);

                copiadatosDescendentesColaEspera = datosDescendentesColaEspera;
                tiempoDescendente = medirTiempo(bubbleSort, copiadatosDescendentesColaEspera, true);
                resultadosColaEsperaInversa.agregarResultado("Bubble Sort", tiempoDescendente);

                copiadatosDescendentesColaEspera = datosDescendentesColaEspera;
                tiempoDescendente = medirTiempo(insertionSort, copiadatosDescendentesColaEspera, true);
                resultadosColaEsperaInversa.agregarResultado("Insertion Sort", tiempoDescendente);

                copiadatosDescendentesColaEspera = datosDescendentesColaEspera;
                tiempoDescendente = medirTiempo(shellSort, copiadatosDescendentesColaEspera, true);
                resultadosColaEsperaInversa.agregarResultado("Shell Sort", tiempoDescendente);

                copiadatosDescendentesColaEspera = datosDescendentesColaEspera;
                tiempoDescendente = medirTiempo(mergeSortWrapper, copiadatosDescendentesColaEspera, true);
                resultadosColaEsperaInversa.agregarResultado("Merge Sort", tiempoDescendente);

                copiadatosDescendentesColaEspera = datosDescendentesColaEspera;
                tiempoDescendente = medirTiempo(quickSortWrapper, copiadatosDescendentesColaEspera, true);
                resultadosColaEsperaInversa.agregarResultado("Quick Sort", tiempoDescendente);

                copiadatosDescendentesColaEspera = datosDescendentesColaEspera;
                tiempoDescendente = medirTiempo(heapSort, copiadatosDescendentesColaEspera, true);
                resultadosColaEsperaInversa.agregarResultado("Heap Sort", tiempoDescendente);

                resultadosColaEsperaInversa.mostrarResumen();

                tiempoConRepeticion = medirTiempo(selectionSort, copiadatosConRepeticionColaEspera, true);
                resultadosColaEsperaConRepeticion.agregarResultado("Selection Sort", tiempoConRepeticion);

                copiadatosConRepeticionColaEspera = datosConRepeticionColaEspera;
                tiempoConRepeticion = medirTiempo(bubbleSort, copiadatosConRepeticionColaEspera, true);
                resultadosColaEsperaConRepeticion.agregarResultado("Bubble Sort", tiempoConRepeticion);

                copiadatosConRepeticionColaEspera = datosConRepeticionColaEspera;
                tiempoConRepeticion = medirTiempo(insertionSort, copiadatosConRepeticionColaEspera, true);
                resultadosColaEsperaConRepeticion.agregarResultado("Insertion Sort", tiempoConRepeticion);

                copiadatosConRepeticionColaEspera = datosConRepeticionColaEspera;
                tiempoConRepeticion = medirTiempo(shellSort, copiadatosConRepeticionColaEspera, true);
                resultadosColaEsperaConRepeticion.agregarResultado("Shell Sort", tiempoConRepeticion);

                copiadatosConRepeticionColaEspera = datosConRepeticionColaEspera;
                tiempoConRepeticion = medirTiempo(mergeSortWrapper, copiadatosConRepeticionColaEspera, true);
                resultadosColaEsperaConRepeticion.agregarResultado("Merge Sort", tiempoConRepeticion);

                copiadatosConRepeticionColaEspera = datosConRepeticionColaEspera;
                tiempoConRepeticion = medirTiempo(quickSortWrapper, copiadatosConRepeticionColaEspera, true);
                resultadosColaEsperaConRepeticion.agregarResultado("Quick Sort", tiempoConRepeticion);

                copiadatosConRepeticionColaEspera = datosConRepeticionColaEspera;
                tiempoConRepeticion = medirTiempo(heapSort, copiadatosConRepeticionColaEspera, true);
                resultadosColaEsperaConRepeticion.agregarResultado("Heap Sort", tiempoConRepeticion);

                resultadosColaEsperaConRepeticion.mostrarResumen();

                tiempoSinRepeticion = medirTiempo(selectionSort, copiadatosSinRepeticionColaEspera, true);
                resultadosColaEsperaSinRepeticion.agregarResultado("Selection Sort", tiempoSinRepeticion);

                copiadatosSinRepeticionColaEspera = vectorDatosSinRepeticionColaEspera;
                tiempoSinRepeticion = medirTiempo(bubbleSort, copiadatosSinRepeticionColaEspera, true);
                resultadosColaEsperaSinRepeticion.agregarResultado("Bubble Sort", tiempoSinRepeticion);

                copiadatosSinRepeticionColaEspera = vectorDatosSinRepeticionColaEspera;
                tiempoSinRepeticion = medirTiempo(insertionSort, copiadatosSinRepeticionColaEspera, true);
                resultadosColaEsperaSinRepeticion.agregarResultado("Insertion Sort", tiempoSinRepeticion);

                copiadatosSinRepeticionColaEspera = vectorDatosSinRepeticionColaEspera;
                tiempoSinRepeticion = medirTiempo(shellSort, copiadatosSinRepeticionColaEspera, true);
                resultadosColaEsperaSinRepeticion.agregarResultado("Shell Sort", tiempoSinRepeticion);

                copiadatosSinRepeticionColaEspera = vectorDatosSinRepeticionColaEspera;
                tiempoSinRepeticion = medirTiempo(mergeSortWrapper, copiadatosSinRepeticionColaEspera, true);
                resultadosColaEsperaSinRepeticion.agregarResultado("Merge Sort", tiempoSinRepeticion);

                copiadatosSinRepeticionColaEspera = vectorDatosSinRepeticionColaEspera;
                tiempoSinRepeticion = medirTiempo(quickSortWrapper, copiadatosSinRepeticionColaEspera, true);
                resultadosColaEsperaSinRepeticion.agregarResultado("Quick Sort", tiempoSinRepeticion);

                copiadatosSinRepeticionColaEspera = vectorDatosSinRepeticionColaEspera;
                tiempoSinRepeticion = medirTiempo(heapSort, copiadatosSinRepeticionColaEspera, true);
                resultadosColaEsperaSinRepeticion.agregarResultado("Heap Sort", tiempoSinRepeticion);

                resultadosColaEsperaSinRepeticion.mostrarResumen();
                break;

                
            case 2:
                cout << "Realizando Carrera de Trazabilidad de Objetos..." << endl;

                tiempoAscendente = medirTiempo(selectionSort, copiadatosAscendentesObjetos, true);
                resultadosObjetosOrdenada.agregarResultado("Selection Sort", tiempoAscendente);

                copiadatosAscendentesObjetos = datosAscendentesObjetos;
                tiempoAscendente = medirTiempo(bubbleSort,copiadatosAscendentesObjetos, true);
                resultadosObjetosOrdenada.agregarResultado("Bubble Sort", tiempoAscendente);

                copiadatosAscendentesObjetos = datosAscendentesObjetos;
                tiempoAscendente = medirTiempo(insertionSort,copiadatosAscendentesObjetos, true);
                resultadosObjetosOrdenada.agregarResultado("Insertion Sort", tiempoAscendente);

                copiadatosAscendentesObjetos = datosAscendentesObjetos;
                tiempoAscendente = medirTiempo(shellSort,copiadatosAscendentesObjetos, true);
                resultadosObjetosOrdenada.agregarResultado("Shell Sort", tiempoAscendente);

                copiadatosAscendentesObjetos = datosAscendentesObjetos;
                tiempoAscendente = medirTiempo(mergeSortWrapper,copiadatosAscendentesObjetos, true);
                resultadosObjetosOrdenada.agregarResultado("Merge Sort", tiempoAscendente);

                copiadatosAscendentesObjetos = datosAscendentesObjetos;
                tiempoAscendente = medirTiempo(quickSortWrapper,copiadatosAscendentesObjetos, true);
                resultadosObjetosOrdenada.agregarResultado("Quick Sort", tiempoAscendente);

                copiadatosAscendentesObjetos = datosAscendentesObjetos;
                tiempoAscendente = medirTiempo(heapSort,copiadatosAscendentesObjetos, true);
                resultadosObjetosOrdenada.agregarResultado("Heap Sort", tiempoAscendente);

                resultadosObjetosOrdenada.mostrarResumen();

                tiempoDescendente = medirTiempo(selectionSort, copiadatosDescendentesObjetos, true);
                resultadosObjetosInversa.agregarResultado("Selection Sort", tiempoDescendente);

                copiadatosDescendentesObjetos = datosDescendentesObjetos;
                tiempoDescendente = medirTiempo(bubbleSort, copiadatosDescendentesObjetos, true);
                resultadosObjetosInversa.agregarResultado("Bubble Sort", tiempoDescendente);

                copiadatosDescendentesObjetos = datosDescendentesObjetos;
                tiempoDescendente = medirTiempo(insertionSort, copiadatosDescendentesObjetos, true);
                resultadosObjetosInversa.agregarResultado("Insertion Sort", tiempoDescendente);

                copiadatosDescendentesObjetos = datosDescendentesObjetos;
                tiempoDescendente = medirTiempo(shellSort, copiadatosDescendentesObjetos, true);
                resultadosObjetosInversa.agregarResultado("Shell Sort", tiempoDescendente);

                copiadatosDescendentesObjetos = datosDescendentesObjetos;
                tiempoDescendente = medirTiempo(mergeSortWrapper, copiadatosDescendentesObjetos, true);
                resultadosObjetosInversa.agregarResultado("Merge Sort", tiempoDescendente);

                copiadatosDescendentesObjetos = datosDescendentesObjetos;
                tiempoDescendente = medirTiempo(quickSortWrapper, copiadatosDescendentesObjetos, true);
                resultadosObjetosInversa.agregarResultado("Quick Sort", tiempoDescendente);

                copiadatosDescendentesObjetos = datosDescendentesObjetos;
                tiempoDescendente = medirTiempo(heapSort, copiadatosDescendentesObjetos, true);
                resultadosObjetosInversa.agregarResultado("Heap Sort", tiempoDescendente);

                resultadosObjetosInversa.mostrarResumen();

                tiempoConRepeticion = medirTiempo(selectionSort, copiadatosConRepeticionObjetos, true);
                resultadosObjetosConRepeticion.agregarResultado("Selection Sort", tiempoConRepeticion);

                copiadatosConRepeticionObjetos = datosConRepeticionObjetos;
                tiempoConRepeticion = medirTiempo(bubbleSort, copiadatosConRepeticionObjetos, true);
                resultadosObjetosConRepeticion.agregarResultado("Bubble Sort", tiempoConRepeticion);

                copiadatosConRepeticionObjetos = datosConRepeticionObjetos;
                tiempoConRepeticion = medirTiempo(insertionSort, copiadatosConRepeticionObjetos, true);
                resultadosObjetosConRepeticion.agregarResultado("Insertion Sort", tiempoConRepeticion);

                copiadatosConRepeticionObjetos = datosConRepeticionObjetos;
                tiempoConRepeticion = medirTiempo(shellSort, copiadatosConRepeticionObjetos, true);
                resultadosObjetosConRepeticion.agregarResultado("Shell Sort", tiempoConRepeticion);

                copiadatosConRepeticionObjetos = datosConRepeticionObjetos;
                tiempoConRepeticion = medirTiempo(mergeSortWrapper, copiadatosConRepeticionObjetos, true);
                resultadosObjetosConRepeticion.agregarResultado("Merge Sort", tiempoConRepeticion);

                copiadatosConRepeticionObjetos = datosConRepeticionObjetos;
                tiempoConRepeticion = medirTiempo(quickSortWrapper, copiadatosConRepeticionObjetos, true);
                resultadosObjetosConRepeticion.agregarResultado("Quick Sort", tiempoConRepeticion);

                copiadatosConRepeticionObjetos = datosConRepeticionObjetos;
                tiempoConRepeticion = medirTiempo(heapSort, copiadatosConRepeticionObjetos, true);
                resultadosObjetosConRepeticion.agregarResultado("Heap Sort", tiempoConRepeticion);

                resultadosObjetosConRepeticion.mostrarResumen();

                tiempoSinRepeticion = medirTiempo(selectionSort, copiadatosSinRepeticionObjetos, true);
                resultadosObjetosSinRepeticion.agregarResultado("Selection Sort", tiempoSinRepeticion);

                copiadatosSinRepeticionObjetos = vectorDatosSinrepeticionObjetos;
                tiempoSinRepeticion = medirTiempo(bubbleSort, copiadatosSinRepeticionObjetos, true);
                resultadosObjetosSinRepeticion.agregarResultado("Bubble Sort", tiempoSinRepeticion);

                copiadatosSinRepeticionObjetos = vectorDatosSinrepeticionObjetos;
                tiempoSinRepeticion = medirTiempo(insertionSort, copiadatosSinRepeticionObjetos, true);
                resultadosObjetosSinRepeticion.agregarResultado("Insertion Sort", tiempoSinRepeticion);

                copiadatosSinRepeticionObjetos = vectorDatosSinrepeticionObjetos;
                tiempoSinRepeticion = medirTiempo(shellSort, copiadatosSinRepeticionObjetos, true);
                resultadosObjetosSinRepeticion.agregarResultado("Shell Sort", tiempoSinRepeticion);

                copiadatosSinRepeticionObjetos = vectorDatosSinrepeticionObjetos;
                tiempoSinRepeticion = medirTiempo(mergeSortWrapper, copiadatosSinRepeticionObjetos, true);
                resultadosObjetosSinRepeticion.agregarResultado("Merge Sort", tiempoSinRepeticion);

                copiadatosSinRepeticionObjetos = vectorDatosSinrepeticionObjetos;
                tiempoSinRepeticion = medirTiempo(quickSortWrapper, copiadatosSinRepeticionObjetos, true);
                resultadosObjetosSinRepeticion.agregarResultado("Quick Sort", tiempoSinRepeticion);

                copiadatosSinRepeticionObjetos = vectorDatosSinrepeticionObjetos;
                tiempoSinRepeticion = medirTiempo(heapSort, copiadatosSinRepeticionObjetos, true);
                resultadosObjetosSinRepeticion.agregarResultado("Heap Sort", tiempoSinRepeticion);

                resultadosObjetosSinRepeticion.mostrarResumen();

                break;


            case 3:
                cout << "Realizando Carrera de Eventos de Escenario..." << endl;

                tiempoAscendente = medirTiempo(selectionSort, copiadatosAscendentesEventos, true);
                resultadosEventosOrdenada.agregarResultado("Selection Sort", tiempoAscendente);

                copiadatosAscendentesEventos = datosAscendentesEventos;
                tiempoAscendente = medirTiempo(bubbleSort,copiadatosAscendentesEventos, true);
                resultadosEventosOrdenada.agregarResultado("Bubble Sort", tiempoAscendente);

                copiadatosAscendentesEventos = datosAscendentesEventos;
                tiempoAscendente = medirTiempo(insertionSort,copiadatosAscendentesEventos, true);
                resultadosEventosOrdenada.agregarResultado("Insertion Sort", tiempoAscendente);

                copiadatosAscendentesEventos = datosAscendentesEventos;
                tiempoAscendente = medirTiempo(shellSort,copiadatosAscendentesEventos, true);
                resultadosEventosOrdenada.agregarResultado("Shell Sort", tiempoAscendente);

                copiadatosAscendentesEventos = datosAscendentesEventos;
                tiempoAscendente = medirTiempo(mergeSortWrapper,copiadatosAscendentesEventos, true);
                resultadosEventosOrdenada.agregarResultado("Merge Sort", tiempoAscendente);

                copiadatosAscendentesEventos = datosAscendentesEventos;
                tiempoAscendente = medirTiempo(quickSortWrapper,copiadatosAscendentesEventos, true);
                resultadosEventosOrdenada.agregarResultado("Quick Sort", tiempoAscendente);

                copiadatosAscendentesEventos = datosAscendentesEventos;
                tiempoAscendente = medirTiempo(heapSort,copiadatosAscendentesEventos, true);
                resultadosEventosOrdenada.agregarResultado("Heap Sort", tiempoAscendente);

                resultadosEventosOrdenada.mostrarResumen();

                tiempoDescendente = medirTiempo(selectionSort, copiadatosDescendentesEventos, true);
                resultadosEventosInversa.agregarResultado("Selection Sort", tiempoDescendente);

                copiadatosDescendentesEventos = datosDescendentesEventos;
                tiempoDescendente = medirTiempo(bubbleSort, copiadatosDescendentesEventos, true);
                resultadosEventosInversa.agregarResultado("Bubble Sort", tiempoDescendente);

                copiadatosDescendentesEventos = datosDescendentesEventos;
                tiempoDescendente = medirTiempo(insertionSort, copiadatosDescendentesEventos, true);
                resultadosEventosInversa.agregarResultado("Insertion Sort", tiempoDescendente);

                copiadatosDescendentesEventos = datosDescendentesEventos;
                tiempoDescendente = medirTiempo(shellSort, copiadatosDescendentesEventos, true);
                resultadosEventosInversa.agregarResultado("Shell Sort", tiempoDescendente);

                copiadatosDescendentesEventos = datosDescendentesEventos;
                tiempoDescendente = medirTiempo(mergeSortWrapper, copiadatosDescendentesEventos, true);
                resultadosEventosInversa.agregarResultado("Merge Sort", tiempoDescendente);

                copiadatosDescendentesEventos = datosDescendentesEventos;
                tiempoDescendente = medirTiempo(quickSortWrapper, copiadatosDescendentesEventos, true);
                resultadosEventosInversa.agregarResultado("Quick Sort", tiempoDescendente);

                copiadatosDescendentesEventos = datosDescendentesEventos;
                tiempoDescendente = medirTiempo(heapSort, copiadatosDescendentesEventos, true);
                resultadosEventosInversa.agregarResultado("Heap Sort", tiempoDescendente);

                resultadosEventosInversa.mostrarResumen();

                tiempoConRepeticion = medirTiempo(selectionSort, copiadatosConRepeticionEventos, true);
                resultadosEventosConRepeticion.agregarResultado("Selection Sort", tiempoConRepeticion);

                copiadatosConRepeticionEventos = datosConRepeticionEventos;
                tiempoConRepeticion = medirTiempo(bubbleSort, copiadatosConRepeticionEventos, true);
                resultadosEventosConRepeticion.agregarResultado("Bubble Sort", tiempoConRepeticion);

                copiadatosConRepeticionEventos = datosConRepeticionEventos;
                tiempoConRepeticion = medirTiempo(insertionSort, copiadatosConRepeticionEventos, true);
                resultadosEventosConRepeticion.agregarResultado("Insertion Sort", tiempoConRepeticion);

                copiadatosConRepeticionEventos = datosConRepeticionEventos;
                tiempoConRepeticion = medirTiempo(shellSort, copiadatosConRepeticionEventos, true);
                resultadosEventosConRepeticion.agregarResultado("Shell Sort", tiempoConRepeticion);

                copiadatosConRepeticionEventos = datosConRepeticionEventos;
                tiempoConRepeticion = medirTiempo(mergeSortWrapper, copiadatosConRepeticionEventos, true);
                resultadosEventosConRepeticion.agregarResultado("Merge Sort", tiempoConRepeticion);

                copiadatosConRepeticionEventos = datosConRepeticionEventos;
                tiempoConRepeticion = medirTiempo(quickSortWrapper, copiadatosConRepeticionEventos, true);
                resultadosEventosConRepeticion.agregarResultado("Quick Sort", tiempoConRepeticion);

                copiadatosConRepeticionEventos = datosConRepeticionEventos;
                tiempoConRepeticion = medirTiempo(heapSort, copiadatosConRepeticionEventos, true);
                resultadosEventosConRepeticion.agregarResultado("Heap Sort", tiempoConRepeticion);

                resultadosEventosConRepeticion.mostrarResumen();

                tiempoSinRepeticion = medirTiempo(selectionSort, copiadatosSinRepeticionEventos, true);
                resultadosEventosSinRepeticion.agregarResultado("Selection Sort", tiempoSinRepeticion);

                copiadatosSinRepeticionEventos = vectorDatosSinrepeticionEventos;
                tiempoSinRepeticion = medirTiempo(bubbleSort, copiadatosSinRepeticionEventos, true);
                resultadosEventosSinRepeticion.agregarResultado("Bubble Sort", tiempoSinRepeticion);

                copiadatosSinRepeticionEventos = vectorDatosSinrepeticionEventos;
                tiempoSinRepeticion = medirTiempo(insertionSort, copiadatosSinRepeticionEventos, true);
                resultadosEventosSinRepeticion.agregarResultado("Insertion Sort", tiempoSinRepeticion);

                copiadatosSinRepeticionEventos = vectorDatosSinrepeticionEventos;
                tiempoSinRepeticion = medirTiempo(shellSort, copiadatosSinRepeticionEventos, true);
                resultadosEventosSinRepeticion.agregarResultado("Shell Sort", tiempoSinRepeticion);

                copiadatosSinRepeticionEventos = vectorDatosSinrepeticionEventos;
                tiempoSinRepeticion = medirTiempo(mergeSortWrapper, copiadatosSinRepeticionEventos, true);
                resultadosEventosSinRepeticion.agregarResultado("Merge Sort", tiempoSinRepeticion);

                copiadatosSinRepeticionEventos = vectorDatosSinrepeticionEventos;
                tiempoSinRepeticion = medirTiempo(quickSortWrapper, copiadatosSinRepeticionEventos, true);
                resultadosEventosSinRepeticion.agregarResultado("Quick Sort", tiempoSinRepeticion);

                copiadatosSinRepeticionEventos = vectorDatosSinrepeticionEventos;
                tiempoSinRepeticion = medirTiempo(heapSort, copiadatosSinRepeticionEventos, true);
                resultadosEventosSinRepeticion.agregarResultado("Heap Sort", tiempoSinRepeticion);

                resultadosEventosSinRepeticion.mostrarResumen();
                break;


            case 4:
                salir = true;
                cout << "Saliendo del programa..." << endl;
                break;


            default:
                cout << "Opcion no valida. Por favor ingrese una opcion valida." << endl;
                break;
        }
    }

    return 0;
}
