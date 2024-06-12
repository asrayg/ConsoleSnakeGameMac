/*
Author: Asray
Date: 12/06/2024

Description:
This is a console snake game that works in Unix-based environments like Linux and macOS.
*/

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#define SNAKE_ARRAY_SIZE 310

#define UP_ARROW 'A'
#define LEFT_ARROW 'D'
#define RIGHT_ARROW 'C'
#define DOWN_ARROW 'B'
#define ENTER_KEY 10

const char SNAKE_HEAD = 'X';
const char SNAKE_BODY = '#';
const char WALL = '#';
const char FOOD = '*';
const char BLANK = ' ';

// Terminal control functions
void gotoxy(int x, int y) {
    printf("%c[%d;%df", 0x1B, y, x);
}

int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

char getch() {
    char c;
    system("stty raw");
    c = getchar();
    system("stty sane");
    return c;
}

void clrscr() {
    system("clear");
}

// Game functions
char waitForAnyKey() {
    while (!kbhit());
    return getch();
}

int getGameSpeed() {
    int speed;
    clrscr();

    do {
        gotoxy(10, 5);
        printf("Select The game speed between 1 and 9: ");
        speed = waitForAnyKey() - '0';
    } while (speed < 1 || speed > 9);
    return speed;
}

void pauseMenu() {
    gotoxy(28, 23);
    printf("**Paused**");
    waitForAnyKey();
    gotoxy(28, 23);
    printf("            ");
}

int checkKeysPressed(int direction) {
    int pressed;

    if (kbhit()) {
        pressed = getch();
        if (direction != pressed) {
            if (pressed == DOWN_ARROW && direction != UP_ARROW)
                direction = pressed;
            else if (pressed == UP_ARROW && direction != DOWN_ARROW)
                direction = pressed;
            else if (pressed == LEFT_ARROW && direction != RIGHT_ARROW)
                direction = pressed;
            else if (pressed == RIGHT_ARROW && direction != LEFT_ARROW)
                direction = pressed;
            else if (pressed == ENTER_KEY)
                pauseMenu();
        }
    }
    return direction;
}

int collisionSnake(int x, int y, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int detect) {
    for (int i = detect; i < snakeLength; i++) {
        if (x == snakeXY[0][i] && y == snakeXY[1][i])
            return 1;
    }
    return 0;
}

int generateFood(int foodXY[], int width, int height, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength) {
    do {
        srand(time(NULL));
        foodXY[0] = rand() % (width - 2) + 2;
        srand(time(NULL));
        foodXY[1] = rand() % (height - 6) + 2;
    } while (collisionSnake(foodXY[0], foodXY[1], snakeXY, snakeLength, 0));

    gotoxy(foodXY[0], foodXY[1]);
    printf("%c", FOOD);

    return 0;
}

void moveSnakeArray(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int direction) {
    for (int i = snakeLength - 1; i >= 1; i--) {
        snakeXY[0][i] = snakeXY[0][i - 1];
        snakeXY[1][i] = snakeXY[1][i - 1];
    }

    switch (direction) {
        case DOWN_ARROW:
            snakeXY[1][0]++;
            break;
        case RIGHT_ARROW:
            snakeXY[0][0]++;
            break;
        case UP_ARROW:
            snakeXY[1][0]--;
            break;
        case LEFT_ARROW:
            snakeXY[0][0]--;
            break;
    }
}

void move(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int direction) {
    int x = snakeXY[0][snakeLength - 1];
    int y = snakeXY[1][snakeLength - 1];

    gotoxy(x, y);
    printf("%c", BLANK);

    gotoxy(snakeXY[0][0], snakeXY[1][0]);
    printf("%c", SNAKE_BODY);

    moveSnakeArray(snakeXY, snakeLength, direction);

    gotoxy(snakeXY[0][0], snakeXY[1][0]);
    printf("%c", SNAKE_HEAD);

    gotoxy(1, 1);
}

int eatFood(int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[]) {
    if (snakeXY[0][0] == foodXY[0] && snakeXY[1][0] == foodXY[1]) {
        foodXY[0] = 0;
        foodXY[1] = 0;
        printf("\7");
        return 1;
    }
    return 0;
}

