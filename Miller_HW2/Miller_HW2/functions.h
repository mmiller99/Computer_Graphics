#pragma once
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include "main.h"

void clipWindow() {
    glColor3f(1, 1, 1);
    glLineStipple(5, 0xAAAA);
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    glVertex2i(xi, yi); glVertex2i(xi, yj);
    glVertex2i(xi, yi); glVertex2i(xj, yi);
    glVertex2i(xj, yi); glVertex2i(xj, yj);
    glVertex2i(xi, yj); glVertex2i(xj, yj);
    glEnd();
    glDisable(GL_LINE_STIPPLE);
}

void viewport() {
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    glVertex2i(vx1, vy1); glVertex2i(vx1, vy2);
    glVertex2i(vx1, vy1); glVertex2i(vx2, vy1);
    glVertex2i(vx2, vy1); glVertex2i(vx2, vy2);
    glVertex2i(vx1, vy2); glVertex2i(vx2, vy2);
    glEnd();
}

void drawPixels() {
    glColor3f(1, 0, 0);
    glBegin(GL_POINTS);
    for (auto p : fill_points) {
        glVertex2i(p.x, p.y);
    }
    glEnd();
}

void drawLines() {
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    for (int i = 0; i < line_points.size(); i++) {
        if (i + 1 < line_points.size()) {
            glVertex2i(line_points[i].x, line_points[i].y);
            glVertex2i(line_points[i + 1].x, line_points[i + 1].y);
        }
        else if (!c) {
            glVertex2i(line_points[i].x, line_points[i].y);
            glVertex2i(line_points[0].x, line_points[0].y);
        }
    }
    glVertex2i(temp_line.x1, temp_line.y1);
    glVertex2i(temp_line.x2, temp_line.y2);
    glEnd();
}

void menu(int value) {
    switch (value) {
    case 1:
        line_points = clipPoly();
        break;
    case 2:
        fill = true; draw = false; movement = false;
        break;
    case 3:
        fill = false; draw = true; movement = false;
        break;
    case 4:
        fill = false; draw = false; movement = true;
        break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    //click top right to scale the viewport
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && drag)
        drag = false;
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN &&
        std::abs(x - vx2) < 30 && std::abs(y - vy1) < 30 && movement)
        drag = true;

    //click top left to move viewport
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && moving)
        moving = false;
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN &&
        std::abs(x - vx1) < 30 && std::abs(y - vy1) < 30 && movement)
        moving = true;

    //click top left to pan the clipping window
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && pan)
        pan = false;
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN &&
        std::abs(x - xi) < 30 && std::abs(y - yi) < 30 && movement)
        pan = true;

    //click bottom right to zoom in/out
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && zoom)
        zoom = false;
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN &&
        std::abs(x - xj) < 30 && std::abs(y - yj) < 30 && movement)
        zoom = true;
    
    //handles line drawing
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && draw) {
        if (line_points.size() > 0 && (abs(x - line_points[0].x) < 30 && abs(y - line_points[0].y) < 30)) {
            temp_line.x2 = line_points[0].x;
            temp_line.y2 = line_points[0].y;
            temp_line.x1 = -1;
            temp_line.x2 = -1;
            temp_line.x1 = -1;
            temp_line.x2 = -1;
            dx = -1;
            dy = -1;
            c = false;
        }
        else if (!c) {
            c = true;
            dx = x;
            dy = y;
            line_points.push_back(point(dx, dy));
        }
        else {
            line_points.push_back(point(x, y));
            dx = x;
            dy = y;
        }
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && fill) {
        int y_temp = glutGet(GLUT_WINDOW_HEIGHT) - y;
        unsigned char original[4];
        unsigned char pixel[4];
        glReadPixels(x, y_temp, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, original);
        glColor3f(1, 0, 0);
        floodFill(x, y, original, pixel);
    }
    glutPostRedisplay();
}

void move(int x, int y) {
    if (c) {
        temp_line.x1 = dx;
        temp_line.y1 = dy;
        temp_line.x2 = x;
        temp_line.y2 = y;

        if (line_points.size() > 0 && (abs(x - line_points[0].x)
            < 30 && abs(y - line_points[0].y) < 30)) {
            temp_line.x2 = line_points[0].x;
            temp_line.y2 = line_points[0].y;
        }

        glutPostRedisplay();
    }
    if (drag) {
        vx2 = x;
        vy1 = y;
        glutPostRedisplay();
    }
    else if (moving) {
        int xdiff = vx2 - vx1;
        int ydiff = vy2 - vy1;
        vx1 = x;
        vy1 = y;
        vx2 = vx1 + xdiff;
        vy2 = vy1 + ydiff;
        glutPostRedisplay();
    }
    else if (pan) {
        int xdiff = xj - xi;
        int ydiff = yj - yi;
        xi = x;
        yi = y;
        xj = xi + xdiff;
        yj = yi + ydiff;
        //update clip locations in case user clips again
        clipLines();
        glutPostRedisplay();
    }
    else if (zoom) {
        xj = x;
        yj = y;
        //update clipp locations in case user clips again
        clipLines();
        glutPostRedisplay();
    }
}
