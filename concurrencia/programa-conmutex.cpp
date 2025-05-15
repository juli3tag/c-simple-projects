#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <cstdlib>

/*  Gonzalez  Julieta 🌷
    
        Consigna: Crear un programa que genere 10 hilos, cada uno de los cuales imprime un mensaje determinado varias veces 
        con un retardo aleatorio entre 100 y 1000 ms. El mensaje y el retardo son parámetros de la función que ejecuta el hilo. 
        El número de repeticiones del mensaje también es aleatorio, entre 1 y 5. 
        Se aplica un mutex para evitar que los mensajes se mezclen en la salida.
        Se utiliza un generador de números aleatorios para determinar el retardo y el número de repeticiones.
*/

using namespace std;

// Mutex para proteger la sección crítica
mutex mtx;

// Clase Proceso que representa un hilo de ejecución
class Proceso {
private:
    int id; // Identificador del proceso

public:
    Proceso(int id_proceso) {
        id = id_proceso; // Inicializa el identificador del proceso
    }

    // Método que simula la ejecución del proceso
    // Imprime un mensaje con un retardo aleatorio y una cantidad aleatoria de repeticiones
    void ejecutar() {
        // Tiempo de espera aleatorio entre 100 y 1000 ms
        int espera = 100 + rand() % 901;

        // Cantidad aleatoria de repeticiones entre 1 y 5
        int repeticiones = 1 + rand() % 5;

        for (int i = 0; i < repeticiones; i++) {
            this_thread::sleep_for(chrono::milliseconds(espera)); 

            // Sección crítica protegida
            {
                lock_guard<mutex> lock(mtx); // Bloqueo del mutex
                cout << "Proceso " << id << ": mensaje " << i + 1 << endl;
            }
        }
    }
};

int main() {
    srand(time(0));
    vector<thread> hilos; // Vector para almacenar los hilos
    vector<Proceso> procesos; // Vector para almacenar los objetos Proceso

    // Crear 10 procesos e instanciarlos
    for (int i = 1; i <= 10; i++) {
        procesos.emplace_back(i); // Agregar un nuevo objeto Proceso al vector
    }

    // Lanzar hilos ejecutando el método de cada objeto
    for (int i = 0; i < 10; i++) {
        hilos.emplace_back(&Proceso::ejecutar, &procesos[i]); // Crear un hilo para cada proceso
    }

    // Esperar que todos los hilos terminen
    for (auto& h : hilos) {
        h.join();
    }

    return 0;
}
