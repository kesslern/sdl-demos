#include <stdio.h>
#include <stdbool.h>
#include "SDL/SDL.h"

#define WIDTH 500
#define HEIGHT 375
#define BPP 4
#define DEPTH 32

/*
 * The y offset is expected to be y=y_coord*width, so one less multiplication
 * needs to be performed here.
 */
void set_pixel(SDL_Surface *screen, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
    Uint32 *pixmem32;
    Uint32 colour;

    colour = SDL_MapRGB( screen->format, r, g, b );

    pixmem32 = (Uint32*) screen->pixels  + y + x;
    *pixmem32 = colour;
}

void draw_screen(SDL_Surface *screen)
{
    int ytimesw = 0;
    int x, y;

    if (SDL_MUSTLOCK(screen)) {
        if (SDL_LockSurface(screen) < 0) return;
    }

    for (y = 0; y < screen->h; y++ ) {
      for (x = 0; x < screen->w; x++ ) {
	set_pixel(screen, x, ytimesw, 100, x % 255, y % 100 + 50);
      }
      ytimesw += screen->pitch/BPP;
    }

    if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
    SDL_Flip(screen);
}

int main(int argc, char* argv[])
{
  SDL_Surface *screen;
  SDL_Event event;
  bool quit = false;

  if (SDL_Init(SDL_INIT_VIDEO) < 0 ) return 1;

  if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_HWSURFACE))) {
    SDL_Quit();
    return 1;
  }

  draw_screen(screen);

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
