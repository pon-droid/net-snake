#pragma once

#include<deque>
#include<SDL2/SDL.h>
#include "Defines.h"
#include<iostream>

struct Segment{
    int x, y;

    bool operator==(const Segment& s) const{
        return x == s.x && y == s.y;
    }
};

// Add snake "owner" variable to Segment
// Maybe a shared_ptr? since multiple segments "share" the same snake owner

struct Snake {
    Snake(int x, int y, SDL_Colour col);

	std::deque<Segment> body;
	int dx, dy;
    SDL_Colour colour;

    bool hit;
    bool end;

    void control();
    void update();

    
};

struct Body_hash{
    size_t operator()(const Segment& s) const
    {
        return s.x * s.y;
    }
};