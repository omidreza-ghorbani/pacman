#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
int currentDirection = 0;
float starTimer = 0.0f;      // تایمر برای انیمیشن ستاره‌ها
int currentStarFrame = 0;     // شمارنده فریم فعلی (۰، ۱، ۲)
#define STAR_FRAME_COUNT 3     // تعداد فریم‌های انیمیشن ستاره
int max_ghost = 7;
bool inMenu = true;
#define STATE_MENU 0
#define STATE_PLAYING 1
#define STATE_GAME_OVER 2
#define STATE_RECORDS 3
#define STATE_SETTINGS 4
#define STATE_REPLAY 5
int selectedMapOption = 0;

Music menu_music;
float currentGhostSpeed = 100.0f;


int selectedLevelOption = 1;
#define MAX_LEVELS 3

float ghostSpeeds[MAX_LEVELS] = {100.0f, 150.0f, 250.0f};

Sound mashroom_sound;
Sound star;
Sound cherry_sound;
Sound apple_sound;
Sound chaleh;
Sound ghost_with_cherry;
Sound ghost_normal;
Sound click;
Sound game_over;
Sound pepper_sound;

bool hasReplay = false;
int totalReplayFrames = 0;

#define ROWS 20
#define COLS 31
#define TILE_SIZE 30
int user_key = 0;
float pacmanX = 1 * TILE_SIZE + TILE_SIZE / 2.0f;
float pacmanY = 1 * TILE_SIZE + TILE_SIZE / 2.0f;
int pacmanCurrentRow = 1, pacmanCurrentCol = 1;


char pacmanBoard2[ROWS][COLS] = {
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '@', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '*', '*', '*', '*', '*', '.', '&', '&', '&', '&', '&', '&', '&', '&', '&', '&', '&', '&', '&', '&', '&', '.', '*', '*', '*', '*', '*', '.', '#'},
        {'#', '.', '*', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '*', '.', '#'}, //1
        {'#', '.', '*', '.', '&', '.', '&', '.', '#', '#', '#', '.', '#', '#', '#', '.', '#', '.', '*', '.', '#', '.', '*', '.', '&', '.', '%', '.', '*', '.', '#'}, //2
        {'#', '.', '.', '.', '.', '.', '&', '.', '#', '.', '.', '.', '#', '-', '#', '.', '#', '.', '.', '.', '#', '.', '.', '.', '&', '.', '.', '.', '.', '.', '#'}, //3
        {'#', '.', '*', '*', '*', '.', '&', '.', '#', '#', '#', '.', '#', '#', '#', '.', '#', '.', '*', '.', '#', '.', '*', '.', '&', '.', '*', '*', '*', '.', '#'}, //4
        {'#', '.', '.', '.', '*', '.', '&', '.', '#', '.', '.', '.', '#', '.', '#', '.', '#', '.', '.', '.', '#', '.', '.', '.', '&', '.', '*', '.', '.', '.', '#'}, //5
        {'#', '.', '&', '.', '*', '.', '&', '.', '#', '.', '*', '.', '#', '.', '#', '.', '#', '#', '#', '.', '#', '#', '#', '.', '&', '.', '*', '.', '&', '.', '#'}, //6
        {'!', '.', '&', '.', '.', '.', '&', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '&', '.', '.', '.', '&', '.', '^'}, //7
        {'#', '.', '&', '.', '*', '.', '&', '.', '#', '#', '#', '.', '#', '#', '#', '.', '#', '#', '#', '.', '#', '.', '#', '.', '&', '.', '*', '.', '&', '.', '#'}, //8
        {'#', '.', '.', '.', '*', '.', '&', '.', '.', '-', '#', '.', '#', '-', '#', '.', '.', '.', '#', '.', '#', '.', '#', '.', '&', '.', '*', '.', '.', '.', '#'}, //9
        {'#', '.', '*', '*', '*', '.', '&', '.', '#', '#', '#', '.', '#', '-', '#', '.', '#', '#', '#', '.', '#', '#', '#', '.', '&', '.', '*', '*', '*', '.', '#'}, //10
        {'#', '.', '.', '.', '.', '.', '&', '.', '#', '-', '.', '.', '#', '-', '#', '.', '#', '.', '.', '.', '.', '.', '#', '.', '&', '.', '.', '.', '.', '.', '#'}, //11
        {'#', '.', '*', '.', '%', '.', '&', '.', '#', '#', '#', '.', '#', '#', '#', '.', '#', '#', '#', '.', '*', '.', '#', '.', '&', '.', '%', '.', '*', '.', '#'}, //12
        {'#', '.', '*', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '*', '.', '#'},
        {'#', '.', '*', '*', '*', '*', '*', '.', '&', '&', '&', '&', '&', '&', '&', '&', '&', '&', '&', '&', '&', '&', '&', '.', '*', '*', '*', '*', '*', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '?', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}, //14
        {'%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%'}, //15
};

char pacmanBoard1[ROWS][COLS] = {
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '*', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '*', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '#', '#', '#', '#', '.', '*', '.', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '.', '*', '.', '#', '#', '#', '#', '.', '#'},
        {'#', '.', '#', '.', 'e', '#', '.', '*', '.', '#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#', '.', '*', '.', '#', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '#', '#', '.', '*', '.', '.', '.', '*', '*', '*', '*', '*', '*', '*', '*', '*', '.', '.', '.', '*', '.', '#', '#', '.', '#', '.', '#'},
        {'#', '.', '#', '.', '.', '.', '.', '.', '.', '#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#', '.', '.', '.', '.', '.', '.', '#', '.', '#'},
        {'#', '.', '#', '#', '#', '#', '.', '#', '.', '#', '#', '#', '#', '#', '.', '*', '.', '#', '#', '#', '#', '#', '.', '#', '.', '#', '#', '#', '#', '.', '#'},
        {'#', '.', '.', '.', '.', '#', '.', '#', '.', '.', '.', '.', '.', '#', '.', '*', '.', '#', '.', '.', '.', '.', '.', '#', '.', '#', '.', '.', '.', '.', '#'},
        {'#', '.', '#', '#', '#', '#', '.', '#', '#', '#', '#', '#', '.', '#', '.', '*', '.', '#', '.', '#', '#', '#', '#', '#', '.', '#', '#', '#', '#', '.', '#'},
        {'!', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '*', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '^'},
        {'#', '.', '#', '#', '#', '#', '.', '#', '#', '#', '#', '#', '.', '#', '.', '*', '.', '#', '.', '#', '#', '#', '#', '#', '.', '#', '#', '#', '#', '.', '#'},
        {'#', '.', '.', '.', '.', '#', '.', '#', '.', '.', '.', '.', '.', '#', '.', '*', '.', '#', '.', '.', '.', '.', '.', '#', '.', '#', '.', '.', '.', '.', '#'},
        {'#', '.', '#', '#', '#', '#', '.', '#', '.', '#', '#', '#', '#', '#', '.', '*', '.', '#', '#', '#', '#', '#', '.', '#', '.', '#', '#', '#', '#', '.', '#'},
        {'#', '.', '#', '.', '.', '.', '.', '.', '.', '#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#', '.', '.', '.', '.', '.', '.', '#', '.', '#'},
        {'#', '.', '#', '.', '#', '#', '.', '*', '.', '.', '.', '*', '*', '*', '*', '*', '*', '*', '*', '*', '.', '.', '.', '*', '.', '#', '#', '.', '#', '.', '#'},
        {'#', '.', '.', '.', '.', '#', '.', '*', '.', '#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#', '.', '*', '.', '#', '$', '.', '.', '.', '#'},
        {'#', '.', '#', '#', '#', '#', '.', '*', '.', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '.', '*', '.', '#', '#', '#', '#', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '*', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '*', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},

        {'%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%'} //15
};

