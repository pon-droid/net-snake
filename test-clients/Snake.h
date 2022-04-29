#pragma once


#include "Defines.h"


struct Segment{
    int x, y;


    bool operator==(const Segment& s) const{
        return x == s.x && y == s.y;
    }
};

// Add snake "owner" variable to Segment
// Maybe a shared_ptr? since multiple segments "share" the same snake owner

struct Snake {
    Snake(int x, int y, SDL_Colour col, std::string username);
    Snake();

	std::deque<Segment> body;
	int dx, dy;
    SDL_Colour colour;
    std::string name;

    bool hit;
    bool end;

    void control();
    void check_col(const std::vector<Snake>& snakes, int element);
    void update();

    int index;
    

    
};

struct Body_hash{
    size_t operator()(const Segment& s) const
    {
        return s.x * s.y;
    }
};