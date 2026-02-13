// Pong game using raylib
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// Global Variables
#define screenWidth 800
#define screenHeight 600
int leftScore = 0;
int rightScore = 0;
float paddleWidth = 20;
float paddleHeight = 100;
float ballRadius = 10;

// Entity Structures : Pong ball , Pong passles and Screen states
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

typedef enum {
    STATE_MENU,
    STATE_SOLO,
    STATE_DUO,
    STATE_PAUSE,
    STATE_GAMEOVER
} GameState;
GameState currentState = STATE_MENU;
GameState previousState = STATE_MENU;


// Function Prototypes
void init_game(circle_entity *ball, rectangle_entity *leftpong , rectangle_entity *rightpong, rectangle_entity *opponentpong); // game variables initialization
void pong_movement(rectangle_entity *leftpong , rectangle_entity *rightpong, GameState state);// pong movement handling
void rendererDuo(circle_entity *ball,rectangle_entity *leftpong , rectangle_entity *rightpong);// Duo mode rendering
void rendererSolo(circle_entity *ball,rectangle_entity *rightpong , rectangle_entity *opponentpong); // Solo mode rendering
void ball_movement(circle_entity *ball ,rectangle_entity *leftpong , rectangle_entity *rightpong,rectangle_entity *opponentpong, GameState state); // ball movement and collision handling
int collision(circle_entity *ball,rectangle_entity *pong); // collision detection between ball and paddle
void resetBall(circle_entity *ball); // ball position and velocity reset
void loadMenu(circle_entity *ball); // Main menu rendering and button handling
void renderPauseOverlay(); // Pause screen rendering
void GameOverScreen(); // Game over screen rendering and handling
void DrawScores(); // Score rendering 
void UpdateScore(circle_entity *ball); // Score updating based on ball position
void opponentLogic(circle_entity *ball, rectangle_entity *opponentpong); // AI opponent logic for solo mode
int btn_logic(); // Button logic for menu selection




int main(void)
{
    circle_entity circle;
    rectangle_entity left_pong_entity;
    rectangle_entity right_pong_entity;
    rectangle_entity opponent_pong;

    srand(time(NULL));

    InitWindow(screenWidth, screenHeight, "Pong Game");
    SetTargetFPS(60);
    init_game(&circle, &left_pong_entity , &right_pong_entity ,&opponent_pong);
    
    while (!WindowShouldClose())
    {
    // Handle pause/resume
    if (IsKeyPressed(KEY_P) && currentState != STATE_MENU) {
        if (currentState == STATE_PAUSE) {
            currentState = previousState;
        } else {
            previousState = currentState;
            currentState = STATE_PAUSE;
        }
    }

    // Update game logic only if not in menu, pause, or game over
if (currentState != STATE_MENU && currentState != STATE_PAUSE && currentState != STATE_GAMEOVER) {
    ball_movement(&circle, &left_pong_entity, &right_pong_entity, &opponent_pong, currentState);
    pong_movement(&left_pong_entity, &right_pong_entity, currentState);
    
    if (currentState == STATE_SOLO) {
        opponentLogic(&circle, &opponent_pong);  
    }
    
    UpdateScore(&circle);
}

    // Rendering
        BeginDrawing();
        ClearBackground(BLACK);

        switch(currentState)
        {
            case STATE_MENU:
              loadMenu(&circle);
              break;
            case STATE_SOLO:
              rendererSolo( &circle , &right_pong_entity,&opponent_pong);
              DrawText("press P to pause", 10, screenHeight - 60, 20, GRAY);
              break;
            case STATE_DUO:
              rendererDuo( &circle,&left_pong_entity,&right_pong_entity);
              DrawText("press P to pause", 10, screenHeight - 60, 20, GRAY);
              DrawScores();
              break;
            case STATE_PAUSE:
              if (IsKeyPressed(KEY_M) && currentState == STATE_PAUSE) 
              {
                currentState = STATE_MENU;
                resetBall(&circle);  // Reset when going back to menu
              }
              if (previousState == STATE_SOLO) 
              {
                rendererSolo(&circle, &right_pong_entity , &opponent_pong);
              } else if (previousState == STATE_DUO) 
              {
                rendererDuo(&circle, &left_pong_entity, &right_pong_entity);
              }
              renderPauseOverlay();
              break;
            case STATE_GAMEOVER:
              GameOverScreen();
              break;
            default:
              loadMenu(&circle);
              break; 
        }
        EndDrawing();

        if (IsKeyPressed(KEY_ESCAPE)) {
            break;
        };
    }

    CloseWindow();
    return 0;
}

