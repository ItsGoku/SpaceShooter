#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

// Player position
GLfloat playerX = WIDTH / 2, playerY = 50;
GLfloat playerSpeed = 5.0f;

// Enemy position
GLfloat enemyX = WIDTH / 2, enemyY = HEIGHT - 100;
GLfloat enemySpeed = 2.0f;

// Projectile position
GLfloat projectileX, projectileY;
bool projectileLaunched = false;

// Keyboard input callback
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        projectileLaunched = true;
        projectileX = playerX; // Set projectile position to player's position
        projectileY = playerY + 20; // Adjust position so the projectile starts above the player
    }
}

// Check for collision between two rectangles
bool isCollision(GLfloat obj1x, GLfloat obj1y, GLfloat obj1Width, GLfloat obj1Height,
    GLfloat obj2x, GLfloat obj2y, GLfloat obj2Width, GLfloat obj2Height) {
    return (obj1x < obj2x + obj2Width &&
        obj1x + obj1Width > obj2x &&
        obj1y < obj2y + obj2Height &&
        obj1y + obj1Height > obj2y);
}

// Draw a polygon representing an asteroid
void drawAsteroid(GLfloat x, GLfloat y, GLfloat radius, GLint numVertices) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < numVertices; ++i) {
        GLfloat angle = 2.0f * 3.14 * i / numVertices;
        GLfloat newX = x + radius * cos(angle);
        GLfloat newY = y + radius * sin(angle);
        glVertex2f(newX, newY);
    }
    glEnd();
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed!" << std::endl;
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Space Shooter", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Set keyboard callback
    glfwSetKeyCallback(window, key_callback);

    // Set viewport and projection matrix
    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, WIDTH, 0.0, HEIGHT, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Keyboard input handling
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && playerX > 0)
            playerX -= playerSpeed;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && playerX < WIDTH)
            playerX += playerSpeed;

        // Draw player's spaceship
        glLoadIdentity();
        glTranslatef(playerX, playerY, 0.0f);
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(-10.0f, 0.0f);
        glVertex2f(0.0f, 20.0f);
        glVertex2f(10.0f, 0.0f);
        glEnd();

        // Draw enemy (asteroid)
        glLoadIdentity();
        glTranslatef(enemyX, enemyY, 0.0f);
        glColor3f(0.5f, 0.5f, 0.5f); // Gray color for asteroid
        drawAsteroid(0.0f, 0.0f, 15.0f, 8); // Asteroid with 8 vertices and radius of 15
        glEnd();

        // Move enemy (asteroid) downwards
        enemyY -= enemySpeed;
        if (enemyY < 0) {
            enemyY = HEIGHT - 100; // Reset enemy position
            enemyX = rand() % (WIDTH - 20); // Randomize enemy position
        }

        // Check for collision between projectile and enemy
        if (projectileLaunched && isCollision(projectileX, projectileY, 4.0f, 10.0f,
            enemyX - 15.0f, enemyY - 15.0f, 30.0f, 30.0f)) {
            // Collision detected, reset both projectile and enemy
            projectileLaunched = false;
            enemyY = HEIGHT - 100;
            enemyX = rand() % (WIDTH - 20);
        }

        // Draw projectile if launched
        if (projectileLaunched) {
            glLoadIdentity();
            glTranslatef(projectileX, projectileY, 0.0f);
            glBegin(GL_TRIANGLES);
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex2f(-2.0f, -5.0f);
            glVertex2f(0.0f, 5.0f);
            glVertex2f(2.0f, -5.0f);
            glEnd();

            projectileY += 5; // Move projectile upwards
            if (projectileY > HEIGHT) // If projectile out of screen, reset
                projectileLaunched = false;
        }

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();
    return 0;
}
