#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int screenWidth = 800;
int screenHeight = 600;

typedef struct{
    float x;
    float y;
    float radius;
    float velocity_x;
    float velocity_y;

} circle_entity;

typedef struct
{
    float x;
    float y;
    float width;
    float height;
    float speed;
} rectangle_entity;


void init_game(circle_entity *ball, rectangle_entity *leftpong , rectangle_entity *rightpong);
void renderer(circle_entity *ball,rectangle_entity *leftpong , rectangle_entity *rightpong);
void pong_movement(rectangle_entity *leftpong , rectangle_entity *rightpong);
int collision(circle_entity *ball,rectangle_entity *pong);
void ball_movement(circle_entity *ball ,rectangle_entity *leftpong , rectangle_entity *rightpong);
void resetBall(circle_entity *ball);



int main(void)
{
    circle_entity circle;
    rectangle_entity left_pong_entity;
    rectangle_entity right_pong_entity;

    srand(time(NULL));


    InitWindow(screenWidth, screenHeight, "Pong Game");
    SetTargetFPS(60);
    init_game(&circle, &left_pong_entity , &right_pong_entity);
        int dir = 20;
    while (!WindowShouldClose())
    {
        pong_movement(&left_pong_entity,&right_pong_entity);
        ball_movement(&circle , &left_pong_entity , &right_pong_entity);

        BeginDrawing();
        ClearBackground(BLACK);
        renderer( &circle,&left_pong_entity,&right_pong_entity);
        EndDrawing();

        if (IsKeyPressed(KEY_ESCAPE)) {
            break;
        };
    }

    CloseWindow();
    return 0;
}

void init_game(circle_entity *ball, rectangle_entity *leftpong , rectangle_entity *rightpong)
{
    // Global Variables
    float paddleWidth = 20;
    float paddleHeight = 100;
    float ballRadius = 10;


    // Initialize left paddle
    leftpong -> width = paddleWidth;
    leftpong -> height = paddleHeight;
    leftpong -> y = screenHeight /2;
    leftpong -> x = 10;
    leftpong -> speed = 6.0f ;

    // Initialize right paddle
    rightpong -> width = paddleWidth;
    rightpong -> height = paddleHeight;
    rightpong -> y = screenHeight /2;
    rightpong -> x = screenWidth - 25;
    rightpong -> speed = 6.0f ;

    // Initialize ball
    ball -> x = screenWidth / 2;
    ball->y = screenHeight / 2;
    ball->radius = ballRadius;

    float speed = 5.0f;
    float angle = ((float)rand() / RAND_MAX * PI / 2) - PI / 4;
    int direction = (rand() % 2 == 0) ? 1 : -1;
    ball->velocity_x = speed * cos(angle) * direction;
    ball->velocity_y = speed * sin(angle);
}

void pong_movement(rectangle_entity *leftpong , rectangle_entity *rightpong)
{
    if (IsKeyDown(KEY_W)) leftpong->y -= leftpong->speed;
    if (IsKeyDown(KEY_S)) leftpong->y += leftpong->speed;
    if (IsKeyDown(KEY_UP)) rightpong->y -= rightpong->speed;
    if (IsKeyDown(KEY_DOWN)) rightpong->y += rightpong->speed;

    if (leftpong->y < 0) leftpong->y = 0;
    if (leftpong->y + leftpong->height > screenHeight) {
        leftpong->y = screenHeight - leftpong->height;
    }

    if (rightpong->y < 0) rightpong->y = 0;
    if (rightpong->y + rightpong->height > screenHeight) {
        rightpong->y = screenHeight - rightpong->height;
    }
}

void renderer(circle_entity *ball,rectangle_entity *leftpong , rectangle_entity *rightpong)
{
    DrawRectangle(leftpong->x, leftpong->y, leftpong->width, leftpong->height, DARKGRAY);
    DrawCircle(ball->x, ball->y, ball->radius, RED);
    DrawRectangle(rightpong->x, rightpong->y, rightpong->width, rightpong->height, DARKGRAY );
    for (int i = 0; i < screenHeight; i += 20) {
        DrawRectangle(screenWidth / 2 - 2, i, 4, 10, DARKGRAY);
    }
    DrawText("W/S - Left Paddle", 10, screenHeight - 40, 20, GRAY);
    DrawText("UP/DOWN - Right Paddle", screenWidth - 230, screenHeight - 40, 20, GRAY);
}

int collision(circle_entity *ball,rectangle_entity *pong)
{

    return (
        ball-> x + ball-> radius >= pong ->x &&
        ball-> x - ball-> radius <= pong ->x + pong -> width &&
        ball-> y + ball-> radius >= pong ->y &&
        ball-> y - ball-> radius <= pong ->y + pong -> height
    );
}

void ball_movement(circle_entity *ball ,rectangle_entity *leftpong , rectangle_entity *rightpong)
{
    ball->x += ball->velocity_x;
    ball->y += ball->velocity_y;

    if (ball->y - ball->radius <= 0 || ball->y + ball->radius >= screenHeight) {
        ball->velocity_y = -ball->velocity_y;
        
        if (ball->y - ball->radius < 0) {
            ball->y = ball->radius;
        }
        if (ball->y + ball->radius > screenHeight) {
            ball->y = screenHeight - ball->radius;
        }
    };

    if (collision(ball , leftpong)){
        ball->velocity_x = -ball->velocity_x;
        ball->x = leftpong->x + leftpong->width + ball->radius;
        float hitPos = (ball->y - (leftpong->y + leftpong->height / 2)) / (leftpong->height / 2);
        ball->velocity_y += hitPos * 2;
    };

    if (collision(ball , rightpong)){
        ball->velocity_x = -ball->velocity_x;
        ball->x = rightpong->x - ball->radius;
        float hitPos = (ball->y - (rightpong->y + rightpong->height / 2)) / (rightpong->height / 2);
        ball->velocity_y += hitPos * 2;
    };
    if (ball->x < 0) {
        resetBall(ball);
    }

    if (ball->x > screenWidth) {
        resetBall(ball);
    }

}

void resetBall(circle_entity *ball)
{
    ball->x = screenWidth / 2;
    ball->y = screenHeight / 2;
    ball->radius = 10;

    float speed = 5.0f;
    float angle = ((float)rand() / RAND_MAX * PI / 2) - PI / 4;
    int direction = (rand() % 2 == 0) ? 1 : -1;
    ball->velocity_x = speed * cos(angle) * direction;
    ball->velocity_y = speed * sin(angle);
}