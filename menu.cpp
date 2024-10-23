#include <iostream>
#include <windows.h>

using namespace std;

const std::string RESET = "\u001b[0m";
const std::string FOREGROUND_BLACK = "\u001b[30m";
const std::string FOREGROUND_REED = "\u001b[31m";
const std::string FOREGROUND_GREEEN = "\u001b[32m";
const std::string FOREGROUND_YELLOW = "\u001b[33m";
const std::string FOREGROUND_BLUEE = "\u001b[34m";
const std::string FOREGROUND_MAGENTA = "\u001b[35m";
const std::string FOREGROUND_CYAN = "\u001b[36m";
const std::string FOREGROUND_WHITE = "\u001b[37m";
const std::string FOREGROUND_DEFAULT = "\u001b[39m";

int gameMode = 1;  // 1v1 por defecto
int ballSpeed = 100;  // Velocidad media por defecto

// Función para mostrar el menú
void showMenu() {
    int speedOption;

    system("cls");
    cout << FOREGROUND_BLACK << "Selecciona el modo de juego:\n" << RESET;
    cout << FOREGROUND_CYAN <<  "1. 1v1 \n" << RESET;
    cout << FOREGROUND_MAGENTA << "2. 1vCPU \n" << RESET;
    cout << FOREGROUND_BLACK << "Opcion: ";
    cin >> gameMode;

    cout << FOREGROUND_BLACK << "\nSelecciona la velocidad de la pelota:\n" << RESET ;
    cout << FOREGROUND_MAGENTA << "1. Baja\n" << RESET;
    cout << FOREGROUND_CYAN << "2. Media\n" << RESET;
    cout << FOREGROUND_YELLOW << "3. Alta\n" << RESET;
    cout << FOREGROUND_BLACK <<  "Opcion: " << RESET;
    cin >> speedOption;

    // Asignamos la velocidad según la selección
    switch (speedOption) {
        case 1: ballSpeed = 200; break; // Baja
        case 2: ballSpeed = 100; break; // Media (default)
        case 3: ballSpeed = 50; break;  // Alta
        default: ballSpeed = 100; break; // Si la opción es inválida, se usa la media
    }

    switch (gameMode) {
        case 1: gameMode = 1; break; // 1v1
        case 2: gameMode = 2; break; // 1vCPU
        default: gameMode = 1; break; // Si la opción es inválida, se usa la media
    }

    // Imprimir la selección para verificar que funciona correctamente
    cout << "\nModo de juego seleccionado: " << (gameMode == 1 ? "1v1" : "1vCPU") << endl;
    cout << "Velocidad de la pelota seleccionada: ";
    if (ballSpeed == 200) cout << "Baja" << endl;
    else if (ballSpeed == 100) cout << "Media" << endl;
    else if (ballSpeed == 50) cout << "Alta" << endl;
}