char pacmanBoard[ROWS][COLS] = {
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '*', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '*', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '&', '&', '&', '&', '.', '*', '.', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '.', '*', '.', '#', '#', '#', '#', '.', '#'},
        {'#', '.', '&', '.', 'e', '&', '.', '*', '.', '#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#', '.', '*', '.', '#', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '&', '&', '.', '*', '.', '.', '.', '*', '*', '*', '*', '*', '*', '*', '*', '*', '.', '.', '.', '*', '.', '#', '#', '.', '#', '.', '#'},
        {'#', '.', '#', '.', '.', '.', '.', '.', '.', '#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#', '.', '.', '.', '.', '.', '.', '#', '.', '#'},
        {'#', '.', '#', '#', '#', '#', '.', '&', '.', '#', '#', '#', '#', '#', '.', '*', '.', '#', '#', '#', '#', '#', '.', '&', '.', '#', '#', '#', '#', '.', '#'},
        {'#', '.', '.', '.', '.', '#', '.', '&', '.', '.', '.', '.', '.', '#', '.', '*', '.', '#', '.', '.', '.', '.', '.', '&', '.', '#', '.', '.', '.', '.', '#'},
        {'#', '.', '#', '#', '#', '#', '.', '&', '&', '&', '&', '&', '.', '#', '.', '*', '.', '#', '.', '&', '&', '&', '&', '&', '.', '#', '#', '#', '#', '.', '#'},
        {'!', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '*', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '^'},
        {'#', '.', '#', '#', '#', '#', '.', '&', '&', '&', '&', '&', '.', '#', '.', '*', '.', '#', '.', '&', '&', '&', '&', '&', '.', '#', '#', '#', '#', '.', '#'},
        {'#', '.', '.', '.', '.', '#', '.', '&', '.', '.', '.', '.', '.', '#', '.', '*', '.', '#', '.', '.', '.', '.', '.', '&', '.', '#', '.', '.', '.', '.', '#'},
        {'#', '.', '#', '#', '#', '#', '.', '&', '.', '#', '#', '#', '#', '#', '.', '*', '.', '#', '#', '#', '#', '#', '.', '&', '.', '#', '#', '#', '#', '.', '#'},
        {'#', '.', '#', '.', '.', '.', '.', '.', '.', '#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#', '.', '.', '.', '.', '.', '.', '#', '.', '#'},
        {'#', '.', '#', '.', '&', '&', '.', '*', '.', '.', '.', '*', '*', '*', '*', '*', '*', '*', '*', '*', '.', '.', '.', '*', '.', '#', '#', '.', '#', '.', '#'},
        {'#', '.', '.', '.', '.', '&', '.', '*', '.', '#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#', '.', '*', '.', '#', '$', '.', '.', '.', '#'},
        {'#', '.', '&', '&', '&', '&', '.', '*', '.', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '.', '*', '.', '#', '#', '#', '#', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '*', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '*', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%', '%'} //15
};


void LoadSelectedMap() {
    if (selectedMapOption == 0) {

        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                pacmanBoard[i][j] = pacmanBoard1[i][j];
            }
        }

    } else if (selectedMapOption == 1) {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                pacmanBoard[i][j] = pacmanBoard2[i][j];
            }
        }
    }
}


typedef struct {
    float pacmanX;
    float pacmanY;
    float ghostX[10];
    float ghostY[10];
} FrameData;
#define MAX_FRAMES 10000
FrameData replayData[MAX_FRAMES];
int frameCount = 0;

int gameState = STATE_MENU;
bool recordMenu = true;

int jan = 3;
int score = 0;


Music menu_music;
bool is_NameEntered = false;

typedef struct {
    char name[50];
    int score;
    char timestamp[100];
} Record;

#define MAX_RECORDS 10000
Record records[MAX_RECORDS];
int recordCount = 0;


float pepperBoostTimer = 0.0f;
float normalSpeed = 150.0f;
float boostedSpeed = 240.0f;


typedef struct {
    Texture2D frames[2];
} PacmanTextures;

PacmanTextures pacmanTextures[4];
Texture2D pacmanCurrentTexture;


float pacmanTimer = 0.0f;
int pacmanFrame = 0;

void LoadPacmanTextures() {
    pacmanTextures[0].frames[0] = LoadTexture("textures/67.png");
    pacmanTextures[0].frames[1] = LoadTexture("textures/68.png");
    pacmanTextures[1].frames[0] = LoadTexture("textures/65.png");
    pacmanTextures[1].frames[1] = LoadTexture("textures/66.png");
    pacmanTextures[2].frames[0] = LoadTexture("textures/69.png");
    pacmanTextures[2].frames[1] = LoadTexture("textures/70.png");
    pacmanTextures[3].frames[0] = LoadTexture("textures/63.png");
    pacmanTextures[3].frames[1] = LoadTexture("textures/64.png");


    pacmanCurrentTexture = pacmanTextures[3].frames[0];
}


void UpdatePacmanFrame(float deltaTime) {
    pacmanTimer += deltaTime;

    if (pacmanTimer >= 0.2f) {
        pacmanFrame = (pacmanFrame + 1) % 2;

        pacmanTimer = 0.0f;
    }
}


typedef struct Cherry {
    float x;
    float y;
    bool athome;
    Texture2D texture;
    bool active;
} Cherry;
typedef struct Pepper {
    float x;
    float y;
    bool active;
    Texture2D texture;
} Pepper;
typedef struct Apple {
    float x;
    float y;
    Texture2D texture;
    bool active;
} Apple;
typedef struct Mushroom {
    float x;
    float y;
    Texture2D texture;
    bool active;
} Mushroom;
typedef struct Ghost {
    float x;
    float y;
    Texture2D texture;
    int direction;
} Ghost;
Ghost ghosts[10];

void LoadCherryTexture(Cherry *cherry) {
    cherry->texture = LoadTexture("textures/7.png");
    cherry->active = true;
}

void LoadGhostTextures(Ghost *ghosts) {
    ghosts[0].texture = LoadTexture("textures/176.png");
    ghosts[1].texture = LoadTexture("textures/37.png");
    ghosts[2].texture = LoadTexture("textures/37.png");
    ghosts[3].texture = LoadTexture("textures/37.png");
    ghosts[4].texture = LoadTexture("textures/37.png");
    ghosts[5].texture = LoadTexture("textures/37.png");
    ghosts[6].texture = LoadTexture("textures/37.png");
    ghosts[7].texture = LoadTexture("textures/37.png");
    ghosts[8].texture = LoadTexture("textures/37.png");
    ghosts[9].texture = LoadTexture("textures/37.png");
    ghosts[10].texture = LoadTexture("textures/37.png");
}


void UnloadGhostTextures(Ghost *ghosts) {
    for (int i = 0; i < max_ghost; i++) {
        UnloadTexture(ghosts[i].texture);
    }
}

void LoadAppleTexture(Apple *apple) {
    apple->texture = LoadTexture("textures/116.png");
    apple->active = true;
}

void LoadMushroomTexture(Mushroom *mushroom) {
    mushroom->texture = LoadTexture("textures/58.png");
    mushroom->active = true;
}

void DrawCherry(Cherry *cherry) {
    if (cherry->active) {
        float scale = (TILE_SIZE / 1.3) / (float) cherry->texture.width;

        DrawTextureEx(
                cherry->texture,
                (Vector2) {cherry->x - TILE_SIZE / 2.2, cherry->y - TILE_SIZE / 2.3}, // موقعیت
                0.0f,
                scale,
                WHITE
        );
    }
}

void DrawMushroom(Mushroom *mushroom) {
    if (mushroom->active) {
        float scale = TILE_SIZE / 1.08 / (float) mushroom->texture.width;

        DrawTextureEx(
                mushroom->texture,
                (Vector2) {mushroom->x - TILE_SIZE / 2, mushroom->y - TILE_SIZE / 2},
                0.0f,
                scale,
                WHITE
        );
    }
}

void DrawApple(Apple *apple) {
    if (apple->active) {
        float scale = TILE_SIZE / 1 / (float) apple->texture.width;

        DrawTextureEx(
                apple->texture,
                (Vector2){apple->x - TILE_SIZE / 1.9f, apple->y - TILE_SIZE / 2.0f},
                0.0f,
                scale,
                WHITE
        );

    }
}

void CheckPepperCollision(Pepper *pepper, float pacmanX, float pacmanY) {
    if (pepper->active) {
        float distX = pacmanX - pepper->x;
        float distY = pacmanY - pepper->y;
        float distance = sqrt(distX * distX + distY * distY);

        if (distance < TILE_SIZE / 2.0f) {
            PlaySound(pepper_sound);
            pepper->active = false;
            pepperBoostTimer = 10.0f;
        }
    }
}


void CheckAppleCollision(Apple *apple, float pacmanX, float pacmanY, int *jan) {
    if (apple->active) {
        float distX = pacmanX - apple->x;
        float distY = pacmanY - apple->y;
        float distance = sqrt(distX * distX + distY * distY);

        if (distance < TILE_SIZE / 2.0f) {
            PlaySound(apple_sound);
            apple->active = false;
            if (*jan < 3) {
                (*jan)++;
            }
        }
    }
}

void DrawGhosts(Ghost *ghosts) {
    for (int i = 0; i < max_ghost; i++) {
        float scale = TILE_SIZE / (float) ghosts[i].texture.height;

        DrawTextureEx(
                ghosts[i].texture,
                (Vector2) {ghosts[i].x - TILE_SIZE / 2.1f,
                           ghosts[i].y - TILE_SIZE / 2.0f},
                0.0f,
                scale,
                WHITE
        );
    }
}

