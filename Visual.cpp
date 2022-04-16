#include "Visual.h"


Visual::Visual(){
    win = SDL_CreateWindow("snake",0,0, SCR_W, SCR_H, SDL_WINDOW_SHOWN);
    rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    MAP.fill(0);
	apple = false;

	TTF_Font *font = TTF_OpenFont("font.ttf",10);

	SDL_Color colour = {255,255,255,255};
	SDL_Surface *surf = TTF_RenderText_Solid(font, "Utter Failure", colour);

	text = SDL_CreateTextureFromSurface(rend, surf);

	TTF_CloseFont(font);

	SDL_FreeSurface(surf);

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
	SDL_SetRenderDrawColor(rend,s.colour.r,s.colour.g,s.colour.b,s.colour.a);

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
		s.hit = true;
		apple = false;
		MAP.fill(0);
	}

	if(!(apple)){
		MAP[rand() % (MAP_W * MAP_H)] = 1;
		apple = true;
	}

}

void Visual::font_render(){
	SDL_Rect rect;
	rect.x = SCR_W/3;
	rect.y = SCR_H/3;
	rect.w = 250;
	rect.h = 50;

	SDL_RenderCopy(rend,text,NULL,&rect);
	SDL_RenderPresent(rend);
}

void Visual::update_buffer(Snake s, Snake m){
	SDL_SetRenderDrawColor(rend,0,0,0,255);
	SDL_RenderClear(rend);

	draw_map();
	draw_snake(s);
	draw_snake(m);
	SDL_RenderPresent(rend);

}

Visual::~Visual(){
	SDL_DestroyWindow(win);
	SDL_DestroyRenderer(rend);
	SDL_DestroyTexture(text);
	
}