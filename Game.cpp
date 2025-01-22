#include "declarations.h"

void initGame(Game* game) {
	game->elapsedTime = 0;
	game->blueFood = { 0, 0, false, 0,  true };
	game->redFood = { 0, 0, true, RED_FOOD_TIME, false };
	game->points = 0;
	game->lastRollTime = 0;
	game->initialTicks = 0;
	game->gameVariables = { 0, 0, 0, false, false, 0 };
	initSnake(&game->snake);
	initSDL(&game->sdlVariables);
	initColors(game->sdlVariables.screen, &game->colors);
	srand((unsigned int)time(0));
}

void printMap(SDL_Surface* screen, int borderColor, int backgroundColor) {
	drawRectangle(screen, (SCREEN_WIDTH - MAP_WIDTH) / 2, (SCREEN_HEIGHT - MAP_HEIGHT) / 2, MAP_WIDTH, MAP_HEIGHT, borderColor, backgroundColor);
}

void printStatusBar(const Game* game) {
	char text[128];
	drawRectangle(game->sdlVariables.screen, 0, 0, SCREEN_WIDTH, 50, game->colors.yellow, game->colors.black);

	sprintf(text, "%d ", (int)game->elapsedTime);
	drawString(game->sdlVariables.screen, SCREEN_WIDTH / 2 + 1, 25, text, game->sdlVariables.charset);


	sprintf(text, "Points: %d ", game->points);
	drawString(game->sdlVariables.screen, SCREEN_WIDTH * .8, 20, text, game->sdlVariables.charset);

	sprintf(text, "Implemented requirements: ");
	drawString(game->sdlVariables.screen, 10, 10, text, game->sdlVariables.charset);

	sprintf(text, "1, 2, 3, 4; A, B, C, D, E ");
	drawString(game->sdlVariables.screen, 10, 25, text, game->sdlVariables.charset);
}

void printGame(const Game* game) {
	printMap(game->sdlVariables.screen, game->colors.yellow, game->colors.black);
	printStatusBar(game);
}

void printEntities(const Game* game) {
	drawSnake(game->sdlVariables.screen, &game->snake, game->colors);
	printFood(game->sdlVariables.screen, game->colors, &game->blueFood);
	printFood(game->sdlVariables.screen, game->colors, &game->redFood);
}

void speedUp(float* moveInterval) {
	*moveInterval *= (float)(100 - SPEED_INCREASE) / 100;
}

void speedDown(float* moveInterval) {
	*moveInterval *= (float)(100 + SPEED_DECREASE) / 100;
}

void handleTimedEvents(Game* game) {
	GameVariables* gameVariables = &game->gameVariables;
	if (gameVariables->worldTime > 1) {
		// Speed up
		if (((int)gameVariables->worldTime % TIME_TO_SPEED_UP == 0) && !gameVariables->increased) {
			speedUp(&gameVariables->moveInterval);
			gameVariables->increased = true;
		}
		else if ((int)gameVariables->worldTime % TIME_TO_SPEED_UP != 0 && gameVariables->increased) {
			gameVariables->increased = false;
		}

		int nextTimeToRoll = game->lastRollTime + ROLL_TIME;
		// Red food
		if ((int)game->elapsedTime == nextTimeToRoll && !gameVariables->rolled && !game->redFood.show) {
			if (rollRedDot()) {
				randomizeCoords(&game->redFood, game->snake);
				game->redFood.show = true;
				game->lastRollTime = (int)gameVariables->worldTime + RED_FOOD_TIME;
			}
			else
			{
				game->lastRollTime = (int)gameVariables->worldTime;
			}

			gameVariables->rolled = true;
		}
		else if (game->redFood.show) {
			gameVariables->rolled = true;
		}
		else if ((int)gameVariables->worldTime % ROLL_TIME != 0) {
			gameVariables->rolled = false;
		}
	}
}

// Snake movement
void handleMovement(Game* game, double delta) {
	GameVariables* gameVariables = &game->gameVariables;
	gameVariables->moveAccumulator += delta;
	if (gameVariables->moveAccumulator >= gameVariables->moveInterval) {
		moveSnake(game);
		//checking blue food
		checkForFood(&game->snake, &game->blueFood, &game->points, &game->gameState);
		int prevLength = game->snake.length;
		//checking red food
		bool foodDetected = checkForFood(&game->snake, &game->redFood, &game->points, &game->gameState);

		if (foodDetected && prevLength == game->snake.length) {
			speedDown(&gameVariables->moveInterval);
		}
		gameVariables->moveAccumulator = 0;
	}
}