typedef struct SpecialCell {
    int row;
    int col;
    bool active;
} SpecialCell;

bool is_mane(char cell) {
    return (cell == '#' || cell == '&' || cell == '*' || cell == '%');
}

bool canMove(char pacmanBoard[ROWS][COLS], float x, float y) {
    int new_place_y = (int) (x / TILE_SIZE);
    int new_place_x = (int) (y / TILE_SIZE);


    if (new_place_y < 0 || new_place_y >= COLS || new_place_x < 0 || new_place_x >= ROWS) {
        return false;
    }

    if (pacmanBoard[new_place_x][new_place_y] == '#' || pacmanBoard[new_place_x][new_place_y] == '&' ||
        pacmanBoard[new_place_x][new_place_y] == '*' || pacmanBoard[new_place_x][new_place_y] == '%') {
        return false;
    }

    return true;
}


void Convert_direction_to_move(int direction, int *deltaRow, int *deltaCol) {
    switch (direction) {
        case -2:
            *deltaRow = -1;
            *deltaCol = 0;
            break;
        case 2:
            *deltaRow = 1;
            *deltaCol = 0;
            break;
        case -1:
            *deltaRow = 0;
            *deltaCol = -1;
            break;
        case 1:
            *deltaRow = 0;
            *deltaCol = 1;
            break;
        default:
            *deltaRow = 0;
            *deltaCol = 0;
            break;
    }
}

bool IsStarHere(SpecialCell Array_star[10], int row, int col) {
    for (int i = 0; i < 10; i++) {
        if (Array_star[i].active && Array_star[i].row == row && Array_star[i].col == col) {
            return true;
        }
    }
    return false;
}

void RandomizeCherryPosition(char pacmanBoard[ROWS][COLS], SpecialCell Array_star[10], Cherry *cherry) {
    int randRow, randCol;
    bool validPosition = false;

    while (!validPosition) {
        randRow = rand() % ROWS;
        randCol = rand() % COLS;

        if (pacmanBoard[randRow][randCol] == '.' && !IsStarHere(Array_star, randRow, randCol)) {
            cherry->x = randCol * TILE_SIZE + TILE_SIZE / 2.0f;
            cherry->y = randRow * TILE_SIZE + TILE_SIZE / 2.0f;
            cherry->active = true;
            validPosition = true;
        }
    }
}

void LoadPepperTexture(Pepper *pepper) {
    pepper->texture = LoadTexture("textures/54.png");
    pepper->active = false;
}

void DrawPepper(Pepper *pepper) {
    if (pepper->active) {
        DrawTextureEx(
                pepper->texture,
                (Vector2) {pepper->x - TILE_SIZE / 2.0f, pepper->y - TILE_SIZE / 2.0f},
                0.0f,
                TILE_SIZE / (float) pepper->texture.width,
                WHITE
        );
    }
}

void RandomizeGhostPosition(char pacmanBoard[ROWS][COLS], Ghost *ghost) {
    int randRow, randCol;
    bool validPosition = false;

    while (!validPosition) {
        randRow = rand() % ROWS;
        randCol = rand() % COLS;

        if (pacmanBoard[randRow][randCol] == '.') {
            ghost->x = randCol * TILE_SIZE + TILE_SIZE / 2.0f;
            ghost->y = randRow * TILE_SIZE + TILE_SIZE / 2.0f;
            validPosition = true;
        }
    }
}

void RandomizePepperPosition(char pacmanBoard[ROWS][COLS], SpecialCell Array_star[10], Pepper *pepper) {
    int randRow, randCol;
    bool validPosition = false;

    while (!validPosition) {
        randRow = rand() % ROWS;
        randCol = rand() % COLS;

        if (pacmanBoard[randRow][randCol] == '.' && !IsStarHere(Array_star, randRow, randCol)) {
            pepper->x = randCol * TILE_SIZE + TILE_SIZE / 2.0f;
            pepper->y = randRow * TILE_SIZE + TILE_SIZE / 2.0f;
            pepper->active = true;
            validPosition = true;
        }
    }
}

void RandomizeApplePosition(char pacmanBoard[ROWS][COLS], SpecialCell Array_star[10], Apple *apple) {
    int randRow, randCol;
    bool validPosition = false;


    while (!validPosition) {
        randRow = rand() % ROWS;
        randCol = rand() % COLS;

        if (pacmanBoard[randRow][randCol] == '.' && !IsStarHere(Array_star, randRow, randCol)) {
            apple->x = randCol * TILE_SIZE + TILE_SIZE / 2.0f;
            apple->y = randRow * TILE_SIZE + TILE_SIZE / 2.0f;
            apple->active = true;
            validPosition = true;
        }
    }
}

void RandomizeMushroomPosition(char pacmanBoard[ROWS][COLS], SpecialCell Array_star[10], Mushroom *mushroom) {
    int randRow, randCol;
    bool validPosition = false;

    while (!validPosition) {
        randRow = rand() % ROWS;
        randCol = rand() % COLS;

        if (pacmanBoard[randRow][randCol] == '.' && !IsStarHere(Array_star, randRow, randCol)) {
            mushroom->x = randCol * TILE_SIZE + TILE_SIZE / 2.0f;
            mushroom->y = randRow * TILE_SIZE + TILE_SIZE / 2.0f;
            mushroom->active = true;
            validPosition = true;
        }
    }
}

void CheckMushroomCollision(Mushroom *mushroom, float pacmanX, float pacmanY, int *jan) {
    if (mushroom->active) {
        float distX = pacmanX - mushroom->x;
        float distY = pacmanY - mushroom->y;
        float distance = sqrt(distX * distX + distY * distY);

        if (distance < TILE_SIZE / 2.0f) {
            PlaySound(mashroom_sound);
            mushroom->active = false;
            (*jan)--;
        }
    }
}

bool cherryEffectActive = false;

void CheckGhostCollision(Ghost *ghosts, int ghostCount, float pacmanX, float pacmanY, int *jan,
                         char pacmanBoard[ROWS][COLS], Cherry *cherry) {
    for (int i = 0; i < max_ghost; i++) {
        float distX = pacmanX - ghosts[i].x;
        float distY = pacmanY - ghosts[i].y;
        float distance = sqrt(distX * distX + distY * distY);

        if (distance < TILE_SIZE / 2.0f) {
            if (cherryEffectActive) {
                cherryEffectActive = false;
                cherry->active = false;
                cherry->athome = false;
                PlaySound(ghost_with_cherry);
                RandomizeGhostPosition(pacmanBoard, &ghosts[i]);
            } else {
                PlaySound(ghost_normal);
                (*jan)--;
                if (*jan > 0) {
                    RandomizeGhostPosition(pacmanBoard, &ghosts[i]);
                }
            }
            break;
        }
    }
}

bool CanGhostMove(char pacmanBoard[ROWS][COLS], float x, float y, int direction) {
    int deltaRow = 0, deltaCol = 0;
    Convert_direction_to_move(direction, &deltaRow, &deltaCol);

    float targetX = x + 0.5 * deltaCol * TILE_SIZE;
    float targetY = y + 0.5 * deltaRow * TILE_SIZE;


    int targetRow = (int) (targetY / TILE_SIZE);
    int targetCol = (int) (targetX / TILE_SIZE);

    if (targetRow < 0 || targetRow >= ROWS || targetCol < 0 || targetCol >= COLS) {
        return false;
    }

    if (is_mane(pacmanBoard[targetRow][targetCol])) {
        return false;
    }

    return true;
}


void MoveGhost(Ghost *ghost, char pacmanBoard[ROWS][COLS], float speed, float deltaTime) {
    int deltaRow = 0, deltaCol = 0;


    if (CanGhostMove(pacmanBoard, ghost->x, ghost->y, ghost->direction)) {
        Convert_direction_to_move(ghost->direction, &deltaRow, &deltaCol);
        ghost->x += deltaCol * speed * deltaTime;
        ghost->y += deltaRow * speed * deltaTime;
    } else {
        do {
            ghost->direction = (rand() % 5) - 2;
        } while (ghost->direction == 0 || !CanGhostMove(pacmanBoard, ghost->x, ghost->y,
                                                        ghost->direction));
    }
}



void UpdatePacmanTexture() {
    if (user_key == -2) {
        pacmanCurrentTexture = pacmanTextures[0].frames[pacmanFrame];
    } else if (user_key == 2) {
        pacmanCurrentTexture = pacmanTextures[1].frames[pacmanFrame];
    } else if (user_key == -1) {
        pacmanCurrentTexture = pacmanTextures[2].frames[pacmanFrame];
    } else if (user_key == 1) {
        pacmanCurrentTexture = pacmanTextures[3].frames[pacmanFrame];
    } else {
        pacmanCurrentTexture = pacmanTextures[3].frames[0];
    }
}



