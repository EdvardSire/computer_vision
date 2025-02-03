#include "SDL_rect.h"
#include <SDL2/SDL.h>


typedef struct {
  int x;
  int y;
} Point;

void drawRect(SDL_Renderer *r, int x, int y, int h, int w) {
  SDL_Rect rect = {x, y, w, h};
  SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
  SDL_RenderDrawRect(r, &rect);
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
    int l = 10;
    SDL_Event e;

    rand();
    while (running) {
        while (SDL_PollEvent(&e)) {
          if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_ESCAPE || e.key.keysym.sym == SDLK_q)))
                running = 0;
          if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_f)
              l += 1;

          if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_g)
              l -= 1;
        }
        SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
        SDL_RenderClear(r);

        drawBackground(r, 1000, 100);

        for(int i = 0; i < l; i++) {
          drawRect(r, i*20, i*20, 20, 20);
        }


        SDL_RenderPresent(r);
        SDL_Delay(16);
    }
    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);
    SDL_Quit();
    return 0;
}