int collisionDetection(int snakeXY[][SNAKE_ARRAY_SIZE], int consoleWidth, int consoleHeight, int snakeLength) {
    if ((snakeXY[0][0] == 1) || (snakeXY[1][0] == 1) || (snakeXY[0][0] == consoleWidth) || (snakeXY[1][0] == consoleHeight - 4))
        return 1;
    else if (collisionSnake(snakeXY[0][0], snakeXY[1][0], snakeXY, snakeLength, 1))
        return 1;
    return 0;
}

void refreshInfoBar(int score, int speed) {
    gotoxy(5, 23);
    printf("Score: %d", score);

    gotoxy(5, 24);
    printf("Speed: %d", speed);

    gotoxy(52, 23);
    printf("Coder: Matthew Vlietstra");

    gotoxy(52, 24);
    printf("Version: 0.5");
}

void startGame(int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[], int consoleWidth, int consoleHeight, int snakeLength, int direction, int score, int speed) {
    int gameOver = 0;
    clock_t endWait;
    int waitMili = CLOCKS_PER_SEC - (speed * CLOCKS_PER_SEC / 10);
    int tempScore = 10 * speed;
    int oldDirection;
    int canChangeDirection = 1;

    endWait = clock() + waitMili;

    do {
        if (canChangeDirection) {
            oldDirection = direction;
            direction = checkKeysPressed(direction);
        }

        if (oldDirection != direction)
            canChangeDirection = 0;

        if (clock() >= endWait) {
            move(snakeXY, snakeLength, direction);
            canChangeDirection = 1;

            if (eatFood(snakeXY, foodXY)) {
                generateFood(foodXY, consoleWidth, consoleHeight, snakeXY, snakeLength);
                snakeLength++;
                score += speed;

                if (score >= 10 * speed + tempScore) {
                    speed++;
                    tempScore = score;

                    if (speed <= 9)
                        waitMili -= CLOCKS_PER_SEC / 10;
                    else if (waitMili >= 40)
                        waitMili -= CLOCKS_PER_SEC / 200;
                }

                refreshInfoBar(score, speed);
            }

            endWait = clock() + waitMili;
        }

        gameOver = collisionDetection(snakeXY, consoleWidth, consoleHeight, snakeLength);

        if (snakeLength >= SNAKE_ARRAY_SIZE - 5) {
            gameOver = 2;
            score += 1500;
        }
    } while (!gameOver);

    if (gameOver == 1) {
        printf("\7");
        printf("\7");
    }

    if (score >= 10) {
        gotoxy(10, 5);
        printf("Your Score: %d", score);
        printf("\nPress any key to continue...");
        waitForAnyKey();
    }
}

void loadEnviroment(int consoleWidth, int consoleHeight) {
    int x = 1, y = 1;
    int rectangleHeight = consoleHeight - 4;
    clrscr();

    for (; y < rectangleHeight; y++) {
        gotoxy(x, y);
        printf("%c", WALL);

        gotoxy(consoleWidth, y);
        printf("%c", WALL);
    }

    y = 1;
    for (; x < consoleWidth + 1; x++) {
        gotoxy(x, y);
        printf("%c", WALL);

        gotoxy(x, rectangleHeight);
        printf("%c", WALL);
    }
}

void loadSnake(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength) {
    for (int i = 0; i < snakeLength; i++) {
        gotoxy(snakeXY[0][i], snakeXY[1][i]);
        printf("%c", SNAKE_BODY);
    }
}

void prepairSnakeArray(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength) {
    int snakeX = snakeXY[0][0];
    int snakeY = snakeXY[1][0];

    for (int i = 1; i <= snakeLength; i++) {
        snakeXY[0][i] = snakeX + i;
        snakeXY[1][i] = snakeY;
    }
}

