#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Node 
{
    int x, y;
    char direction;
    Node* next;
};

struct Snake 
{
    Node* head;
    Node* tail;
};

int width = 0;
int height = 0;
char** field = nullptr;
Snake snake = { nullptr, nullptr };
int foodX = 0;
int foodY = 0;
int score = 0;
bool gameOver = false;

void Initialize() 
{
    field = new char* [height];
    for (int i = 0; i < height; ++i) 
        field[i] = new char[width];
    int startX = rand() % (width - 2) + 1;
    int startY = rand() % (height - 2) + 1;
    char directions[] = { 'W', 'A', 'S', 'D' };
    snake.head = new Node{ startX, startY, directions[rand() % 4], nullptr };
    snake.tail = snake.head;

    do 
    {
        foodX = rand() % (width - 2) + 1;
        foodY = rand() % (height - 2) + 1;
    }
    while (foodX == startX && foodY == startY);
}

void Input() 
{
    if (_kbhit()) 
    {
        char c = _getch();
        if (c == 'l' || c == 'L')
        {
            cout << "\nВыполнен выход" << endl;
            exit(0);
        }
        if ((c == 'w' || c == 'W') && snake.head->direction != 'S')
            snake.head->direction = 'W';
        else if ((c == 'a' || c == 'A') && snake.head->direction != 'D') 
            snake.head->direction = 'A';
        else if ((c == 's' || c == 'S') && snake.head->direction != 'W') 
            snake.head->direction = 'S';
        else if ((c == 'd' || c == 'D') && snake.head->direction != 'A') 
            snake.head->direction = 'D';
    }
}

void Move() 
{
    int nextX = snake.head->x, nextY = snake.head->y;
    switch (snake.head->direction) 
    {
        case 'W': nextY--; 
            break; 
        case 'A': nextX--; 
            break;
        case 'S': nextY++; 
            break; 
        case 'D': nextX++; 
            break;
    }
    if (nextX < 0 || nextX >= width || nextY < 0 || nextY >= height) 
    { 
        gameOver = true; 
        return; 
    }
    for (Node* p = snake.head; p; p = p->next)
        if (p->x == nextX && p->y == nextY) 
        { 
            gameOver = true; 
            return; 
        }

    Node* n = new Node{ nextX, nextY, snake.head->direction, snake.head };
    snake.head = n;

    if (nextX == foodX && nextY == foodY) 
    {
        score++;
        bool isPositionValid;
        do 
        {
            foodX = rand() % (width - 2) + 1; 
            foodY = rand() % (height - 2) + 1; 
            isPositionValid = true;
            for (Node* p = snake.head; p; p = p->next)
                if (p->x == foodX && p->y == foodY) 
                { 
                    isPositionValid = false;
                    break; 
                }
        } while (!isPositionValid);
    }
    else 
    {
        Node* p = snake.head;
        while (p->next->next) 
            p = p->next;
        delete p->next;
        p->next = nullptr;
        snake.tail = p;
    }
}

void Draw() {
    system("cls");
    int padding = (width * 2 - 8) / 2;
    for (int i = 0; i < padding; ++i) 
        cout << ' ';
    cout << "Счет: " << score << endl;

    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j) 
            field[i][j] = '.';
    field[foodY][foodX] = '*';

    Node* p = snake.head;
    while (p) 
    {
        if (p == snake.head) 
            field[p->y][p->x] = gameOver ? 'X' : '@';
        else if (p == snake.tail) 
            field[p->y][p->x] = 'O';
        else 
        {
            char c = p->direction, n = p->next->direction;            
            if (c == n) field[p->y][p->x] = (c == 'A' || c == 'D') ? '-' : '|';
            else field[p->y][p->x] = ((c == 'W' && n == 'D') || (c == 'D' && n == 'W') || (c == 'S' && n == 'A') || (c == 'A' && n == 'S')) ? '/' : '\\';
        }
        p = p->next;
    }

    for (int i = 0; i < height; ++i) 
    {
        for (int j = 0; j < width; ++j) cout << field[i][j] << " ";
        cout << endl;
    }
}

void Cleanup() 
{
    Node* p = snake.head;
    while (p) 
    { 
        Node* t = p; 
        p = p->next; 
        delete t; 
    }
    for (int i = 0; i < height; ++i) 
        delete[] field[i];
    delete[] field;
}

int main() 
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    srand(time(nullptr));
    cout << "Размер поля (мин. 5x5, макс. 50x50): ";
    cin >> width >> height;
    if (width < 5 || height < 5 || width > 50 || height > 50)
    {
        cout << "Минимальный размер поля - 5x5" << endl;
        cout << "Максимальный размер поля - 50x50" << endl;
        return 1;
    }
    Initialize();
    Sleep(1000);
    while (!gameOver) 
    {
        Input();
        Move();
        if (!gameOver) 
        { 
            Draw(); 
            Sleep(150); 
        }
    }
    Draw();
    Sleep(1000);
    cout << "\nGAME OVER! Счет: " << score << endl;
    Cleanup();
    return 0;
}