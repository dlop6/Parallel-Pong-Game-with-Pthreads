
#include <iostream>
#include <windows.h>  
#include <thread>
#include <chrono>
#include <pthread.h>

using namespace std;

const int WIDTH = 70;   // Ancho del campo
const int HEIGHT = 20;  // Altura del campo
const int MAX_SCORE = 10;  // Puntaje máximo para ganar

int PLANE [WIDTH][HEIGHT];
// Convención para el plano
// 0 coordenada vacía
// 1 rebote vertical (techo)
// 2 rebote horizontal (pared)
// 3 anotación izquierda
// 4 anotación derecha
// 5 ping pong

int player1Score = 0;   // Puntaje jugador 1
int player2Score = 0;   // Puntaje jugador 2 (CPU controlado)


pthread_mutex_t consoleMutex;


void displayScore();
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
    system("cls");
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
            PLANE[x][y + i] = 2;
            cout << "p";
        }
    }

    void clearPreviousPosition() {
        for (int i = 0; i < 3; i++) {
            gotoxy(x, y + i);
            PLANE[x][y + i] = 0;
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
        
        if(PLANE[x][y] == 0 || PLANE[x][y] == 5){
            gotoxy(x, y);
            PLANE[x][y] = 5;
            cout << "O";
        }
        
    }

    void clearPreviousPosition() {
        if(PLANE[x][y] == 0 || PLANE[x][y] == 5){
            gotoxy(x, y);
            PLANE[x][y] = 0;
            cout << " ";
        }
        
    }

    void updateBall() {
        
        if (PLANE[x][y] == 1) {
            dy *= -1;
        }

        if(PLANE[x][y] == 2){
            dx *= -1;   
        }
        // Rebote en las paletas

        // Condiciones de puntuación
        if(PLANE[x][y] == 3){
            player2Score++;
            displayScore();
            resetBall();
        }else if(PLANE[x][y] == 4) {
            player1Score++;
            displayScore();
            resetBall();
        }

        clearPreviousPosition();
        x += dx;
        y += dy;
        checkBoundaries();
        // Rebote en la parte superior e inferior
        

    }

    void checkBoundaries(){
        if(x < 0){
            x = 0;
        }
        if(x > WIDTH - 1){
            x = WIDTH - 1;
        }

        if(y < 0){
            y = 0;
        }
        if(y > HEIGHT -1 ){
            y = HEIGHT - 1;
        }
    }

    void resetBall() {
        x = WIDTH / 2;
        y = HEIGHT / 2;
        dx *= -1;
        //dy = (dy == 0) ? -1 : dy;  // evitar que la bola quede en línea recta
    }
};

void configurePlane(){
    for (int i = 0; i < WIDTH; i++) {
        PLANE [i][0] = 1;           // Lógica borde superior
        PLANE [i][HEIGHT -1] = 1;   // Lógica borde inferior
    }

    for (int i = 0; i < HEIGHT; i++) {
        PLANE [0][i] = 3;           // Lógica borde izquierdo
        PLANE [WIDTH-1][i] = 4;     // Lógica borde derecho
    }
}

Ball pongBall(WIDTH / 2, 1, -1, -1);
int getBallY(){
    
    return pongBall.y;
}

void displayScore() {
    gotoxy(0, HEIGHT);
    cout << "Player 1: " << player1Score << " | CPU: " << player2Score << endl;
}

void* playerPaddleThread(void* arg){
    Paddle* pad = (Paddle *)arg;

    pad->drawPaddle();
    while(true){
        if (GetAsyncKeyState(0x57)) {
            pthread_mutex_lock(&consoleMutex);  // 'W' (código ASCII)
            pad->moveUp();  // Mover la paleta del jugador 1 hacia arriba
            pthread_mutex_unlock(&consoleMutex);
        } else if (GetAsyncKeyState(0x53)) {  // 'S' (código ASCII)
            pthread_mutex_lock(&consoleMutex);
            pad->moveDown();  // Mover la paleta del jugador 1 hacia abajo
            pthread_mutex_unlock(&consoleMutex);
        }

        if (GetAsyncKeyState(0x26)) {  // Flecha hacia arriba
            //pthread_mutex_lock(&consoleMutex);
            pad->moveUp();
            //pthread_mutex_unlock(&consoleMutex);
        } else if (GetAsyncKeyState(0x28)) {  // Flecha hacia abajo 
            //pthread_mutex_lock(&consoleMutex);
            pad->moveDown();
            //pthread_mutex_unlock(&consoleMutex);
        }
       std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}


void* aiPaddleThread(void* arg){
    Paddle* pad = (Paddle *)arg;

    int ballY;
    while(true){
        ballY = getBallY();
        pthread_mutex_lock(&consoleMutex);
        pad->CPUmove(ballY);
        pthread_mutex_unlock(&consoleMutex);
        
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}



void* moveBall(void* arg) {
    Ball* ball = (Ball*)arg;
    
    while (true) {
        
        pthread_mutex_lock(&consoleMutex);
        ball->updateBall();
        ball->drawBall();
        pthread_mutex_unlock(&consoleMutex);

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

    return NULL;
}



int main() {
    
    
    Paddle pad1(2, HEIGHT / 2);   // dejar espacio a los bordes
    Paddle pad2(WIDTH - 3, HEIGHT / 2);  // Dejar espacio a los bordes

    pthread_t ballThread;
    pthread_t playe1Thread;
    pthread_t aiThread;
    configurePlane();
    hideCursor();
    drawBoundary();
    displayScore();

    pad1.drawPaddle();
    pad2.drawPaddle();
    pthread_mutex_init(&consoleMutex, NULL);
    pthread_create(&ballThread, NULL, moveBall, &pongBall);
    pthread_create(&playe1Thread, NULL, playerPaddleThread, &pad1);
    pthread_create(&aiThread, NULL, aiPaddleThread, &pad2);


    pthread_join(ballThread, NULL);
    pthread_join(playe1Thread, NULL);
    pthread_join(aiThread, NULL);
    pthread_mutex_destroy(&consoleMutex);
    
    return 0;
}
