#pragma once

#include <GL/glut.h>
#include <iostream>
#include <vector>

const int width = 1000;
const int height = 600;

struct point {
	int x, y;
	point() = default;
	point(int new_x, int new_y) {
		x = new_x;
		y = new_y;
	}
};

struct line {
	int x1, y1, x2, y2;
};

bool draw = true;
bool movement = false;
bool fill = false;

bool c = true;

int dx = -1;
int dy = -1;

line temp_line;

std::vector<point> line_points;
std::vector<point> fill_points;

line l_left, l_right, l_up, l_down;
int xi = 1, yi = 1, xj = 400, yj = 400;

bool drag = false;
bool moving = false;
bool pan = false;
bool zoom = false;
int vx1 = width - 100, vy1 = height - 100, vx2 = width, vy2 = height;

std::vector<point> clipPoly();
void floodFill(int x, int y, unsigned char* color, unsigned char* pixel);
void clipLines();
