#include <GL/glut.h>
#include <stdlib.h>
#include <malloc.h>
#include <vector>
#include <cmath>
#include <iostream>
#include "flower.h"
#include "structs.h"

#pragma warning(disable : 4996)

Triangle::Triangle(Point pt1, Point pt2, Point pt3) {
    p1 = pt1;
    p2 = pt2;
    p3 = pt3;
}

bool Triangle::point_in_tri(Point p) {
    BaryCentric pb = convert_cart(p);
    bool l1_valid = (pb.l1 >= 0) && (pb.l1 <= 1);
    bool l2_valid = (pb.l2 >= 0) && (pb.l2 <= 1);
    bool l3_valid = (pb.l3 >= 0) && (pb.l3 <= 1);
    return l1_valid && l2_valid && l3_valid;
}

Point Triangle::convert_bary(BaryCentric b) {
    float x = b.l1 * p1.x + b.l2 * p2.x + b.l3 * p3.x;
    float y = b.l1 * p1.y + b.l2 * p2.y + b.l3 * p3.y;

    return Point(x, y);
}

BaryCentric Triangle::convert_cart(Point p) {
    float lam1_num =
        (p2.y - p3.y) * (p.x - p3.x) + (p3.x - p2.x) * (p.y - p3.y);
    float lam1_denom =
        (p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y);
    float lam2_num =
        (p3.y - p1.y) * (p.x - p3.x) + (p1.x - p3.x) * (p.y - p3.y);
    float lam2_denom =
        (p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y);

    float lam1 = lam1_num / lam1_denom;
    float lam2 = lam2_num / lam2_denom;
    float lam3 = 1 - lam1 - lam2;

    return BaryCentric(lam1, lam2, lam3);
}

void Triangle::draw(int x_start, int y_start) {

    glBegin(GL_POINTS);
    for (auto texel : texture) {
        Point p = convert_bary(texel.first);
        p.rotate(p3, rot);
        Pixel color = texel.second;
        glColor3ub(color.r, color.g, color.b);
        glVertex2i(p.x + x_start, p.y + y_start);
        glVertex2i(p.x + 1 + x_start, p.y + 1 + y_start);
    }
    glEnd();
}

Flower::Flower() { 
    read((char *)"./objects/flower.bmp");
    int xf = height;
    int yf = width;
    int xi = 0;
    int yi = 0;
    //center point
    Point p((xi + xf) / 2, (yi + yf) / 2);
    //generate 4 triangles
    Point tl = Point(xi, yi);
    Point tr = Point(xf, yi);
    Point bl = Point(xi, yf);
    Point br = Point(xf, yf);
    Triangle top = Triangle(tl, tr, p);
    Triangle right = Triangle(tr, br, p);
    Triangle bottom = Triangle(bl, br, p);
    Triangle left = Triangle(bl, tl, p);

    //fill list of triangles
    triangles = { top, right, bottom, left };

    make_texture();
}

void Flower::read(char * filename) {
    const int HEADER_SIZE = 54;
    int WIDTH_LOCATION = 18;
    int HEIGHT_LOCATION = 22;
    unsigned char header[54];
    FILE* fd = fopen(filename, "rb");
    if (!fd) {
        perror("Unable to open flower.bmp");
        return;
    }
    fread(header, sizeof(unsigned char), HEADER_SIZE, fd);
    this->width = *(int*)&header[WIDTH_LOCATION];
    this->height = *(int*)&header[HEIGHT_LOCATION];
    int size = 3 * width * height;

    unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char) * size);
    fread(data, sizeof(unsigned char), size, fd);// read the data
    fclose(fd);


    //convert pixels from BGR to RGB.
    for (int i = 0; i < size; i += 3) {
        unsigned char temp = data[i];
        data[i] = data[i + 2];
        data[i + 2] = temp;
    }

    //convert to 2d vector
    std::vector<std::vector<Pixel>> pixels;
    for (int h = 0; h < height; h++) {
        pixels.push_back(std::vector<Pixel>());
    }

    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            pixels[h].push_back(Pixel(data[width * h * 3 + w * 3],
                data[width * h * 3 + w * 3 + 1], data[width * h * 3 + w * 3 + 2]));
        }
    }
    free(data);
    this->pixels = pixels;
}


void Flower::clicked(Point p) {
    click = Point(p.x - offsetx, p.y - offsety);
    //update the 4 triangles
    for (auto& t : triangles) {
        t.p3 = click;
    }
}

void Flower::make_texture() {
    //generate texture for each triangle
    for (int row = 0; row < pixels.size(); row++) {
        for (int col = 0; col < pixels[0].size(); col++) {
            for (Triangle& t : triangles) {
                Point curr = Point(row, col);
                if (t.point_in_tri(curr)) {
                    //std::cout << "TEST\n";
                    t.texture.push_back(std::make_pair(t.convert_cart(curr), pixels[row][col]));
                    break;
                }
            }
        }
    }
}

void Flower::draw() {
    //printf("draw flower\n");
    for (auto t : triangles) {
        t.draw(offsetx, offsety);
    }
}

