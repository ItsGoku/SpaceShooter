import pygame
import random

# Initialize Pygame
pygame.init()

# Set up the screen
screen_width = 800
screen_height = 600
screen = pygame.display.set_mode((screen_width, screen_height))
pygame.display.set_caption("Space Shooter")

# Colors
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
RED = (255, 0, 0)

# Player
player_width = 64
player_height = 64
player_x = (screen_width - player_width) // 2
player_y = screen_height - player_height - 20
player_speed = 5

# Enemies
enemy_width = 64
enemy_height = 64
enemy_speed = 3
enemies = []

# Bullets
bullet_width = 8
bullet_height = 32
bullet_speed = 7
bullets = []

# Score
score = 0
font = pygame.font.Font(None, 36)

# Functions
def draw_player(x, y):
    pygame.draw.rect(screen, WHITE, (x, y, player_width, player_height))

def draw_enemy(x, y):
    pygame.draw.rect(screen, RED, (x, y, enemy_width, enemy_height))

def draw_bullet(x, y):
    pygame.draw.rect(screen, WHITE, (x, y, bullet_width, bullet_height))

def collision(obj1_x, obj1_y, obj1_width, obj1_height, obj2_x, obj2_y, obj2_width, obj2_height):
    if (obj1_x < obj2_x + obj2_width and
        obj1_x + obj1_width > obj2_x and
        obj1_y < obj2_y + obj2_height and
        obj1_y + obj1_height > obj2_y):
        return True
    return False

# Game loop
running = True
clock = pygame.time.Clock()

while running:
    screen.fill(BLACK)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_SPACE:
                bullet_x = player_x + player_width // 2 - bullet_width // 2
                bullet_y = player_y
                bullets.append([bullet_x, bullet_y])

    # Player movement
    keys = pygame.key.get_pressed()
    if keys[pygame.K_LEFT] and player_x > 0:
        player_x -= player_speed
    if keys[pygame.K_RIGHT] and player_x < screen_width - player_width:
        player_x += player_speed

    # Enemy spawning
    if len(enemies) < 5:
        enemy_x = random.randint(0, screen_width - enemy_width)
        enemy_y = random.randint(-500, -enemy_height)
        enemies.append([enemy_x, enemy_y])

    # Update enemies
    for enemy in enemies:
        enemy[1] += enemy_speed
        if enemy[1] > screen_height:
            enemies.remove(enemy)
            score += 1

    # Bullet movement and collision
    for bullet in bullets:
        bullet[1] -= bullet_speed
        if bullet[1] < 0:
            bullets.remove(bullet)
        for enemy in enemies:
            if collision(bullet[0], bullet[1], bullet_width, bullet_height,
                         enemy[0], enemy[1], enemy_width, enemy_height):
                bullets.remove(bullet)
                enemies.remove(enemy)
                score += 10

    # Drawing
    draw_player(player_x, player_y)
    for enemy in enemies:
        draw_enemy(enemy[0], enemy[1])
    for bullet in bullets:
        draw_bullet(bullet[0], bullet[1])

    # Display score
    score_text = font.render("Score: " + str(score), True, WHITE)
    screen.blit(score_text, (10, 10))

    pygame.display.update()
    clock.tick(60)

# Quit
pygame.quit()