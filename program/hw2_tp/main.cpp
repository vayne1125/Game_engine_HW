#define _CRT_SECURE_NO_WARNINGS
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
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <ctime>
#include <cmath>
#include"../glad/glad.h"
#include "mesh.h"
#include "myobj.h"
#include "mytex.h"
//#include "mymath.h"
#define ll long long
#define   PI   3.1415927
//location
#define DIR_LIGHT_POS    4
#define DIR_LIGHT_DIR    5
#define DIR_LIGHT_COLOR  6
#define DIR_LIGHT_EXPO   7
#define DIR_LIGHT_STR    8
#define CAMERA_POS       10
#define AMBIENT          11
#define SPOT_LIGHT_POS 13
#define SPOT_LIGHT_DIR 14
#define SPOT_LIGHT_COLOR 15
#define SPOT_LIGHT_EXPO 16
#define SPOT_LIGHT_STR 17
//object
using namespace std;
using namespace glm;
GLuint programID;
myobj* myObj;
mytex* myTex;
mesh* billboard;
float   eyeAngx = 0.0, eyeAngy = 90.0, eyeAngz = 0.0;
float   u[3][3] = { {1.0,0.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0} };
float   eye[3] = { 0 };
float   eyeDis = 0;
double zNear = 0.1, zFar = 2000, aspect = 800 / (double)800, fovy;
//float   eye[3] = { 30.0, 10.0, 80.0 };
float   cv = cos(5.0 * PI / 180.0), sv = sin(5.0 * PI / 180.0); /* cos(5.0) and sin(5.0) */
//float   pos[3] = { 110,0,65 };
float   pos[3] = { 116,0,165 };
int     preKey = -1;
float   eyeMtx[16] = {0};
float   elfPos[3] = { 0 };
bool elfPosFlag = 0;
int eevee_ani = 0;
int dirLightStr = 2.3;
bool isDirLightOpen = 1;
int floorX = 232, floorY = 203;
float fountainPos[3] = { 116,0,124 };
vector<vector<float>> cloud = { {100,40,96},{180,40,150},{55,40,170} };
bool flag = 0;
vec3 w{0,0,0};
vec3 v{0,0,0};
quat rot{1,0,0,0}; //init
vec3 possphere{0,0,0};
float dt = 0;
int pretime = 0;
void applyLinearForce(const vec3& F){
        float m = 100;
        vec3 dv = F/m;
        v += dv*dt;
}
void applyRotJ(const vec3& J){
    mat3 sphereIInverse(1/1600.0);
    vec3 dw = sphereIInverse*J;
        w += dw*dt;
}
void applyRotForce(const vec3& F){
        mat3 sphereIInverse(1/1600.0);
        vec3 cm{0,0,0};                           //質心
        vec3 impactPoint{10,0,0};
        vec3 r = impactPoint - cm;
        vec3 J = cross(r,F);
        vec3 dw = sphereIInverse*J;
        w += dw*dt;
}
void keybaord_fun(unsigned char key, int X, int Y) {
    if(key == 'p' || key == 'P'){
        flag = 1;
        
        //if(!flag) return; 
        //2/5 * m * r^2 = 2/5*100*400 = 1600
        // mat3 sphereIInverse(1/1600.0);
        
        // vec3 cm{0,0,0};                           //質心
        // vec3 impactPoint{10,0,0};
        // vec3 r = impactPoint - cm;
        // vec3 J = cross(r,F);   

        // vec3 dw = sphereIInverse*J;
        // w += dw*dt;

        // float m = 100;
        // vec3 dv = F/m;
        // v += dv*dt;

        //mat3 sphereIInverse(1/1600.0);
        vec3 F{0,3000,0};
        vec3 cm{0,0,0};                           //質心
        vec3 impactPoint{10,0,0};
        vec3 r = impactPoint - cm;
        vec3 J = cross(r,F);

        applyLinearForce(F);
        //applyRotForce(vec3{0,5,0});
        applyRotJ(J);
    }
}
void myInit() {
    pretime = clock();
    myTex = new mytex(programID);
    myObj = new myobj(programID);
    
    eyeDis = 30;
    fovy = 100;
    eye[0] = 0;
    eye[1] = 0;
    eye[2] = 0;


}

