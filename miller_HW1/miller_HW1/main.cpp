#include <glut.h>
#include <iostream>
#include <vector>

using namespace std;

struct line {
    int x1, y1, x2, y2;
};

line temp_line;
std::vector<line> lines;


int dyn_line = 0;
int prevy = -1;
int prevx = -1;
int stipples = 0;


void bresLine(int, int, int, int);
void midLine(int, int, int, int);

void bresWake() {
    //draw wake
    //start W
    bresLine(50, 76, 70, 186);
    bresLine(70, 186, 100, 102);
    bresLine(100, 102, 120, 183);
    bresLine(120, 183, 150, 76);
    //start A
    bresLine(200, 177, 250, 77);
    bresLine(250, 77, 300, 171);
    bresLine(230, 123, 270, 123);
    //start K
    bresLine(330, 66, 330, 167);
    bresLine(330, 114, 370, 68);
    bresLine(330, 114, 370, 167);
    //start E
    bresLine(410, 68, 410, 161);
    bresLine(410, 161, 460, 161);
    bresLine(410, 117, 450, 117);
    bresLine(410, 69, 460, 69);
    //start xyz
    bresLine(215, 353, 215, 227);
    bresLine(215, 353, 342, 353);
    bresLine(215, 353, 115, 423);
}

void openWake() {
    //set stripple variables
    glLineStipple(stipples, 0xAAAA);
    glEnable(GL_LINE_STIPPLE);
    if (stipples == 0) {
        glDisable(GL_LINE_STIPPLE);
    }
    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    //draw wake
    //start W
    glVertex2i(50 + 500, 76); glVertex2i(70 + 500, 186);
    glVertex2i(70 + 500, 186); glVertex2i(100 + 500, 102);
    glVertex2i(100 + 500, 102); glVertex2i(120 + 500, 183);
    glVertex2i(120 + 500, 183); glVertex2i(150 + 500, 76);
    //start A
    glVertex2i(200 + 500, 177); glVertex2i(250 + 500, 77);
    glVertex2i(250 + 500, 77); glVertex2i(300 + 500, 171);
    glVertex2i(230 + 500, 123); glVertex2i(270 + 500, 123);
    //start K
    glVertex2i(330 + 500, 66); glVertex2i(330 + 500, 167);
    glVertex2i(330 + 500, 114); glVertex2i(370 + 500, 68);
    glVertex2i(330 + 500, 114); glVertex2i(370 + 500, 167);
    //start E
    glVertex2i(410 + 500, 68); glVertex2i(410 + 500, 161);
    glVertex2i(410 + 500, 161); glVertex2i(460 + 500, 161);
    glVertex2i(410 + 500, 117); glVertex2i(450 + 500, 117);
    glVertex2i(410 + 500, 69); glVertex2i(460 + 500, 69);
    //start xyz
    glVertex2i(215 + 500, 353); glVertex2i(215 + 500, 227);
    glVertex2i(215 + 500, 353); glVertex2i(342 + 500, 353);
    glVertex2i(215 + 500, 353); glVertex2i(115 + 500, 423);
    glEnd();
}

void display() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    openWake();
    glBegin(GL_POINTS);
    bresWake();

    //draw buffer lines
    for (line l : lines) {
            midLine(l.x1, l.y1, l.x2, l.y2);
    }
    midLine(temp_line.x1, temp_line.y1, temp_line.x2, temp_line.y2);
    glEnd();

    //mirror bresenham and opengl lines
    glColor3f(1, 1, 1);
    //set stripples
    glLineStipple(stipples, 0xAAAA);
    glEnable(GL_LINE_STIPPLE);
    if (stipples == 0) {
        glDisable(GL_LINE_STIPPLE);
    }
    //draw opengl version
    glBegin(GL_LINES);
    for (line l : lines) {
        glVertex2i(l.x1 + 500, l.y1);
        glVertex2i(l.x2 + 500, l.y2);
    }
    glVertex2i(temp_line.x1 + 500, temp_line.y1);
    glVertex2i(temp_line.x2 + 500, temp_line.y2);
    glEnd();
    glFlush();

}

void key(unsigned char key, int x, int y) {
    if (key == '1')
        stipples = 3;
    else if (key == '2')
        stipples = 10;
    else if (key == '3')
        stipples = 15;
    else if (key == '4')
        stipples = 0;
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (!dyn_line) {
            //first click
            //set the first point and begin drawing the dynamic line to the mouse
            dyn_line = 1;
            prevx = x;
            prevy = y;
        }
        else {
            lines.push_back(temp_line);
            prevx = x;
            prevy = y;
        }
        cout << x << " " << y << '\n';
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        lines.push_back(temp_line);
        dyn_line = 0;
        prevx = -1;
        prevy = -1;
    }
}

void move(int x, int y) {
    if (dyn_line) {
        temp_line.x1 = prevx;
        temp_line.y1 = prevy;
        temp_line.x2 = x;
        temp_line.y2 = y;
        glutPostRedisplay();
    }
}

