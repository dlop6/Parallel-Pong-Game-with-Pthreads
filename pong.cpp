#include <iostream>
#include <windows.h>  
#include <thread>
#include <chrono>
#include <pthread.h>

using namespace std;

const int WIDTH = 60;   // Ancho del campo
const int HEIGHT = 20;  // Altura del campo

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

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

    void move() {
        // Verifica la colisión con bordes ANTES de actualizar la posición
        detectarColisionBordes();

        // Limpia la posición anterior
        gotoxy(x, y);
        cout << " ";  // Limpia la posición anterior de la bola

        // Actualiza la posición
        x += dx;
        y += dy;

        // Dibuja la nueva posición
        gotoxy(x, y);
        cout << "O";
    }

    void bounce_y() {
        dy = -dy;  // Invierte la dirección vertical
    }

    void bounce_x() {
        dx = -dx;  // Invierte la dirección horizontal
    }

    void reset_position() {
        x = WIDTH / 2;
        y = HEIGHT / 2;
        bounce_x();  // Cambia la dirección horizontal al reiniciar
    }

private:
    void detectarColisionBordes() {
        // Verifica si la bola choca con los bordes superior o inferior antes de actualizar su posición
        if (y + dy <= 0 || y + dy >= HEIGHT - 1) {
            bounce_y();
        }
    }
};

void detectarColisionPaddle(Ball& ball, int paddleX, int paddleY, int paddleWidth) {
    // Verifica si la bola choca con el paddle
    if (ball.y == paddleY && ball.x >= paddleX && ball.x < paddleX + paddleWidth) {
        ball.bounce_x();
    }
}


void* moverBola(void* arg) {
    Ball* ball = (Ball*)arg;

    while (true) {
        ball->move();
        this_thread::sleep_for(chrono::milliseconds(100));  // Control de la velocidad de la bola
    }

    pthread_exit(NULL);
}

void* dibujarPantalla(void* arg) {
    while (true) {
       
        this_thread::sleep_for(chrono::milliseconds(100));  

    pthread_exit(NULL);
}

int main() {
    Ball ball(WIDTH / 2, HEIGHT / 2, 1, 1); // Inicializa la bola en el centro
    int paddleX = 5; // Posición del paddle
    int paddleY = HEIGHT - 2; // Posición del paddle
    int paddleWidth = 5; // Ancho del paddle

    while (true) {
        system("cls");  // Limpia la pantalla
        ball.move();
        detectarColisionPaddle(ball, paddleX, paddleY, paddleWidth);

        this_thread::sleep_for(chrono::milliseconds(100));
    }

    return 0;
}
