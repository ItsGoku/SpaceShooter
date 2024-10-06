#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

const int width = 20;
const int height = 20;
int x, y, bulletX, bulletY, enemyX, enemyY, score;
bool bulletActive, gameOver;

void Setup() {
    x = width / 2;
    y = height - 1;
    bulletX = 0;
    bulletY = -1;
    enemyX = rand() % width;
    enemyY = 0;
    score = 0;
    bulletActive = false;
    gameOver = false;
}

void Draw() {
    system("cls");
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                cout << "#";
            if (i == y && j == x)
                cout << "A";
            else if (i == bulletY && j == bulletX)
                cout << "-";
            else if (i == enemyY && j == enemyX)
                cout << "E";
            else
                cout << " ";
            if (j == width - 1)
                cout << "#";
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;
    cout << "Score: " << score << endl;
}

void Input() {
    if (_kbhit()) {
        char key = _getch();
        switch (key) {
            case 'a':
                if(x > 0) // Check if moving left won't go beyond the boundary
                    x--;
                break;
            case 'd':
                if(x < width - 1) // Check if moving right won't go beyond the boundary
                    x++;
                break;
            case ' ':
                if (!bulletActive) {
                    bulletX = x;
                    bulletY = y - 1;
                    bulletActive = true;
                }
                break;
            case 'q':
                gameOver = true;
                break;
        }
    }
}

void Logic() {
    if (bulletActive) {
        bulletY--;
        if (bulletY == 0)
            bulletActive = false;
    }

    if (enemyY == height - 1) {
        enemyX = rand() % width;
        enemyY = 0;
    }
    if (enemyY == bulletY && enemyX == bulletX) {
        score++;
        enemyX = rand() % width;
        enemyY = 0;
        bulletActive = false;
    }

    enemyY++;

    if (x < 0 || x >= width || y < 0 || y >= height)
        gameOver = true;
}

int main() {

    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(100); // Adjust game speed
    }
    cout << "Game Over!" << endl;
    cout << "Your score is: " << score << endl;
    return 0;
}
