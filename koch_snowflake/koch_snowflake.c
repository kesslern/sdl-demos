#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "list.h"
#include "SDL/SDL.h"

#define WIDTH 500
#define HEIGHT 375
#define BPP 4
#define DEPTH 32

void set_color(SDL_Surface *screen, point p, Uint32 color)
{
  *((Uint32*) screen->pixels + p.y * screen->pitch / BPP + p.x) = color;
}

void draw_line(SDL_Surface *screen, point p1, point p2, Uint32 color)
{
  int deltax;
  int deltay;
  point plot;
  double error = 0;
  double delta;
  
  if (p2.x < p1.x) {
    plot = p1;
    p1 = p2;
    p2 = plot;
  }

  deltax = p2.x - p1.x;
  deltay = p2.y - p1.y;
  delta = (double) deltay / deltax;
  
  if (delta < 0) delta *= -1;

  plot.y = p1.y;
  for (plot.x = p1.x; plot.x < p2.x; plot.x++) {
    set_color(screen, plot, color);
      
    error += delta;
    if (error > 0.5) {
      ++plot.y;
      error -= 1.0;
    }
  }
 
  if (deltay == 0) exit(1);
}

void clear_screen(SDL_Surface *screen)
{
  if (SDL_MUSTLOCK(screen)) {
    if (SDL_LockSurface(screen) < 0) return;
  }

  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

  if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
  SDL_Flip(screen);
}

/*
 * Contains logic to draw the screen on the provided SDL_Surface.
 */
void draw_screen(SDL_Surface *screen)
{
  point p;
  point p2;

  p.x = rand() % screen->w;
  p.y = rand() % screen->h;
  p2.x = rand() % screen->w;
  p2.y =rand() % screen->h;
  
  if (SDL_MUSTLOCK(screen)) {
    if (SDL_LockSurface(screen) < 0) return;
  }

  /*  for (p.y = 0; p.y < screen->h; p.y++ ) {
    for (p.x = 0; p.x < screen->w; p.x++ ) {
      set_color(screen, p, SDL_MapRGB(screen->format, 100, p.x % 255, p.y % 100 + 50));
    }
    }*/

  clear_screen(screen);
  draw_line(screen, p, p2, SDL_MapRGB(screen->format, 100, 125, 150));
  set_color(screen, p, SDL_MapRGB(screen->format, 255, 255, 255));
  set_color(screen, p2, SDL_MapRGB(screen->format, 255, 255, 255));

  if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
  SDL_Flip(screen);
}

int main(int argc, char* argv[])
{
  SDL_Surface *screen;
  SDL_Event event;
  bool quit = false;

  srand(time(NULL));
  if (SDL_Init(SDL_INIT_VIDEO) < 0 ) return 1;

  if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_HWSURFACE))) {
    SDL_Quit();
    return 1;
  }

  draw_screen(screen);
  
  point p1;
  node* current;
  p1.x = 10;
  p1.y = 10;

  list* list = list_create();
  current = list_insert(list->head, p1);
  p1.x += 10;
  p1.y += 10;
  current = list_insert(current, p1);
  p1.x += 10;
  p1.y -= 10;
  current = list_insert(current, p1);
  current = list->head;

  while (current->next->next != NULL) {
    current = current->next;
    printf("x: %d, y:%d\n", current->p.x, current->p.y);
  }
  
  list_delete(list);

  while(!quit) {
    SDL_PollEvent(&event);

      switch (event.type) {
      case SDL_QUIT:
	quit = true;
	break;
      case SDL_KEYDOWN:
	if (event.key.keysym.sym == SDLK_q){
	  quit = true;
	}
	break;
      }
  }

  SDL_Quit();
  return 0;
}
