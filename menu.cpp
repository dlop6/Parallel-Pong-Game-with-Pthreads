#include <iostream>

using namespace std;

int gameMode = 1;  // 1v1 por defecto
int ballSpeed = 100;  // Velocidad media por defecto

// Función para mostrar el menú
void showMenu() {
    int speedOption;

    cout << "Selecciona el modo de juego:\n";
    cout << "1. 1v1 \n";
    cout << "2. 1vCPU \n";
    cout << "Opcion: ";
    cin >> gameMode;

    cout << "\nSelecciona la velocidad de la pelota:\n";
    cout << "1. Baja\n";
    cout << "2. Media\n";
    cout << "3. Alta\n";
    cout << "Opcion: ";
    cin >> speedOption;

    // Asignamos la velocidad según la selección
    switch (speedOption) {
        case 1: ballSpeed = 200; break; // Baja
        case 2: ballSpeed = 100; break; // Media (default)
        case 3: ballSpeed = 50; break;  // Alta
        default: ballSpeed = 100; break; // Si la opción es inválida, se usa la media
    }

    // Imprimir la selección para verificar que funciona correctamente
    cout << "\nModo de juego seleccionado: " << (gameMode == 1 ? "1v1" : "1vCPU") << endl;
    cout << "Velocidad de la pelota seleccionada: ";
    if (ballSpeed == 200) cout << "Baja" << endl;
    else if (ballSpeed == 100) cout << "Media" << endl;
    else if (ballSpeed == 50) cout << "Alta" << endl;
}
