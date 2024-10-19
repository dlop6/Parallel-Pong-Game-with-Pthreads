#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char* args[]) {
    // Inicializar SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Error al inicializar SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Crear ventana
    SDL_Window* window = SDL_CreateWindow("Ventana SDL2",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          640, 480,
                                          SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Error al crear la ventana: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Crear un surface (superficie) para la ventana
    SDL_Surface* screenSurface = SDL_GetWindowSurface(window);
    SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00)); // Color negro
    SDL_UpdateWindowSurface(window);

    // Mantener la ventana abierta durante 5 segundos o hasta que el usuario la cierre
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
    }

    // Limpiar y cerrar SDL
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
