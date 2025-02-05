#include "declarations.h"

// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images
void drawString(SDL_Surface* screen, int x, int y, const char* text,
	SDL_Surface* charset) {
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
void drawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor) {
	int i;
	drawLine(screen, x, y, k, 0, 1, outlineColor);
	drawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	drawLine(screen, x, y, l, 1, 0, outlineColor);
	drawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		drawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};


// main
//#ifdef __cplusplus
//extern "C"
//#endif
//int main(int argc, char** argv) {
//	int t1, t2, quit, frames, rc;
//	double delta, worldTime, fpsTimer, fps, distance, etiSpeed;
//	SDL_Event event;
//	SDL_Surface* screen, * charset;
//	SDL_Surface* eti;
//	SDL_Texture* scrtex;
//	SDL_Window* window;
//	SDL_Renderer* renderer;
//
//	// console window is not visible, to see the printf output
//	// the option:
//	// project -> szablon2 properties -> Linker -> System -> Subsystem
//	// must be changed to "Console"
//	printf("wyjscie printfa trafia do tego okienka\n");
//	printf("printf output goes here\n");
//
//	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
//		printf("SDL_Init error: %s\n", SDL_GetError());
//		return 1;
//	}
//
//	// fullscreen mode
////	rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP &window, &renderer);
//
//	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
//		&window, &renderer);
//	if (rc != 0) {
//		SDL_Quit();
//		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
//		return 1;
//	};
//
//	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
//	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
//	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//
//	SDL_SetWindowTitle(window, "Snake");
//
//
//	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
//		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
//
//	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
//		SDL_TEXTUREACCESS_STREAMING,
//		SCREEN_WIDTH, SCREEN_HEIGHT);
//
//
//	SDL_ShowCursor(SDL_DISABLE);
//
//	charset = SDL_LoadBMP("./cs8x8.bmp");
//	if (charset == NULL) {
//		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
//		SDL_FreeSurface(screen);
//		SDL_DestroyTexture(scrtex);
//		SDL_DestroyWindow(window);
//		SDL_DestroyRenderer(renderer);
//		SDL_Quit();
//		return 1;
//	};
//	SDL_SetColorKey(charset, true, 0x000000);
//
//	eti = SDL_LoadBMP("./eti.bmp");
//	if (eti == NULL) {
//		printf("SDL_LoadBMP(eti.bmp) error: %s\n", SDL_GetError());
//		SDL_FreeSurface(charset);
//		SDL_FreeSurface(screen);
//		SDL_DestroyTexture(scrtex);
//		SDL_DestroyWindow(window);
//		SDL_DestroyRenderer(renderer);
//		SDL_Quit();
//		return 1;
//	};
//
//	char text[128];
//	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
//	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
//	int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
//	int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);
//
//	t1 = SDL_GetTicks();
//
//	frames = 0;
//	fpsTimer = 0;
//	fps = 0;
//	quit = 0;
//	worldTime = 0;
//	distance = 0;
//	etiSpeed = 1;
//
//	while (!quit) {
//		t2 = SDL_GetTicks();
//
//		// here t2-t1 is the time in milliseconds since
//		// the last screen was drawn
//		// delta is the same time in seconds
//		delta = (t2 - t1) * 0.001;
//		t1 = t2;
//
//		worldTime += delta;
//
//		distance += etiSpeed * delta;
//
//		SDL_FillRect(screen, NULL, czarny);
//
//		drawSurface(screen, eti,
//			SCREEN_WIDTH / 2 + sin(distance) * SCREEN_HEIGHT / 3,
//			SCREEN_HEIGHT / 2 + cos(distance) * SCREEN_HEIGHT / 3);
//
//		fpsTimer += delta;
//		if (fpsTimer > 0.5) {
//			fps = frames * 2;
//			frames = 0;
//			fpsTimer -= 0.5;
//		};
//
//		// tekst informacyjny / info text
//		drawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 36, czerwony, niebieski);
//		//            "template for the second project, elapsed time = %.1lf s  %.0lf frames / s"
//		sprintf(text, "Szablon drugiego zadania, czas trwania = %.1lf s  %.0lf klatek / s", worldTime, fps);
//		drawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
//		//	      "Esc - exit, \030 - faster, \031 - slower"
//		sprintf(text, "Esc - wyjscie, \030 - przyspieszenie, \031 - zwolnienie");
//		drawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset);
//
//		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
//		//		SDL_RenderClear(renderer);
//		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
//		SDL_RenderPresent(renderer);
//
//		while (SDL_PollEvent(&event)) {
//			switch (event.type) {
//			case SDL_KEYDOWN:
//				if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
//				else if (event.key.keysym.sym == SDLK_UP) etiSpeed = 2.0;
//				else if (event.key.keysym.sym == SDLK_DOWN) etiSpeed = 0.3;
//				break;
//			case SDL_KEYUP:
//				etiSpeed = 1.0;
//				break;
//			case SDL_QUIT:
//				quit = 1;
//				break;
//			};
//		};
//		frames++;
//	};
//
//	// zwolnienie powierzchni / freeing all surfaces
//	SDL_FreeSurface(charset);
//	SDL_FreeSurface(screen);
//	SDL_DestroyTexture(scrtex);
//	SDL_DestroyRenderer(renderer);
//	SDL_DestroyWindow(window);
//
//	SDL_Quit();
//	return 0;
//};
