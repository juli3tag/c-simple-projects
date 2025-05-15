#include <iostream>     // Para usar cout y cin
#include <unistd.h>     // Para usar fork y sleep
#include <sys/wait.h>   // Para usar waitpid
#include <random>       // Para usar random_device y mt19937
#include <vector>       // Para usar vector
#include <cstdlib>      // Para usar exit()

/*  Gonzalez  Julieta 🌷

    Descripción: Simulador de concurrencia con procesos.
    Este programa crea dos tipos de procesos: uno que incrementa una variable local
    y otro que lee el valor de esa variable local. Los procesos se ejecutan sin
    compartir memoria y se espera a que todos los procesos terminen antes de finalizar el programa.
    El programa también incluye una función para generar esperas aleatorias entre 0 y 2 segundos
*/

using namespace std;

// Espera aleatoria entre 0 y 2 segundos
void esperarAleatorio() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, 2);
    sleep(dist(gen));
}

int main() {
    int N, M;

    cout << "\n  ---  Simulador de Concurrencia con Procesos  ---   " << endl;
    cout << " - Proceso tipo 1: duerme, incrementa variable." << endl;
    cout << " - Proceso tipo 2: duerme, lee e imprime la variable." << endl;
    cout << "=============================================" << endl;

    cout << "Ingrese la cantidad de procesos tipo 1 (que incrementan la variable): ";
    if (!(cin >> N) || N < 0) {
        cerr << "Entrada invalida. Debe ingresar un numero entero no negativo." << endl;
        return 1;
    }

    cout << "Ingrese la cantidad de procesos tipo 2 (que leen la variable): ";
    if (!(cin >> M) || M < 0) {
        cerr << "Entrada invalida. Debe ingresar un numero entero no negativo." << endl;
        return 1;
    }

    vector<pid_t> hijos;
    int valorCompartido = 0; // Variable local que cada proceso puede leer/modificar

    // Procesos tipo 1
    for (int i = 0; i < N; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            cout << "Instancia " << i << " del proceso 1" << endl;
            esperarAleatorio();
            valorCompartido++;
            cout << "Proceso 1 (" << i << ") incrementa la variable a " << valorCompartido << endl;
            exit(0);
        } else if (pid > 0) {
            hijos.push_back(pid);
        } else {
            cerr << "Error al crear el proceso tipo 1" << endl;
            return 1;
        }
    }

    // Procesos tipo 2
    for (int i = 0; i < M; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            cout << "Instancia " << i << " del proceso 2" << endl;
            esperarAleatorio();
            cout << "Proceso 2 (" << i << ") lee el valor de la variable: " << valorCompartido << endl;
            exit(0);
        } else if (pid > 0) {
            hijos.push_back(pid);
        } else {
            cerr << "Error al crear el proceso tipo 2" << endl;
            return 1;
        }
    }

    // Esperar todos los procesos
    for (pid_t pid : hijos) {
        if (waitpid(pid, NULL, 0) == -1) {
            cerr << "Error al esperar al proceso con PID " << pid << endl;
        }
    }

    cout << "=============================================" << endl;
    cout << "\n === Todos los procesos han terminado su ejecucion ===" << endl;
    cout << "Valor final de la variable: " << valorCompartido << endl;
    cout << "=============================================" << endl;

    return 0;
}

// Fin del programa