void loadGame() {
    int snakeXY[2][SNAKE_ARRAY_SIZE];
    int snakeLength = 4;
    int direction = LEFT_ARROW;
    int foodXY[] = {5, 5};
    int score = 0;
    int consoleWidth = 80;
    int consoleHeight = 25;
    int speed = getGameSpeed();

    snakeXY[0][0] = 40;
    snakeXY[1][0] = 10;

    loadEnviroment(consoleWidth, consoleHeight);
    prepairSnakeArray(snakeXY, snakeLength);
    loadSnake(snakeXY, snakeLength);
    generateFood(foodXY, consoleWidth, consoleHeight, snakeXY, snakeLength);
    refreshInfoBar(score, speed);
    startGame(snakeXY, foodXY, consoleWidth, consoleHeight, snakeLength, direction, score, speed);
}

void welcomeArt() {
    clrscr();
    printf("\n");
    printf("\t\t    _________         _________ \n");
    printf("\t\t   /         \\       /         \\ \n");
    printf("\t\t  /  /~~~~~\\  \\     /  /~~~~~\\  \\ \n");
    printf("\t\t  |  |     |  |     |  |     |  | \n");
    printf("\t\t  |  |     |  |     |  |     |  | \n");
    printf("\t\t  |  |     |  |     |  |     |  | \n");
    printf("\t\t (o  o)    \\  \\_____/  /     \\  \\_____/ / \n");
    printf("\t\t  \\__/      \\         /       \\        / \n");
    printf("\t\t    |        ~~~~~~~~~         ~~~~~~~~ \n");
    printf("\t\t    ^ \n");
    printf("\t\tWelcome To The Snake Game!\n");
    printf("\tPress Any Key To Continue...\n");
    printf("\n");

    waitForAnyKey();
}

void controls() {
    int x = 10, y = 5;
    clrscr();
    gotoxy(x, y++);
    printf("Controls\n");
    gotoxy(x++, y++);
    printf("Use the following arrow keys to direct the snake to the food: ");
    gotoxy(x, y++);
    printf("Right Arrow");
    gotoxy(x, y++);
    printf("Left Arrow");
    gotoxy(x, y++);
    printf("Top Arrow");
    gotoxy(x, y++);
    printf("Bottom Arrow");
    gotoxy(x, y++);
    gotoxy(x, y++);
    printf("Press any key to continue...");
    waitForAnyKey();
}

void exitYN() {
    char pressed;
    gotoxy(9, 8);
    printf("Are you sure you want to exit(Y/N)\n");

    do {
        pressed = waitForAnyKey();
        pressed = tolower(pressed);
    } while (!(pressed == 'y' || pressed == 'n'));

    if (pressed == 'y') {
        clrscr();
        exit(1);
    }
}

int mainMenu() {
    int x = 10, y = 5;
    int yStart = y;
    int selected;

    clrscr();
    gotoxy(x, y++);
    printf("New Game\n");
    gotoxy(x, y++);
    printf("Controls\n");
    gotoxy(x, y++);
    printf("Exit\n");
    gotoxy(x, y++);

    selected = menuSelector(x, y, yStart);
    return selected;
}

int menuSelector(int x, int y, int yStart) {
    char key;
    int i = 0;
    x = x - 2;
    gotoxy(x, yStart);

    printf(">");

    gotoxy(1, 1);

    do {
        key = waitForAnyKey();
        if (key == (char)UP_ARROW) {
            gotoxy(x, yStart + i);
            printf(" ");

            if (yStart >= yStart + i)
                i = y - yStart - 2;
            else
                i--;
            gotoxy(x, yStart + i);
            printf(">");
        } else if (key == (char)DOWN_ARROW) {
            gotoxy(x, yStart + i);
            printf(" ");

            if (i + 2 >= y - yStart)
                i = 0;
            else
                i++;
            gotoxy(x, yStart + i);
            printf(">");
        }
    } while (key != (char)ENTER_KEY);
    return i;
}

int main() {
    welcomeArt();

    do {
        switch (mainMenu()) {
            case 0:
                loadGame();
                break;
            case 1:
                controls();
                break;
            case 2:
                exitYN();
                break;
        }
    } while (1);

    return 0;
}
