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



struct Snake {
    Snake(int x, int y);

	std::deque<Segment> body;
	int dx, dy;

    bool hit;
    bool end;

    void control();
    void update();

    
};

struct Body_hash{
    size_t operator()(const Segment& s) const
    {
        return s.x / s.y;
    }
};