void Star(char pacmanBoard[ROWS][COLS], SpecialCell Array_star[10], Apple *apple, Cherry *cherry, Mushroom *mushroom,
          Pepper *pepper) {
    int count = 0;
    bool CherryInBoard = false;
    bool appleInBoard = false;
    bool MushroomInBoard = false;
    bool PepperInBoard = false;

    for (int i = 0; i < 10; i++) {
        if (apple->active) {
            appleInBoard = true;
            break;
        }
    }

    for (int i = 0; i < 10; i++) {
        if (mushroom->active) {
            MushroomInBoard = true;
            break;
        }
    }

    for (int i = 0; i < 10; i++) {
        if (pepper->active) {
            PepperInBoard = true;
            break;
        }
    }

    for (int i = 0; i < 10; i++) {
        if (cherry->active) {
            CherryInBoard = true;
            break;
        }
    }

    while (count < 10) {
        int randRow = rand() % ROWS;
        int randCol = rand() % COLS;

        if (!appleInBoard && pacmanBoard[randRow][randCol] == '.' &&
            !(randRow == pacmanCurrentRow && randCol == pacmanCurrentCol)) {
            apple->x = randCol * TILE_SIZE + TILE_SIZE / 2.0f;
            apple->y = randRow * TILE_SIZE + TILE_SIZE / 2.0f;
            apple->active = true;
        }

        randRow = rand() % ROWS;
        randCol = rand() % COLS;

        if (!CherryInBoard && pacmanBoard[randRow][randCol] == '.' &&
            !(randRow == pacmanCurrentRow && randCol == pacmanCurrentCol)) {
            cherry->x = randCol * TILE_SIZE + TILE_SIZE / 2.0f;
            cherry->y = randRow * TILE_SIZE + TILE_SIZE / 2.0f;
            cherry->active = true;
            cherry->athome = false;
        }

        randRow = rand() % ROWS;
        randCol = rand() % COLS;

        if (!MushroomInBoard && pacmanBoard[randRow][randCol] == '.' &&
            !(randRow == pacmanCurrentRow && randCol == pacmanCurrentCol)) {
            mushroom->x = randCol * TILE_SIZE + TILE_SIZE / 2.0f;
            mushroom->y = randRow * TILE_SIZE + TILE_SIZE / 2.0f;
            mushroom->active = true;
        }

        randRow = rand() % ROWS;
        randCol = rand() % COLS;

        if (!PepperInBoard && pacmanBoard[randRow][randCol] == '.' &&
            !(randRow == pacmanCurrentRow && randCol == pacmanCurrentCol)) {
            pepper->x = randCol * TILE_SIZE + TILE_SIZE / 2.0f;
            pepper->y = randRow * TILE_SIZE + TILE_SIZE / 2.0f;
            pepper->active = true;
        }

        randRow = rand() % ROWS;
        randCol = rand() % COLS;


        if (pacmanBoard[randRow][randCol] == '.' &&
            !(randRow == pacmanCurrentRow && randCol == pacmanCurrentCol)) {

            bool tekrar = false;
            for (int i = 0; i < count; i++) {
                if (Array_star[i].row == randRow && Array_star[i].col == randCol) {
                    tekrar = true;
                    break;
                }
            }

            if (!tekrar) {
                Array_star[count].row = randRow;
                Array_star[count].col = randCol;
                Array_star[count].active = true;
                count++;
            }
        }
    }

    if (!cherry->active && !cherry->athome) {
        RandomizeCherryPosition(pacmanBoard, Array_star, cherry);
        cherryEffectActive = false;
    }
}

void ReadAndSortRecords() {
    FILE *file = fopen("C:\\Users\\A_H\\Desktop\\Neww_project\\cmake-build-debug\\high_scores.txt", "r+");
    if (file != NULL) {
        recordCount = 0;
        while (fscanf(file, "%s %d %s\n", records[recordCount].name, &records[recordCount].score,
                      records[recordCount].timestamp) == 3) {
            recordCount++;
            if (recordCount >= MAX_RECORDS) {
                break;
            }
        }
        fclose(file);

        for (int i = 0; i < recordCount - 1; i++) {
            for (int j = i + 1; j < recordCount; j++) {
                if (records[i].score < records[j].score) {
                    Record temp = records[i];
                    records[i] = records[j];
                    records[j] = temp;
                }
            }
        }
    } else {
        printf("Error opening file!\n");
    }
}


void GetCurrentTimestamp(char *timestamp) {
    time_t now = time(NULL);
    struct tm *tm_struct = localtime(&now);
    strftime(timestamp, 150, "%Y_%m_%d...%H:%M:%S", tm_struct);
}

void SaveScore(int score, char *playerName) {
    char timestamp[150];
    GetCurrentTimestamp(timestamp);

    Record newRecord;
    newRecord.score = score;
    strcpy(newRecord.name, playerName);
    strcpy(newRecord.timestamp, timestamp);

    FILE *file = fopen("C:\\Users\\A_H\\Desktop\\Neww_project\\cmake-build-debug\\high_scores.txt", "a+");

    if (file != NULL) {
        fprintf(file, "%s %d %s\n", newRecord.name, newRecord.score, newRecord.timestamp);
        fclose(file);  // بستن فایل
    } else {
        printf("Error opening file!\n");
    }
}

void DisplayTopRecords() {
    static Texture2D background = {0};
    if (!background.id) {
        background = LoadTexture("C:/Users/Victus/Desktop/Pacman_images/78.png");
    }

    ReadAndSortRecords();
    recordMenu = true;
    while (recordMenu) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        ClearBackground(BLACK);
        DrawTexture(background, 30, 15, WHITE);

        for (int i = 0; i < 10; i++) {
            char recordText[150];
            snprintf(recordText, sizeof(recordText), "%-5d :  %-10s Score: %-10d %-25s", i + 1, records[i].name,
                     records[i].score, records[i].timestamp);
            DrawText(recordText, 35, 50 + i * 30, 20, WHITE);
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            CloseWindow();
            exit(0);
        }
        if (IsKeyPressed(KEY_SPACE)) {
            recordMenu = false;
            gameState = 0;
        }

        EndDrawing();
    }
}


void GameOverScreen() {
    bool gameOverMenu = true;
    while (gameOverMenu) {
        BeginDrawing();
        ClearBackground((Color) {255, 255, 0, 255});
        char scoreText[50];
        sprintf(scoreText, "SCORE: %d", score);
        DrawText("GAME OVER!", 235, 150, 40, BLACK);
        DrawText(scoreText, 280, 250, 30, BLACK);
        DrawText("Press Enter to Return to Menu", 165, 350, 20, BLACK);
        EndDrawing();

        if (IsKeyPressed(KEY_SPACE)) {
            gameOverMenu = false;
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            CloseWindow();
            exit(0);
        }
    }

}


void EnterPlayerName(char *playerName, int maxLength) {
    Texture2D background_name;
    background_name = LoadTexture("textures/101.png");
    int playerNameLength = strlen(playerName);
    bool isNameEntered = false;

    while (!isNameEntered) {
        BeginDrawing();
        DrawTexture(background_name, 0, 0, WHITE);
        const char *prompt = "Enter your name : ";
        const char *displayName = playerName;
        int promptWidth = MeasureText(prompt, 30);
        int nameWidth = MeasureText(displayName, 30);
        int promptX = (GetScreenWidth() - promptWidth) / 2;
        int nameX = (GetScreenWidth() - nameWidth) / 2;
        DrawText(prompt, promptX, 190, 30, YELLOW);
        DrawText(displayName, nameX - 10, 260 + 10, 30, WHITE);

        if (playerNameLength < maxLength - 1) {
            if (IsKeyPressed(KEY_BACKSPACE) && playerNameLength > 0) {
                playerName[--playerNameLength] = '\0';
            } else {
                char key = GetCharPressed();
                while (key > 0) {
                    if (playerNameLength < maxLength - 1 && key >= 32 && key <= 125) {
                        playerName[playerNameLength++] = (char) key;
                        playerName[playerNameLength] = '\0';
                    }
                    key = GetCharPressed();
                }
            }
        }
        if (IsKeyPressed(KEY_ENTER) && playerNameLength > 0) {
            PlaySound(click);
            isNameEntered = true;
            is_NameEntered = true;
        }
        if (IsKeyPressed(KEY_ESCAPE)) {
            CloseWindow();
            exit(0);
        }
        EndDrawing();
    }
}

