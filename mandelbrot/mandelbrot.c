#include <stdio.h>
#include <stdbool.h>
#include "SDL/SDL.h"
#include "complex.h"

#define WIDTH 500
#define HEIGHT 375
#define BPP 4
#define DEPTH 32
#define MAX_ITERATIONS 400
#define ZOOM_PERCENTAGE .2
#define COLORS 100

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

struct window_t {
  Uint32 *palette;
  long double min_x;
  long double min_y;
  long double max_x;
  long double max_y;
};

struct point_t {
  int x;
  int y;
};

typedef struct point_t point;
typedef struct window_t window;

int is_in_set(complex *c);
complex scale(window *win, SDL_Surface *screen, point *p);

void setpixel(SDL_Surface *screen, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
    Uint32 *pixmem32;
    Uint32 colour;

    colour = SDL_MapRGB( screen->format, r, g, b );

    pixmem32 = (Uint32*) screen->pixels  + y + x;
    *pixmem32 = colour;
}

void setcolor(SDL_Surface *screen, int x, int y, Uint32* color)
{
  Uint32 *pixmem32;
  pixmem32 = (Uint32*) screen->pixels  + y + x;
  *pixmem32 = *color;  
}

void DrawScreen(SDL_Surface *screen, window *win)
{
    int ytimesw = 0;
    point p;
    complex c;
    int i;

    if (SDL_MUSTLOCK(screen)) {
        if (SDL_LockSurface(screen) < 0) return;
    }

    for (p.y = 0; p.y < screen->h; p.y++ ) {
        for (p.x = 0; p.x < screen->w; p.x++ ) {
	  c = scale(win, screen, &p);
	  if ((i = is_in_set(&c))) {
	    setcolor(screen, p.x, ytimesw, win->palette + i%COLORS);
	  } else {
	    setpixel(screen, p.x, ytimesw, 0, 0, 0);
	  }
        }
      ytimesw += screen->pitch/BPP;
    }

    if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
    SDL_Flip(screen);
}

Uint32* create_palette(SDL_Surface *screen, double freq, double phase1, double phase2, double phase3, int center, int width, int len)
{
  Uint32 *palette = malloc(sizeof(Uint32) * len);
  int i;
  
  for (i = 0; i < len; i++) {
    *palette = SDL_MapRGB(screen->format,
			 sin(freq * i + phase1) * width + center,
			 sin(freq * i + phase2) * width + center,
			 sin(freq * i + phase3) * width + center);
    palette++;
  }

  return palette - len;
}

void draw_edge(SDL_Surface *screen, window *win, int dir)
{
  point p;
  complex c;
  int ytimesw = 0;
  int i;
  
  p.x = 0;
  p.y = 0;
  
  if (SDL_MUSTLOCK(screen)) {
    if (SDL_LockSurface(screen) < 0) return;
  }

  if (dir == UP || dir == DOWN) {
    if (dir == DOWN) {
      ytimesw = (HEIGHT - 1) * screen->pitch / BPP;
      p.y = HEIGHT -1;
    }
    for (p.x = 0; p.x < WIDTH; p.x++) {
      c = scale(win, screen, &p);
      if ((i = is_in_set(&c))) {
	setcolor(screen, p.x, ytimesw, win->palette + i%COLORS);
      } else {
	setpixel(screen, p.x, ytimesw, 0, 0, 0);
      }
    }
  } else {
    if (dir ==  RIGHT) {
      p.x = WIDTH - 1;
    }
    for (p.y = 0; p.y < HEIGHT; p.y++) {
      c = scale(win, screen, &p);
      if ((i = is_in_set(&c))) {
	setcolor(screen, p.x, ytimesw, win->palette + i%COLORS);
      } else {
	setpixel(screen, p.x, ytimesw, 0, 0, 0);
      }
      ytimesw += screen->pitch / BPP;
    }
  }
  
  if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
  SDL_Flip(screen);
}

void scroll(SDL_Surface *screen, window *win, int dir)
{
  SDL_Rect grab, dest;
  
  switch (dir) {
  case RIGHT:
    grab.x = 1; grab.y = 0;
    grab.w = WIDTH - 1;
    grab.h = HEIGHT;

    dest.x = 0, dest.y = 0;
    dest.w = WIDTH -1;
    dest.h = HEIGHT;
    break;

  case LEFT:
    grab.x = 0; grab.y = 0;
    grab.w = WIDTH - 1;
    grab.h = HEIGHT;

    dest.x = 1, dest.y = 0;
    dest.w = WIDTH - 1;
    dest.h = HEIGHT;
    break;

  case UP:
    grab.x = 0; grab.y = 0;
    grab.w = WIDTH;
    grab.h = HEIGHT - 1;

    dest.x = 0, dest.y = 1;
    dest.w = WIDTH;
    dest.h = HEIGHT - 1;
    break;

  case DOWN:
    grab.x = 0; grab.y = 1;
    grab.w = WIDTH;
    grab.h = HEIGHT - 1;

    dest.x = 0, dest.y = 0;
    dest.w = WIDTH;
    dest.h = HEIGHT - 1;
    break;
  }

  SDL_Surface *scrollSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
  SDL_BlitSurface(screen, &grab, scrollSurface, NULL);
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
  SDL_BlitSurface(scrollSurface, NULL, screen, &dest);
  SDL_FreeSurface(scrollSurface);

  draw_edge(screen, win, dir);
}

