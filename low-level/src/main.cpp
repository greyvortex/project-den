#include <SDL3/SDL.h>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("SDL3 Test", 640, 480, SDL_WINDOW_OPENGL);
    SDL_Delay(2000000);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}