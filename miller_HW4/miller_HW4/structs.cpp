#include <stdlib.h>
#include <malloc.h>
#include <vector>
#include <cmath>
#include "structs.h"

#define PI 3.14159

BaryCentric::BaryCentric(float lam1, float lam2, float lam3) {
    l1 = lam1;
    l2 = lam2;
    l3 = lam3;
}

Point::Point(int xi, int yi) {
    x = xi;
    y = yi;
}

void Point::rotate(Point origin, float angle) {
    float s = std::sin(angle * PI / 180);
    float c = std::cos(angle * PI / 180);

    x -= origin.x;
    y -= origin.y;

    float xnew = x * c - y * s;
    float ynew = x * s + y * c;

    x = xnew + origin.x;
    y = ynew + origin.y;
}

Pixel::Pixel(unsigned char red, unsigned char green, unsigned char blue) {
    r = red;
    g = green;
    b = blue;
}
