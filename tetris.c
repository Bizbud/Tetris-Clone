#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define WINDOW_NAME "Tetris"

const int WIDTH = 800, HEIGHT = 1280;

SDL_Window *window = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *blocks = NULL;
SDL_Rect testRect;

bool grid[16][21];

typedef struct Block
{
    int x, y;
    bool squares[4][4];
    int r, g, b;
} Block;

bool L[4][4] = {{false, false, true, false},
                {false, false, true, false},
                {false, false, true, false},
                {false, true, true, false}};

bool S[4][4] = {{false, false, true, true},
                {false, true, true, false},
                {false, false, false, false},
                {false, false, false, false}};

void newL(Block *current);
void newS(Block *current);
int initSDL();
int render(Block *currentBlock);

int main()
{
    // Init SDL/Window
    SDL_Init(SDL_INIT_VIDEO);
    
    window = SDL_CreateWindow(
        WINDOW_NAME,                       
        SDL_WINDOWPOS_UNDEFINED,        
        SDL_WINDOWPOS_UNDEFINED,           
        WIDTH,                           
        HEIGHT,                           
        SDL_WINDOW_MAXIMIZED
    );
    if(window == NULL) 
    {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    // Init surfaces
    screen = SDL_GetWindowSurface(window);
    blocks = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0, 0, 0, 0);

    // Init draw rect
    testRect.x = 0;
    testRect.y = 0;
    testRect.w = 50;
    testRect.h = 50;

    // Create new block
    Block *currentBlock = malloc(sizeof(Block));
    newL(currentBlock);


    bool isRunning = true;
    bool isFalling = true;

    for(int i = 0; i < 16; i++)     // Set base collisions
    {
        grid[i][20] = true;
    }

    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    SDL_Event event;
    while(isRunning)
    {
        while(SDL_PollEvent(&event)) 
        {
            if(event.type == SDL_QUIT) 
            {
                isRunning = false;
                break;
            }
        }

        for(int x = 0; x < 4; x++)
        {
            for(int y = 0; y < 4; y++)
            {
                int realX = currentBlock->x + x;
                int realY = currentBlock->y + y;

                if(grid[realX][realY + 1])
                {
                    isFalling = false;
                    goto FINISH;
                }
            }
        }

        if(keys[SDL_SCANCODE_D])
        {
            currentBlock->x += 1;
        }
        else if(keys[SDL_SCANCODE_A])
        {
            currentBlock->x -= 1;
        }
        currentBlock->y += 1;
        printf("y: %d\n", currentBlock->y + 4);

        FINISH:  
        render(currentBlock);
    }
}

int initSDL()
{
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        WINDOW_NAME,                       
        SDL_WINDOWPOS_UNDEFINED,        
        SDL_WINDOWPOS_UNDEFINED,           
        WIDTH,                           
        HEIGHT,                           
        SDL_WINDOW_MAXIMIZED
    );

    if(window == NULL) 
    {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    screen = SDL_GetWindowSurface(window);
    blocks = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
    return 0;
}

int render(Block *currentBlock)
{
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));        
    SDL_BlitSurface(blocks, NULL, screen, NULL); 
    if(isFalling)
    {
        for(int x = 0; x < 4; x++)
        {
            for(int y = 0; y < 4; y++)
            {
                if(currentBlock->squares[x][y])
                {
                    testRect.x = (currentBlock->x + x) * 50;
                    testRect.y = (currentBlock->y + y) * 50;

                    SDL_FillRect(screen, &testRect, SDL_MapRGB(screen->format, currentBlock->r, currentBlock->g, currentBlock->b));
                }
            }
        }
    }
    else
    {
        for(int x = 0; x < 4; x++)
        {
            for(int y = 0; y < 4; y++)
            {
                if(currentBlock->squares[x][y])
                {
                    testRect.x = (currentBlock->x + x) * 50;
                    testRect.y = (currentBlock->y + y) * 50;
                        
                    SDL_FillRect(blocks, &testRect, SDL_MapRGB(screen->format, currentBlock->r, currentBlock->g, currentBlock->b));

                    grid[currentBlock->x + x][currentBlock->y + y] = true;
                }
            }
        }
        newL(currentBlock);
        isFalling = true;
    }

    for(int i = 0; i < 16; i++)
    {
        testRect.x = i * 50;
        testRect.y = 19 * 50;
        SDL_FillRect(screen, &testRect, SDL_MapRGB(screen->format, 255, 255, 1));
    } 
    SDL_UpdateWindowSurface(window); 
}

void newL(Block *current)
{
    current->x = 5;
    current->y = 1;
    current->r = 255;
    current->g = 1;
    current->b = 1;
    memcpy(current->squares, L, sizeof(current->squares));
}

void newS(Block *current)
{
    current->x = 5;
    current->y = 1;
    current->r = 255;
    current->g = 255;
    current->b = 255;
    memcpy(current->squares, S, sizeof(current->squares));
}