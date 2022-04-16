#include"Snake.h"
#include <unordered_map>

Snake::Snake(int x, int y, SDL_Colour col){

    dx = 0; dy = -1;

    body.push_back({x,y});
    body.push_back({x,y+1});
    body.push_back({x,y+2});

    hit = end = false;

    colour = col;

}

void Snake::control(){
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if(keys[SDL_SCANCODE_RIGHT] && !(dx == -1) && !(dy == 0)){
        dx = 1;
        dy = 0;
        return;
    }
    if(keys[SDL_SCANCODE_LEFT] && !(dx == 1) && !(dy == 0)){
        dx = -1;
        dy = 0;
        return;
    }
    if(keys[SDL_SCANCODE_UP] && !(dx == 0) && !(dy == 1)){
        dx = 0;
        dy = -1;
        return;
    }
    if(keys[SDL_SCANCODE_DOWN] && !(dx == 0) && !(dy == -1)){
        dx = 0;
        dy = 1;
        return;
    }
}

void Snake::update(){

    control();


    body.push_front({body.front().x + dx, body.front().y + dy});


    if(hit){
        hit = false;
        return;
    }

    body.pop_back();

   
    if(body.front().x >= MAP_W || body.front().x < 0 || body.front().y >= MAP_H || body.front().y < 0){
        end = true;
    }


    std::unordered_map<Segment,int,Body_hash> doubles;


    for(const auto& i: body){
        doubles[i]++;
    }
    for(const auto& i: doubles){
        if(i.second > 1){
            end = true;
        }
    }






    

}

