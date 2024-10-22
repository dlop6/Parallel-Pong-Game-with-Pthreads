
#include <iostream>
#include <windows.h>  
#include <thread>
#include <chrono>
#include <pthread.h>

using namespace std;

const int WIDTH = 50;   // Ancho del campo
const int HEIGHT = 10;  // Altura del campo
const int MAX_SCORE = 10;  // Puntaje máximo para ganar

int player1Score = 0;   // Puntaje jugador 1
int player2Score = 0;   // Puntaje jugador 2 (CPU controlado)

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

void drawBoundary() {
    for (int i = 0; i < WIDTH; i++) {
        gotoxy(i, 0);
        cout << "#";  // Dibujar borde superior
        gotoxy(i, HEIGHT - 1);
        cout << "#";  // Dibujar borde inferior
    }

    for (int i = 0; i < HEIGHT; i++) {
        gotoxy(0, i);
        cout << "#";  // Borde izquierdo
        gotoxy(WIDTH - 1, i);
        cout << "#";  // Borde derecho
    }

    for (int i = 0; i < HEIGHT; i++) {
        gotoxy(WIDTH / 2, i);
        cout << "|";  // Línea central
    }
}

class Paddle {
public:
    int x, y;  // Posición de la paleta
    Paddle(int startX, int startY) {
        x = startX;
        y = startY;
    }

    void drawPaddle() {
        for (int i = 0; i < 3; i++) {
            gotoxy(x, y + i);
            cout << "||";
        }
    }

    void clearPreviousPosition() {
        for (int i = 0; i < 3; i++) {
            gotoxy(x, y + i);
            cout << "  ";
        }
    }

    void moveUp() {
        if (y > 1) {  // Ajuste para evitar pasar del borde superior
            clearPreviousPosition();
            y--;
            drawPaddle();
        }
    }

    void moveDown() {
        if (y + 3 < HEIGHT - 1) {  // Ajuste para evitar pasar del borde inferior
            clearPreviousPosition();
            y++;
            drawPaddle();
        }
    }

    void CPUmove(int ballY) {
        if (y + 1 < ballY) {
            moveDown();
        } else if (y > ballY) {
            moveUp();
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

    void drawBall() {
        gotoxy(x, y);
        cout << "O";
    }

    void clearPreviousPosition() {
        gotoxy(x, y);
        cout << " ";
    }

    void updateBall(Paddle &pad1, Paddle &pad2) {
        clearPreviousPosition();
        x += dx;
        y += dy;

        // Rebote en la parte superior e inferior
        if (y <= 1 || y >= HEIGHT - 2) {
            dy *= -1;
        }

        // Rebote en las paletas
        if (x == pad1.x && (y >= pad1.y && y <= pad1.y + 2)) {
            dx *= -1;
        } else if (x == pad2.x && (y >= pad2.y && y <= pad2.y + 2)) {
            dx *= -1;
        }

        // Condiciones de puntuación
        if (x <= 1) {
            player2Score++;
            resetBall();
        } else if (x >= WIDTH - 2) {
            player1Score++;
            resetBall();
        }
    }

    void resetBall() {
        x = WIDTH / 2;
        y = HEIGHT / 2;
        dx *= -1;
        dy = (dy == 0) ? 1 : dy;  // evitar que la bola quede en línea recta
    }
};

void displayScore() {
    gotoxy(0, HEIGHT);
    cout << "Player 1: " << player1Score << " | CPU: " << player2Score << endl;
}

void* moveBall(void* arg) {
    Ball* ball = (Ball*)arg;
    Paddle pad1(2, HEIGHT / 2);   // dejar espacio a los bordes
    Paddle pad2(WIDTH - 3, HEIGHT / 2);  // Dejar espacio a los bordes

    pad1.drawPaddle();
    pad2.drawPaddle();

    while (true) {
        ball->updateBall(pad1, pad2);
        ball->drawBall();
        displayScore();
        drawBoundary();

        pad2.CPUmove(ball->y);  // Mover la paleta de la CPU

        // Comprobar si algún jugador ha ganado
        if (player1Score >= MAX_SCORE) {
            gotoxy(WIDTH / 2 - 5, HEIGHT / 2);
            cout << "Player 1 Wins!";
            break;
        } else if (player2Score >= MAX_SCORE) {
            gotoxy(WIDTH / 2 - 5, HEIGHT / 2);
            cout << "CPU Wins!";
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    Ball ball(WIDTH / 2, HEIGHT / 2, 1, 1);
    hideCursor();

    pthread_t ballThread;
    pthread_create(&ballThread, NULL, moveBall, &ball);

    Paddle pad1(2, HEIGHT / 2);

    while (true) {
        if (GetAsyncKeyState(0x57)) {  // 'W' (código ASCII)
            pad1.moveUp();  // Mover la paleta del jugador 1 hacia arriba
        } else if (GetAsyncKeyState(0x53)) {  // 'S' (código ASCII)
            pad1.moveDown();  // Mover la paleta del jugador 1 hacia abajo
        }

        if (GetAsyncKeyState(0x26)) {  // Flecha hacia arriba
            pad1.moveUp();
        } else if (GetAsyncKeyState(0x28)) {  // Flecha hacia abajo 
            pad1.moveDown();
        }
    }

    pthread_join(ballThread, NULL);
    return 0;
}
