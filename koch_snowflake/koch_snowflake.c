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
  /* From Rosetta Code */
  int dx = abs(p2.x-p1.x), sx = p1.x<p2.x ? 1 : -1;
  int dy = abs(p2.y-p1.y), sy = p1.y<p2.y ? 1 : -1;
  int err = (dx>dy ? dx : -dy)/2, e2;

  for(;;){
    set_color(screen, p1, color);
    if (p1.x==p2.x && p1.y==p2.y) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; p1.x += sx; }
    if (e2 < dy) { err += dx; p1.y += sy; }
  }
}

/*
 * Contains logic to draw the screen on the provided SDL_Surface.
 */
void draw_screen(SDL_Surface *screen, list *ll)
{
  node* start = ll->head->next;
  
  if (SDL_MUSTLOCK(screen)) {
    if (SDL_LockSurface(screen) < 0) return;
  }

  while (start->next->next != NULL) {
    draw_line(screen, start->p, start->next->p, SDL_MapRGB(screen->format, rand()%255, rand()%255, rand()%255));
    start = start->next;
  }

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

  point p1;
  node* current;
  p1.x = 10;
  p1.y = 10;

  list* list = list_create();
  int i;
  current = list->head;
  for (i = 0; i < 10000; i++) {
    p1.x = rand() % screen->w;
    p1.y = rand() % screen->h;
    current = list_insert(current, p1);
  }

  draw_screen(screen, list);
  
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

  list_delete(list);

  SDL_Quit();
  return 0;
}
