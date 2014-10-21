#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "list.h"
#include "SDL/SDL.h"

#define WIDTH 1024
#define HEIGHT 600
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

  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

  while (start->next->next != NULL) {
    draw_line(screen, start->p, start->next->p,
	      SDL_MapRGB(screen->format,
			 (Uint8) rand()%255,
			 (Uint8) rand()%255,
			 (Uint8) rand()%255));
    start = start->next;
  }

  if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
  SDL_Flip(screen);
}

void iterate(node* n)
{
  point p1 = n->p;
  point p2 = n->next->p;

  int xlen = p2.x - p1.x;
  int ylen = p2.y - p1.y;

  point new;
  new.x = p1.x + xlen / 3;
  new.y = p1.y + ylen / 3;
  node* current = list_insert(n, new);
  new.x = p1.x + 2 * xlen / 3;
  new.y = p1.y + 2 * ylen / 3;
  list_insert(current, new);

  int len = (int)(sqrt(xlen*xlen + ylen*ylen) + 0.5);
  double theta = atan(ylen / xlen) + M_PI/6.0;

  int deltax = (int)( (len / 3.0) * sin(theta) );
  int deltay = (int)( (len / 3.0) * cos(theta) );

  if (p2.x < p1.x) {
    deltax *= -1;
    deltay *= -1;
  }
  
  new.x = current->p.x + deltax;
  new.y = current->p.y - deltay;
  list_insert(current, new);
}

/*
 * Creates a linked list representing an equilateral triangle that
 * will fill the provided SDL_Surface, positioned opitmally for
 * koch snowflake iteration.
 */
list* generate(SDL_Surface* screen) {
  point top, left, right;

  list* ll = list_create();
  
  top.y = screen->h / 4 - screen->h/12;
  left.y = 3 * screen->h / 4 - screen->h/12;
  right.y = left.y;

  left.x = screen->w / 4;
  right.x = 3 * screen->w / 4;
  top.x = screen->w / 2;
  
  
  node* current = list_insert(ll->head, top);
  current = list_insert(current, right);
  current = list_insert(current, left);
  current = list_insert(current, top);

  return ll;
}

void wait_for_input()
{
  bool quit = false;
  SDL_Event event;
  
  while(!quit) {
    SDL_PollEvent(&event);

    switch (event.type) {
    case SDL_QUIT:
      quit = true;
      break;
    case SDL_KEYDOWN:
      quit = true;
      break;
    }
  }
}

int main()
{
  SDL_Surface *screen;

  srand((unsigned int) time(NULL));
  if (SDL_Init(SDL_INIT_VIDEO) < 0 ) return 1;

  if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_FULLSCREEN))) {
    SDL_Quit();
    return 1;
  }

  node* current;
  list* list = generate(screen);
  int i;

  draw_screen(screen, list);
  wait_for_input();
  
  current = list->head->next;
  for (i = 0; i < 4; i++) {

    current = list->head->next;
    while (current->next->next != NULL) {
      current = current->next;
      iterate(current->prev);
    }
    draw_screen(screen, list);
    wait_for_input();
  }
  
  list_delete(list);

  SDL_Quit();
  return 0;
}