bool user_input(SDL_Surface *screen, window *win, SDL_Event input)
{
  long double delta;

  switch (input.key.keysym.sym) {
  case SDLK_a:
    /* Pan to the left. */
    delta = (win->max_x - win->min_x) / WIDTH;
    win->max_x -= delta;
    win->min_x -= delta;
    scroll(screen, win, LEFT);
    break;
  case SDLK_e:
    /* Pan to the right. */
    delta = (win->max_x - win->min_x) / WIDTH;
    win->max_x += delta;
    win->min_x += delta;
    scroll(screen, win, RIGHT);
    break;
  case SDLK_o:
    /* Pan down. */
    delta = (win->max_y - win->min_y)/ HEIGHT;
    win->max_y += delta;
    win->min_y += delta;
    scroll(screen, win, DOWN);
    break;
  case SDLK_COMMA:
    /* Pan up. */
    delta = (win->max_y - win->min_y) / HEIGHT;
    win->max_y -= delta;
    win->min_y -= delta;
    scroll(screen, win, UP);
    break;
  case SDLK_q:
    /* Zoom out, y direction. */
    delta = ZOOM_PERCENTAGE * (win->max_y - win->min_y);
    win->max_y += delta;
    win->min_y -= delta;
    /* Zoom out, x direction. */
    delta = ZOOM_PERCENTAGE * (win->max_x - win->min_x);
    win->max_x += delta;
    win->min_x -= delta;
    return true;
    break;
  case SDLK_PERIOD:
    /* Zoom in, y direction. */
    delta = ZOOM_PERCENTAGE * (win->max_y - win->min_y);
    win->max_y -= delta;
    win->min_y += delta;
    /* Zoom in, x direction. */
    delta = ZOOM_PERCENTAGE * (win->max_x - win->min_x);
    win->max_x -= delta;
    win->min_x += delta;
    return true;
    break;
  case SDLK_c:
    create_palette(screen, M_PI*2/COLORS*3, 0, M_PI*2/3, M_PI*4/3, 128, 127, COLORS);
    break;
  default:
    break;
  }
  return false;
}

int main(int argc, char* argv[])
{
  SDL_Surface *screen;
  SDL_Event event;
  window win;
  bool quit = false;

  win.min_x = -2;
  win.max_x = 1;
  win.min_y = -1;
  win.max_y = 1;

  if (SDL_Init(SDL_INIT_VIDEO) < 0 ) return 1;

  if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_HWSURFACE))) {
    SDL_Quit();
    return 1;
  }

  win.palette = create_palette(screen, M_PI*2/COLORS*3, 0, M_PI*2/3, M_PI*4/3, 128, 127, COLORS);

  
  DrawScreen(screen, &win);

  while(!quit) {
    SDL_PollEvent(&event);

      switch (event.type) {
      case SDL_QUIT:
	quit = true;
	break;
      case SDL_KEYDOWN:
	if (event.key.keysym.sym == SDLK_q){
	  quit = true;
	} else {
	  if (user_input(screen, &win, event)) {
	    DrawScreen(screen, &win);
	  }
	}
	break;
      }
  }
  SDL_Quit();
  free(win.palette);
  return 0;
}

complex scale(window *win, SDL_Surface *screen, point *p)
{
  /* Pass the win struct by address for speed. */
  complex c;

  c.a = (long double) p->x / screen->w
        * (win->max_x - win->min_x) + win->min_x;
  c.b = (long double) p->y / screen->h
        * (win->max_y - win->min_y) + win->min_y;

  return c;
}

int is_in_set(complex *c)
{
  /* We want to store the final value of c in the original
   * place in memory so we can use that value in the bmp
   * color generation process. */
  complex z = *c;
  complex lastc;
  int i = 0;

  while (i++ <= MAX_ITERATIONS) {
    *c = complex_add(complex_multiply(*c, *c), z);
    if (c->a*c->a + c->b*c->b >= 4.0) {
      return i;
    }

    if (fabsl(lastc.a - c->a) < 0.001 && fabsl(lastc.b - c->b) < 0.001) {
      return 0;
    }

    lastc = *c;
  }

  return 0;
}
