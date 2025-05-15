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
    thread  th_1(&misaludo, "Hola, soy A", 100, 10),
            th_2(&misaludo, "\t\tHola, soy B",150, 15),
            th_3(&misaludo, "\t\t\t\tHola, soy C", 300, 5);

    th_1.join();
    th_2.join();    
    th_3.join();
    cout << "Fin del programa \n";
    return 0; 

}