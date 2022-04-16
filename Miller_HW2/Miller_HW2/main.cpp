#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdlib.h>

#define ANGLE 1
#define RADIANS (M_PI/180)

struct point {
    float x, y, z;
    point(float px, float py, float pz) {
        x = px; y = py; z = pz;
    }
};
GLUquadricObj* quadric;
int width, height;
point cam(0, 0, 40);
point n(0, 0, -1);
point u(0, 1, 0);
point v(1, 0, 0);
//camera rotations
float crotx = 0, croty = 0, crotz = 0;
//lever rotations
float rotlever = 0;

bool keys[255];

void key(unsigned char key, int x, int y) {
    //printf("ex: %f, ey: %f, ez: %f, cx: %f, cy: %f, cz: %f, ux: %f, uy: %f, uz: %f\n",
            //cam.x, cam.y, cam.z, cam.x+n.x, cam.y+n.y, cam.z+n.z, u.x, u.y, u.z);
    keys[key] = true;
}
void key_up(unsigned char key, int x, int y) {
    keys[key] = false;
}

void roll(float angle) {
    croty += angle;
    float ux_t = u.x;
    u.x = std::cos(angle * RADIANS) * ux_t - std::sin(angle * RADIANS) * u.y;
    u.y = std::sin(angle * RADIANS) * ux_t + std::cos(angle * RADIANS) * u.y;
    float vx_t = v.x;
    v.x = std::cos(angle * RADIANS) * vx_t - std::sin(angle * RADIANS) * v.y;
    v.y = std::sin(angle * RADIANS) * vx_t + std::cos(angle * RADIANS) * v.y;
}

void yaw(float angle) {
    crotz += angle;
    //rotate center
    float nx_t = n.x;
    n.x = std::cos(angle * RADIANS) * nx_t - std::sin(angle * RADIANS) * n.z;
    n.z = std::sin(angle * RADIANS) * nx_t + std::cos(angle * RADIANS) * n.z;
    //rotate side
    float vx_t = v.x;
    v.x = std::cos(angle * RADIANS) * vx_t - std::sin(angle * RADIANS) * v.z;
    v.z = std::sin(angle * RADIANS) * vx_t + std::cos(angle * RADIANS) * v.z;
}

void pitch(float angle) {
    crotx -= angle;
    //rotate center
    float nz_t = n.z;
    n.z = std::cos(angle * RADIANS) * nz_t - std::sin(angle * RADIANS) * n.y;
    n.y = std::sin(angle * RADIANS) * nz_t + std::cos(angle * RADIANS) * n.y;
    //rotate up
    float uz_t = u.z;
    u.z = std::cos(angle * RADIANS) * uz_t - std::sin(angle * RADIANS) * u.y;
    u.y = std::sin(angle * RADIANS) * uz_t + std::cos(angle * RADIANS) * u.y;
}

void slide_forward(float dist) {
    cam.x += dist * n.x;
    cam.y += dist * n.y;
    cam.z += dist * n.z;
}
void slide_right(float dist) {
    cam.x += dist * v.x;
    cam.y += dist * v.y;
    cam.z += dist * v.z;
}
void slide_up(float dist) {
    cam.x += dist * u.x;
    cam.y += dist * u.y;
    cam.z += dist * u.z;
}

void camera() {
    if (keys['q'])
        roll(ANGLE);
    if (keys['e'])
        roll(-ANGLE);
    if (keys['w'])
        pitch(ANGLE);
    if (keys['s'])
        pitch(-ANGLE);
    if (keys['a'])
        yaw(-ANGLE);
    if (keys['d'])
        yaw(ANGLE);
    if (keys['i'])
        slide_forward(ANGLE);
    if (keys['k'])
        slide_forward(-ANGLE);
    if (keys['o'])
        slide_up(ANGLE);
    if (keys['u'])
        slide_up(-ANGLE);
    if (keys['j'])
        slide_right(-ANGLE);
    if (keys['l'])
        slide_right(ANGLE);
    if (keys['r'])
        rotlever += 1;
}

void draw_ground() {
    glPushMatrix();
    glTranslatef(0, -10, 0);
    glBegin(GL_LINES);
    for (int x = -100; x <= 100; x++) {
        if (x % 5 == 0) {
            glVertex3f(x, 0, -100);
            glVertex3f(x, 0, 100);
        }
    }
    for (int z = -100; z <= 100; z++) {
        if (z % 5 == 0) {
            glVertex3f(-100, 0, z);
            glVertex3f(100, 0, z);
        }
    }
    glEnd();
    glPopMatrix();
}

void viewport(int width, int height) {
    float ratio = (float)width / height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, ratio, 1, 256);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void draw_lever() {
    glColor3f(1, 0, 0);
    glTranslatef(0, -5, 0);

    //main cylinder
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    gluCylinder(quadric, 1, 1, 10, 10, 10);
    glPopMatrix();

    //center sphere
    glRotatef(rotlever, 0, 1, 0);
    glutSolidSphere(1, 10, 10);
    glPushMatrix();

    //top center cylinder
    glTranslatef(0, 0, -10);
    gluCylinder(quadric, 1, 1, 20, 10, 10);

    //right side sphere
    glRotatef(rotlever, 0, 0, 1);
    glutSolidSphere(1, 10, 10);

    //rest of right side
    glTranslatef(0, -5, 0);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(quadric, 1, 0, 5, 10, 10);
    glutSolidSphere(1.5, 10, 10);
    glPopMatrix();

    //left side sphere
    glTranslatef(0, 0, 10);
    glRotatef(-rotlever, 0, 0, 1);
    glutSolidSphere(1, 10, 10);

    //the rest of left side
    glTranslatef(0, -5, 0);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(quadric, 1, 0, 5, 10, 10);
    glutSolidSphere(1.5, 10, 10);


    glColor3f(1, 1, 1);
}

void draw_viewports() {
    //Viewport 1
    glViewport(width / 2.0, 0, width / 2.0, height / 2.0);
    viewport(width / 2.0, height / 2.0);
    glPushMatrix();
    gluLookAt(cam.x, cam.y, cam.z, cam.x + n.x, cam.y + n.y, cam.z + n.z, u.x, u.y, u.z);
    draw_ground();
    draw_lever();
    glPopMatrix();
    //Viewport top
    glViewport(0, 0, width / 2.0, height / 2.0);
    viewport(width / 2.0, height / 2.0);
    glPushMatrix();
    gluLookAt(0, 40, 0, 0, 0, 0, 1, 0, 0);
    draw_ground();
    draw_lever();
    glPopMatrix();
    //Viewport side
    glViewport(width / 2.0, height / 2.0, width / 2.0, height / 2.0);
    viewport(width / 2.0, height / 2.0);
    glPushMatrix();
    gluLookAt(40, 0, 0, 0, 0, 0, 0, 1, 0);
    draw_ground();
    draw_lever();
    glPopMatrix();
    //Viewport front
    glViewport(0, height / 2.0, width / 2.0, height / 2.0);
    viewport(width / 2.0, height / 2.0);
    glPushMatrix();
    gluLookAt(0, 0, -40, 0, 0, 0, 0, 1, 0);
    draw_ground();
    draw_lever();
    glPopMatrix();

}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    //process camera controls
    camera();
    //draw all the viewports
    draw_viewports();
}

void reshape(int w, int h) {
    width = w;
    height = h;
    glViewport(0, 0, w, h);
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
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(key);
    glutKeyboardUpFunc(key_up);
    quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_LINE);
    timer(0);
    glutMainLoop();
    return EXIT_SUCCESS;
}