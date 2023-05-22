#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include"../glad/glad.h"
#include "UI.h"
using namespace std;
UI* ui = new UI((1920/990.0));
Object* obj = new Object();
vector<float> pos={0,0,0};
void myDisplay(void)
{

    //glEnable(GL_DEPTH_TEST);
    glClearColor(0.70, 0.70, 0.70, 1.0); // Dark grey background
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    int windowWidth = 1920;
    int windowHeight = 990;
    ui -> draw(*obj,pos,pos,100);
    glutSwapBuffers();
    glutPostRedisplay();
    usleep(1000); //micro sleep
}

void win(int x,int y){
    cout << x << " " << y<<"\n";
}
void key(unsigned char a,int b,int c){
    if(a == 'p') pos[0]++;
    if(a == 'k') pos[1]++;
    if(a == 'l') pos[2]++;
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
    glutKeyboardFunc(key);
    glutMainLoop();
    return 0;
}