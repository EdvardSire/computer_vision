#include <SDL2/SDL.h>
#include <stdbool.h>

#define N_RANDOM_POINTS 800
#define N_LINE_POINTS 100
#define WIN_X 800
#define WIN_Y 800
#define CONSENSUS_WIDTH 20
#define MAX_ITERATIONS 500


float tfx(float x) { return x + WIN_X/2.f; }

float tfy(float y) { return WIN_Y/2.f - y; }

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

void drawLine(SDL_Renderer* r, SDL_Point* p1, SDL_Point* p2) {
  SDL_SetRenderDrawColor(r, 0, 255, 0, 255);
  SDL_RenderDrawLine(r, tfx(p1->x), tfy(p1->y), tfx(p2->x), tfy(p2->y));
}

void randomPoints(SDL_Point* points) {
  for(int i = 0; i < N_RANDOM_POINTS; i++) {
    points[i].x = (rand() % WIN_X) - WIN_X/2.f;
    points[i].y = (rand() % WIN_Y) - WIN_Y/2.f;
  }
}

void sampleLine(float A, float b, SDL_Point* points) {
  for(int i = 0; i < N_LINE_POINTS; i++) {
    float x = (rand() % WIN_Y) - WIN_Y/2.f;
    float y = A*x + b;
    points[i].x = x;
    points[i].y = y;
  }
}

SDL_Texture* createTexture(SDL_Renderer* r) {
    int width = 5; int height = 5; // we don't care
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

void drawConsensus(SDL_Renderer* r, SDL_Texture* texture, SDL_Point* p1, SDL_Point* p2, bool show_winner) {
    float cx = (p1->x+p2->x)/2.f;
    float cy = (p1->y+p2->y)/2.f;
    float dy = p2->y-p1->y;
    float dx = p2->x-p1->x;
    int w = show_winner ? 100000 : sqrt(pow(dy, 2) + pow(dx, 2)); // long bar if winner
    int h = CONSENSUS_WIDTH;
    double angle = -atan(dy/dx)*(180.f / M_PI);

    SDL_RenderCopyEx(r,
        texture,
        NULL,
        &(SDL_Rect){tfx(cx-w/2.f), tfy(cy+h/2.f), w, h},
        angle,
        &(SDL_Point){w/2.f, h/2.f},
        SDL_FLIP_NONE);
    drawLine(r, p1, p2);
}

int scoreSample(SDL_Point* all_points, SDL_Point* p1, SDL_Point* p2){
  //https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line#Line_defined_by_two_points
  int score = 0;

  float norm = sqrt(pow(p2->y - p1->y, 2) + pow(p2->x - p1->x, 2));
  float dy = p2->y - p1->y;
  float dx = p2->x - p1->x;
  float term1 = p2->x * p1->y;
  float term2 = p2->y * p1->x;
    
  for(int i = 0 ; i < N_RANDOM_POINTS+N_LINE_POINTS; i++) {
    float x0 = all_points[i].x; float y0 = all_points[i].y;
    float dist = fabsf(dy*x0 - dx*y0 + term1 - term2)/norm;
    if(dist < CONSENSUS_WIDTH)
      score += 1;
  }

  return score;
}


int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Rect", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_X, WIN_Y, 0);
    SDL_Renderer *r = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture* texture = createTexture(r);
    SDL_Event e;
    int running = 1;

    SDL_Point all_points[N_RANDOM_POINTS+N_LINE_POINTS];
    SDL_Point random_points[N_RANDOM_POINTS];
    SDL_Point line_points[N_LINE_POINTS];
    randomPoints(random_points);
    sampleLine(1, 20, line_points);
    memcpy(all_points                , random_points, N_RANDOM_POINTS*sizeof(SDL_Point));
    memcpy(all_points+N_RANDOM_POINTS, line_points, N_LINE_POINTS*sizeof(SDL_Point));
    
    SDL_Point* p1;
    SDL_Point* p2;
    SDL_Point* best_p1;
    SDL_Point* best_p2;
    int score = 0;
    int current_score;
    int iterations = 0;

    while (running) {
        while (SDL_PollEvent(&e)) {
          if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_ESCAPE || e.key.keysym.sym == SDLK_q)))
                running = 0;
          if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_f) {}
        }

        SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
        SDL_RenderClear(r);

        drawBackground(r, 1000, 100);

        // algo
        if(iterations < MAX_ITERATIONS) {
          p1 = &all_points[rand() % N_RANDOM_POINTS+N_LINE_POINTS];
          p2 = &all_points[rand() % N_RANDOM_POINTS+N_LINE_POINTS];
          current_score = scoreSample(all_points, p1, p2);
          if(current_score > score){
            best_p1 = p1; best_p2 = p2; 
            score = current_score;
          }
          drawConsensus(r, texture, p1, p2, false);
        } else {
          drawConsensus(r, texture, best_p1, best_p2, true);
        }
        iterations++;
        //

        for(int i = 0; i < N_RANDOM_POINTS; i++)
          drawRect(r, random_points[i]);
        for(int i = 0; i < N_LINE_POINTS; i++)
          drawRect(r, line_points[i]);


        SDL_RenderPresent(r);
        SDL_Delay(5);
    }
    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
