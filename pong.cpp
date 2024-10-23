
#include <iostream>
#include <windows.h> 
#include <mmsystem.h> 
#include <thread>
#include <chrono>
#include <pthread.h>
#include <string>
#include "menu.cpp"
  

using namespace std;


//Colores

const int LONG_COLOR_CYCLES = 5;
const int SHORT_COLOR_CYCLES = 2;
//Final colores

const int WIDTH = 66;   // Ancho del campo
const int HEIGHT = 15;  // Altura del campo
const int MAX_SCORE = 3;  // Puntaje máximo para ganar

bool playAgain = true;
bool continueGame = true;
bool colorLeftPaddle;
bool colorRightPaddle;


int PLANE [WIDTH][HEIGHT];
const int PLANE_EMPTY = 0;
const int PLANE_VERTICAL = 1;
const int PADDLE1 = 2;
const int PADDLE2 = 3;
const int LEFTSCORE = 4;
const int RIGHTSCORE = 5;
const int BALL = 6;

int colorPaddle1 = 0;
int colorPaddle2 = 0;
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
    
    SetConsoleOutputCP(65001);
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
    

    for (int i = 0; i < HEIGHT; i++) {
        gotoxy(0, i);
        cout << FOREGROUND_BLUEE <<"[" << RESET;  // Borde izquierdo
        gotoxy(WIDTH - 1, i);
        cout << FOREGROUND_REED << "]" << RESET;  // Borde derecho
    }

    for (int i = 0; i < WIDTH; i++) {
        gotoxy(i, 0);
        cout << FOREGROUND_GREEEN <<"-" << RESET;  // Dibujar borde superior
        gotoxy(i, HEIGHT - 1);
        cout << FOREGROUND_GREEEN << "-" << RESET;  // Dibujar borde inferior
    }

    for (int i = 0; i < HEIGHT; i++) {
        gotoxy(WIDTH / 2, i);
        cout << FOREGROUND_GREEEN << "|" << RESET;  // Línea central
    }
}


class Paddle {
public: 
    int x, y, paddleCode;  // Posición de la paleta
    string paddleColor;

    Paddle(int startX, int startY, int code, string color) {
        x = startX;
        y = startY;
        paddleCode = code;
        paddleColor = color;
    }



    void drawPaddle() {
        for (int i = 0; i < 3; i++) {
            gotoxy(x, y + i);
            PLANE[x][y + i] = paddleCode;
            cout << "I";
        }
    }

    void drawPaddleWithColor(){
        for (int i = 0; i < 3; i++) {
            gotoxy(x, y + i);
            PLANE[x][y + i] = paddleCode;
            cout << paddleColor <<  "I" << RESET;
        }
    }

    void clearPreviousPosition() {
        for (int i = 0; i < 3; i++) {
            gotoxy(x, y + i);
            PLANE[x][y + i] = PLANE_EMPTY;
            cout << "  ";
        }
    }

    void moveUp() {
        if (y > 1) {  // Ajuste para evitar pasar del borde superior
            clearPreviousPosition();
            y--;
        }
    }

