#pragma once

#include <vector>
#include <string>
#include "structs.h"

struct Triangle {
    Point p1, p2, p3;
    float rot = 90;
    std::vector<std::pair<BaryCentric, Pixel>> texture;

    Triangle() = default;
    Triangle(Point pt1, Point pt2, Point pt3);
    bool point_in_tri(Point p);
    Point convert_bary(BaryCentric b);
    BaryCentric convert_cart(Point p);
    void draw(int x_start, int y_start);
};

struct Flower {
    int offsetx = 200, offsety = 200;
    int rot;
    int width;
    int height;
    Point click;
    Point curr;
    std::vector<std::vector<Pixel>> pixels;
    std::vector<Triangle> triangles;

    Flower();
    void read(char * filename);
    void make_texture();
    void draw();
    void clicked(Point p);
};