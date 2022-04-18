#pragma once




#include"Snake.h"


struct Visual{
    SDL_Renderer *rend;
    SDL_Window *win;


    TTF_Font *font;

    

    std::array<int, (MAP_W*MAP_H)> MAP;
    bool apple;
    int last_pos;

    Visual();
    void draw_map();
    void draw_snake(const Snake& s);

    void update_state(Snake& s, int random);
    void font_render(const SDL_Colour colour, const std::string info);
    void update_buffer(const std::vector<Snake>& snakes);
    ~Visual();
    
    
};