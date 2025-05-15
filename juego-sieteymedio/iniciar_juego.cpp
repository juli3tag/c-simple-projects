// iniciar_juego.cpp
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstring> 
#include <cerrno>  

/*  Gonzalez  Julieta 🌷

    Descripción: Juego de cartas "7 y medio" en C++.
    El programa crea un proceso padre que se encarga de gestionar el juego y varios procesos hijos que representan a los jugadores.
    Cada jugador recibe cartas y toma decisiones (pedir carta, plantarse o abandonar) hasta que se planta o se pasa de 7.5 puntos.
    El proceso padre envía cartas a los jugadores y recibe sus decisiones. Al final, se determina el ganador o si hubo abandono.   
    */

using namespace std;

// Definimos las cartas posibles
const int num_cartas = 8;
float cartas[num_cartas] = {1, 2, 3, 4, 5, 6, 7, 0.5};

// Estructura para guardar información del jugador
struct Jugador {
    float puntos = 0;
    bool plantado = false;
    bool abandono = false;
    pid_t pid = 0; // PID del proceso jugador
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " N_Jugadores" << endl;
        return 1;
    }

    srand(time(NULL));
    int N = atoi(argv[1]);
    if (N <= 0) {
        cerr << "Error: El número de jugadores debe ser mayor que 0." << endl;
        return 1;
    }

    vector<Jugador> jugadores(N);

    int pipes_padre_a_hijo[N][2];
    int pipes_hijo_a_padre[N][2];

    // Crear procesos jugadores y pipes
    for (int i = 0; i < N; i++) {
        if (pipe(pipes_padre_a_hijo[i]) == -1) {
            perror("Error creando pipe padre a hijo");
            exit(EXIT_FAILURE);
        }
        if (pipe(pipes_hijo_a_padre[i]) == -1) {
            perror("Error creando pipe hijo a padre");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();
        if (pid == -1) {
            perror("Error al hacer fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // PROCESO HIJO - Jugador
            close(pipes_padre_a_hijo[i][1]); // Cierra escritura padre->hijo
            close(pipes_hijo_a_padre[i][0]); // Cierra lectura hijo->padre

            float puntos = 0;
            bool activo = true;

            while (activo) {
                float carta;
                ssize_t leido = read(pipes_padre_a_hijo[i][0], &carta, sizeof(float));
                if (leido <= 0) {
                    perror("Error leyendo carta");
                    break;
                }

                if (carta == -1) {
                    break; // Fin del juego
                }

                puntos += carta;

                int decision = 0; // 0: pedir carta, 1: plantarse, 2: abandonar
                if (puntos > 7.5) {
                    decision = 2; // Se pasa, abandona
                } else {
                    int probabilidad = rand() % 100;
                    if (probabilidad < 50)
                        decision = 0; // Sigue
                    else
                        decision = 1; // Se planta
                }

                if (write(pipes_hijo_a_padre[i][1], &decision, sizeof(int)) == -1) {
                    perror("Error escribiendo decision");
                    break;
                }

                if (decision != 0) {
                    activo = false;
                }
            }

            close(pipes_padre_a_hijo[i][0]);
            close(pipes_hijo_a_padre[i][1]);
            exit(0);
        } else {
            // PROCESO PADRE
            jugadores[i].pid = pid;
            close(pipes_padre_a_hijo[i][0]); // Cierra lectura padre->hijo
            close(pipes_hijo_a_padre[i][1]); // Cierra escritura hijo->padre
        }
    }

    // Lógica de partida en el servidor
    bool jugadores_activos = true;

    while (jugadores_activos) {
        jugadores_activos = false;

        for (int i = 0; i < N; i++) {
            if (!jugadores[i].plantado && !jugadores[i].abandono) {
                jugadores_activos = true;

                float carta = cartas[rand() % num_cartas];
                if (write(pipes_padre_a_hijo[i][1], &carta, sizeof(float)) == -1) {
                    perror("Error enviando carta al jugador");
                    continue;
                }
                jugadores[i].puntos += carta;

                int decision;
                ssize_t recibido = read(pipes_hijo_a_padre[i][0], &decision, sizeof(int));
                if (recibido <= 0) {
                    perror("Error leyendo decision del jugador");
                    continue;
                }

                if (decision == 1) {
                    jugadores[i].plantado = true;
                } else if (decision == 2) {
                    jugadores[i].abandono = true;
                }
            }
        }
    }

    // Mandar fin del juego a todos
    for (int i = 0; i < N; i++) {
        float fin = -1;
        write(pipes_padre_a_hijo[i][1], &fin, sizeof(float));
    }

    // Esperar a que terminen los procesos hijos
    for (int i = 0; i < N; i++) {
        wait(NULL);
    }

    // Mostrar resultados
    cout << "\n=== RESULTADOS FINALES ===\n";
    cout << "Jugador\tPuntos\tEstado\n";
    cout << "----------------------------\n";

    int ganador = -1;
    float mejor_puntaje = 0.0;

    for (int i = 0; i < N; i++) {
        cout << i + 1 << "\t" << jugadores[i].puntos << "\t";
        if (jugadores[i].abandono) {
            cout << "Abandonó";
        } else if (jugadores[i].plantado) {
            cout << "Plantado";
        } else {
            cout << "Activo";
        }
        cout << endl;

        if (!jugadores[i].abandono && jugadores[i].puntos <= 7.5) {
            if (jugadores[i].puntos > mejor_puntaje) {
                mejor_puntaje = jugadores[i].puntos;
                ganador = i;
            }
        }
    }

    if (ganador != -1) {
        cout << "\n🏆 El ganador es el Jugador " << ganador + 1 << " con " << mejor_puntaje << " puntos! 🏆\n";
    } else {
        cout << "\nNo hubo ganador.\n";
    }

    return 0;
}
