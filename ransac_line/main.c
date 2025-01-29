#include <SDL2/SDL.h>

void drawRect(SDL_Renderer *r, int i) {
  SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
  SDL_Rect shape = {i, i, i+20, i+20};
  SDL_RenderDrawRect(r, &shape);
}
void drawBackground(SDL_Renderer *r, int height, int width) {
  SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
  SDL_RenderDrawLine(r, 0, 500, 1000, 500);
  SDL_RenderDrawLine(r, 500, 0, 500, 1000);
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *w = SDL_CreateWindow("Rect", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);
    SDL_Renderer *r = SDL_CreateRenderer(w, -1, 0);


    int running = 1;
    int i = 200;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
          if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_ESCAPE || e.key.keysym.sym == SDLK_q)))
                running = 0;
          if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_f)
              i += 10;

          if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_g)
              i -= 10;
        }
        SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
        SDL_RenderClear(r);

        drawBackground(r, 1000, 100);
        drawRect(r, i);


        SDL_RenderPresent(r);
        SDL_Delay(16);
    }
    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);
    SDL_Quit();
    return 0;
}
