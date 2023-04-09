#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <set>
#include <functional>
#include <fstream>
#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <ctime>
#include <cmath>
#include"../glad/glad.h"
int day = 200; // 0~360
void display(void)
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75, 1, 1, 400);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, -200, 200, 0, 0, 0, 0, 0, 1);
    glColor3f(1.0f, 0.0f, 0.0f);
    glutWireSphere(35, 30, 30);
    glColor3f(0.0f, 0.0f, 1.0f);
    glRotatef(day / 360.0 * 360.0, 0.0f, 0.0f, -1.0f);
    glTranslatef(120, 0.0f, 0.0f);
    glutWireSphere(15, 25, 25);
    glColor3f(1.0f, 1.0f, 0.0f);
    glRotatef(day / 30.0 * 360.0 - day / 360.0 * 360.0, 0.0f, 0.0f, -1.0f);
    glTranslatef(32, 0.0f, 0.0f);
    glutWireSphere(10, 10, 10);
    glFlush();
    glutSwapBuffers();
    usleep(12000);
}
void idle(void)
{
    ++day;
    if( day >= 360 ) {
    day = 0;
    }
    display();
    //usleep(1200);
}
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB  | GLUT_DEPTH| GLUT_DOUBLE) ;
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(640, 500);
    glutCreateWindow("12");
    glutDisplayFunc(display);
    glutIdleFunc(idle);

        if (!gladLoadGL()) {
        printf("Something went wrong!\n");
        exit(-1);
    }
    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
        glGetString(GL_SHADING_LANGUAGE_VERSION));
    //Setup the shaders and create my program object
    //Program objects and shaders must be created after glad has been initiated!!!
    glewInit();

    glutMainLoop();
    return 0;
}