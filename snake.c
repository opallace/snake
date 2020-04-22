#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define UP    87
#define DOWN  83
#define LEFT  65
#define RIGHT 68

#define VELOCITY 100
#define FRUITTIMER 1000
#define MAPSIZE 22

typedef struct {
	int size;
	int **body;
	int lastBody[2];
	int direction;
} Snake;

typedef struct {
	int x;
	int y;
	int eaten;
} Fruit;

typedef struct {
	int size;
	int **map;
} Map;

int** createMap(int size){
	int ** map = (int **)malloc(sizeof(int)*size);
	
	int i;
	for(i = 0; i < size; i++){
		map[i] = malloc(sizeof(int)*size);
	}
	
	return map;
}

int isSnake(Snake snake, int y, int x){
	int i;
	for(i = 0; i < snake.size; i++){
		if(snake.body[i][0] == y && snake.body[i][1] == x){
			return 1;
		}
	}
	
	return 0;
}

void showMap(Map map, Fruit fruit, Snake snake){
	system("cls");
	int i, j;
	for(i = 0; i < map.size; i++){
		for(j = 0; j < map.size; j++){
			if(i == 0 || i == map.size -1 || j == 0 || j == map.size - 1){
				printf("%c ", 220);
			}else {
				
				if(isSnake(snake, i, j)){
					printf("%c ", 220);	
				}else if(fruit.y == i && fruit.x == j && !fruit.eaten){
					printf("O ");
				}else {
					printf("  ");
				}
			}
		}
		printf("\n");
	}
}

void setDirection(Snake *snake){
	int i;
	for(i = 65; i <= 90; i++){
		if(GetAsyncKeyState(i) == -32767){
			if(i == UP){
				if(snake->direction != DOWN){
					snake->direction = UP;	
					break;
				}
			}else if(i == DOWN){
				if(snake->direction != UP){
					snake->direction = DOWN;
					break;
				}
			}else if(i == LEFT){
				if(snake->direction != RIGHT){
					snake->direction = LEFT;
					break;
				}
			}else if(i == RIGHT){
				if(snake->direction != LEFT){
					snake->direction = RIGHT;
					break;
				}
			}
		}
	}
}

void move(Snake *snake){
	
	snake->lastBody[0] = snake->body[snake->size-1][0];
	snake->lastBody[1] = snake->body[snake->size-1][1];
	
	int i;
	for(i = snake->size - 1; i > 0; i--){
		snake->body[i][0] = snake->body[i-1][0];
		snake->body[i][1] = snake->body[i-1][1];
	}
	
	switch(snake->direction){
		case UP:
			snake->body[0][0]--;
			break;
		case DOWN:
			snake->body[0][0]++;
			break;
		case LEFT:
			snake->body[0][1]--;
			break;
		case RIGHT:
			snake->body[0][1]++;
			break;
	}	
}

void addBody(Snake *snake){
	snake->body = (int **)realloc(snake->body, sizeof(int *)*(++snake->size));
	snake->body[snake->size - 1]    = malloc(sizeof(int)*2);
	
	if(snake->size - 1 == 0){
		snake->body[snake->size - 1][0] = 10;
		snake->body[snake->size - 1][1] = 10;	
	}else {
		snake->body[snake->size - 1][0] = snake->lastBody[0];
		snake->body[snake->size - 1][1] = snake->lastBody[1];
	}
}

int verifyGameOver(Map map, Snake *snake){
	if(snake->body[0][0] == map.size - 1 || snake->body[0][0] == 0 || snake->body[0][1] == 0 || snake->body[0][1] == map.size - 1){
		return 1;
	}else {
		int i;
		for(i = 1; i < snake->size; i++){
			if(snake->body[0][0] == snake->body[i][0] && snake->body[0][1] == snake->body[i][1]){
				return 1;
			}
		}
	}
	
	return 0;
}

void createFruit(Map map, Fruit *fruit, Snake snake){
	if(fruit->eaten){
		
		int invalid;
		int y, x;
		
		do{
			
			invalid = 0;
			
			x = (rand() % (map.size-2)) + 1;
			y = (rand() % (map.size-2)) + 1;
			
			int i;
			for(i = 0; i < snake.size; i++){
				if(snake.body[i][0] == y && snake.body[i][1] == x){
					invalid = 1;
				}
			}
			
		}while(invalid);
		
		fruit->eaten = 0;
		fruit->y = y;
		fruit->x = x;
	}
}

void eat(Map map, Fruit *fruit, Snake *snake){
	if(snake->body[0][0] == fruit->y && snake->body[0][1] == fruit->x){
		fruit->eaten = 1;
		addBody(snake);
	}
}

int main(){
	
	srand(time(NULL));
	
	Map map;
	map.size = MAPSIZE;
	map.map = createMap(map.size);
	
	Snake snake;
	snake.size = 0;
	snake.body = NULL;
	snake.direction = RIGHT;
	
	Fruit fruit;
	fruit.eaten = 1;
	
	addBody(&snake);
	
	showMap(map, fruit, snake);
	
	clock_t velocityTimer       = clock();
	clock_t fruitTimer          = clock();
	
	while(!verifyGameOver(map, &snake)){
		if(clock() - fruitTimer > FRUITTIMER){
			fruitTimer = clock();
			createFruit(map, &fruit, snake);
		}
		
		if(clock() - velocityTimer > VELOCITY){
			velocityTimer = clock();
			setDirection(&snake);
			move(&snake);
			eat(map, &fruit, &snake);
			showMap(map, fruit, snake);	
		}
	}
	
	printf("\n[GAMEOVER]\n");
	printf("SCORE: %i\n", snake.size);
	
	
	return 0;
}
