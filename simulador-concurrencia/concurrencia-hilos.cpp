#include <iostream>
#include <pthread.h>
#include <unistd.h> // usleep
#include <random>
#include <vector>
#include <stdexcept>
#include <cstdlib>

using namespace std;

/*  Gonzalez  Julieta 🌷

    Descripción: Simulador de concurrencia con hilos.
    Este programa crea dos tipos de hilos: uno que incrementa una variable compartida
    y otro que lee el valor de esa variable. Se utiliza un mutex para proteger
    la sección crítica y evitar condiciones de carrera. Los hilos se ejecutan
    de manera concurrente y se espera a que todos terminen antes de finalizar el programa.
    El programa también incluye una función para generar esperas aleatorias entre 0 y 2 segundos
*/

int compartida = 0;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

// Estructura para pasar argumentos a los hilos
struct ArgumentoHilo {
    int id;
};

// Función de espera aleatoria entre 0 y 2 segundos
void esperarAleatorio() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, 2000);
    usleep(dist(gen) * 1000); // convertir a microsegundos
}

// Funciones que ejecutarán los hilos
void* hiloTipo1(void* arg) {
    ArgumentoHilo* datos = (ArgumentoHilo*)arg;

    pthread_mutex_lock(&mtx);
    cout << "Instancia " << datos->id << " del hilo 1" << endl;
    pthread_mutex_unlock(&mtx);

    esperarAleatorio();

    pthread_mutex_lock(&mtx);
    compartida++;
    pthread_mutex_unlock(&mtx);

    pthread_exit(NULL);
}

void* hiloTipo2(void* arg) {
    ArgumentoHilo* datos = (ArgumentoHilo*)arg;

    pthread_mutex_lock(&mtx);
    cout << "Instancia " << datos->id << " del hilo 2" << endl;
    pthread_mutex_unlock(&mtx);

    esperarAleatorio();

    pthread_mutex_lock(&mtx);
    int valorLeido = compartida;
    cout << "Hilo 2 (" << datos->id << ") lee el valor de 'compartida': " << valorLeido << endl;
    pthread_mutex_unlock(&mtx);

    pthread_exit(NULL);
}

// Leer entero con validación
int leerEnteroPositivo(const string& mensaje) {
    int valor;
    cout << mensaje;
    cin >> valor;

    if (cin.fail() || valor < 0) {
        throw invalid_argument("Debe ingresar un número entero no negativo.");
    }

    return valor;
}

int main() {
    int N, M;

    cout << "\n  ---  Simulador de Concurrencia con pthreads  ---   " << endl;
    cout << "Este programa crea hilos tipo 1 y tipo 2:" << endl;
    cout << " - Hilo tipo 1: duerme, incrementa variable compartida." << endl;
    cout << " - Hilo tipo 2: duerme, lee e imprime la variable compartida." << endl;
    cout << "=============================================\n" << endl;

    try {
        N = leerEnteroPositivo("Ingrese la cantidad de hilos tipo 1: ");
        M = leerEnteroPositivo("Ingrese la cantidad de hilos tipo 2: ");
    } catch (const exception& e) {
        cerr << "Se produjo un error: " << e.what() << endl;
        return 1;
    }

    // Vectores de hilos y argumentos
    vector<pthread_t> hilosTipo1(N);
    vector<pthread_t> hilosTipo2(M);
    vector<ArgumentoHilo> argumentosTipo1(N);
    vector<ArgumentoHilo> argumentosTipo2(M);

    // Crear hilos tipo 1
    for (int i = 0; i < N; ++i) {
        argumentosTipo1[i].id = i;
        pthread_create(&hilosTipo1[i], NULL, hiloTipo1, &argumentosTipo1[i]);
    }

    // Crear hilos tipo 2
    for (int i = 0; i < M; ++i) {
        argumentosTipo2[i].id = i;
        pthread_create(&hilosTipo2[i], NULL, hiloTipo2, &argumentosTipo2[i]);
    }

    // Esperar que todos los hilos terminen
    for (int i = 0; i < N; ++i) {
        pthread_join(hilosTipo1[i], NULL);
    }

    for (int i = 0; i < M; ++i) {
        pthread_join(hilosTipo2[i], NULL);
    }

    // Mostrar valor final de la variable compartida
    pthread_mutex_lock(&mtx);
    cout << "=============================================" << endl;
    cout << "\nSe ha finalizado la ejecución" << endl;
    cout << "Valor final de la variable compartida: " << compartida << endl;
    cout << "=============================================" << endl;
    pthread_mutex_unlock(&mtx);

    return 0;
}