void press(int *user_key, Texture2D *pacmanCurrentTexture, PacmanTextures pacmanTextures[], int pacmanFrame) {
    if (IsKeyPressed(KEY_UP)) {
        *user_key = -2;
        *pacmanCurrentTexture = pacmanTextures[0].frames[pacmanFrame];
    } else if (IsKeyPressed(KEY_DOWN)) {
        *user_key = 2;
        *pacmanCurrentTexture = pacmanTextures[1].frames[pacmanFrame];
    } else if (IsKeyPressed(KEY_LEFT)) {
        *user_key = -1;
        *pacmanCurrentTexture = pacmanTextures[2].frames[pacmanFrame];
    } else if (IsKeyPressed(KEY_RIGHT)) {
        *user_key = 1;
        *pacmanCurrentTexture = pacmanTextures[3].frames[pacmanFrame];
    }
}

int selectedOptionSettings = 0;


void DisplaySettingsMenu() {
    static Texture2D background = {0};
    if (!background.id) {
        background = LoadTexture("C:/Users/Victus/Desktop/Pacman_images/78.png");
    }

    bool inSettingsMenu = true;

    while (inSettingsMenu) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(background, 30, 15, WHITE);

        if (selectedOptionSettings == 0) {
            DrawText(" >>> Level", 70, 120, 30, WHITE);
            DrawText("Map", 70, 180, 30, RED);


            const char *levels[MAX_LEVELS] = {"Easy", "Medium", "Hard"};
            for (int i = 0; i < MAX_LEVELS; i++) {
                if (selectedLevelOption == i) {
                    DrawText(levels[i], 150 + i * 150, 240, 30, YELLOW); // سطح انتخاب شده
                } else {
                    DrawText(levels[i], 150 + i * 150, 240, 30, RED); // سایر سطوح
                }
            }
            if (selectedLevelOption == 0) { // Easy
                max_ghost = 5;
            } else if (selectedLevelOption == 1) { // Medium
                max_ghost = 7;
            } else if (selectedLevelOption == 2) { // Hard
                max_ghost = 10;
            }
            if (IsKeyPressed(KEY_LEFT)) {
                selectedLevelOption--;
                if (selectedLevelOption < 0) selectedLevelOption = MAX_LEVELS - 1;
            }
            if (IsKeyPressed(KEY_RIGHT)) {
                selectedLevelOption++;
                if (selectedLevelOption >= MAX_LEVELS) selectedLevelOption = 0;
            }
        } else if (selectedOptionSettings == 1) {
            DrawText("Level", 70, 120, 30, RED);
            DrawText(" >>> Map", 70, 180, 30, WHITE);

            const char *maps[2] = {"Map 1", "Map 2"};
            for (int i = 0; i < 2; i++) {
                if (selectedMapOption == i) {
                    DrawText(maps[i], 150 + i * 150, 240, 30, YELLOW);
                } else {
                    DrawText(maps[i], 150 + i * 150, 240, 30, RED);
                }
            }

            if (IsKeyPressed(KEY_LEFT)) {
                selectedMapOption = 0;
            }
            if (IsKeyPressed(KEY_RIGHT)) {
                selectedMapOption = 1;
            }
        }

        if (IsKeyPressed(KEY_UP)) {
            selectedOptionSettings--;
            if (selectedOptionSettings < 0) selectedOptionSettings = 1;
        }
        if (IsKeyPressed(KEY_DOWN)) {
            selectedOptionSettings++;
            if (selectedOptionSettings > 1) selectedOptionSettings = 0;
        }

        if (IsKeyPressed(KEY_ENTER)) {
            PlaySound(click);
            if (selectedOptionSettings == 0) {
                currentGhostSpeed = ghostSpeeds[selectedLevelOption];
            } else if (selectedOptionSettings == 1) {
                LoadSelectedMap();
            }
        }

        if (IsKeyPressed(KEY_SPACE)) {
            inSettingsMenu = false;
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            CloseWindow();
            exit(0);
        }
        EndDrawing();
    }
}


void MainMenu() {
    static Texture2D background = {0};
    if (!background.id) {
        background = LoadTexture("C:/Users/Victus/Desktop/Pacman_images/78.png");
    }

    int selectedOption = 0;


    while (inMenu) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(background, 30, 15, WHITE);
        UpdateMusicStream(menu_music);
        if (selectedOption == 0) {
            DrawText(" >>> Play", 60, 60, 30, YELLOW);
            DrawText("Settings", 70, 120, 30, RED);
            DrawText("Replay", 70, 180, 30, RED);
            DrawText("Records", 70, 240, 30, RED);
            DrawText("Exit", 70, 300, 30, RED);
        } else if (selectedOption == 1) {
            DrawText("Play", 70, 60, 30, RED);
            DrawText(" >>> Settings", 60, 120, 30, YELLOW);
            DrawText("Replay", 70, 180, 30, RED);
            DrawText("Records", 70, 240, 30, RED);
            DrawText("Exit", 70, 300, 30, RED);
        } else if (selectedOption == 2) {
            DrawText("Play", 70, 60, 30, RED);
            DrawText("Settings", 70, 120, 30, RED);
            DrawText(" >>> Replay", 60, 180, 30, YELLOW);
            DrawText("Records", 70, 240, 30, RED);
            DrawText("Exit", 70, 300, 30, RED);
        } else if (selectedOption == 3) {
            DrawText("Play", 70, 60, 30, RED);
            DrawText("Settings", 70, 120, 30, RED);
            DrawText("Replay", 70, 180, 30, RED);
            DrawText(" >>> Records", 60, 240, 30, YELLOW);
            DrawText("Exit", 70, 300, 30, RED);
        } else if (selectedOption == 4) {
            DrawText("Play", 70, 60, 30, RED);
            DrawText("Settings", 70, 120, 30, RED);
            DrawText("Replay", 70, 180, 30, RED);
            DrawText("Records", 70, 240, 30, RED);
            DrawText(" >>> Exit", 60, 300, 30, YELLOW);
        }

        if (IsKeyPressed(KEY_UP)) {
            selectedOption--;
            if (selectedOption < 0) selectedOption = 4;
        }
        if (IsKeyPressed(KEY_DOWN)) {
            selectedOption++;
            if (selectedOption > 4) selectedOption = 0;
        }

        if (IsKeyPressed(KEY_ENTER)) {
            PlaySound(click);
            if (selectedOption == 0) {
                inMenu = false;
                gameState = STATE_PLAYING;
            } else if (selectedOption == 1) {

                DisplaySettingsMenu();
                LoadSelectedMap();
                selectedOption = 0;
            } else if (selectedOption == 2) {
                inMenu = false;
                gameState = STATE_REPLAY;
            } else if (selectedOption == 3) {
                recordMenu = true;
                while (recordMenu) {
                    BeginDrawing();
                    ClearBackground(RAYWHITE);
                    ReadAndSortRecords();
                    DisplayTopRecords();
                    EndDrawing();
                }
                selectedOption = 0;
            } else if (selectedOption == 4) {
                CloseWindow();
                exit(0);
            }
        }
        EndDrawing();
    }
}

void CheckCherryCollision(Cherry *cherry, float pacmanX, float pacmanY) {
    if (cherry->active && !cherry->athome) {
        float distX = pacmanX - cherry->x;
        float distY = pacmanY - cherry->y;
        float distance = sqrt(distX * distX + distY * distY);

        if (distance < TILE_SIZE / 2.0f) {
            PlaySound(cherry_sound);
            cherry->x = 437;
            cherry->y = 546;
            cherry->active = true;
            cherry->athome = true;
            cherryEffectActive = true;
        }
    }
}

void DisplayReplay() {
    frameCount = 0;
    while (1) {
        BeginDrawing();
        ClearBackground((Color) {0, 0, 70, 255});

        DrawTextureEx(
                pacmanCurrentTexture,
                (Vector2) {replayData[frameCount].pacmanX - TILE_SIZE / 2,
                           replayData[frameCount].pacmanY - TILE_SIZE / 2},
                0.0f,
                (TILE_SIZE / (float) pacmanCurrentTexture.width),
                WHITE
        );

        for (int j = 0; j < 5; j++) {
            DrawTextureEx(
                    ghosts[j].texture,
                    (Vector2) {replayData[frameCount].ghostX[j] - TILE_SIZE / 2,
                               replayData[frameCount].ghostY[j] - TILE_SIZE / 2},
                    0.0f,
                    TILE_SIZE / (float) ghosts[j].texture.height,
                    WHITE
            );
        }

        EndDrawing();
        frameCount++;
        float frameDelay = 1.0f / 60.0f;
        while (GetFrameTime() < frameDelay) {
        }
    }
}





