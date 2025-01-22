#include "declarations.h"

int initSDL(SDLVariables* sdlVariables) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}

	if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &sdlVariables->window, &sdlVariables->renderer) != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(sdlVariables->renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(sdlVariables->renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(sdlVariables->window, "Snake");

	sdlVariables->screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	sdlVariables->scrtex = SDL_CreateTexture(sdlVariables->renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_ShowCursor(SDL_DISABLE);

	sdlVariables->charset = SDL_LoadBMP("./cs8x8.bmp");

	return 0;
}

void initColors(SDL_Surface* screen, Colors* colors) {
	colors->black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	colors->green = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	colors->yellow = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0x00);
	colors->white = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);
	colors->blue = SDL_MapRGB(screen->format, 0x00, 0x00, 0xFF);
	colors->red = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
}

void clearMap(SDL_Surface* screen, int color) {
	drawRectangle(screen, (SCREEN_WIDTH - MAP_WIDTH) / 2 + 2, (SCREEN_HEIGHT - MAP_HEIGHT - 1) / 2 + 2, MAP_WIDTH - 3, MAP_HEIGHT - 2, color, color);
}

void quitSDL(SDLVariables* sdlVariables) {
	SDL_FreeSurface(sdlVariables->charset);
	SDL_FreeSurface(sdlVariables->screen);
	SDL_DestroyTexture(sdlVariables->scrtex);
	SDL_DestroyRenderer(sdlVariables->renderer);
	SDL_DestroyWindow(sdlVariables->window);

	SDL_Quit();
}


// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images
void drawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};


// draw a surface sprite on a surface screen in point (x, y)
// (x, y) is the center of sprite on screen
void drawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};


// draw a single pixel
void drawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32*)p = color;
};

// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
void drawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		drawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};

// draw a rectangle of size l by k
void drawRectangle(SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor) {
	int i;
	drawLine(screen, x, y, k, 0, 1, outlineColor);
	drawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	drawLine(screen, x, y, l, 1, 0, outlineColor);
	drawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		drawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};