void myDisplay(void)
{

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.70, 0.70, 0.70, 1.0);  //Dark grey background
    //glClearColor(0.10, 0.10, 0.10, 1.0);  //Dark grey background
    //glClearColor(0.10, 0.10, 0.10, 1.0);  //Dark grey background

    //glClearColor(1, 1, 1, 1.0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
        {
        glUniform4f(DIR_LIGHT_POS, -100, 50, 0, 0);
        glUniform3f(DIR_LIGHT_DIR, -1, -1, -1);
        glUniform3f(DIR_LIGHT_COLOR, 1, 1, 1);
        glUniform1f(DIR_LIGHT_EXPO, 1);
        glUniform1f(DIR_LIGHT_STR, dirLightStr); //1.7

        glUniform4f(SPOT_LIGHT_POS, elfPos[0], elfPos[1], elfPos[2], 1);  // 0 平行
        glUniform3f(SPOT_LIGHT_DIR, 0, -1, 0);
        glUniform3f(SPOT_LIGHT_COLOR, 1, 1, 1);
        glUniform1f(SPOT_LIGHT_EXPO, 1);                                  // 0 point
        glUniform1f(SPOT_LIGHT_STR, 40);

        glUniform3f(CAMERA_POS, eye[0], eye[1], eye[2]);
        glUniform1f(AMBIENT, 0.2);
    }
    {   //projection
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        //aspect = W/(double)H
        //glOrtho(-60, 60, -60, 60,-1000, 1000);
        gluPerspective(90, 1, 0.1, 200);
        float gluPers[16];
        glGetFloatv(GL_PROJECTION_MATRIX, gluPers);
        glUniformMatrix4fv(1, 1, GL_FALSE, gluPers);
        glLoadIdentity();
    }
    {   //lookat
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0,25,100,0,25,0,0,1,0);
        //gluLookAt(eye[0], eye[1] , eye[2], pos[0], pos[1] + 20, pos[2], u[1][0], u[1][1], u[1][2]);
        //gluLookAt(eye[0], eye[1], eye[2], eye[0] - u[2][0], eye[1] - u[2][1], eye[2] - u[2][2], u[1][0], u[1][1], u[1][2]);
        glGetFloatv(GL_MODELVIEW_MATRIX, eyeMtx);
        glUniformMatrix4fv(0, 1, GL_FALSE, eyeMtx);
    }

        float objMtx[16];
        // glPushMatrix();
        // glScalef(30, 30, 30);
        // //glTranslatef(0.5, 0.5, 0.5);
        // //glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        // //glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        // myTex->cheese ->use(programID);
        // myObj->solidsphere->draw(programID);
        // glPopMatrix();
    
        //j ＝ I x w = r x F
        //求 I ,F

        //I = 3 , I^-1 = 1/3
        //I = 2/5 m r^2 , I^-1 = 
        int pt = clock();
         dt = (pt - pretime)/1000.f;
        pretime = pt;


            //v *= 0.999;        
            //w *= 0.99;


            if(length(w) != 0)
                rot = angleAxis(length(w),normalize(w))*rot;
            //translate(mat4(1),possphere)*toMat4(rot)*
            possphere += (v*dt);
            mat4 tp = translate(mat4(1),possphere)*toMat4(rot)*scale(mat4(1),{20,20,20});

            float k = 0.5;
            vec3 dragforce = -v*k;
            applyLinearForce(dragforce);
            
            if(flag && possphere[1]>0) applyLinearForce({0,-9.8*100,0});

            if(possphere[1]<=0-0.00001) {
                v[1] = -v[1]*0.6 ;
                if(fabs(v[1]) < 5) v[1] = 0;
                possphere[1] = 0;
            }

            //applyRotForce(dragforce);
            applyRotJ(-100*k*w);  //-k(r^2w)

            //glGetFloatv(GL_MODELVIEW_MATRIX, &tp[0][0]);
            glUniformMatrix4fv(2, 1, GL_FALSE, &tp[0][0]);

            myTex->cheese ->use(programID);
            myObj->solidsphere->draw(programID);
            
        



        







    glutSwapBuffers();
    //cout << "swap\n";
    glutPostRedisplay();
    
    usleep(1000); //micro sleep
    
}
int mouseX = 0, mouseY = 0,mouseBtn = 0;
void motion_func(int  x, int y) {
    //cout << x << " " << y << "\n";
    if (mouseBtn == GLUT_RIGHT_BUTTON) {
        if (x > mouseX) eyeAngy += 0.5;
        else eyeAngy -= 0.5;

        if (eyeAngy >= 360) eyeAngy -= 360;
        if (eyeAngy <= 0) eyeAngy += 360;

        if (y > mouseY)  eye[1] = fmin(eye[1] + 0.5, 80.0);
        else eye[1] = fmax(eye[1] - 0.5, 10.0);
    }
    else if (mouseBtn == GLUT_LEFT_BUTTON) {
        //cout << "oo\n";
    
    }

    mouseX = x;
    mouseY = y;
};
void mouseWheel_fun(int button, int dir, int x, int y) {
    //cout << "kkk\n";
    if (dir > 0) fovy = fmax(fovy - 2, 70);
    else fovy = fmin(fovy + 2, 150);

}
void mouseClick_fun(int btn, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        //cout << btn << "\n";
        if (btn == GLUT_RIGHT_BUTTON) {
            mouseBtn = GLUT_RIGHT_BUTTON;
        }
        else if(btn == 3){
            fovy = fmax(fovy - 2, 70);
            //cout << "pos: " << pos[0] << " " << pos[2] << "\n";
        }else if(btn == 4){
            fovy = fmin(fovy + 2, 150);
        }else if(btn == 0){
            cout << "pos: " << pos[0] << " " << pos[2] << "\n";
        }
    }
    else if (state == GLUT_UP) {
        mouseBtn = -1;
    }
    //cout << GLUT_LEFT_BUTTON << "\n";
}


