#include"Snake.h"

Snake::Snake(int x, int y){
    dx = dy = 0;
    body.push_back({x,y});
    body.push_back({x,y+1});
    body.push_back({x,y+2});
    //minus d vars from back of deque to add 
}

void Snake::control(){
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if(keys[SDL_SCANCODE_RIGHT]){
        dx = 1;
        dy = 0;
        return;
    }
    if(keys[SDL_SCANCODE_LEFT]){
        dx = -1;
        dy = 0;
        return;
    }
    if(keys[SDL_SCANCODE_UP]){
        dx = 0;
        dy = -1;
        return;
    }
    if(keys[SDL_SCANCODE_DOWN]){
        dx = 0;
        dy = 1;
        return;
    }
}

void Snake::update(){
    control();

    body.push_front({body.front().x + dx, body.front().y + dy});
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if(keys[SDL_SCANCODE_9]){
        return;
    }
    body.pop_back();

    

}

