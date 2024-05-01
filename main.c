#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "elem.h"
#include "mov.h"
#include "coll.h"
SDL_Window *window;
SDL_Renderer *renderer;
const MAP_WIDTH = 10;
const MAP_HEIGHT = 8;
const TILE_WIDTH= 64;
const TILE_HEIGHT = 32;
char map[] = {
    'a','a','a','a','a','a','a','a','a','a',
    'a','a','a','a','a','a','a','a','a','a',
    'a','a','a','a','a','a','a','a','a','a',
    'a','a','a','a','a','a','a','a','a','a',
    'a','a','a','a','a','a','a','a','a','a',
    'a','a','a','a','a','a','a','a','a','a',
    'a','a','a','a','a','a','a','a','a','a',
    'a','a','a','a','a','a','a','a','a','a',


};

Ball ball = {
        {320, 360}, {8, 8}, 4.0, {1.0,1.0}
};

Paddle paddle = {
        {320, 460}, {128, 16}, 2.0, 0.0 
};

int running = 1;

void game() {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
                printf("SDL_Init Error: %s\n", SDL_GetError());
        }

        window = SDL_CreateWindow("Breakout", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, 640, 480,
                                  SDL_WINDOW_SHOWN);
        if (window == NULL) {
                printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
                SDL_Quit();
        }

        renderer = SDL_CreateRenderer(
            window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer == NULL) {
                SDL_DestroyWindow(window);
                printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
                SDL_Quit();
        }
        
        Tile tiles[MAP_HEIGHT*MAP_WIDTH];
        int row=0;
        int col=0;
        for (int i = 0; i < MAP_HEIGHT * MAP_WIDTH; i++) {
                if (col > MAP_WIDTH) {
                        col = 0;
                        row++;
                }
                Tile tile = {
                        {
                            col * TILE_WIDTH, row * TILE_HEIGHT
                        },
                        {
                            TILE_WIDTH, TILE_HEIGHT
                        },
                        Def,
                        1,
                };
                col++;
                tiles[i] = tile;
        }
        while (running) {
                SDL_Event event;
                while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                                running = 0;
                        }
                        if (event.key.keysym.sym == SDLK_ESCAPE) {
                                running = 0;
                        }
                        if (event.key.keysym.sym == SDLK_UP) {
                        }
                        if (event.type == SDL_KEYUP) {
                                if (event.key.keysym.sym == SDLK_UP) {
                                }
                        }
                        if (event.key.keysym.sym == SDLK_DOWN) {
                        }
                        if (event.type == SDL_KEYUP) {
                                if (event.key.keysym.sym == SDLK_DOWN) {
                                }
                        }
                        if (event.key.keysym.sym == SDLK_LEFT) {
                            paddle.p.x -= 16;
                        }

                        if (event.type == SDL_KEYUP) {
                                if (event.key.keysym.sym == SDLK_LEFT) {
                                }
                        }
                        if (event.key.keysym.sym == SDLK_RIGHT) {
                            paddle.p.x += 16;
                        }
                        if (event.type == SDL_KEYUP) {
                                if (event.key.keysym.sym == SDLK_RIGHT) {
                                }
                        }
                        if (event.key.keysym.sym == SDLK_SPACE) {
                        }
                        if (event.type == SDL_KEYUP) {
                                if (event.key.keysym.sym == SDLK_SPACE) {
                                }
                        }
                }
                // move ball
                moveBall(&ball, &paddle);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                // paddle
                SDL_Rect paddleR = { paddle.p.x, paddle.p.y, paddle.size.x, paddle.size.y }; // x, y, width, height
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Set color to red
                SDL_RenderFillRect(renderer, &paddleR);

                for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++) {
                        Tile* tile = &tiles[i];
                        collideWithTile(&ball, tile);
                        if (tile->alive == 0) {
                                continue;
                        }
                        SDL_Rect tileR = { tile->p.x, tile->p.y, tile->size.x, tile->size.y }; // x, y, width, height
                        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Set color to red
                        SDL_RenderFillRect(renderer, &tileR);
                }
                // ball
                SDL_Rect ballR = { ball.p.x, ball.p.y, ball.size.y, ball.size.y }; // x, y, width, height
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Set color to red
                SDL_RenderFillRect(renderer, &ballR);
                SDL_RenderPresent(renderer);
        }
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
}
int main() {
        game();
        return 1;
}