void init_game(circle_entity *ball, rectangle_entity *leftpong , rectangle_entity *rightpong , rectangle_entity *opponentpong)
{



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

    //Opponent Pong (For Solo Mode)
    opponentpong -> width = paddleWidth;
    opponentpong -> height = paddleHeight;
    opponentpong -> y = screenHeight /2;
    opponentpong -> x = 10;
    opponentpong -> speed = 5.0f ;


    // Initialize ball
    ball -> x = screenWidth / 2;
    ball->y = screenHeight / 2;
    ball->radius = ballRadius;

    float speed = 8.0f;

    float angle = ((float)rand() / RAND_MAX * PI / 2) - PI / 4;
    int direction = (rand() % 2 == 0) ? 1 : -1;
    ball->velocity_x = speed * cos(angle) * direction;
    ball->velocity_y = speed * sin(angle);
}


void pong_movement(rectangle_entity *leftpong, rectangle_entity *rightpong, GameState state)
{
    if (state == STATE_DUO) {
        if (IsKeyDown(KEY_W)) leftpong->y -= leftpong->speed;
        if (IsKeyDown(KEY_S)) leftpong->y += leftpong->speed;
        // boundary checks for left...
    }
    
    if (IsKeyDown(KEY_UP)) rightpong->y -= rightpong->speed;
    if (IsKeyDown(KEY_DOWN)) rightpong->y += rightpong->speed;

    // boundary checks for right...
        if (leftpong->y < 0) leftpong->y = 0;
    if (leftpong->y + leftpong->height > screenHeight) {
        leftpong->y = screenHeight - leftpong->height;
    }

    if (rightpong->y < 0) rightpong->y = 0;
    if (rightpong->y + rightpong->height > screenHeight) {
        rightpong->y = screenHeight - rightpong->height;
    }
}

void rendererDuo(circle_entity *ball,rectangle_entity *leftpong , rectangle_entity *rightpong)
{ 
    DrawRectangle(leftpong->x, leftpong->y, leftpong->width, leftpong->height, DARKGRAY);
    DrawCircle(ball->x, ball->y, ball->radius, RED);
    DrawRectangle(rightpong->x, rightpong->y, rightpong->width, rightpong->height, DARKGRAY );
    for (int i = 0; i < screenHeight; i += 20) {
        DrawRectangle(screenWidth / 2 - 2, i, 4, 10, DARKGRAY);
    }
    DrawText("W/S - Left Paddle", 10, screenHeight - 40, 20, GRAY);
    DrawText("UP/DOWN - Right Paddle", screenWidth - 230, screenHeight - 40, 20, GRAY);
    DrawScores();
}

void rendererSolo(circle_entity *ball,rectangle_entity *pong , rectangle_entity *opponentpong)
{
    DrawRectangle(pong->x, pong->y, pong->width, pong->height, DARKGRAY);
    DrawRectangle(opponentpong->x, opponentpong->y, opponentpong->width, opponentpong->height, DARKGRAY);
    DrawCircle(ball->x, ball->y, ball->radius, RED);
    for (int i = 0; i < screenHeight; i += 20) {
        DrawRectangle(screenWidth / 2 - 2, i, 4, 10, DARKGRAY);
    }
    DrawText("Up arrow/Down arrow - Paddle", 10, screenHeight - 40, 20, GRAY);
    DrawScores();
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

void ball_movement(circle_entity *ball ,rectangle_entity *leftpong , rectangle_entity *rightpong,rectangle_entity *opponentpong, GameState state)
{
    ball->x += ball->velocity_x;
    ball->y += ball->velocity_y;
    switch(state)
    {
        case STATE_SOLO:
          if (collision(ball , opponentpong)){
            ball->velocity_x = -ball->velocity_x;
            ball->x = leftpong->x + leftpong->width + ball->radius;
          }
          break;
        case STATE_DUO:

            if (collision(ball , leftpong))
            {
            ball->velocity_x = -ball->velocity_x;
            ball->x = leftpong->x + leftpong->width + ball->radius;
            };
    
          break;
    }


    if (collision(ball , rightpong))
    {
        ball->velocity_x = -ball->velocity_x;
        ball->x = rightpong->x - ball->radius;
    };

    if (ball->y - ball->radius <= 0 || ball->y + ball->radius >= screenHeight) 
    {
            ball->velocity_y = -ball->velocity_y;
        
            if (ball->y - ball->radius < 0) {
                ball->y = ball->radius;
            }
            if (ball->y + ball->radius > screenHeight) {
                ball->y = screenHeight - ball->radius;
            }
    };

}

void resetBall(circle_entity *ball)
{
    ball->x = screenWidth / 2;
    ball->y = screenHeight / 2;
    ball->radius = 10;

    float speed = 8.0f;
    float angle = ((float)rand() / RAND_MAX * PI / 2) - PI / 4;
    int direction = (rand() % 2 == 0) ? 1 : -1;

    ball->velocity_x = speed * cos(angle) * direction;
    ball->velocity_y = speed * sin(angle);
}

void loadMenu(circle_entity *ball) {
   
    DrawRectangle(250, 450, 100, 50, DARKBLUE);
    DrawText("Solo",260, 460, 30, WHITE);
    DrawRectangle(450, 450, 100, 50, DARKBLUE);
    DrawText("Duo", 460, 460 , 30, WHITE);
    DrawText("PONG GAME", screenWidth / 2 - 100, screenHeight / 2 - 100, 40, WHITE);
    DrawText("Press ESC to Exit", screenWidth / 2 - 120, screenHeight / 2, 20, GRAY);

    switch(btn_logic()){
        case 1:
            currentState = STATE_SOLO;
            resetBall(ball);
            break;
        case 2:
            currentState = STATE_DUO;
            resetBall(ball);
            break;
        default:
            currentState = STATE_MENU;
            break;
    }

}

int btn_logic(){
    Vector2 mousePoint = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(mousePoint, (Rectangle){250, 450, 100, 50}))
        {
            return 1; // Solo mode
        }
        if (CheckCollisionPointRec(mousePoint, (Rectangle){450, 450, 100, 50}))
        {
            return 2; // Duo mode
        }
    }
    return 0;
}

