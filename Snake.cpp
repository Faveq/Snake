#include "declarations.h"
#include "SDL2-2.0.10/include/begin_code.h"
#include "SDL2-2.0.10/include/SDL_surface.h"
#include <malloc.h>

void initSnake(Snake* snake) {
	snake->size = SNAKE_SIZE;
	snake->length = START_LENGTH;

	snake->snakeHead = nullptr;

	SnakePart snakePart = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, { 1, 0 } };
	Node* head = createNode(snakePart);

	snake->snakeHead = head;
	Node* current = head;

	for (int i = 0; i < snake->length; i++)
	{
		SnakePart newPart = { current->part.x - snake->size, current->part.y, current->part.direction };
		Node* newNode = createNode(newPart);
		current->next = newNode;
		current = newNode;
	}
}

Node* createNode(SnakePart part) {
	Node* newNode = (Node*)malloc(sizeof(Node));

	if (!newNode) {
		return NULL;
	}

	newNode->part = part;
	newNode->next = NULL;
	return newNode;
}

void addPart(Snake* snake) {
	Node* current = snake->snakeHead;
	while (current->next != NULL) {
		current = current->next;
	}
	SnakePart newPart = current->part;
	newPart.x += current->part.direction.x * snake->size;
	newPart.y += current->part.direction.y * snake->size;
	Node* lastCopy = createNode(newPart);
	current->next = lastCopy;
	snake->length++;
}

void removePart(Snake* snake) {
	if (snake->snakeHead->next == NULL) {
		free(snake->snakeHead);
		snake->snakeHead = NULL;
	}
	else {
		Node* current = snake->snakeHead;
		while (current->next->next != NULL) {
			current = current->next;
		}
		free(current->next);
		current->next = NULL;
	}
	snake->length--;
}

void drawSnake(SDL_Surface* screen, const Snake* snake, Colors colors) {
	printMap(screen, colors.yellow, colors.black);


	drawRectangle(screen,
		snake->snakeHead->part.x,
		snake->snakeHead->part.y,
		snake->size,
		snake->size,
		colors.yellow,
		colors.yellow);

	Node* current = snake->snakeHead->next;
	while (current->next != NULL) {
		drawRectangle(screen,
			current->part.x,
			current->part.y,
			snake->size,
			snake->size,
			colors.green,
			colors.green);

		current = current->next;
	}
}

//Checking if snake won't move outside the bounds
bool canMove(Snake snake) {
	int leftBorderX = (SCREEN_WIDTH - MAP_WIDTH) / 2;
	int rightBorderX = leftBorderX + MAP_WIDTH;
	int topBorderY = (SCREEN_HEIGHT - MAP_HEIGHT) / 2;
	int bottomBorderY = topBorderY + MAP_HEIGHT;

	int nextX = snake.snakeHead->part.x + (snake.snakeHead->part.direction.x * snake.size);
	int nextY = snake.snakeHead->part.y + (snake.snakeHead->part.direction.y * snake.size);

	if (snake.snakeHead->part.direction.x != 0) {
		return (nextX > leftBorderX && nextX + snake.size < rightBorderX && snake.snakeHead->part.direction.x != (-1) * snake.snakeHead->next->part.direction.x);
	}
	else {
		return (nextY > topBorderY && nextY + snake.size < bottomBorderY && snake.snakeHead->part.direction.y != (-1) * snake.snakeHead->next->part.direction.y);
	}
}

// Checking for snake's head colision with body
bool checkForColision(const Snake* snake) {
	Node* current = snake->snakeHead->next;
	while (current->next != NULL) {
		if (snake->snakeHead->part.x == current->part.x && snake->snakeHead->part.y == current->part.y) {
			return true;
		}
		current = current->next;
	}

	return false;
}

void moveSnake(Game* game) {
	Snake* snake;
	snake = &game->snake;

	if (!canMove(*snake)) {
		turn(game);
		return;
	}

	int prevX = snake->snakeHead->part.x;
	int prevY = snake->snakeHead->part.y;
	Direction prevDir = snake->snakeHead->part.direction;

	snake->snakeHead->part.x += snake->size * snake->snakeHead->part.direction.x;
	snake->snakeHead->part.y += snake->size * snake->snakeHead->part.direction.y;

	Node* current = snake->snakeHead->next;
	while (current != NULL) {
		int tempX = current->part.x;
		int tempY = current->part.y;
		Direction tempDir = current->part.direction;

		current->part.x = prevX;
		current->part.y = prevY;
		current->part.direction = prevDir;

		prevX = tempX;
		prevY = tempY;
		prevDir = tempDir;

		current = current->next;
	}

	if (checkForColision(snake)) {
		game->gameState = END;
	}
}

// Bouncing of a wall
void turn(Game* game) {
	Snake* snake = &game->snake;
	Direction currentDir = snake->snakeHead->part.direction;
	Direction originalDir = currentDir;


	// Horizontal movement
	if (currentDir.x != 0) {
		snake->snakeHead->part.direction = { 0, currentDir.x };
	}
	// Vertical movement
	else {
		snake->snakeHead->part.direction = { -currentDir.y, 0 };
	}

	// If cant move, checking alternative direction
	if (!canMove(*snake)) {
		snake->snakeHead->part.direction = originalDir;
		if (originalDir.x != 0) {
			snake->snakeHead->part.direction = { 0, -originalDir.x };
		}
		else {
			snake->snakeHead->part.direction = { originalDir.y, 0 };
		}
	}
	moveSnake(game);
}

bool turningInWall(const Snake* snake, int dx, int dy) {
	int leftBorderX = (SCREEN_WIDTH - MAP_WIDTH) / 2;
	int rightBorderX = leftBorderX + MAP_WIDTH;
	int topBorderY = (SCREEN_HEIGHT - MAP_HEIGHT) / 2;
	int bottomBorderY = topBorderY + MAP_HEIGHT;

	int currentX = snake->snakeHead->part.x;
	int currentY = snake->snakeHead->part.y;

	bool isAtLeftWall = (currentX <= leftBorderX + snake->size);
	bool isAtRightWall = (currentX >= rightBorderX - 2 * snake->size);
	bool isAtTopWall = (currentY <= topBorderY + snake->size);
	bool isAtBottomWall = (currentY >= bottomBorderY - 2 * snake->size);

	// Ignoring turning in a wall
	if ((isAtLeftWall && dx < 0) || (isAtRightWall && dx > 0) ||
		(isAtTopWall && dy < 0) || (isAtBottomWall && dy > 0)) {
		return 1;
	}
	return 0;
}

// Turning by a user
void changeDirection(Snake* snake, int dx, int dy) {

	if (!turningInWall(snake, dx, dy))
	{
		if (dx != 0 && snake->snakeHead->part.direction.x == 0) {
			snake->snakeHead->part.direction = { dx, 0 };
		}
		else if (dy != 0 && snake->snakeHead->part.direction.y == 0) {
			snake->snakeHead->part.direction = { 0, dy };
		}
	}
}