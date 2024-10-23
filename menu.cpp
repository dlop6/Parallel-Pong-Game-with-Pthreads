#include <iostream>

using namespace std;

int gameMode = 1;  // 1v1 por defecto
int ballSpeed = 100;  // Velocidad media por defecto

// Función para mostrar el menú
void showMenu() {
    int speedOption;

    // Validar la selección del modo de juego
    do {
        cout << "Selecciona el modo de juego:\n";
        cout << "1. 1v1 \n";
        cout << "2. 1vCPU \n";
        cout << "Opcion: ";
        cin >> gameMode;

        if (gameMode != 1 && gameMode != 2) {
            cout << "Opción inválida. Por favor selecciona 1 o 2.\n";
        }
    } while (gameMode != 1 && gameMode != 2);

    // Validar la selección de la velocidad
    do {
        cout << "\nSelecciona la velocidad de la pelota:\n";
        cout << "1. Baja\n";
        cout << "2. Media\n";
        cout << "3. Alta\n";
        cout << "Opcion: ";
        cin >> speedOption;

        if (speedOption < 1 || speedOption > 3) {
            cout << "Opción inválida. Por favor selecciona entre 1 y 3.\n";
        }
    } while (speedOption < 1 || speedOption > 3);

    // Asignamos la velocidad según la selección
    switch (speedOption) {
        case 1: ballSpeed = 200; break; // Baja
        case 2: ballSpeed = 100; break; // Media (default)
        case 3: ballSpeed = 50; break;  // Alta
    }

    // Imprimir la selección para verificar que funciona correctamente
    cout << "\nModo de juego seleccionado: " << (gameMode == 1 ? "1v1" : "1vCPU") << endl;
    cout << "Velocidad de la pelota seleccionada: ";
    if (ballSpeed == 200) cout << "Baja" << endl;
    else if (ballSpeed == 100) cout << "Media" << endl;
    else if (ballSpeed == 50) cout << "Alta" << endl;
}

