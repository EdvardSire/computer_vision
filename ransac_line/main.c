#include "SDL_rect.h"
#include <SDL2/SDL.h>


#define N_POINTS 200
#define WIN_X 1000
#define WIN_Y 1000

typedef struct {
  int x;
  int y;
} Point;

void drawRect(SDL_Renderer *r, Point point) {
  int size = 3;
  SDL_Rect rect = {point.x + WIN_X/2, point.y + WIN_Y/2, size, size};
  SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
  SDL_RenderDrawRect(r, &rect);
}

void drawBackground(SDL_Renderer *r, int height, int width) {
  SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
  SDL_RenderDrawLine(r, 0, WIN_Y/2, WIN_X, WIN_Y/2);
  SDL_RenderDrawLine(r, WIN_X/2, 0, WIN_X/2, WIN_Y);
}

void randomPoints(Point points[N_POINTS]) {
  for(int i = 0; i < N_POINTS; i++) {
    points[i].x = (rand() % WIN_X) - WIN_X/2;
    points[i].y = (rand() % WIN_Y) - WIN_Y/2;
  }
}


int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *w = SDL_CreateWindow("Rect", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_X, WIN_Y, 0);
    SDL_Renderer *r = SDL_CreateRenderer(w, -1, 0);


    int running = 1;
    int l = 10;
    SDL_Event e;

    
    Point points[N_POINTS];
    randomPoints(points);

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


        for(int i = 0; i < N_POINTS; i++) {
          drawRect(r, points[i]);
        }


        SDL_RenderPresent(r);
        SDL_Delay(16);
    }
    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);
    SDL_Quit();
    return 0;
}
