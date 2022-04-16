#pragma once
#include <GL/glut.h>
#include <iostream>
#include <vector>

#define _USE_MATH_DEFINES

struct point {
	float x, y, z;
	point(float new_x, float new_y, float new_z) {
		x = new_x;
		y = new_y;
		z = new_z;
	}
};

bool k[200];
bool rotate_right = false;
bool rotate_left = false;

void reshape(int, int);

int width;
int height;

point cam(0, 0, 40);
point cam_u(0, 1, 0);
point cam_v(1, 0, 0);
point cam_n(0, 0, -1);

float lever = 0;


GLUquadricObj* quadric;