void StopBackgroundMusic() {
    StopMusicStream(menu_music);
    UnloadMusicStream(menu_music);
}

float speedScale = 0.03f;

void MoveGhostDiscrete(Ghost *ghost, char pacmanBoard[ROWS][COLS], float pacmanX, float pacmanY) {
    int currentRow = (int)(ghost->y / TILE_SIZE);
    int currentCol = (int)(ghost->x / TILE_SIZE);
    float centerX = currentCol * TILE_SIZE + TILE_SIZE / 2.0f;
    float centerY = currentRow * TILE_SIZE + TILE_SIZE / 2.0f;



    if (fabs(ghost->x - centerX) < 1.0f && fabs(ghost->y - centerY) < 1.0f) {
        float diffX = pacmanX - ghost->x;
        float diffY = pacmanY - ghost->y;

        if (fabs(diffX) > fabs(diffY)) {
            if (diffX > 0 && !is_mane(pacmanBoard[currentRow][currentCol + 1])) {
                ghost->direction = 1;
            } else if (diffX < 0 && !is_mane(pacmanBoard[currentRow][currentCol - 1])) {
                ghost->direction = -1;
            } else {
                ghost->direction = 0;
            }
        } else {
            if (diffY > 0 && !is_mane(pacmanBoard[currentRow + 1][currentCol])) {
                ghost->direction = 2;
            } else if (diffY < 0 && !is_mane(pacmanBoard[currentRow - 1][currentCol])) {
                ghost->direction = -2;
            } else {
                ghost->direction = 0;
            }
        }

        if (ghost->direction == 0 || !CanGhostMove(pacmanBoard, ghost->x, ghost->y, ghost->direction)) {
            do {
                ghost->direction = (rand() % 4) * 2 - 2;
            } while (!CanGhostMove(pacmanBoard, ghost->x, ghost->y, ghost->direction));
        }
    }

    switch (ghost->direction) {
        case 1:
            ghost->x += (TILE_SIZE / 2.0f) * speedScale;
            break;
        case -1:
            ghost->x -= (TILE_SIZE / 2.0f) * speedScale;
            break;
        case 2:
            ghost->y += (TILE_SIZE / 2.0f) * speedScale;
            break;
        case -2:
            ghost->y -= (TILE_SIZE / 2.0f) * speedScale;
            break;
        default:
            break;
    }
}



void InitGame(float *pacmanX, float *pacmanY, Ghost *ghosts, int ghostCount) {
    jan = 3;
    score = 0;

    *pacmanX = 1 * TILE_SIZE + TILE_SIZE / 2.0f;
    *pacmanY = 1 * TILE_SIZE + TILE_SIZE / 2.0f;
    user_key = 0;
    currentDirection = 0;

    ghosts[0].x = 14 * TILE_SIZE + TILE_SIZE / 2.0f;
    ghosts[0].y = 15 * TILE_SIZE + TILE_SIZE / 2.0f;
    ghosts[1].x = 29 * TILE_SIZE + TILE_SIZE / 2.0f;
    ghosts[1].y = TILE_SIZE + TILE_SIZE / 2.0f;
    ghosts[2].x = 1 * TILE_SIZE + TILE_SIZE / 2.0f;
    ghosts[2].y = 17 * TILE_SIZE + TILE_SIZE / 2.0f;
    ghosts[3].x = 29 * TILE_SIZE + TILE_SIZE / 2.0f;
    ghosts[3].y = 13 * TILE_SIZE + TILE_SIZE / 2.0f;
    ghosts[4].x = 3 * TILE_SIZE + TILE_SIZE / 2.0f;
    ghosts[4].y = 3 * TILE_SIZE + TILE_SIZE / 2.0f;
    ghosts[5].x = 3 * TILE_SIZE + TILE_SIZE / 2.0f;
    ghosts[5].y = 3 * TILE_SIZE + TILE_SIZE / 2.0f;
    ghosts[6].x = 3 * TILE_SIZE + TILE_SIZE / 2.0f;
    ghosts[6].y = 3 * TILE_SIZE + TILE_SIZE / 2.0f;
    ghosts[7].x = 3 * TILE_SIZE + TILE_SIZE / 2.0f;
    ghosts[7].y = 3 * TILE_SIZE + TILE_SIZE / 2.0f;
    ghosts[8].x = 3 * TILE_SIZE + TILE_SIZE / 2.0f;
    ghosts[8].y = 3 * TILE_SIZE + TILE_SIZE / 2.0f;
    ghosts[9].x = 3 * TILE_SIZE + TILE_SIZE / 2.0f;
    ghosts[9].y = 3 * TILE_SIZE + TILE_SIZE / 2.0f;
    ghosts[10].x = 3 * TILE_SIZE + TILE_SIZE / 2.0f;
    ghosts[10].y = 3 * TILE_SIZE + TILE_SIZE / 2.0f;




    for (int i = 0; i < 10; i++) {
        ghosts[i].direction = 1;
    }

    currentGhostSpeed = ghostSpeeds[selectedLevelOption];
}




