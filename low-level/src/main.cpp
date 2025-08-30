#include <SDL3/SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "Starting SDL init..." << std::endl;

    SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);
    SDL_Log("Starting SDL...");

    int num_drivers = SDL_GetNumVideoDrivers();
    
    SDL_Log("Available video drivers: %d", num_drivers);
    for (int i = 0; i < num_drivers; ++i) {
        SDL_Log("Driver %d: %s", i, SDL_GetVideoDriver(i));
    }

    SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "dummy");
    SDL_SetHint(SDL_HINT_APP_NAME, "TestApp");
    SDL_SetHint(SDL_HINT_APP_ID, "com.example.testapp");
SDL_Init(0);
if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
    SDL_Log("SDL_InitSubSystem (VIDEO) failed: %s", SDL_GetError());
    return 1;
}


    SDL_Log("Using video driver: %s", SDL_GetCurrentVideoDriver());
    std::cout << "SDL initialized successfully!" << std::endl;

    // Create window
    SDL_Window* window = SDL_CreateWindow("hello", 800, 600, SDL_WINDOW_OPENGL);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Set draw color and clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(renderer);

    // Load font and render text (requires SDL_ttf)
    // For now, we'll simulate "Hello world" with a simple rectangle and no actual text

    // Draw a white rectangle (placeholder for text)
    SDL_FRect rect = {300.0f, 250.0f, 200.0f, 100.0f};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
    SDL_RenderFillRect(renderer, &rect);

    // Present the renderer
    SDL_RenderPresent(renderer);

    // Wait for 3 seconds
    SDL_Delay(3000);

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}