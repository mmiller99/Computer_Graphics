#include <GL/glut.h>
#include "main.h"

#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>

#define R (M_PI/180)

void keyDown(unsigned char key, int x, int y) {
    k[key] = true;
    if (key == 'c') {
        cam.x = 0;
        cam.y = 0;
        cam.z = 40;

        cam_n.x = 0;
        cam_n.y = 0;
        cam_n.z = -1;

        cam_u.x = 0;
        cam_u.y = 1;
        cam_u.z = 0;

        cam_v.x = 1;
        cam_v.y = 0;
        cam_v.z = 0;
    }
}

void keyUp(unsigned char key, int x, int y) {
    k[key] = false;
}

void processMenu(int option) {
    switch (option) {
    case 1:
        if (rotate_right) {
            rotate_right = false;
        }
        else {
            rotate_right = true;
        }
        break;
    case 2:
        if (rotate_left) {
            rotate_left = false;
        }
        else {
            rotate_left = true;
        }
        break;
    case 3:
        rotate_left = false;
        rotate_right = false;
    }   
}

void forward_back(float d) {
    cam.x += d * cam_n.x;
    cam.y += d * cam_n.y;
    cam.z += d * cam_n.z;
}

void left_right(float d) {
    cam.x += d * cam_v.x;
    cam.y += d * cam_v.y;
    cam.z += d * cam_v.z;
}

void up_down(float d) {
    cam.x += d * cam_u.x;
    cam.y += d * cam_u.y;
    cam.z += d * cam_u.z;
}

void roll(float d) {
    float temp_u = cam_u.x;
    cam_u.x = cos(d*R) * temp_u - sin(d*R) * cam_u.y;
    cam_u.y = sin(d*R) * temp_u + cos(d*R) * cam_u.y;

    float temp_v = cam_v.x;
    cam_v.x = cos(d*R) * temp_v - sin(d*R) * cam_v.y;
    cam_v.y = sin(d*R) * temp_v + cos(d*R) * cam_v.y;
}

void yaw(float d) {
    float temp_n = cam_n.x;
    cam_n.x = cos(d*R) * temp_n - sin(d*R) * cam_n.z;
    cam_n.z = sin(d*R) * temp_n + cos(d*R) * cam_n.z;

    float temp_v = cam_v.x;
    cam_v.x = cos(d*R) * temp_v - sin(d*R) * cam_v.z;
    cam_v.z = sin(d*R) * temp_v + cos(d*R) * cam_v.z;
}

void pitch(float d) {

    float temp_n = cam_n.z;
    cam_n.z = cos(d*R) * temp_n - sin(d*R) * cam_n.y;
    cam_n.y = sin(d*R) * temp_n + cos(d*R) * cam_n.y;

    float temp_u = cam_u.z;
    cam_u.z = cos(d*R) * temp_u - sin(d*R) * cam_u.y;
    cam_u.y = sin(d*R) * temp_u + cos(d*R) * cam_u.y;
}

void controls(float d) {

    if (k['i']) forward_back(d);
    if (k['k']) forward_back(-d);
    if (k['u']) up_down(d);
    if (k['o']) up_down(-d);
    if (k['j']) left_right(-d);
    if (k['l']) left_right(d);
    if (k['e']) roll(-d);
    if (k['q']) roll(d);
    if (k['s']) pitch(-d);
    if (k['w']) pitch(d);
    if (k['a']) yaw(-d);
    if (k['d']) yaw(d);

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
    glColor3f(.5, 0, 1);
    glTranslatef(0, -5, 0);

    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    gluCylinder(quadric, 1, 1, 10, 10, 10);
    glPopMatrix();

    glRotatef(lever, 0, 1, 0);
    glutSolidSphere(1, 10, 10);
    glPushMatrix();

    glTranslatef(0, 0, -10);
    gluCylinder(quadric, 1, 1, 20, 10, 10);

    glRotatef(lever, 0, 0, 1);
    glutSolidSphere(1, 10, 10);

    glTranslatef(0, -5, 0);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(quadric, 1, 0, 5, 10, 10);
    glutSolidSphere(1.5, 10, 10);
    glPopMatrix();

    glTranslatef(0, 0, 10);
    glRotatef(-lever, 0, 0, 1);
    glutSolidSphere(1, 10, 10);

    glTranslatef(0, -5, 0);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(quadric, 1, 0, 5, 10, 10);
    glutSolidSphere(1.5, 10, 10);


    glColor3f(1, 1, 1);
}

void draw_viewports() {
    //V1
    glViewport(width / 2.0, 0, width / 2.0, height / 2.0);
    viewport(width / 2.0, height / 2.0);
    glPushMatrix();
    gluLookAt(cam.x, cam.y, cam.z, cam.x + cam_n.x, cam.y + cam_n.y, cam.z + cam_n.z, cam_u.x, cam_u.y, cam_u.z);
    draw_ground();
    draw_lever();
    glPopMatrix();
    //V2
    glViewport(0, 0, width / 2.0, height / 2.0);
    viewport(width / 2.0, height / 2.0);
    glPushMatrix();
    gluLookAt(0, 40, 0, 0, 0, 0, 1, 0, 0);
    draw_ground();
    draw_lever();
    glPopMatrix();
    //V3
    glViewport(width / 2.0, height / 2.0, width / 2.0, height / 2.0);
    viewport(width / 2.0, height / 2.0);
    glPushMatrix();
    gluLookAt(40, 0, 0, 0, 0, 0, 0, 1, 0);
    draw_ground();
    draw_lever();
    glPopMatrix();
    //V4
    glViewport(0, height / 2.0, width / 2.0, height / 2.0);
    viewport(width / 2.0, height / 2.0);
    glPushMatrix();
    gluLookAt(0, 0, -40, 0, 0, 0, 0, 1, 0);
    draw_ground();
    draw_lever();
    glPopMatrix();
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


void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    controls(1);

    if (rotate_right) {
        lever++;
    }
    else if (rotate_left) {
        lever--;
    }

    draw_viewports();
}

void timer(int) {
    glutSwapBuffers();
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 0);

    glutCreateWindow("Miles Miller CS460 Assign 3");
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);

    glutCreateMenu(processMenu);
    glutAddMenuEntry("Rotate Right", 1);
    glutAddMenuEntry("Rotate Left", 2);
    glutAddMenuEntry("Stop Rotation", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_LINE);
    timer(0);
    glutMainLoop();
}