#include <SDL3/SDL.h>
#include <stdio.h>
#include <GL/gl.h>

int main() {
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);
    SDL_Log("Starting SDL...");
    SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "dummy");
    SDL_SetHint(SDL_HINT_APP_NAME, "TestApp");
    SDL_SetHint(SDL_HINT_APP_ID, "com.example.testapp");
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window *win = SDL_CreateWindow("hello", 800, 600, SDL_WINDOW_OPENGL);
    if (!win) {
        printf("Window creation failed: %s\n", SDL_GetError());
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(win, NULL);
    if (!renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    SDL_ShowWindow(win);
    SDL_Delay(3000); // Show window for 3 seconds
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
