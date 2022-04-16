#include <GL/glut.h>

#include <cmath>
#include <iostream>
#include <vector>
#include <cmath>
#include "objects.h"
#include "structs.h"
#include "flower.h"

#define TEAPOT 1
#define FLOWER 2

int WIDTH = 0, HEIGHT = 0;

//TEAPOT DATA
Object teapot;
float rotatex = 0, rotatey = 0, zoom = 1;

//CONTROL DATA
bool clicked = false;
int x_0 = 0, y_0 = 0;
int mx = 0, my = 0;
int mode = 2;

//FLOWER DATA
Flower flower;

bool keys[255];

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !clicked && mode == TEAPOT) {
        clicked = true;
        x_0 = x;
        y_0 = y;
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !clicked && mode == FLOWER) {
        clicked = true;
        flower.clicked(Point(x, y));
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && clicked)
        clicked = false;
}

void move(int x, int y) {
    if (clicked && mode == TEAPOT) {
        rotatex = (x - x_0);
        rotatey = (y - y_0);
    }
    else if (clicked && mode == FLOWER) {
        flower.clicked(Point(x, y));
    }
}

void key(unsigned char key, int x, int y) {
    if (key == 'f')
        mode = FLOWER;
    if (key == 't')
        mode = TEAPOT;
    if (key == 'r' && mode == FLOWER) {
        flower.rot += 30;
        std::cout << flower.rot << std::endl;
        for (Triangle& t : flower.triangles) {
            t.rot += 30;
        }
    }
    keys[key] = true;
}
void key_up(unsigned char key, int x, int y) {
    keys[key] = false;
}

void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (mode == TEAPOT) {
        glColor3f(1, 1, 1);
        if (keys['w'])
            zoom += .1;
        if (keys['s'])
            zoom -= .1;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60, 1, 1, 500000);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);

        glRotatef(rotatex, 0, 1, 0);
        glRotatef(rotatey, 1, 0, 0);
        glScalef(zoom, zoom, zoom);
        teapot.draw();
    }
    else if (mode == FLOWER) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0.0, WIDTH, HEIGHT, 0.0);

        flower.draw();
        glColor3f(1, 0, 0);
    }
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    WIDTH = w;
    HEIGHT = h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int) {
    glutSwapBuffers();
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 25);
    glutCreateWindow("Miller CS460 Program 4");
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(key);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(move);
    glutKeyboardUpFunc(key_up);
    teapot.read("./objects/teapot.OBJ");
    timer(0);
    glutMainLoop();
    return EXIT_SUCCESS;
}
