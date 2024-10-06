#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <ctime>

using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;
const int SHIP_WIDTH = 50;
const int SHIP_HEIGHT = 50;
const int PROJECTILE_WIDTH = 5;
const int PROJECTILE_HEIGHT = 10;
const int MAX_PROJECTILES = 10;
const int ASTEROID_WIDTH = 50;
const int ASTEROID_HEIGHT = 50;
const int MAX_ASTEROIDS = 10;
const int SHOOT_DELAY = 200;

int shipX, shipY;
bool moveLeft, moveRight, shoot;
int asteroidX[MAX_ASTEROIDS], asteroidY[MAX_ASTEROIDS];
bool asteroidActive[MAX_ASTEROIDS];
int score = 0;
int projectilesX[MAX_PROJECTILES], projectilesY[MAX_PROJECTILES];
bool projectileActive[MAX_PROJECTILES];

void drawShip() {
    setfillstyle(SOLID_FILL, WHITE);
    bar(shipX, shipY, shipX + SHIP_WIDTH, shipY + SHIP_HEIGHT);
}

void moveShip() {
    if (moveLeft && shipX > 0) {
        shipX -= 5;
    }
    if (moveRight && shipX + SHIP_WIDTH < WIDTH) {
        shipX += 5;
    }
}

void drawProjectile(int index) {
    setfillstyle(SOLID_FILL, GREEN);
    bar(projectilesX[index], projectilesY[index], projectilesX[index] + PROJECTILE_WIDTH, projectilesY[index] + PROJECTILE_HEIGHT);
}

void moveProjectiles() {
    for (int i = 0; i < MAX_PROJECTILES; ++i) {
        if (projectileActive[i]) {
            projectilesY[i] -= 5;
            if (projectilesY[i] < 0) {
                projectileActive[i] = false;
            }
        }
    }
}

void drawAsteroid(int index) {
    setfillstyle(SOLID_FILL, RED);
    bar(asteroidX[index], asteroidY[index], asteroidX[index] + ASTEROID_WIDTH, asteroidY[index] + ASTEROID_HEIGHT);
}

void moveAsteroids() {
    for (int i = 0; i < MAX_ASTEROIDS; ++i) {
        if (asteroidActive[i]) {
            asteroidY[i] += 2;
            if (asteroidY[i] > HEIGHT) {
                asteroidActive[i] = false;
                score++;
            }
        }
    }
}

void drawScore() {
    setcolor(YELLOW);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    char scoreStr[20];
    sprintf(scoreStr, "Score: %d", score);
    outtextxy(10, 10, scoreStr);
}

bool checkCollision(int asteroidIndex, int projectileIndex) {
    if (projectileActive[projectileIndex] &&
        projectilesX[projectileIndex] + PROJECTILE_WIDTH >= asteroidX[asteroidIndex] &&
        projectilesX[projectileIndex] <= asteroidX[asteroidIndex] + ASTEROID_WIDTH &&
        projectilesY[projectileIndex] + PROJECTILE_HEIGHT >= asteroidY[asteroidIndex] &&
        projectilesY[projectileIndex] <= asteroidY[asteroidIndex] + ASTEROID_HEIGHT) {
        return true;
    }
    return false;
}

int main() {
    initwindow(WIDTH, HEIGHT, "Space Shooter");

    shipX = WIDTH / 2 - SHIP_WIDTH / 2;
    shipY = HEIGHT - SHIP_HEIGHT - 20;

    srand(time(0));

    for (int i = 0; i < MAX_ASTEROIDS; ++i) {
        asteroidX[i] = rand() % (WIDTH - ASTEROID_WIDTH);
        asteroidY[i] = -ASTEROID_HEIGHT - rand() % 500;
        asteroidActive[i] = true;
    }

    while (true) {
        cleardevice();

        drawShip();
        moveShip();

        for (int i = 0; i < MAX_PROJECTILES; ++i) {
            if (projectileActive[i]) {
                drawProjectile(i);
            }
        }

        for (int i = 0; i < MAX_ASTEROIDS; ++i) {
            if (asteroidActive[i]) {
                drawAsteroid(i);
                for (int j = 0; j < MAX_PROJECTILES; ++j) {
                    if (checkCollision(i, j)) {
                        projectileActive[j] = false;
                        asteroidActive[i] = false;
                        score += 10;
                        break; // Move to the next asteroid
                    }
                }
            }
        }

        drawScore();

        moveAsteroids();
        moveProjectiles();

        delay(10);

        if (kbhit()) {
            char key = getch();
            if (key == 27) // ASCII code for Escape key
                break;
            else if (key == 75) // ASCII code for left arrow key
                moveLeft = true;
            else if (key == 77) // ASCII code for right arrow key
                moveRight = true;
            else if (key == 32) { // ASCII code for spacebar
                for (int i = 0; i < MAX_PROJECTILES; ++i) {
                    if (!projectileActive[i]) {
                        projectileActive[i] = true;
                        projectilesX[i] = shipX + SHIP_WIDTH / 2 - PROJECTILE_WIDTH / 2;
                        projectilesY[i] = shipY;
                        break;
                    }
                }
            }
        } else {
            moveLeft = false;
            moveRight = false;
            shoot = false;
        }
    }

    closegraph();
    return 0;
}
