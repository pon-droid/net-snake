#pragma once


#include<array>


#include"Snake.h"
#include <SDL2/SDL_ttf.h>



struct Visual{
    SDL_Renderer *rend;
    SDL_Window *win;

    SDL_Texture *text;

    

    std::array<int, (MAP_W*MAP_H)> MAP;
    bool apple;

    Visual();
    void draw_map();
    void draw_snake(Snake s);

    void update_state(Snake& s);
    void font_render();
    void update_buffer(Snake s, Snake m);
    ~Visual();
    
    
};