void renderPauseOverlay() {
    // Semi-transparent dark overlay
    DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 150});
    
    // Pause text
    DrawText("PAUSED", screenWidth/2 - 80, screenHeight/2 - 50, 50, WHITE);
    DrawText("Press P to Resume", screenWidth/2 - 120, screenHeight/2 + 20, 25, LIGHTGRAY);
    DrawText("Press M for Menu", screenWidth/2 - 110, screenHeight/2 + 60, 25, LIGHTGRAY);
}

void GameOverScreen() {
    DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 200});
    DrawText("GAME OVER", screenWidth/2 - 100, screenHeight/2 - 50, 50, RED);
    DrawText("Press M for Menu", screenWidth/2 - 110, screenHeight/2 + 20, 25, LIGHTGRAY);
    if(IsKeyPressed(KEY_M)) {
        currentState = STATE_MENU;
        leftScore = 0;
        rightScore = 0;
    }
}

void DrawScores() {
    DrawText(TextFormat("%d", leftScore), screenWidth / 4, 20, 40, WHITE);
    DrawText(TextFormat("%d", rightScore), 3 * screenWidth / 4, 20, 40, WHITE);
}

void UpdateScore(circle_entity *ball) {
    // Check if ball went off left edge (right player scores)
    if (ball->x + ball->radius < 0) 
    {
        rightScore++;
        resetBall(ball);
        // Check for game over
        if (leftScore >= 10 || rightScore >= 10) {
            currentState = STATE_GAMEOVER;
        }
    }
    
    // Check if ball went off right edge (left player scores)
    if (ball->x + ball->radius > screenWidth) 
    {
        leftScore++;
        resetBall(ball);
        
        // Check for game over
        if (leftScore >= 10 || rightScore >= 10) {
            currentState = STATE_GAMEOVER;
        }
    }
}


// opponent AI logic for solo mode

void opponentLogic(circle_entity *ball, rectangle_entity *opponentpong) {
    // Only react when ball is coming toward AI
    if (ball->velocity_x < 0) {
        // Predict where ball will be
        float timeToReach = (ball->x - opponentpong->x) / fabs(ball->velocity_x);
        float predictedY = ball->y + (ball->velocity_y * timeToReach);
        
        // Clamp prediction to screen bounds
        if (predictedY < 0) predictedY = 0;
        if (predictedY > screenHeight) predictedY = screenHeight;
        
        float paddleCenter = opponentpong->y + opponentpong->height / 2;
        float distance = predictedY - paddleCenter;
        
        if (distance > 10) {
            opponentpong->y += opponentpong->speed * 0.7;  // Slower = easier
        } else if (distance < -10) {
            opponentpong->y -= opponentpong->speed * 0.7;
        }
    }
    if (opponentpong->y < 0) {
        opponentpong->y = 0;
    }
    if (opponentpong->y + opponentpong->height > screenHeight) {
        opponentpong->y = screenHeight - opponentpong->height;
    }
}