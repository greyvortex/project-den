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

typedef enum {
    STATE_MENU,
    STATE_SOLO,
    STATE_DUO,
    STATE_PAUSE,
    STATE_GAMEOVER
} GameState;

GameState currentState = STATE_MENU;
GameState previousState = STATE_MENU;

void init_game(circle_entity *ball, rectangle_entity *leftpong , rectangle_entity *rightpong );
void rendererDuo(circle_entity *ball,rectangle_entity *leftpong , rectangle_entity *rightpong);
void rendererSolo(circle_entity *ball,rectangle_entity *rightpong);
void pong_movement(rectangle_entity *leftpong , rectangle_entity *rightpong, GameState state);
int collision(circle_entity *ball,rectangle_entity *pong);
void ball_movement(circle_entity *ball ,rectangle_entity *leftpong , rectangle_entity *rightpong,GameState state);
void resetBall(circle_entity *ball);
void loadMenu(GameState *state , circle_entity *ball);
int btn_logic();
void renderPauseOverlay();


int main(void)
{
    circle_entity circle;
    rectangle_entity left_pong_entity;
    rectangle_entity right_pong_entity;

    srand(time(NULL));


    InitWindow(screenWidth, screenHeight, "Pong Game");
    SetTargetFPS(60);
    init_game(&circle, &left_pong_entity , &right_pong_entity );
    
    while (!WindowShouldClose())
    {
    if (IsKeyPressed(KEY_P) && currentState != STATE_MENU) {
        if (currentState == STATE_PAUSE) {
            currentState = previousState;
        } else {
            previousState = currentState;
            currentState = STATE_PAUSE;
        }
    }

    if (currentState != STATE_MENU && currentState != STATE_PAUSE) {
        ball_movement(&circle, &left_pong_entity, &right_pong_entity, currentState);
        pong_movement(&left_pong_entity, &right_pong_entity, currentState);
    }
       

        BeginDrawing();
        ClearBackground(BLACK);
        //rendererDuo( &circle,&left_pong_entity,&right_pong_entity);
        switch(currentState){
            case STATE_MENU:
              loadMenu(&currentState, &circle);
              break;
            case STATE_SOLO:
              rendererSolo( &circle , &right_pong_entity);
              DrawText("press P to pause", 10, screenHeight - 60, 20, GRAY);
              break;
            case STATE_DUO:
              rendererDuo( &circle,&left_pong_entity,&right_pong_entity);
              DrawText("press P to pause", 10, screenHeight - 60, 20, GRAY);
              break;
            default:
              loadMenu(&currentState, &circle);
              break; 
            case STATE_PAUSE:
            if (IsKeyPressed(KEY_M) && currentState == STATE_PAUSE) {
            currentState = STATE_MENU;
            resetBall(&circle);  // Reset when going back to menu
        }
            if (previousState == STATE_SOLO) {
                rendererSolo(&circle, &right_pong_entity);
            } else if (previousState == STATE_DUO) {
                rendererDuo(&circle, &left_pong_entity, &right_pong_entity);
            }
            renderPauseOverlay();
            break;
        }
        printf("%d %d\n", currentState, previousState);

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
}

void rendererSolo(circle_entity *ball,rectangle_entity *pong)
{
    DrawRectangle(pong->x, pong->y, pong->width, pong->height, DARKGRAY);
    DrawCircle(ball->x, ball->y, ball->radius, RED);
    for (int i = 0; i < screenHeight; i += 20) {
        DrawRectangle(screenWidth / 2 - 2, i, 4, 10, DARKGRAY);
    }
    DrawText("Up arrow/Down arrow - Paddle", 10, screenHeight - 40, 20, GRAY);
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

void ball_movement(circle_entity *ball ,rectangle_entity *leftpong , rectangle_entity *rightpong,GameState state)
{
    ball->x += ball->velocity_x;
    ball->y += ball->velocity_y;
    switch(state){
        case STATE_SOLO:
        if (ball->x - ball->radius <= 0){
            ball->velocity_x = -ball->velocity_x;
            ball->x = ball->radius;
        }
          break;
        case STATE_DUO:

            if (collision(ball , leftpong))
            {
            ball->velocity_x = -ball->velocity_x;
            ball->x = leftpong->x + leftpong->width + ball->radius;
            float hitPos = (ball->y - (leftpong->y + leftpong->height / 2)) / (leftpong->height / 2);
            ball->velocity_y += hitPos * 2;
            };
    
          break;
    }


    if (collision(ball , rightpong)){
        ball->velocity_x = -ball->velocity_x;
        ball->x = rightpong->x - ball->radius;
        float hitPos = (ball->y - (rightpong->y + rightpong->height / 2)) / (rightpong->height / 2);
        ball->velocity_y += hitPos * 2;
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

void loadMenu(GameState *state,circle_entity *ball) {
   
    DrawRectangle(250, 450, 100, 50, DARKBLUE);
    DrawText("Solo",260, 460, 30, WHITE);
    DrawRectangle(450, 450, 100, 50, DARKBLUE);
    DrawText("Duo", 460, 460 , 30, WHITE);
    DrawText("PONG GAME", screenWidth / 2 - 100, screenHeight / 2 - 100, 40, WHITE);
    DrawText("Press ESC to Exit", screenWidth / 2 - 120, screenHeight / 2, 20, GRAY);

    switch(btn_logic()){
        case 1:
        *state = STATE_SOLO;
        resetBall(ball);
        break;
        case 2:
        *state = STATE_DUO;
        resetBall(ball);
        break;
        default:
        *state = STATE_MENU;
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
