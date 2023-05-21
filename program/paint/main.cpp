#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include"../glad/glad.h"
using namespace std;
void myDisplay(void)
{

    //glEnable(GL_DEPTH_TEST);
    glClearColor(0.70, 0.70, 0.70, 1.0); // Dark grey background
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(208 / 255.0, 159 / 255.0, 159 / 255.0,0.5);
    glBegin(GL_QUADS);
    glVertex2f(0.95,-0.3);
    glVertex2f(0.5,-0.3);
    glVertex2f(0.5,0.95);
    glVertex2f(0.95,0.95);
    glEnd();
    glDisable(GL_BLEND);

    glColor3f(0,0,0);
    

    vector<string> vecs = {"--object info--","ID: cube","pos: (1,2,3)","weight: 1 kg",
    "velocity: (0.1, 0.1, 0.1)m/s","angular velocity:  (0.1, 0.1, 0.1)m/s","drag force (0) press Z to switch",
    "gravity (0) press X to switch"};
    vector<string> vecs2 = {"--bullet info--","pos: (1,1,1)","dir: (1,1,1)","force: 100N press +/- to change"}; 
    string s;
    for(int i=0;i<vecs.size();i++){
        s = vecs[i];
        glRasterPos2f(0.52,0.88 - i*0.1); 
        for (int j = 0; j < s.size(); j++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, int(s[j]));
        }
    }
    for(int i=0;i<vecs2.size();i++){
        s = vecs2[i];
        glRasterPos2f(0.52,0.04 - i*0.1); 
        for (int j = 0; j < s.size(); j++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, int(s[j]));
        }
    }
    glutSwapBuffers();
    glutPostRedisplay();
    usleep(1000); //micro sleep
}

void win(int x,int y){
    cout << x << " " << y<<"\n";
}
int main(int argc, char **argv)
{
       glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    //glutInitWindowPosition(100, 100);

    glutInitWindowSize(1920, 990);
    glutCreateWindow("Shader Sample Program"); //Window title bar
    glutDisplayFunc(myDisplay); //Display callback function

    //Initiate glad by calling gladLoadGL(void)
    //We have to initialize OpenGL before starting glad. Error checking
    if (!gladLoadGL()) {
        printf("Something went wrong!\n");
        exit(-1);
    }

    //print out OpenGL and GLSL versions
    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
        glGetString(GL_SHADING_LANGUAGE_VERSION));
    //Setup the shaders and create my program object
    //Program objects and shaders must be created after glad has been initiated!!!
    glewInit();
    // The main loop.
    
    glutReshapeFunc(win);

    glutMainLoop();
    return 0;
}