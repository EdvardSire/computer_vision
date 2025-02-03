#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include <SDL2/SDL.h>


#define N_POINTS 200
#define WIN_X 1000
#define WIN_Y 1000


void drawRect(SDL_Renderer *r, SDL_Point point) {
  int size = 3;
  SDL_Rect rect = {point.x + WIN_X/2, WIN_Y/2 - point.y, size, size};
  SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
  SDL_RenderDrawRect(r, &rect);
}

void drawBackground(SDL_Renderer *r, int height, int width) {
  SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
  SDL_RenderDrawLine(r, 0, WIN_Y/2, WIN_X, WIN_Y/2);
  SDL_RenderDrawLine(r, WIN_X/2, 0, WIN_X/2, WIN_Y);
}

void randomPoints(SDL_Point points[N_POINTS]) {
  for(int i = 0; i < N_POINTS; i++) {
    points[i].x = (rand() % WIN_X) - WIN_X/2.f;
    points[i].y = (rand() % WIN_Y) - WIN_Y/2.f;
  }
}

void sampleLine(float A, float b, SDL_Point points[N_POINTS]) {
  int noise_level = 10;
  for(int i = 0; i < N_POINTS; i++) {
    float x = (rand() % WIN_Y) - WIN_Y/2.f;
    float y = A*x + b;
    points[i].x = x;
    points[i].y = y;
  }
}


SDL_Texture* createTexture(SDL_Renderer* r, const int width, const int height) {
    Uint32 R_MASK = 0xFF000000;
    Uint32 G_MASK = 0x00FF0000;
    Uint32 B_MASK = 0x0000FF00;
    Uint32 A_MASK = 0x000000FF;
    SDL_Surface *surface = SDL_CreateRGBSurface(0, width, height, 32, R_MASK, G_MASK, B_MASK, A_MASK);
    SDL_Rect rect = {0, 0, width, height};
    Uint32 color = SDL_MapRGB(surface->format, 0x00, 0x00, 0xFF);
    SDL_FillRect(surface, &rect, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(r, surface);
    SDL_FreeSurface(surface);

    return texture;
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *w = SDL_CreateWindow("Rect", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_X, WIN_Y, 0);
    SDL_Renderer *r = SDL_CreateRenderer(w, -1, 0);


    int running = 1;
    int l = 10;
    SDL_Event e;


    SDL_Point random_points[N_POINTS];
    SDL_Point line_points[N_POINTS];
    randomPoints(random_points);
    sampleLine(1, 20, line_points);
    

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


        const int width = 200;
        const int height = 200;
        const double angle = 10;
        SDL_Texture* texture = createTexture(r, width, height);

        SDL_Rect destRect = {200, 200, width, height};
        SDL_Point center = {width/2, height/2};
        SDL_RenderCopyEx(r,
                   texture,
                   NULL,
                   &destRect,
                   angle,
                   &center,
                   SDL_FLIP_NONE);
                     


        for(int i = 0; i < N_POINTS; i++) {
          drawRect(r, random_points[i]);
          drawRect(r, line_points[i]);
        }


        SDL_RenderPresent(r);
        SDL_Delay(16);
    }
    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);
    SDL_Quit();
    return 0;
}