void handleInput(Game* game) {
	while (SDL_PollEvent(&game->sdlVariables.event)) {
		switch (game->sdlVariables.event.type) {
		case SDL_KEYDOWN:
			if (game->gameState == START) {
				switch (game->sdlVariables.event.key.keysym.sym) {
				case SDLK_ESCAPE:
					game->gameState = QUIT;
					break;
				case SDLK_s:
					saveGame(game);
					break;
				case SDLK_l:
					loadGame(game);
					game->loaded = false;
					break;
				case SDLK_UP:
					changeDirection(&game->snake, 0, -1);
					break;
				case SDLK_DOWN:
					changeDirection(&game->snake, 0, 1);
					break;
				case SDLK_RIGHT:
					changeDirection(&game->snake, 1, 0);
					break;
				case SDLK_LEFT:
					changeDirection(&game->snake, -1, 0);
					break;
				}
			}
			else if (game->gameState == END) {
				if (game->sdlVariables.event.key.keysym.sym == SDLK_n) {
					restartGame(game);
					game->gameState = START;
				}
				else if (game->sdlVariables.event.key.keysym.sym == SDLK_ESCAPE)
				{
					game->gameState = QUIT;
				}
			}
			break;

		case SDL_QUIT:
			game->gameState = QUIT;
			break;
		}
	}
}

void restartGame(Game* game) {
	initSnake(&game->snake);
	game->elapsedTime = 0;
	game->blueFood = { 0, 0, false, 0,  true };
	game->redFood = { 0, 0, true, RED_FOOD_TIME, false };
	game->initialTicks = 0;
	hideProgressBar(game->sdlVariables.screen, game->colors);
	randomizeCoords(&game->blueFood, game->snake);
	game->gameState = START;
}

void handleGameStart(Game* game) {
	drawRectangle(game->sdlVariables.screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, game->colors.black, game->colors.black);
	game->gameVariables = { 0, 0, DEFAULT_MOVE_COOLDOWN, false, false, 0 };

	int t1 = SDL_GetTicks(), t2;
	char text[128];
	printGame(game);
	randomizeCoords(&game->blueFood, game->snake);

	while (game->gameState == START) {

		t2 = SDL_GetTicks();
		if (game->initialTicks != 0 && !game->loaded)
		{
			t2 = game->initialTicks;
			game->gameVariables.worldTime = game->elapsedTime;
			game->loaded = true;
		}

		double delta = (t2 - t1) * 0.001;
		t1 = t2;
		game->gameVariables.worldTime += delta;
		game->elapsedTime = game->gameVariables.worldTime;


		handleTimedEvents(game);
		handleRedFood(game);
		handleMovement(game, delta);

		printStatusBar(game);
		printEntities(game);
		handleProgressBar(game);


		SDL_UpdateTexture(game->sdlVariables.scrtex, NULL, game->sdlVariables.screen->pixels,
			game->sdlVariables.screen->pitch);
		SDL_RenderCopy(game->sdlVariables.renderer, game->sdlVariables.scrtex, NULL, NULL);
		SDL_RenderPresent(game->sdlVariables.renderer);


		handleInput(game);
	}
}

void handleGameEnd(Game* game) {
	while (game->gameState == END) {
		char text[128];

		handleInput(game);

		drawRectangle(game->sdlVariables.screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, game->colors.black, game->colors.black);

		sprintf(text, "You have lost. Press n to try again or ESC to quit");
		drawString(game->sdlVariables.screen, 10, SCREEN_HEIGHT / 2, text, game->sdlVariables.charset);

		SDL_UpdateTexture(game->sdlVariables.scrtex, NULL, game->sdlVariables.screen->pixels, game->sdlVariables.screen->pitch);
		SDL_RenderCopy(game->sdlVariables.renderer, game->sdlVariables.scrtex, NULL, NULL);
		SDL_RenderPresent(game->sdlVariables.renderer);
	}
}

void handleGameState(Game* game) {
	switch (game->gameState)
	{
	case START:
		handleGameStart(game);
		break;

	case END:
		handleGameEnd(game);
		break;

	case QUIT:
		break;
	default:
		break;
	}
}

int main(int argc, char** argv) {
	Game game;
	game.gameState = START;
	initGame(&game);

	while (game.gameState != QUIT)
	{
		handleGameState(&game);
	}

	quitSDL(&game.sdlVariables);

	return 0;
}