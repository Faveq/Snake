#include "declarations.h"

void randomizeCoords(Food* food, Snake snake) {
	int leftBorderX = (SCREEN_WIDTH - MAP_WIDTH) / 2;
	int topBorderY = (SCREEN_HEIGHT - MAP_HEIGHT) / 2;

	int gridWidth = MAP_WIDTH / SNAKE_SIZE;
	int gridHeight = MAP_HEIGHT / SNAKE_SIZE;

	bool validPosition = 0;
	while (!validPosition) {

		food->x = (rand() % gridWidth) * SNAKE_SIZE + leftBorderX + 1;
		food->y = (rand() % gridHeight) * SNAKE_SIZE + topBorderY + 1;

		validPosition = true;

		Node* current = snake.snakeHead;
		while (current != NULL) {
			
			if (food->x == current->part.x && food->y == current->part.y) {
				validPosition = false;
				break;
			}
			current = current->next;
		}
	}
}

bool rollRedDot() {
	return rand() % 100 + 1 <= RED_FOOD_CHANCE;
}

void printFood(SDL_Surface* screen, Colors colors, const Food* food) {
	if (food->show)
	{
		if (!food->isRed)
		{
			drawRectangle(screen,
				food->x,
				food->y,
				SNAKE_SIZE,
				SNAKE_SIZE,
				colors.blue,
				colors.blue);
		}
		else {
			drawRectangle(screen,
				food->x,
				food->y,
				SNAKE_SIZE,
				SNAKE_SIZE,
				colors.red,
				colors.red);
		}
	}
}

bool checkForFood(Snake* snake, Food* food, int *points, GameState* gameState) {
	if (food->show)
	{
		if (!food->isRed)
		{
			if (snake->snakeHead->part.x == food->x && snake->snakeHead->part.y == food->y) {
				addPart(snake);
				randomizeCoords(food, *snake);
				*points += POINTS_BONUS;
				return true;
			}
		}
		else {
			if (snake->snakeHead->part.x == food->x && snake->snakeHead->part.y == food->y) {
				
				// 50% for swhortening the snake, otherwise triggering game slow down
				if (rand() % 100 + 1 < 50)
				{
					removePart(snake);	
					if (snake->length <= 1)
					{
						*gameState = END;
					}
					*points -= POINTS_PENALTY;
				}

				resetRedFood(food);

				return true;
				
			}
		}
	}
	return false;
}

// Handling red food when it's displayed
void handleRedFood(Game* game) {
	GameVariables* gameVariables = &game->gameVariables;
	if (game->redFood.show && gameVariables->prevTime != (int)gameVariables->worldTime) {
		game->redFood.time--;
		if (game->redFood.time == 0) {
			resetRedFood(&game->redFood);
		}
		// Preventing from multiple decreasing in single second
		gameVariables->prevTime = (int)gameVariables->worldTime;
	}
}

void hideProgressBar(SDL_Surface* screen, Colors colors) {
	const int topOffset = 30, progressLength = 200, progressHeight = 20;
	drawRectangle(screen, (SCREEN_WIDTH - 200) / 2, (SCREEN_HEIGHT - MAP_HEIGHT) / 2 - topOffset, progressLength, progressHeight, colors.black, colors.black);
}

void printProgressBar(SDL_Surface* screen, float timeLeft, Colors colors) {
	const int topOffset = 30, progressLength = 200, progressHeight = 20;
	drawRectangle(screen, (SCREEN_WIDTH - 200) / 2, (SCREEN_HEIGHT - MAP_HEIGHT) / 2 - topOffset, progressLength, progressHeight, colors.white, colors.black);
	drawRectangle(screen, (SCREEN_WIDTH - 200) / 2, (SCREEN_HEIGHT - MAP_HEIGHT) / 2 - topOffset, (timeLeft / RED_FOOD_TIME) * progressLength, progressHeight, colors.white, colors.white);
}

void handleProgressBar(const Game* game) {
	if (game->redFood.show) {
		printProgressBar(game->sdlVariables.screen, game->redFood.time, game->colors);
	}
	else
	{
		hideProgressBar(game->sdlVariables.screen, game->colors);
	}
}

void resetRedFood(Food* redFood) {
	redFood->time = RED_FOOD_TIME;
	redFood->show = false;
}
