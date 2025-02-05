#include "SDL_render.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>


#define N_POINTS 200
#define WIN_X 1000
#define WIN_Y 1000
#define CONSENSUS_WIDTH 20


float tfx(float x) {
  return x + WIN_X/2.f;
}

float tfy(float y) {
  return WIN_Y/2.f - y;
}


void drawRect(SDL_Renderer* r, SDL_Point point) {
  int size = 3;
  SDL_Rect rect = {tfx(point.x), tfy(point.y), size, size};
  SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
  SDL_RenderDrawRect(r, &rect);
}

void drawBackground(SDL_Renderer* r, int height, int width) {
  SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
  SDL_RenderDrawLine(r, 0, WIN_Y/2, WIN_X, WIN_Y/2);
  SDL_RenderDrawLine(r, WIN_X/2, 0, WIN_X/2, WIN_Y);
}

void drawLine(SDL_Renderer* r, SDL_Point p1, SDL_Point p2) {
  SDL_SetRenderDrawColor(r, 0, 255, 0, 255);
  SDL_RenderDrawLine(r, tfx(p1.x), tfy(p1.y), tfx(p2.x), tfy(p2.y));
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
    SDL_Window *window = SDL_CreateWindow("Rect", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_X, WIN_Y, 0);
    SDL_Renderer *r = SDL_CreateRenderer(window, -1, 0);


    int running = 1;
    int l = 10;
    SDL_Event e;
    double angle = 0;


    SDL_Point random_points[N_POINTS];
    SDL_Point line_points[N_POINTS];
    randomPoints(random_points);
    sampleLine(1, 20, line_points);
    SDL_Texture* texture = createTexture(r, 5, 5);
    
    SDL_Point p1 = {-200, -50};
    SDL_Point p2 = {200,  50};
    float cx = (p1.x+p2.x)/2.f;
    float cy = (p1.y+p2.y)/2.f;
    float dy = p2.y-p1.y;
    float dx = p2.x-p1.x;
    int w = sqrt(pow(dy, 2) + pow(dx, 2));
    int h = CONSENSUS_WIDTH;
    angle = -atan(dy/dx)*(180.f / M_PI);
    printf("%f\n", tfx(cx));
    printf("%f\n", tfy(cy));

    while (running) {
        while (SDL_PollEvent(&e)) {
          if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_ESCAPE || e.key.keysym.sym == SDLK_q)))
                running = 0;
          if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_f)
              angle += 1;

          if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_g)
              angle -= 1;
        }
        SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
        SDL_RenderClear(r);
        drawBackground(r, 1000, 100);

        SDL_RenderCopyEx(r,
                   texture,
                   NULL,
                   &(SDL_Rect){tfx(cx-w/2.f), tfy(cy+h/2.f), w, h},
                   angle,
                   &(SDL_Point){w/2.f, h/2.f},
                   SDL_FLIP_NONE);
        drawLine(r, p1, p2);
                     


        for(int i = 0; i < N_POINTS; i++) {
          drawRect(r, random_points[i]);
          drawRect(r, line_points[i]);
        }


        SDL_RenderPresent(r);
        SDL_Delay(16);
    }
    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
