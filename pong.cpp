#include <iostream>
#include <windows.h>  
#include <thread>
#include <chrono>
#include <pthread.h>

using namespace std;

const int WIDTH = 50;   // Ancho del campo
const int HEIGHT = 10;  // Altura del campo

void gotoxy(int x, int y) {
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

    void drawPaddle(){
        for (int i = 0; i < 3; i++) {
            gotoxy(x, y + i);
            cout << "||";
        }
    }

    void clearPreviousPosition(){
        for (int i = 0; i < 3; i++){
            gotoxy(x, y + i);
            cout << "  ";
        }
    }

    void updateUP(){
        if (y > 0) {
            clearPreviousPosition();
            y--;
            drawPaddle();
        }
    }

    void updateDOWN(){
        if (y + 3 < HEIGHT) {
            clearPreviousPosition();
            y++;
            drawPaddle();
        }
    }

    void CPUmove(int ballY){
        if (y + 1 < ballY){
            updateDOWN();
        } else if (y > ballY){
            updateUP();
        }
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
        gotoxy(x, y);
        cout << " ";
    }

    void updateBall(){
        clearPreviousPosition(); // Borrar la bola en la posición anterior
        x += dx;
        y += dy;

        if (y <= 0 || y >= HEIGHT - 1) {
            dy *= -1;  // Rebote vertical
        }
       
    }

    void checkCollisionwithPad(Paddle pad){
        if (dx < 0 && x == pad.x + 2 && (y >= pad.y && y <= pad.y + 2)) {
            dx *= -1; 
        } else if (dx > 0 && x == pad.x - 1 && (y >= pad.y && y <= pad.y + 2)) {
            dx *= -1; 
        }
    }
};

int main() {
    Ball ball(WIDTH / 2, HEIGHT / 2, 1, 1);  
    Paddle paddle1(5, HEIGHT / 2 - 1);  
    Paddle paddle2(WIDTH - 5, HEIGHT / 2 - 1);  

    hideCursor();  // Oculta el cursor

    while (true) {
        system("cls");  
        // Dibuja los paddles
        paddle1.drawPaddle();
        paddle2.drawPaddle();

        // Detección de colisiones con paddles
        ball.checkCollisionwithPad(paddle1);
        ball.checkCollisionwithPad(paddle2);

        // Mueve la bola y el paddle de la CPU
        ball.updateBall();
        paddle1.CPUmove(ball.y);
        

        // Dibuja la bola actualizada
        ball.drawBall();

        // Controla la velocidad del juego
        this_thread::sleep_for(chrono::milliseconds(50));
    }

    showCursor();  // Muestra el cursor al final (aunque no se alcanza en este bucle)

    return 0;
}
