/*
 * Computer Graphics 2019
 * Problem 2.1
 * Dushan Terzikj
 * d.terzikj@jacobs-university.de
 */

#include <GL/glut.h>
#include <GL/freeglut.h>
#include <vector>
#include <string>

using namespace std;

GLint screenWidth = 600;
GLint screenHeight = 600;
GLfloat skinColor[4];
GLfloat red[] = {1, 0, 0, 1};
GLfloat green[] = {0, 1, 0, 1};
GLfloat blue[] = {0, 0, 1, 1};
GLdouble headRadius = 0.8;
GLdouble armRadius = 0.25;
GLdouble legRadius = 0.3;

/**
 * @brief   Maps RGBA colors from 0 to 255 to 0 to 1.
 */
void mapColor(vector<GLfloat>& arr) {
    for(int i = 0; i < 4; i++) {
        arr[i] = static_cast<float>((1 / 127.5) * arr[i] - 1.0);
    }
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    /* head */
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_DIFFUSE, skinColor);
    glTranslatef(0, 2.5, 0);
    glutSolidSphere(headRadius, (GLint) (headRadius*20), (GLint) (headRadius*20));
    glPopMatrix();

    /* nose */
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
    glTranslatef(0, 2.5, 0);
    glutSolidCone(0.25, 2, 5, 5);
    glPopMatrix();

    /* torso */
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_DIFFUSE, skinColor);
    glRotatef(90, 1, 0, 0);
    glTranslatef(0, 0, -1.75f);
    glutSolidCylinder(headRadius*1.5, 3, static_cast<GLint>(headRadius * 20 * 1.5),
                      static_cast<GLint>(headRadius * 20 * 1.5));
    glPopMatrix();

    /* arms: left, right */
    glPushMatrix();
    glTranslatef(-1, 1, 0);
    glRotatef(-90, 0, 1, 0);
    glRotatef(45, 1, 0, 0);
    glutSolidCylinder(armRadius, armRadius*8, static_cast<GLint>(armRadius * 20), static_cast<GLint>(armRadius * 20));
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1, 1, 0);
    glRotatef(-90, 0, 1, 0);
    glRotatef(135, 1, 0, 0);
    glutSolidCylinder(armRadius, armRadius*8, static_cast<GLint>(armRadius * 20), static_cast<GLint>(armRadius * 20));
    glPopMatrix();

    /* legs: right, left */
    glPushMatrix();
    glTranslatef(0.5, -3, 0);
    glRotatef(-90, 1, 0, 0);
    glutSolidCylinder(legRadius, legRadius*10, static_cast<GLint>(legRadius * 20), static_cast<GLint>(legRadius * 20));
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.5f, -3, 0);
    glRotatef(-90, 1, 0, 0);
    glutSolidCylinder(legRadius, legRadius*10, static_cast<GLint>(legRadius * 20), static_cast<GLint>(legRadius * 20));
    glPopMatrix();

    /* ground */
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_SPECULAR, green);
    glBegin(GL_TRIANGLES);
    glVertex3f(0, -3, 3);
    glVertex3f(0, -3, -5);
    glVertex3f(-3, -3, 0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glRotatef(180, 0, 1, 0);
    glTranslatef(0, 0, 2);
    glBegin(GL_TRIANGLES);
    glVertex3f(0, -3, 3);
    glVertex3f(0, -3, -5);
    glVertex3f(-3, -3, -1.5f);
    glEnd();
    glPopMatrix();

    /* College name BONUS PART */

    glPushMatrix();
    glRasterPos3f(-0.5, 4, 0);
    string college = "College3";
    for(char ch : college) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ch);
    }
    glPopMatrix();

    // Swap and flush buffers to screen
    glutSwapBuffers();        
      
}

void my_setup() {

    /* Map global skin color tuple and put into GLfloat[] */
    vector<GLfloat> vSkinColor = {234, 192, 134, 1};
    mapColor(vSkinColor);
    for(int i = 0 ; i < 4; i++) {
        skinColor[i] = vSkinColor[i];
    }

    vector<GLfloat> bgColor = {8, 155, 251, 1};
    mapColor(bgColor);
    glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);

    GLfloat lightZeroPosition[] = {10, 4, 10, 1};
    GLfloat lightZeroColor[] = {0.8, 1, 0.8, 1};
    glLightfv(GL_LIGHT0, GL_POSITION, lightZeroPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.1);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05);
    glEnable(GL_LIGHT0);
}

int main (int argc, char **argv) {
    // Initial setup and window opening
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("Problem 1: Human body");

    my_setup();
    
    // Callbacks definitions
    glutDisplayFunc(display);
    
    glMatrixMode(GL_PROJECTION);
    gluPerspective(40, 1, 1, 40);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(0, 2, 13, 0, 0, 0, 0, 1, 0);
    glPushMatrix();

    // Here we go!
    glutMainLoop();
    
    return 0;
}
