#ifndef DECLARATIONS_H
#define DECLARATIONS_H


#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "SDL2-2.0.10/include/SDL.h"

#define SCREEN_WIDTH	640 //pixels
#define SCREEN_HEIGHT	480 //pixels

#define MAP_WIDTH		322 //pixels //2 pixels extra to prevent snake from 'riding' on the border
#define MAP_HEIGHT		222 //pixels

#define SNAKE_SIZE		10 //pixels
#define DEFAULT_MOVE_COOLDOWN 0.1 //seconds
#define START_LENGTH	6 //parts (including head)
#define TIME_TO_SPEED_UP 3 //seconds
#define SPEED_INCREASE  5 //percent
#define SPEED_DECREASE  15 //percent

#define RED_FOOD_TIME   5 //seconds
#define RED_FOOD_CHANCE 20 //percent
#define ROLL_TIME       2 //seconds
#define POINTS_BONUS    10 //points (int)
#define POINTS_PENALTY  5 //points (int)



typedef struct {
	SDL_Event event;
	SDL_Surface* screen, * charset;
	SDL_Window* window;
	SDL_Texture* scrtex;
	SDL_Renderer* renderer;
} SDLVariables;

typedef struct {
	int x;
	int y;
} Direction;

typedef struct {
	int black;
	int green;
	int white;
	int yellow;
	int blue;
	int red;
} Colors;

typedef	struct {
	int x;
	int y;
	bool isRed;
	int time;
	bool show;
} Food;

typedef struct  {
	int x;
	int y;
	Direction direction;
}SnakePart;

typedef struct Node {
    SnakePart part;
    struct Node* next;
} Node;

typedef struct {
	SnakePart parts[START_LENGTH];
	Node* snakeHead;
	int size;
	int length;
}Snake;

enum GameState {
	START,
	END,
	QUIT
};

typedef struct {
	double worldTime;
	float moveAccumulator;
	float moveInterval;
	bool increased;
	bool rolled;
	int prevTime;
} GameVariables;

typedef struct
{
	GameState gameState;
	int elapsedTime;
	int points;
	Food blueFood;
	Food redFood;
	Snake snake;
	SDLVariables sdlVariables;
	Colors colors;
	int lastRollTime;
	int initialTicks;
	bool loaded;
	int moveInt;
	GameVariables gameVariables;
}Game;



//Food.cpp
void randomizeCoords(Food* food, Snake snake);
bool rollRedDot();
void printFood(SDL_Surface* screen, Colors colors, const Food* food);
bool checkForFood(Snake* snake, Food* food, int* points, GameState* gameState);
void handleRedFood(Game* game);
void hideProgressBar(SDL_Surface* screen, Colors colors);
void printProgressBar(SDL_Surface* screen, float timeLeft, Colors colors);
void handleProgressBar(const Game* game);
void resetRedFood(Food* redFood);

//Game.cpp
void initGame(Game* game);
void printMap(SDL_Surface* screen, int borderColor, int backgroundColor);
void printStatusBar(const Game* game);
void printGame(const Game* game);
void printEntities(const Game* game);
void speedUp(float* moveInterval);
void speedDown(float* moveInterval);
void handleTimedEvents(Game* game);
void handleMovement(Game* game, double delta);
void handleInput(Game* game);
void restartGame(Game* game);
void handleGameStart(Game* game);
void handleGameEnd(Game* game);
void handleGameState(Game* game);
int main(int argc, char** argv);

// Sdl.cpp
int initSDL(SDLVariables* sdlVariables);
void initColors(SDL_Surface* screen, Colors* colors);
void clearMap(SDL_Surface* screen, int color);
void quitSDL(SDLVariables* sdlVariables);
void drawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset);
void drawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y);
void drawPixel(SDL_Surface* surface, int x, int y, Uint32 color);
void drawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color);
void drawRectangle(SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor);

// Snake.cpp
void initSnake(Snake* snake);
Node* createNode(SnakePart part);
void addPart(Snake* snake);
void removePart(Snake* snake);
void drawSnake(SDL_Surface* screen, const Snake* snake, Colors colors);
bool canMove(Snake snake);
bool checkForColision(const Snake* snake);
void moveSnake(Game* game);
void turn(Game* game);
bool turningInWall(const Snake* snake, int dx, int dy);
void changeDirection(Snake* snake, int dx, int dy);


// Files.cpp
bool saveGame(const Game* game);
bool loadGame(Game* game);


#endif