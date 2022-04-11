#pragma once


#include<array>


#include"Snake.h"



struct Visual{
    SDL_Renderer *rend;
    SDL_Window *win;

    

    std::array<int, (MAP_W*MAP_H)> MAP;

    Visual();
    void draw_map();
    void draw_snake(Snake s);

    void update_state(Snake& s);
    void update_buffer(Snake s);
    ~Visual();
    
    
};