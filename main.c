#include <SDL2/SDL.h>
#include <curses.h>
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "core.h"
#include "image.h"
#include "sound.h"

typedef struct GameArgs {
        Node* nodes;
        int nodes_size;
} GameArgs;
Node newNode(char* name) {
        Node n = {
            0,
            0,
            0,
            0,
            randI(),
            name,
            NULL,

        };
        return n;
}
SDL_Window *window;
SDL_Renderer *renderer;
int running = 1;
int randI() {
        int random_number;
        srand(time(NULL));
        random_number = rand();
        return random_number;
}
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
void openVim(const char* filename) {
    char command[100];
    snprintf(command, sizeof(command), "vim project/scripts/%s.lua", filename);
    system(command);
}
void pushNodeToLua(lua_State *L, Node entity) {
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
void getNodeFromLua(lua_State *L, Node *entity) {
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
void tui(GameArgs *args) {

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
        curs_set(0);
        init_pair(1, COLOR_CYAN, COLOR_BLACK);  // Define color pair
        // Create a window for the menu
        int maxy = getmaxy(menu_win);
        menu_win = newwin(24, 80, 0, 1);
        box(menu_win, 0, 0);
        int height, width;
        getmaxyx(menu_win, height, width);
        refresh();
        wrefresh(menu_win);
        // Menu options
        char *choices[] = {"Option 1", "Option 2", "Option 3", "Exit"};
        wrefresh(menu_win);
        while (true) {
                for (int y = 0; y < height - 2; y++) {
                    mvaddch(y + 1, width / 4, ACS_VLINE);
                }
                refresh();
                wrefresh(menu_win);
                char c = wgetch(menu_win);
                switch (c) {
                        case 'w':
                                if (highlight == 1)
                                        highlight = args->nodes_size;
                                else
                                        --highlight;
                                break;
                        case 's':
                                if (highlight == args->nodes_size)
                                        highlight = 1;
                                else
                                        ++highlight;
                                break;
                        case 'n':
                                args->nodes_size++;
                                args->nodes = (Node *)realloc(args->nodes, args->nodes_size * sizeof(Node));

                                char name[32];
                                echo();
                                nodelay(menu_win, FALSE);
                                printw("\nInput ");
                                refresh();
                                scanw("%s", name);
                                nodelay(menu_win, TRUE);
                                noecho();
                                args->nodes[args->nodes_size-1] = newNode(name);
                                refresh();
                                break;
                        case 'e':
                                choice = highlight;
                                char buffer[100];
                                snprintf(buffer, sizeof(buffer), "%s", args->nodes[args->nodes_size-1].name);
                                openVim(buffer); 
                                endwin();
                                refresh();
                                break;
                        case 'q':
                                endwin();
                                return;
                                break;
                        default:
                                refresh();
                                break;
                }
                for (int i = 0; i < args->nodes_size; i++) {
                        if (highlight == i + 1) {
                                wattron(menu_win, A_REVERSE);
                                mvwprintw(menu_win, i + 1, 1, "%s", args->nodes[i].name);
                                wattroff(menu_win, A_REVERSE);

                        } else {
                               mvwprintw(menu_win, i + 1, 1, "%s", args->nodes[i].name);
                        }
                }
        }
        getch();
        endwin();  // End curses mode
}
void *game(void* game_args) {

        GameArgs *args = (GameArgs*) game_args;
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
                printf("SDL_Init Error: %s\n", SDL_GetError());
        }

        window = SDL_CreateWindow("Uraani", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, 800, 600,
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

        lua_State *L = loadLuaScript("project/scripts/script.lua");
        SDL_Rect rect = {100, 100, 50, 50};
        Node player = {
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

                pushNodeToLua(L, player);
                lua_setglobal(L, "player");
                lua_getglobal(L, "update");
                if (lua_pcall(L, 0, 0, 0) != 0) {
                        printf("Error running Lua script: %s\n",
                               lua_tostring(L, -1));
                }
                for (int i = 0; i < args->nodes_size; i++) {
                        Node entity = args->nodes[i];
                        getNodeFromLua(L, &entity);
                        rect.x = entity.x;
                        rect.y = entity.y;
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                        SDL_RenderFillRect(renderer, &rect);
                }
                // SDL_RenderCopy(renderer, texture, NULL, NULL);
                SDL_RenderPresent(renderer);
        }
        freeTGA(image);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
}
int main() {

        int nodes_size = 0;
        Node *nodes = (Node *)malloc(nodes_size * sizeof(Node));
        pthread_t thread;
        GameArgs game_args = {
                nodes,
                nodes_size,
        };
        if (pthread_create(&thread, NULL, game, (void*)&game_args)!= 0) {
                fprintf(stderr, "Error: Failed to create thread\n");
                return 1;
        }
        tui(nodes);
        return 1;
}
