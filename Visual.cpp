#include "Visual.h"


Visual::Visual(){
    win = SDL_CreateWindow("snake",0,0, SCR_W, SCR_H, SDL_WINDOW_SHOWN);
    rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    MAP.fill(0);

}


void Visual::draw_map(){
    SDL_Rect rect;
    rect.w = CELL_W;
    rect.h = CELL_H;
    
	
	for(int y = 0; y < MAP_H; y++){
		for(int x = 0; x < MAP_W; x++){
			if(MAP[y*MAP_W+x]){
				SDL_SetRenderDrawColor(rend,0,0,255,255);
			} else {
				SDL_SetRenderDrawColor(rend,0,0,0,255);
			}

			rect.x = x * CELL_W;
			rect.y = y * CELL_H;

			SDL_RenderFillRect(rend,&rect);
		}
	}
}

void Visual::draw_snake(Snake s){
	SDL_SetRenderDrawColor(rend,0,255,0,255);

	SDL_Rect rect;
	rect.w = CELL_W;
	rect.h = CELL_H;

	for(const auto& i : s.body){
		rect.x = i.x * CELL_W;
		rect.y = i.y * CELL_H;
		SDL_RenderFillRect(rend,&rect);
	}
}

void Visual::update_state(Snake& s){
	int element = (s.body.front().y) * MAP_W + (s.body.front().x);
	if(MAP[element]){
		std::cout << "HIT\n";
	}
}


void Visual::update_buffer(Snake s){
	SDL_SetRenderDrawColor(rend,0,0,0,255);
	SDL_RenderClear(rend);

	draw_map();
	draw_snake(s);
	SDL_RenderPresent(rend);

}

Visual::~Visual(){
	SDL_DestroyWindow(win);
	SDL_DestroyRenderer(rend);
	
}