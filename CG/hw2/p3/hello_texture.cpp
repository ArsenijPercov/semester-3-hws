/**
 * Computer Graphics 2019
 * Problem 2.3
 * Dushan Terzikj
 * d.terzikj@jacobs-university.de
 */

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

#define checkImageWidth 120
#define checkImageHeight 120
#define screenWidth     600
#define screenHeight    600
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

static GLuint texName;

void makeCheckImage()
{
    int i, j, c;
    
    for (i = 0; i < checkImageHeight; i++) {
        for (j = 0; j < checkImageWidth; j++) {
            c = ((((i & 0x8) == 0) ^ (((j | 0x8)) == 0)))*255;
            checkImage[i][j][0] = (GLubyte) c;
            checkImage[i][j][3] = (GLubyte) 255;
        }
    }
}

void init()
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    
    makeCheckImage();
    
    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);
    glMatrixMode(GL_TEXTURE);
    glRotatef(40, 0, 0, 1);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
                 checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 checkImage);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_TEXTURE_2D);

    /* GL_REPEAT is already set here from init(), see above */
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0, 0);
    glVertex3f(-3, 0, 0);
    glTexCoord2f(1, 0);
    glVertex3f(-1, 0, 0);
    glTexCoord2f(0, 1);
    glVertex3f(-2, 2.0, 0);
    glEnd();

    glBegin(GL_TRIANGLES);
    glTexCoord2f(0, 0);
    glVertex3f(0, 0, 0);
    glTexCoord2f(1, 0);
    glVertex3f(2, 0, 0);
    glTexCoord2f(0, 1);
    glVertex3f(2, 2, 0);

    glTexCoord2f(0, 0);
    glVertex3f(0, 0, 0);
    glTexCoord2f(1, 0);
    glVertex3f(0, 2, 0);
    glTexCoord2f(0, 1);
    glVertex3f(2, 2, 0);
    glEnd();
    
    /* Set GL_MIRRORED_REPEAT */
    glMatrixMode(GL_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glBegin(GL_TRIANGLES);
    glTexCoord2f(0, 0); 
    glVertex3f(-3, -3, 0);
    glTexCoord2f(1, 0); 
    glVertex3f(-1, -3, 0);
    glTexCoord2f(0, 1); 
    glVertex3f(-1, -1, 0);

    glTexCoord2f(0, 0); 
    glVertex3f(-3, -3, 0);
    glTexCoord2f(1, 0); 
    glVertex3f(-3, -1, 0);
    glTexCoord2f(0, 1); 
    glVertex3f(-1, -1, 0);
    glEnd();

    /* Set GL_CLAMP_TO_EDGE */
    glMatrixMode(GL_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0, 0); 
    glVertex3f(0, -3, 0);
    glTexCoord2f(1, 0);
    glVertex3f(2, -3, 0);
    glTexCoord2f(0, 1); 
    glVertex3f(2, -1, 0);

    glTexCoord2f(0, 0);
    glVertex3f(0, -3, 0);
    glTexCoord2f(1, 0);
    glVertex3f(0, -1, 0);
    glTexCoord2f(0, 1); 
    glVertex3f(2, -1, 0);
    glEnd();

    glFlush();
    
    glDisable(GL_TEXTURE_2D);
}


void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 30.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -8.0f);
}

void keyboard (unsigned char key, int x, int y)
{
    switch (key) {
        case 27:
            exit(0);
            break;
        default:
            break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(screenWidth, screenHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0; 
}
