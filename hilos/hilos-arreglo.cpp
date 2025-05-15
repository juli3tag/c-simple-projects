#include <iostream>
#include <thread>
#include <chrono>
#include <string>

//  Gonzalez  Julieta 🌷

using namespace std;
void misaludo (string m, int demora, int veces) {
    for (int i = 1; i <= veces; i++) {
        cout << m + "\n";
        this_thread::sleep_for(chrono::milliseconds(demora));
    }
}

int main() {
    thread P[3];
    P[0] = thread(&misaludo, "Hola, soy A", 100, 10);
    P[1] = thread(&misaludo, "\t\t Hola, soy B",150, 15);
    P[2] = thread(&misaludo, "\t\t\t\t Hola, soy C", 300, 5);

    P[0].join();
    P[1].join();
    P[2].join();

    cout << "Fin del programa \n";
}