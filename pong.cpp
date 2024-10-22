#include <iostream>
#include <windows.h>  
#include <thread>
#include <chrono>
#include <pthread.h>

using namespace std;

const int WIDTH = 50;   // Ancho del campo
const int HEIGHT = 10;  // Altura del campo

void gotoxy(int x, int y) {
    system("cls");
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void showCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

class Paddle {

    public:
    int x, y;

    Paddle(int startX, int startY){
        x = startX;
        y = startY;
    }

    void funcionesPaddle(){
        return;
    }


};


class Ball {
public:
    int x, y;  // Posición de la bola
    int dx, dy;  // Dirección de la bola

    Ball(int startX, int startY, int startDX, int startDY) {
        x = startX;
        y = startY;
        dx = startDX;
        dy = startDY;
    }

    void drawBall(){
        gotoxy(x, y);
        cout << "O";
    }

    void clearPreviousPosition(){
        gotoxy(x, y); // Borra la posición anterior
        cout << " ";
    }

    void updateBall(){
        clearPreviousPosition(); // Borrar la bola en la posición anterior
        x += dx;
        y += dy;

        if (y <= 0 || y >= HEIGHT - 1) {
            dy *= -1;  // Rebote vertical
        }
        if (x <= 0 || x >= WIDTH - 1) {
           // TO DO: IMPLEMENTAR LÓGICA DE PUNTO A LA HORA DE TOCAR LOS BORDES HORIZONTALES
        }
    }

    void checkCollisionwithPad(Paddle pad){
        
        if (x == pad.x && y == pad.y){
            dx *= -1;
        }

    }
};




void* moverBola(void* arg) {
    Ball* ball = (Ball*)arg;

    while (true) {
        ball->drawBall();
        this_thread::sleep_for(chrono::milliseconds(100));  // Control de la velocidad de la bola
    }

    pthread_exit(NULL);
}

void* dibujarPantalla(void* arg) {
    while (true) {
        // TO DO: DIBUJAR EL CAMPO DE JUEGO
       
        this_thread::sleep_for(chrono::milliseconds(100));  

    pthread_exit(NULL);
}
}

int main() {
    Ball ball(WIDTH / 2, HEIGHT / 2, 1, 1); // Inicializa la bola en el centro
    int paddleX = 5; // Posición del paddle
    int paddleY = HEIGHT - 2; // Posición del paddle
    int paddleWidth = 5; // Ancho del paddle


    hideCursor();  // Oculta el cursor
    while (true) {
        ball.updateBall();  // Actualiza la posición de la bola
        ball.drawBall();  // Dibuja la bola
       

        this_thread::sleep_for(chrono::milliseconds(50));
    }
    showCursor();  // Muestra el cursor

    return 0;
}
