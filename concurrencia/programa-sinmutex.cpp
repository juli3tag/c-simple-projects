#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <random>

/*  Gonzalez  Julieta 🌷

    Consigna: Crear un programa que genere 15 hilos, cada uno de los cuales imprime un mensaje determinado varias veces 
    con un retardo aleatorio entre 100 y 500 ms. El mensaje y el retardo son parámetros de la función que ejecuta el hilo. 
    El número de repeticiones del mensaje también es aleatorio, entre 5 y 15. 
    Se utiliza un generador de números aleatorios para determinar el retardo y el número de repeticiones.
    */
    

using namespace std;

// Funcion que imprime un mensaje determinado varias veces con un retardo
void misaludo(int id, int demora, int veces) {
    for (int i = 1; i <= veces; i++) {
        cout << "Soy el proceso " << id << endl;
        this_thread::sleep_for(chrono::milliseconds(demora));
    }
}

int main() {
    const int N = 15;
    thread procesos[N];

    // Generadores aleatorios
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist_tiempo(100, 500); // Milisegundos
    uniform_int_distribution<> dist_repeticiones(5, 15);

    // Crear e iniciar los hilos
    for (int i = 0; i < N; ++i) {
        int demora = dist_tiempo(gen);
        int repeticiones = dist_repeticiones(gen);
        procesos[i] = thread(&misaludo, i + 1, demora, repeticiones);
    }

    // Esperar que terminen todos los hilos
    for (int i = 0; i < N; ++i) {
        procesos[i].join();
    }

    cout << "Fin del programa" << endl;
    return 0;
}
