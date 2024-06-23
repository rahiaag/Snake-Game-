#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>

using namespace std;

#define MAX_LENGTH 1000

// Directions
const char DIR_UP = 'U';
const char DIR_DOWN = 'D';
const char DIR_LEFT = 'L';
const char DIR_RIGHT = 'R';

int consoleWidth, consoleHeight;

// Function to initialize the console screen size
void initScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

// Class to represent a point on the board
class point
{
public:
    int xCoord;
    int yCoord;
    point() {}
    point(int x, int y)
    {
        xCoord = x;
        yCoord = y;
    }
};

// Class to represent the Snake
class Snake
{
    int length;
    char direction;

public:
    point body[MAX_LENGTH];
    Snake(int x, int y)
    {
        length = 1;
        body[0] = point(x, y);
        direction = DIR_RIGHT;
    }

    int getlength()
    {
        return length;
    }

    // Change direction of the snake
    void changeDirection(char newDirection)
    {
        if (newDirection == DIR_UP && direction != DIR_DOWN)
        {
            direction = newDirection;
        }
        else if (newDirection == DIR_DOWN && direction != DIR_UP)
        {
            direction = newDirection;
        }
        else if (newDirection == DIR_LEFT && direction != DIR_RIGHT)
        {
            direction = newDirection;
        }
        else if (newDirection == DIR_RIGHT && direction != DIR_LEFT)
        {
            direction = newDirection;
        }
    }

    // Move the snake in the current direction
    bool move(point food)
    {
        for (int i = length - 1; i > 0; i--)
        {
            body[i] = body[i - 1];
        }
        switch (direction)
        {
        case DIR_UP:
            body[0].yCoord--;
            break;
        case DIR_DOWN:
            body[0].yCoord++;
            break;
        case DIR_RIGHT:
            body[0].xCoord++;
            break;
        case DIR_LEFT:
            body[0].xCoord--;
            break;
        }

        // Check if the snake bites itself
        for (int i = 1; i < length; i++)
        {
            if (body[0].xCoord == body[i].xCoord && body[0].yCoord == body[i].yCoord)
            {
                return false;
            }
        }

        // Check if the snake eats food
        if (food.xCoord == body[0].xCoord && food.yCoord == body[0].yCoord)
        {
            body[length] = point(body[length - 1].xCoord, body[length - 1].yCoord);
            length++;
        }

        return true;
    }
};

// Class to represent the game board
class Board
{
    Snake *snake;
    const char SNAKE_BODY = 'O';
    point food;
    const string FOOD = "o";
    int score;

public:
    Board()
    {
        spawnFood();
        snake = new Snake(10, 10);
        score = 0;
    }

    ~Board()
    {
        delete snake;
    }

    int getScore()
    {
        return score;
    }

    // Function to spawn food at a random position
    void spawnFood()
    {
        int x = rand() % consoleWidth;
        int y = rand() % consoleHeight;
        food = point(x, y);
    }

    // Display current score on the console
    void displayCurrentScore()
    {
        gotoxy(consoleWidth / 2, 0);
        cout << "Current Score : " << score;
    }

    // Function to move the cursor to a specific position on the console
    void gotoxy(int x, int y)
    {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    // Draw the snake and food on the console
    void draw()
    {
        system("cls");
        for (int i = 0; i < snake->getlength(); i++)
        {
            gotoxy(snake->body[i].xCoord, snake->body[i].yCoord);
            cout << SNAKE_BODY;
        }
        gotoxy(food.xCoord, food.yCoord);
        cout << FOOD;

        displayCurrentScore();
    }

    // Update the state of the board
    bool update()
    {
        bool isAlive = snake->move(food);
        if (!isAlive)
        {
            return false;
        }

        if (food.xCoord == snake->body[0].xCoord && food.yCoord == snake->body[0].yCoord)
        {
            score++;
            spawnFood();
        }
        return true;
    }

    // Get input from the user for changing the snake's direction
    void getInput()
    {
        if (kbhit())
        {
            int key = getch();
            if (key == 224) // Arrow keys have a prefix of 224
            {
                key = getch(); // Get the actual key
                switch (key)
                {
                case 72: // Arrow up
                    snake->changeDirection(DIR_UP);
                    break;
                case 75: // Arrow left
                    snake->changeDirection(DIR_LEFT);
                    break;
                case 80: // Arrow down
                    snake->changeDirection(DIR_DOWN);
                    break;
                case 77: // Arrow right
                    snake->changeDirection(DIR_RIGHT);
                    break;
                }
            }
        }
    }
};

int main()
{
    srand(time(0)); // Initialize random seed
    initScreen(); // Initialize console screen dimensions
    Board *board = new Board();
    while (board->update())
    {
        board->getInput();
        board->draw();
        Sleep(100); // Delay to control the speed of the game
    }

    // Game over, display final score
    system("cls");
    cout << "Game over" << endl;
    cout << "Final score is: " << board->getScore() << endl;
    delete board;
    return 0;
}