void midLine(int x1, int y1, int x2, int y2) {
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    y1 = height - y1;
    y2 = height - y2;
    int x = x1;
    int y = y1;
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int d = dy - dx / 2;
    glVertex2i(x1, height - y1);

    //horizontal line
    if (dy == 0) {
        if (x2 > x1) {
            for (x = x1; x < x2; x++) {
                if (stipples == 0 || x / stipples % 3 == 0)
                    glVertex2i(x, height - y);
            }
        }
        else {
            for (x = x2; x < x1; x++) {
                if (stipples == 0 || x / stipples % 3 == 0)
                    glVertex2i(x, height - y);
            }
        }
    }
    //verticle line
    else if (dx == 0) {
        if (y2 > y1) {
            for (y = y1; y < y2; y++) {
                if (stipples == 0 || y / stipples % 3 == 0)
                    glVertex2i(x, height - y);
            }
        }
        else {
            for (y = y2; y < y1; y++) {
                if (stipples == 0 || y / stipples % 3 == 0)
                    glVertex2i(x, height - y);
            }
        }
    }

    //slope < 1
    else if ((float)dy / (float)dx < 1.0) {
        while (abs(x - x1) < abs(x2 - x1)) {
            x++;
            if (d < 0)
                d += dy;
            else {
                d += (dy - dx);
                y++;
            }
            //quarter 1
            if (y1 < y2 && x2 > x1) {
                if (stipples == 0 || x / stipples % 3 == 0)
                    glVertex2i(x, height - y);
            }
            //quarter 2
            else if (y1 > y2 && x2 < x1) {
                if (stipples == 0 || x / stipples % 3 == 0)
                    glVertex2i(2 * x1 - x, height + y - 2 * y1);
            } 
            //quarter 3
            else if (y1 < y2 && x2 < x1) {
                if (stipples == 0 || x / stipples % 3 == 0)
                    glVertex2i(2 * x1 - x, height - y);
            }
            //quarter 4
            else if (y1 > y2 && x2 > x1) {
                if (stipples == 0 || x / stipples % 3 == 0)
                    glVertex2i(x, height + y - 2 * y1);
            }
        }
    }
    //slope > 1
    else {
        while (abs(y - y1) < abs(y2 - y1)) {
            y++;
            if (d < 0)
                d += dx;
            else {
                d += (dx - dy);
                x++;
            }
            //quarter 1
            if (y1 < y2 && x2 > x1) {
                if (stipples == 0 || y / stipples % 3 == 0)
                    glVertex2i(x, height - y);
            }
            //quarter 2
            else if (y1 > y2 && x2 < x1) {
                if (stipples == 0 || y / stipples % 3 == 0)
                    glVertex2i(2 * x1 - x, height + y - 2 * y1);
            }
            //quarter 3
            else if (y1 < y2 && x2 < x1) {
                if (stipples == 0 || y / stipples % 3 == 0)
                    glVertex2i(2 * x1 - x, height - y);
            }
            //quarter 4
            else if (y1 > y2 && x2 > x1) {
                if (stipples == 0 || y / stipples % 3 == 0)
                    glVertex2i(x, height + y - 2 * y1);
            }
        }
    }
}



void bresLine(int x1, int y1, int x2, int y2) {
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    float dx = x2 - x1;
    float dy = y2 - y1;
    float d1 = 0.0;
    float d2 = 0.0;
    float e = 0.0;
    
    if (dx != 0) {
        d1 = abs(dy / dx);
    }
    if (dy != 0) {
        d2 = abs(dx / dy);
    }

    int x = x1;
    int y = y1;

    if (d1 < 1.0 && dx != 0) {
        if (dx > 0) {
            for (x = x1; x < x2; x++) {
                if (stipples == 0 || y / stipples % 3 == 0) glVertex2i(x, y);
                e += d1;
                while (e >= .5) {
                    y += dy / abs(dy);
                    e -= 1.0;
                }
            }
        }
        else {
            for (x = x1; x > x2; x--) {
                if (stipples == 0 || y / stipples % 3 == 0) glVertex2i(x, y);
                e += d1;
                while (e >= .5) {
                    y += dy / abs(dy);
                    e -= 1.0;
                }
            }
        }
    }
    else {
        if (dy > 0) {
            for (y = y1; y < y2; y++) {
                if (stipples == 0 || y / stipples % 3 == 0) glVertex2i(x, y);
                e += d2;
                while (e >= .5) {
                    x += dx / abs(dx);
                    e -= 1.0;
                }
            }
        }
        else {
            for (y = y1; y > y2; y--) {
                if (stipples == 0 || y / stipples % 3 == 0) glVertex2i(x, y);
                e += d2;
                while (e >= .5) {
                    x += dx / abs(dx);
                    e -= 1.0;
                }
            }
        }
    }
}


int main(int argc, char* argv[]) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowPosition(80, 80);
    glutInitWindowSize(1000, 600);
    glutCreateWindow("Miles Miller CS460 Assign 1");
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glutMouseFunc(mouse);
    glutKeyboardFunc(key);
    glutPassiveMotionFunc(move);
    glLoadIdentity();
    gluOrtho2D(0.0, 1000.0, 600.0, 0.0);
    glFlush();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}