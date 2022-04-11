#pragma once

#include<deque>
#include<SDL2/SDL.h>
#include "Defines.h"
#include<iostream>

struct Segment{
    int x, y;
};

struct Snake {
    Snake(int x, int y);

	std::deque<Segment> body;
	int dx, dy;

    void control();
    void update();
};