int main(void) {

    SetConfigFlags(FLAG_WINDOW_UNDECORATED);


    InitWindow(COLS * TILE_SIZE, ROWS * TILE_SIZE, "Pac-Man");
    InitAudioDevice();

    mashroom_sound = LoadSound("C:\\Users\\A_H\\Desktop\\Pacman_images\\mushroom (mp3cut.net).wav");
    star = LoadSound("C:\\Users\\A_H\\Desktop\\Pacman_images\\star (mp3cut.net).wav");
    menu_music = LoadMusicStream("C:\\Users\\A_H\\Desktop\\Pacman_images\\128.wav");
    cherry_sound = LoadSound("C:\\Users\\A_H\\Desktop\\Pacman_images\\star (mp3cut.net).wav");
    apple_sound = LoadSound("C:\\Users\\A_H\\Desktop\\Pacman_images\\star (mp3cut.net).wav");
    chaleh = LoadSound("C:\\Users\\A_H\\Desktop\\Pacman_images\\star (mp3cut.net).wav");
    ghost_with_cherry = LoadSound("C:\\Users\\A_H\\Desktop\\Pacman_images\\star (mp3cut.net).wav");
    ghost_normal = LoadSound("C:\\Users\\A_H\\Desktop\\Pacman_images\\star (mp3cut.net).wav");
    click = LoadSound("C:\\Users\\A_H\\Desktop\\Pacman_images\\star (mp3cut.net).wav");
    game_over = LoadSound("C:\\Users\\A_H\\Desktop\\Pacman_images\\star (mp3cut.net).wav");
    pepper_sound = LoadSound("C:\\Users\\A_H\\Desktop\\Pacman_images\\star (mp3cut.net).wav");

    PlayMusicStream(menu_music);
    SetTargetFPS(300);
    LoadPacmanTextures();

    char playerName[50] = "";
    int playerNameLength = 0;
    bool isNameEntered = false;

    LoadPacmanTextures();

    Texture2D textureWall = LoadTexture("textures/159.png");
    Texture2D textureEnemy = LoadTexture("textures/137.png");
    Texture2D texturePlayer = LoadTexture("textures/172.png");
    Texture2D textureFood = LoadTexture("textures/125.png");
    Texture2D textureBonus = LoadTexture("textures/161.png");


    Cherry cherry;
    LoadCherryTexture(&cherry);
    Pepper pepper;
    LoadPepperTexture(&pepper);
    Apple apple;
    LoadAppleTexture(&apple);
    Mushroom mushroom;
    LoadMushroomTexture(&mushroom);

    LoadGhostTextures(ghosts);
    Texture2D starTexture = LoadTexture("textures/57.png");
    Texture2D starTexture1 = LoadTexture("textures/57"); // تصویر اول ستاره
    Texture2D starTexture2 = LoadTexture("textures/52"); // تصویر دوم ستاره
    Texture2D starTexture3 = LoadTexture("textures/111"); // فریم سوم ستاره


    int pacmanCurrentRow = 1;
    int pacmanCurrentCol = 1;
    float pacmanX = 1 * TILE_SIZE + TILE_SIZE / 2.0f;
    float pacmanY = 1 * TILE_SIZE + TILE_SIZE / 2.0f;


    SpecialCell Array_star[10];
    srand(time(NULL));


    gameState = 0;


    while (!WindowShouldClose()) {


        if (gameState == 0) {
            inMenu = true;
            MainMenu();
            Star(pacmanBoard, Array_star, &apple, &cherry, &mushroom, &pepper);
            if (!cherry.active && cherry.athome) {
                RandomizeCherryPosition(pacmanBoard, Array_star, &cherry);
                cherry.athome = false;
            }
            RandomizeCherryPosition(pacmanBoard, Array_star, &cherry);
            RandomizePepperPosition(pacmanBoard, Array_star, &pepper);
            RandomizeApplePosition(pacmanBoard, Array_star, &apple);
            RandomizeMushroomPosition(pacmanBoard, Array_star, &mushroom);
            if (is_NameEntered == false) {
                EnterPlayerName(playerName, sizeof(playerName));
            }
            InitGame(&pacmanX, &pacmanY, ghosts, 10);
            gameState = 1;

        } else if (gameState == 1) {
            if (frameCount < MAX_FRAMES) {
                replayData[frameCount].pacmanX = pacmanX;
                replayData[frameCount].pacmanY = pacmanY;
                for (int i = 0; i < 5; i++) {
                    replayData[frameCount].ghostX[i] = ghosts[i].x;
                    replayData[frameCount].ghostY[i] = ghosts[i].y;
                }
                frameCount++;
            }
            float deltaTime = GetFrameTime();



            starTimer += deltaTime;

            starTimer += deltaTime;

            if (starTimer >= 0.2f) {
                currentStarFrame = (currentStarFrame + 1) % STAR_FRAME_COUNT;
                starTimer = 0.0f;
            }

            UpdatePacmanFrame(deltaTime);
            UpdatePacmanTexture();

            pacmanTimer += deltaTime;

            CheckCherryCollision(&cherry, pacmanX, pacmanY);
            CheckGhostCollision(ghosts, 10, pacmanX, pacmanY, &jan, pacmanBoard, &cherry);


            if (pepperBoostTimer > 0.0f) {
                pepperBoostTimer -= deltaTime;
            }

            press(&user_key, &pacmanCurrentTexture, pacmanTextures, pacmanFrame);
            float offsetX = pacmanX - (pacmanCurrentCol * TILE_SIZE + TILE_SIZE / 2.0f);
            float offsetY = pacmanY - (pacmanCurrentRow * TILE_SIZE + TILE_SIZE / 2.0f);
            float way_to_center = sqrt(offsetX * offsetX + offsetY * offsetY);

            CheckAppleCollision(&apple, pacmanX, pacmanY, &jan);
            CheckPepperCollision(&pepper, pacmanX, pacmanY);
            CheckMushroomCollision(&mushroom, pacmanX, pacmanY, &jan);

            if (pepperBoostTimer > 0.0f) {
                pepperBoostTimer -= deltaTime;
            }

            float pacmanSpeed = (pepperBoostTimer > 0.0f) ? boostedSpeed : normalSpeed;

            if (jan <= 0) {
                SaveScore(score, playerName);
                gameState = 2;
            }

            if (way_to_center < 1.5f) {
                if (user_key != 0) {
                    int deltaRow, deltaCol;
                    Convert_direction_to_move(user_key, &deltaRow, &deltaCol);
                    int targetRow = pacmanCurrentRow + deltaRow;
                    int targetCol = pacmanCurrentCol + deltaCol;

                    if (targetRow >= 0 && targetRow < ROWS && targetCol >= 0 && targetCol < COLS &&
                        !is_mane(pacmanBoard[targetRow][targetCol])) {
                        currentDirection = user_key;
                    }
                }


                if (currentDirection != 0) {
                    int deltaRow, deltaCol;
                    Convert_direction_to_move(currentDirection, &deltaRow, &deltaCol);
                    int targetRow = pacmanCurrentRow + deltaRow;
                    int targetCol = pacmanCurrentCol + deltaCol;

                    if (targetRow >= 0 && targetRow < ROWS && targetCol >= 0 && targetCol < COLS &&
                        is_mane(pacmanBoard[targetRow][targetCol])) {
                        currentDirection = 0;
                    }
                }
            }

            int deltaRow, deltaCol;
            Convert_direction_to_move(currentDirection, &deltaRow, &deltaCol);

            float moveX = deltaCol * pacmanSpeed * deltaTime;
            float moveY = deltaRow * pacmanSpeed * deltaTime;

            float potentialX = pacmanX + moveX;
            if (canMove(pacmanBoard, potentialX, pacmanY)) {
                pacmanX = potentialX;
            } else {
                moveX = 0;
            }

            float potentialY = pacmanY + moveY;
            if (canMove(pacmanBoard, pacmanX, potentialY)) {
                pacmanY = potentialY;
            } else {
                moveY = 0;
            }

            int new_place_y = (int) (pacmanX / TILE_SIZE);
            int new_place_x = (int) (pacmanY / TILE_SIZE);

            if (new_place_y >= 0 && new_place_y < COLS && new_place_x >= 0 && new_place_x < ROWS &&
                !is_mane(pacmanBoard[new_place_x][new_place_y])) {
                pacmanCurrentCol = new_place_y;
                pacmanCurrentRow = new_place_x;
            } else {
                currentDirection = 0;
            }

            for (int i = 0; i < 10; i++) {
                if (Array_star[i].active) {
                    int cellX = Array_star[i].col * TILE_SIZE + TILE_SIZE / 2;
                    int cellY = Array_star[i].row * TILE_SIZE + TILE_SIZE / 2;

                    float distX = pacmanX - cellX;
                    float distY = pacmanY - cellY;
                    float distance = sqrt(distX * distX + distY * distY);

                    if (distance < TILE_SIZE / 2.0f) {
                        Array_star[i].active = false;
                        score += 17;
                        PlaySound(star);
                    }
                }
            }


            CheckGhostCollision(ghosts, 10, pacmanX, pacmanY, &jan, pacmanBoard, &cherry);

            bool allInactive = true;
            for (int i = 0; i < 10; i++) {
                if (Array_star[i].active) {
                    allInactive = false;
                    break;
                }
            }
            if (allInactive && !cherry.athome) {
                Star(pacmanBoard, Array_star, &apple, &cherry, &mushroom, &pepper);
            }

            for (int i = 1; i < max_ghost; i++) {
                MoveGhost(&ghosts[i], pacmanBoard, currentGhostSpeed, deltaTime);
            }


            MoveGhostDiscrete(&ghosts[0], pacmanBoard, pacmanX, pacmanY);
            BeginDrawing();
            ClearBackground((Color) {0, 0, 70, 255});

            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    Vector2 position = {j * TILE_SIZE, i * TILE_SIZE};


                    //            ######################################################################

                    //move

                    float pacmanSpeed = 150.0f;
                    float moveX = 0.0f;
                    float moveY = 0.0f;


                    if (user_key == -2) {
                        moveY = -pacmanSpeed * deltaTime;
                    } else if (user_key == 2) {
                        moveY = pacmanSpeed * deltaTime;
                    } else if (user_key == -1) {
                        moveX = -pacmanSpeed * deltaTime;
                    } else if (user_key == 1) {
                        moveX = pacmanSpeed * deltaTime;
                    }

                    if (pacmanBoard[pacmanCurrentRow][pacmanCurrentCol] == 'e') {

                        float centerX = 4 * TILE_SIZE + TILE_SIZE / 2.0f;
                        float centerY = 3 * TILE_SIZE + TILE_SIZE / 2.0f;

                        float distX = pacmanX - centerX;
                        float distY = pacmanY - centerY;
                        float distance = sqrt(distX * distX + distY * distY);


                        if (distance < TILE_SIZE / 10.0f) {
                            PlaySound(chaleh);
                            pacmanX = 27 * TILE_SIZE;
                            pacmanY = 15 * TILE_SIZE + TILE_SIZE / 2.0f;

                        }
                    }


                    if (pacmanBoard[pacmanCurrentRow][pacmanCurrentCol] == '$') {

                        float centerX = 26 * TILE_SIZE + TILE_SIZE / 2.0f;
                        float centerY = 15 * TILE_SIZE + TILE_SIZE / 2.0f;

                        float distX = pacmanX - centerX;
                        float distY = pacmanY - centerY;
                        float distance = sqrt(distX * distX + distY * distY);


                        if (distance < TILE_SIZE / 10.0f) {
                            PlaySound(chaleh);
                            pacmanX = 4 * TILE_SIZE;
                            pacmanY = 3 * TILE_SIZE + TILE_SIZE / 2.0f;


                            pacmanCurrentRow = 4;
                            pacmanCurrentCol = 3;

                        }
                    }


                    if (pacmanBoard[pacmanCurrentRow][pacmanCurrentCol] == '!') {

                        float centerX = 0 * TILE_SIZE;
                        float centerY = 9 * TILE_SIZE + TILE_SIZE / 2.0f;

                        float distX = pacmanX - centerX;
                        float distY = pacmanY - centerY;
                        float distance = sqrt(distX * distX + distY * distY);


                        // بررسی فاصله
                        if (distance < TILE_SIZE / 10.0f) {
                            PlaySound(chaleh);
                            // تنظیم موقعیت جدید Pac-Man در صورت برخورد با 'e'
                            pacmanX = 30 * TILE_SIZE + TILE_SIZE / 2.0f;
                            pacmanY = 9 * TILE_SIZE + TILE_SIZE / 2.0f;

                            // به‌روزرسانی موقعیت فعلی Pac-Man در خانه‌ها
                            pacmanCurrentRow = 30;
                            pacmanCurrentCol = 9;

                        }
                    }


                    if (pacmanBoard[pacmanCurrentRow][pacmanCurrentCol] == '^') {

                        float centerX = 31 * TILE_SIZE;
                        float centerY = 9 * TILE_SIZE + TILE_SIZE / 2.0f;

                        float distX = pacmanX - centerX;
                        float distY = pacmanY - centerY;
                        float distance = sqrt(distX * distX + distY * distY);


                        if (distance < TILE_SIZE / 10.0f) {
                            PlaySound(chaleh);
                            pacmanX = 0 * TILE_SIZE + TILE_SIZE / 2.0;
                            pacmanY = 9 * TILE_SIZE + TILE_SIZE / 2.0f;


                            pacmanCurrentRow = 0;
                            pacmanCurrentCol = 9;

                        }
                    }
//                    ********************************************************************8
//map2

                    if (pacmanBoard[pacmanCurrentRow][pacmanCurrentCol] == '@'&&user_key == 1) {

                        // محاسبه مرکز خانه‌ای که 'e' در آن قرار دارد
                        float centerX = 15 * TILE_SIZE + TILE_SIZE / 2.0f;
                        float centerY = 1 * TILE_SIZE + TILE_SIZE / 2.0f;

                        // محاسبه فاصله Pac-Man از مرکز خانه
                        float distX = pacmanX - centerX;
                        float distY = pacmanY - centerY;
                        float distance = sqrt(distX * distX + distY * distY);


                        // بررسی فاصله
                        if (distance < TILE_SIZE / 10.0f) {
                            PlaySound(chaleh);
                            // تنظیم موقعیت جدید Pac-Man در صورت برخورد با 'e'
                            pacmanX = 16 * TILE_SIZE;
                            pacmanY = 17 * TILE_SIZE + TILE_SIZE / 2.0f;

                        }
                    }


                    if (pacmanBoard[pacmanCurrentRow][pacmanCurrentCol] == '?'&&user_key == -1) {

                        float centerX = 15 * TILE_SIZE + TILE_SIZE / 2.0f;
                        float centerY = 17 * TILE_SIZE + TILE_SIZE / 2.0f;

                        float distX = pacmanX - centerX;
                        float distY = pacmanY - centerY;
                        float distance = sqrt(distX * distX + distY * distY);


                        if (distance < TILE_SIZE / 10.0f) {
                            PlaySound(chaleh);
                            pacmanX = 15 * TILE_SIZE;
                            pacmanY = 1 * TILE_SIZE + TILE_SIZE / 2.0f;


                            pacmanCurrentRow = 4;
                            pacmanCurrentCol = 3;

                        }
                    }

                    if (pacmanBoard[pacmanCurrentRow][pacmanCurrentCol] == '@'&&user_key == -1) {

                        // محاسبه مرکز خانه‌ای که 'e' در آن قرار دارد
                        float centerX = 15 * TILE_SIZE + TILE_SIZE / 2.0f;
                        float centerY = 1 * TILE_SIZE + TILE_SIZE / 2.0f;

                        // محاسبه فاصله Pac-Man از مرکز خانه
                        float distX = pacmanX - centerX;
                        float distY = pacmanY - centerY;
                        float distance = sqrt(distX * distX + distY * distY);


                        // بررسی فاصله
                        if (distance < TILE_SIZE / 10.0f) {
                            PlaySound(chaleh);
                            // تنظیم موقعیت جدید Pac-Man در صورت برخورد با 'e'
                            pacmanX = 15 * TILE_SIZE;
                            pacmanY = 17 * TILE_SIZE + TILE_SIZE / 2.0f;

                        }
                    }


                    if (pacmanBoard[pacmanCurrentRow][pacmanCurrentCol] == '?'&&user_key == 1) {

                        float centerX = 15 * TILE_SIZE + TILE_SIZE / 2.0f;
                        float centerY = 17 * TILE_SIZE + TILE_SIZE / 2.0f;

                        float distX = pacmanX - centerX;
                        float distY = pacmanY - centerY;
                        float distance = sqrt(distX * distX + distY * distY);


                        if (distance < TILE_SIZE / 10.0f) {
                            PlaySound(chaleh);
                            pacmanX = 16 * TILE_SIZE;
                            pacmanY = 1 * TILE_SIZE + TILE_SIZE / 2.0f;


                            pacmanCurrentRow = 4;
                            pacmanCurrentCol = 3;


                        }
                    }

                    //            ######################################################################


                    Rectangle tile = {position.x, position.y, TILE_SIZE, TILE_SIZE};

                    if (pacmanBoard[i][j] == '#') {
                        DrawTexture(textureEnemy, position.x, position.y, WHITE);
                    } else if (pacmanBoard[i][j] == '&') {
                        DrawTexture(textureBonus, position.x, position.y, WHITE);
                    } else if (pacmanBoard[i][j] == '*') {
                        DrawTexture(textureWall, position.x, position.y, WHITE);
                    } else if (pacmanBoard[i][j] == '%') {
                        DrawTexture(texturePlayer, position.x, position.y, WHITE);
                    } else if (pacmanBoard[i][j] == '+') {
                        DrawRectangleRec(tile, (Color) {255, 255, 0, 255});
                    } else if (pacmanBoard[i][j] == 'e' || pacmanBoard[i][j] == '$'||pacmanBoard[i][j]=='@'||
                               pacmanBoard[i][j]=='?') {
                        DrawTexture(textureFood, position.x, position.y, WHITE);
                    }

// تعریف آرایه‌ای از بافت‌های ستاره
                    Texture2D starTextures[STAR_FRAME_COUNT] = {starTexture1, starTexture2, starTexture3};

// در تابع رسم بازی (مثلاً در حلقه رسم ستاره‌ها)
                    for (int k = 0; k < 10; k++) {
                        if (Array_star[k].active && Array_star[k].row == i && Array_star[k].col == j &&
                            pacmanBoard[i][j] == '.') {
                            // انتخاب فریم فعلی
                            Texture2D currentStarTexture = starTextures[currentStarFrame];

                            // رسم ستاره با استفاده از فریم فعلی
                            DrawTextureEx(
                                    currentStarTexture,
                                    (Vector2) {j * TILE_SIZE, i * TILE_SIZE},
                                    0.0f,
                                    ((float) TILE_SIZE / currentStarTexture.width),
                                    WHITE
                            );
                        }
                    }
                }
            }

            DrawGhosts(ghosts);
            DrawCherry(&cherry);
            DrawPepper(&pepper);
            DrawApple(&apple);
            DrawMushroom(&mushroom);

            char scoreText[50];
            sprintf(scoreText, "SCORE : %d", score);
            DrawText(scoreText, 34, 536, 20, (Color) {0, 0, 0, 255});

            char lifeText[50];
            sprintf(lifeText, "LIFE : %d", jan);
            DrawText(lifeText, 753, 536, 20, (Color) {0, 0, 0, 255});

            DrawTextureEx(
                    pacmanCurrentTexture,
                    (Vector2) {pacmanX - TILE_SIZE / 1.18 / 2, pacmanY - TILE_SIZE / 1.1 / 2},
                    0.0f,
                    (TILE_SIZE / 1.2 / pacmanCurrentTexture.width),
                    WHITE
            );

            EndDrawing();
        } else if (gameState == 2) {
            GameOverScreen();
            gameState = 0; // بازگشت به منوی اصلی پس از نمایش صفحه‌ی Game Over
        } else if (gameState == STATE_REPLAY) {
            frameCount = 0;
            DisplayReplay(); // نمایش replay
            gameState = STATE_MENU; // بازگشت به منوی اصلی پس از نمایش replay
        }
    }


    // آزادسازی بافت‌ها در انتهای برنامه
    UnloadTexture(starTexture1);
    UnloadTexture(starTexture2);
    UnloadTexture(starTexture3);
    UnloadGhostTextures(ghosts);
    UnloadTexture(pepper.texture);
    UnloadTexture(apple.texture);
    UnloadTexture(mushroom.texture);
    for (int i = 0; i < 4; i++) {
        UnloadTexture(pacmanTextures[i].frames[0]);
        UnloadTexture(pacmanTextures[i].frames[1]);
    }


    StopBackgroundMusic();
    CloseWindow();

    return 0;
}
