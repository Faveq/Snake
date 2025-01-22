#include "declarations.h"

bool saveGame(const Game* game) {
    FILE* file = fopen("Save.txt", "w");
    if (file) {
        fprintf(file, "elapsedTime = %d\n", game->elapsedTime);
        fprintf(file, "points = %d\n", game->points);
        fprintf(file, "moveInterval = %f\n", game->gameVariables.moveInterval);
        fprintf(file, "blueFoodX = %d\n", game->blueFood.x);
        fprintf(file, "blueFoodY = %d\n", game->blueFood.y);
        fprintf(file, "redFoodX = %d\n", game->redFood.x);
        fprintf(file, "redFoodY = %d\n", game->redFood.y);
        fprintf(file, "lastRollTime = %d\n", game->lastRollTime);
        fprintf(file, "redFoodTime = %d\n", game->redFood.time);
        fprintf(file, "prevTime = %d\n", game->gameVariables.prevTime);
        fprintf(file, "redFoodShow = %c\n", game->redFood.show ? 'T' : 'F');
        fprintf(file, "snakeHeadX = %d\n", game->snake.snakeHead->part.x);
        fprintf(file, "snakeHeadY = %d\n", game->snake.snakeHead->part.y);
        fprintf(file, "snakeHeadDX = %d\n", game->snake.snakeHead->part.direction.x);
        fprintf(file, "snakeHeadDY = %d\n", game->snake.snakeHead->part.direction.y);


        Node* current = game->snake.snakeHead;
        while (current->next != NULL) {
            fprintf(file, "snakePartX = %d\n", current->part.x);
            fprintf(file, "snakePartY = %d\n", current->part.y);
            fprintf(file, "snakePartDX = %d\n", current->part.direction.x);
            fprintf(file, "snakePartDY = %d\n", current->part.direction.y);
            current = current->next;
        }
        fclose(file);
        return true;
    }
    return false;
}

bool loadGame(Game* game) {
    FILE* file = fopen("Save.txt", "r");
    if (file) {
        char showTemp;

        fscanf(file, "elapsedTime = %d\n", &game->elapsedTime);
        fscanf(file, "points = %d\n", &game->points);
        fscanf(file, "moveInterval = %f\n", &game->gameVariables.moveInterval);
        
        fscanf(file, "blueFoodX = %d\n", &game->blueFood.x);
        fscanf(file, "blueFoodY = %d\n", &game->blueFood.y);

        fscanf(file, "redFoodX = %d\n", &game->redFood.x);
        fscanf(file, "redFoodY = %d\n", &game->redFood.y);
        fscanf(file, "lastRollTime = %d\n", &game->lastRollTime);
        fscanf(file, "redFoodTime = %d\n", &game->redFood.time);
        fscanf(file, "prevTime = %d\n", &game->gameVariables.prevTime);
        fscanf(file, "redFoodShow = %c\n", &showTemp, 1);

        game->redFood.show = (showTemp == 'T');

        initSnake(&game->snake);

        

        fscanf(file, "snakeHeadX = %d\n", &game->snake.snakeHead->part.x);
        fscanf(file, "snakeHeadY = %d\n", &game->snake.snakeHead->part.y);
        fscanf(file, "snakeHeadDX = %d\n", &game->snake.snakeHead->part.direction.x);
        fscanf(file, "snakeHeadDY = %d\n", &game->snake.snakeHead->part.direction.y);


        // Ceaning old snake
        Node* current = game->snake.snakeHead->next;
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
        game->snake.snakeHead->next = NULL;
        game->snake.length = 1;

        int x, y, dx, dy;
        while (fscanf(file, "snakePartX = %d\n", &x) == 1) {
            fscanf(file, "snakePartY = %d\n", &y);
            fscanf(file, "snakePartDX = %d\n", &dx);
            fscanf(file, "snakePartDY = %d\n", &dy);

            SnakePart newPart = { x, y, { dx, dy } };
            Node* newNode = createNode(newPart);

            current = game->snake.snakeHead;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newNode;
            game->snake.length++;
        }

        game->initialTicks = SDL_GetTicks();
       
        fclose(file);
        return true;
    }
    return false;
}