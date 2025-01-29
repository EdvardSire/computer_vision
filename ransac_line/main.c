#include <SDL2/SDL.h>

void drawPoint(SDL_Renderer *r, int x, int y) {
  int width = 5;
  SDL_SetRenderDrawColor(r, 255, 0, 0, 255);

}
void updateWindow(SDL_Renderer *r) {
    SDL_Rect rect = {0, 0, 100, 100}; 
    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);

    SDL_RenderClear(r); // Clear the scree
    SDL_RenderFillRect(r, &rect); // Draw the rectangle
                                  //
    SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
    SDL_RenderFillRect(r, &point); // Draw the rectangle
    SDL_RenderPresent(r); // Update the screen
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *w = SDL_CreateWindow("Rect", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);
    SDL_Renderer *r = SDL_CreateRenderer(w, -1, 0);

    int running = 1;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
          if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_ESCAPE || e.key.keysym.sym == SDLK_q)))
                running = 0;
        }

        updateWindow(r);

        SDL_Delay(10);
    }

    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);
    SDL_Quit();
    return 0;
}
