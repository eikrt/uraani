#include <SDL2/SDL.h>
#include <curses.h>
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "core.h"
#include "image.h"
#include "sound.h"

SDL_Window *window;
SDL_Renderer *renderer;
int running = 1;

lua_State *loadLuaScript(const char *filename) {
        lua_State *L = luaL_newstate();
        luaL_openlibs(L);

        if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0)) {
                printf("Error: %s\n", lua_tostring(L, -1));
                exit(1);
        }

        lua_getglobal(L, "update");
        if (!lua_isfunction(L, -1)) {
                printf("Error: 'update' function not found in Lua script\n");
                exit(1);
        }
        return L;
}
void pushEntityToLua(lua_State *L, Entity entity) {
        lua_newtable(L);
        lua_pushinteger(L, entity.x);
        lua_setfield(L, -2, "x");
        lua_pushinteger(L, entity.y);
        lua_setfield(L, -2, "y");
        lua_pushinteger(L, entity.width);
        lua_setfield(L, -2, "width");
        lua_pushinteger(L, entity.height);
        lua_setfield(L, -2, "height");
}
void getEntityFromLua(lua_State *L, Entity *entity) {
        lua_getglobal(L, "player");
        lua_getfield(L, -1, "x");
        entity->x = lua_tointeger(L, -1);
        lua_pop(L, 1);
        lua_getfield(L, -1, "y");
        entity->y = lua_tointeger(L, -1);
        lua_pop(L, 1);  // Pop y value
        lua_getfield(L, -1, "width");
        entity->width = lua_tointeger(L, -1);
        lua_pop(L, 1);  // Pop width value
        lua_getfield(L, -1, "height");
        entity->height = lua_tointeger(L, -1);
        lua_pop(L, 2);  // Pop
}
int tui() {
        WINDOW *menu_win;
        int highlight = 1;
        int choice = 0;
        int c;
        initscr();
        clear();               // Clear the screen
        noecho();              // Don't echo user input
        cbreak();              // Line buffering disabled
        start_color();         // Enable colors if supported
        keypad(stdscr, TRUE);  // Enable keyboard input

        init_pair(1, COLOR_CYAN, COLOR_BLACK);  // Define color pair

        // Create a window for the menu
        int maxy = getmaxy(menu_win);
        menu_win = newwin(24, 80, 0, 1);
        box(menu_win, 0, 0);
        refresh();
        wrefresh(menu_win);

        // Menu options
        char *choices[] = {"Option 1", "Option 2", "Option 3", "Exit"};

        // Display menu options
        for (int i = 0; i < 4; i++) {
                if (highlight == i + 1) {
                        wattron(menu_win, A_REVERSE);
                        mvwprintw(menu_win, i + 1, 1, "%s", choices[i]);
                        wattroff(menu_win, A_REVERSE);
                } else {
                        mvwprintw(menu_win, i + 1, 1, "%s", choices[i]);
                }
        }
        wrefresh(menu_win);
        while (true) {
                char c = wgetch(menu_win);
                switch (c) {
                        case 'w':
                                if (highlight == 1)
                                        highlight = 4;
                                else
                                        --highlight;
                                break;
                        case 's':
                                if (highlight == 4)
                                        highlight = 1;
                                else
                                        ++highlight;
                                break;
                        case 10:
                                choice = highlight;
                                break;
                        default:
                                refresh();
                                break;
                }
                for (int i = 0; i < 4; i++) {
                        if (highlight == i + 1) {
                                wattron(menu_win, A_REVERSE);
                                mvwprintw(menu_win, i + 1, 1, "%s", choices[i]);
                                wattroff(menu_win, A_REVERSE);

                        } else {
                                mvwprintw(menu_win, i + 1, 1, "%s", choices[i]);
                        }
                }
                wrefresh(menu_win);
                if (choice != 0) {
                        mvprintw(22, 2,
                                 "You chose option %d with choice string %s",
                                 choice, choices[choice - 1]);

                        refresh();
                }
        }
        getch();
        endwin();  // End curses mode
}
void *game(void* arg) {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
                printf("SDL_Init Error: %s\n", SDL_GetError());
                return 1;
        }

        window = SDL_CreateWindow("Uraani", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, 800, 600,
                                  SDL_WINDOW_SHOWN);
        if (window == NULL) {
                printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
                SDL_Quit();
                return 1;
        }

        renderer = SDL_CreateRenderer(
            window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer == NULL) {
                SDL_DestroyWindow(window);
                printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
                SDL_Quit();
                return 1;
        }

        lua_State *L = loadLuaScript("project/scripts/script.lua");
        SDL_Rect rect = {100, 100, 50, 50};
        Entity player = {
            0,
            0,
            0,
            0,
        };
        pthread_t thread;
        char *fileName = "project/music/song.ini";
        // if (pthread_create(&thread, NULL, playSound, (void *)&fileName) != 0)
        // {
        //                fprintf(stderr, "Error: Failed to create thread\n");
        //                return 1;
        //}

        TGAImage *image = loadTGA("project/images/test.tga");
        if (image == NULL) {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 1;
        }

        // Create SDL texture from TGA image data
        SDL_Texture *texture = SDL_CreateTexture(
            renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC,
            image->width, image->height);
        SDL_UpdateTexture(texture, NULL, image->data, image->width * 4);
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
                                lua_pushboolean(L, 1);
                                lua_setglobal(L, "up");
                        }
                        if (event.type == SDL_KEYUP) {
                                if (event.key.keysym.sym == SDLK_UP) {
                                        lua_pushboolean(L, 0);
                                        lua_setglobal(L, "up");
                                }
                        }
                        if (event.key.keysym.sym == SDLK_DOWN) {
                                lua_pushboolean(L, 1);
                                lua_setglobal(L, "down");
                        }
                        if (event.type == SDL_KEYUP) {
                                if (event.key.keysym.sym == SDLK_DOWN) {
                                        lua_pushboolean(L, 0);
                                        lua_setglobal(L, "down");
                                }
                        }
                        if (event.key.keysym.sym == SDLK_LEFT) {
                                lua_pushboolean(L, 1);
                                lua_setglobal(L, "left");
                        }

                        if (event.type == SDL_KEYUP) {
                                if (event.key.keysym.sym == SDLK_LEFT) {
                                        lua_pushboolean(L, 0);
                                        lua_setglobal(L, "left");
                                }
                        }
                        if (event.key.keysym.sym == SDLK_RIGHT) {
                                lua_pushboolean(L, 1);
                                lua_setglobal(L, "right");
                        }
                        if (event.type == SDL_KEYUP) {
                                if (event.key.keysym.sym == SDLK_RIGHT) {
                                        lua_pushboolean(L, 0);
                                        lua_setglobal(L, "right");
                                }
                        }
                        if (event.key.keysym.sym == SDLK_SPACE) {
                                lua_pushboolean(L, 1);
                                lua_setglobal(L, "space");
                        }
                        if (event.type == SDL_KEYUP) {
                                if (event.key.keysym.sym == SDLK_SPACE) {
                                        lua_pushboolean(L, 0);
                                        lua_setglobal(L, "space");
                                }
                        }
                }

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                pushEntityToLua(L, player);
                lua_setglobal(L, "player");
                lua_getglobal(L, "update");
                if (lua_pcall(L, 0, 0, 0) != 0) {
                        printf("Error running Lua script: %s\n",
                               lua_tostring(L, -1));
                }

                getEntityFromLua(L, &player);
                rect.x = player.x;
                rect.y = player.y;
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &rect);
                // SDL_RenderCopy(renderer, texture, NULL, NULL);
                SDL_RenderPresent(renderer);
        }
        freeTGA(image);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
}
int main() {
        pthread_t thread;
        if (pthread_create(&thread, NULL, game, NULL )!= 0) {
                fprintf(stderr, "Error: Failed to create thread\n");
                return 1;
        }
        tui();
}