    void moveDown() {
        if (y + 3 < HEIGHT - 1) {  // Ajuste para evitar pasar del borde inferior
            clearPreviousPosition();
            y++;
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
        
        if(PLANE[x][y] == PLANE_EMPTY || PLANE[x][y] == BALL){
            gotoxy(x, y);
            PLANE[x][y] = BALL;
            cout << "o";
        }
        
    }

    void clearPreviousPosition() {
        if(PLANE[x][y] == PLANE_EMPTY || PLANE[x][y] == BALL){
            gotoxy(x, y);
            PLANE[x][y] = PLANE_EMPTY;
            cout << " ";
        }
        
    }

    void updateBall() {
        
        

        if(PLANE[x][y] == PADDLE1){
            PlaySound(TEXT("wall.wav"), NULL, SND_ASYNC);
            colorPaddle1 = LONG_COLOR_CYCLES;
            dx *= -1;   
        }
        else if(PLANE[x][y] == PADDLE2){
            PlaySound(TEXT("paddle.wav"), NULL, SND_ASYNC);
            colorPaddle2 = LONG_COLOR_CYCLES;
            dx *= -1;   
        }
        else if (PLANE[x][y] == PLANE_VERTICAL) {
            PlaySound(TEXT("paddle.wav"), NULL,  SND_ASYNC);
            dy *= -1;
        }
        // Rebote en las paletas

        // Condiciones de puntuación
        if(PLANE[x][y] == LEFTSCORE){
            player2Score++;
            displayScore();
            resetBall();
        }else if(PLANE[x][y] == RIGHTSCORE) {
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
        PLANE [i][0] = PLANE_VERTICAL;           // Lógica borde superior
        PLANE [i][HEIGHT -1] = PLANE_VERTICAL;   // Lógica borde inferior
    }

    for (int i = 0; i < HEIGHT; i++) {
        PLANE [0][i] = LEFTSCORE;           // Lógica borde izquierdo
        PLANE [WIDTH-1][i] = RIGHTSCORE;     // Lógica borde derecho
    }
}

Ball pongBall(WIDTH / 2, 1, -1, -1);
int getBallY(){
    
    return pongBall.y;
}

void displayScore() {
    gotoxy(WIDTH / 2 - 12, HEIGHT);
    string current;
    if(gameMode == 1){
        current = "Player 2: ";
    }else{
        current = "CPU: ";
    }
    cout << FOREGROUND_BLACK << "Player 1: " << player1Score << " | " << current << player2Score <<  RESET << endl;
}

void* playerPaddleThread(void* arg){
    Paddle* pad = (Paddle *)arg;

    while(continueGame){
        if(pad->paddleCode == PADDLE1){
                if (GetAsyncKeyState(0x57)) { // 'W' (código ASCII)
                pthread_mutex_lock(&consoleMutex);  
                pad->moveUp();  // Mover la paleta del jugador 1 hacia arriba
                pthread_mutex_unlock(&consoleMutex);
            } else if (GetAsyncKeyState(0x53)) {  // 'S' (código ASCII)
                pthread_mutex_lock(&consoleMutex);
                pad->moveDown();  // Mover la paleta del jugador 1 hacia abajo;
                pthread_mutex_unlock(&consoleMutex);
            }
        }
        
        else if(gameMode == 1 && pad->paddleCode == PADDLE2){
            if (GetAsyncKeyState(0x26)) {  // Flecha hacia arriba
                pthread_mutex_lock(&consoleMutex);
                pad->moveUp();
                pthread_mutex_unlock(&consoleMutex);
            } else if (GetAsyncKeyState(0x28)) {  // Flecha hacia abajo 
                pthread_mutex_lock(&consoleMutex);
                pad->moveDown();
                pthread_mutex_unlock(&consoleMutex);
            }
        }
        
        
        

        if(colorPaddle1 > 0 && pad->paddleCode == PADDLE1){
                pthread_mutex_lock(&consoleMutex);
                pad->drawPaddleWithColor();
                pthread_mutex_unlock(&consoleMutex);
                colorPaddle1--;
                //colorPaddle1 = false;
            }
            else if(colorPaddle2 > 0 && pad->paddleCode == PADDLE2){
                pthread_mutex_lock(&consoleMutex);
                pad->drawPaddleWithColor();
                pthread_mutex_unlock(&consoleMutex);
                colorPaddle2--;
                //colorPaddle2 = false;
            }else{
                pthread_mutex_lock(&consoleMutex);
                pad->drawPaddle();
                pthread_mutex_unlock(&consoleMutex);
            }
       std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    return NULL;
}


void* aiPaddleThread(void* arg){
    Paddle* pad = (Paddle *)arg;

    
    int ballY;
    while(continueGame){
        ballY = getBallY();
        pthread_mutex_lock(&consoleMutex);
        pad->CPUmove(ballY);
        pthread_mutex_unlock(&consoleMutex);
        if(colorPaddle1 > 0 && pad->paddleCode == PADDLE1){
            pthread_mutex_lock(&consoleMutex);
            pad->drawPaddleWithColor();
            pthread_mutex_unlock(&consoleMutex);
            colorPaddle1--;
        }
        else if(colorPaddle2 > 0 && pad->paddleCode == PADDLE2){
            pthread_mutex_lock(&consoleMutex);
            pad->drawPaddleWithColor();
            pthread_mutex_unlock(&consoleMutex);
            colorPaddle2--;
        }else{
            pthread_mutex_lock(&consoleMutex);
            pad->drawPaddle();
            pthread_mutex_unlock(&consoleMutex);
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(125));
    }
    
    return NULL;
}



void* moveBall(void* arg) {
    Ball* ball = (Ball*)arg;
    
    while (continueGame) {
        
        pthread_mutex_lock(&consoleMutex);
        ball->updateBall();
        ball->drawBall();
        pthread_mutex_unlock(&consoleMutex);

        // Comprobar si algún jugador ha ganado
        if (player1Score >= MAX_SCORE) {
            pthread_mutex_lock(&consoleMutex);

            string win = "Player 1 Wins!";
            PlaySound(TEXT("victory.wav"), NULL,  SND_ASYNC);
            for(int i = 0; i < (int) win.length(); i++){
                gotoxy(WIDTH / 2 - 5, HEIGHT / 2);
                cout << FOREGROUND_YELLOW << win.substr(0, i) << RESET; 
                std::this_thread::sleep_for(std::chrono::milliseconds(75));
            }

            continueGame = false;

            pthread_mutex_unlock(&consoleMutex);
            break;
        } else if (player2Score >= MAX_SCORE) {
            pthread_mutex_lock(&consoleMutex);
            
            PlaySound(TEXT("victory.wav"), NULL,  SND_ASYNC);
            string win = "";
            if(gameMode = 1){
                win = "Player 2 Wins!";
            }else{
                win = "CPU Wins!";
            }
            for(int i = 0; i < (int) win.length(); i++){
                gotoxy(WIDTH / 2 - 5, HEIGHT / 2);
                cout << FOREGROUND_YELLOW << win.substr(0, i) << RESET; 
                std::this_thread::sleep_for(std::chrono::milliseconds(75));
            }
            continueGame = false;

            pthread_mutex_unlock(&consoleMutex);
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(ballSpeed));
    }

    return NULL;
}

void create1vCPU(){
    Paddle pad1(7, HEIGHT / 2, PADDLE1, FOREGROUND_BLUEE);   // dejar espacio a los bordes
    Paddle pad2(WIDTH - 7, HEIGHT / 2, PADDLE2, FOREGROUND_REED);  // Dejar espacio a los bordes

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
}

void create1v1(){
    Paddle pad1(7, HEIGHT / 2, PADDLE1, FOREGROUND_BLUEE);   // dejar espacio a los bordes
    Paddle pad2(WIDTH - 7, HEIGHT / 2, PADDLE2, FOREGROUND_REED);  // Dejar espacio a los bordes

    pthread_t ballThread;
    pthread_t playe1Thread;
    pthread_t playe2Thread;
    configurePlane();
    hideCursor();
    drawBoundary();
    displayScore();

    pad1.drawPaddle();
    pad2.drawPaddle();

    pthread_mutex_init(&consoleMutex, NULL);
    pthread_create(&ballThread, NULL, moveBall, &pongBall);
    pthread_create(&playe1Thread, NULL, playerPaddleThread, &pad1);
    pthread_create(&playe2Thread, NULL, playerPaddleThread, &pad2);

    pthread_join(ballThread, NULL);
    pthread_join(playe1Thread, NULL);
    pthread_join(playe2Thread, NULL);
    pthread_mutex_destroy(&consoleMutex);
}


int main() {
    
    while(playAgain){
        showMenu();  
    
        if(gameMode == 1){
            create1v1();
        }else if(gameMode == 2){
            create1vCPU();
        }


        gotoxy(0, HEIGHT);
        cout << "\n";

        gotoxy(WIDTH / 2 - 12, HEIGHT);
        showCursor();

        int jugar;
        cout << FOREGROUND_YELLOW << "¡Presione 1 para volver a jugar!\n" << RESET;
        cout << flush;
        cin >> jugar;
        if(jugar != 1){
            playAgain = false;
        }
        cout << flush;
        player1Score = 0;
        player2Score = 0;
        continueGame = true;
    }
    
    
    
    return 0;
}