int main(int argc, char** argv) {
    srand(time(NULL));
    std::cout << "Hello World!\n";
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    //glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
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
    programID = glCreateProgram(); //此程式的id(gpu)
    GLuint vertID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragID = glCreateShader(GL_FRAGMENT_SHADER);
    
    //開檔讀黨
    fstream file("Phong.frag",std::fstream::in | std::fstream::ate); //ate -> 移到最後面
    int ffsz = file.tellg();  //回傳當前指針位置( ate在尾巴 -> 檔案大小 )
    file.seekg(0); //移到頭
    string fragS;
    fragS.resize(ffsz);
    file.read((char*)fragS.data(), ffsz);
    file.close();

    file = fstream("Phong.vert", std::fstream::in | std::fstream::ate);
    int vfsz = file.tellg();
    file.seekg(0); //移到頭
    string vertS;
    vertS.resize(vfsz);
    file.read((char*)vertS.data(), vfsz);
    file.close();

    char* vertS_ = (char*)vertS.data();
    glShaderSource(vertID, 1, &vertS_, &vfsz);
    glCompileShader(vertID);

    char* fragS_ = (char*)fragS.data();
    glShaderSource(fragID, 1, &fragS_, &ffsz);
    glCompileShader(fragID);

    glAttachShader(programID, vertID);
    glAttachShader(programID, fragID);

    glLinkProgram(programID);

    glUseProgram(programID);
    //The main loop.
    myInit();
    glutKeyboardFunc(keybaord_fun);
    glutMotionFunc(motion_func);/* Mouse motion event callback func */
    glutMouseWheelFunc(mouseWheel_fun);
    glutMouseFunc(mouseClick_fun);
    glutMainLoop();
    return 0;
}