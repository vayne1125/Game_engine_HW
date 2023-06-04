#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include"../glad/glad.h"
#include "mesh.h"
#include "GraphicObj.h"
#include "mytex.h"
#include "Robot.h"
#include "MyRobot.h"
#include "SceneVendor.h"
#include "ScenePhysicalExpFiled.h"
#include "Billboard.h"
#include "PhyObj.h"
#include "Object.h"
#include "UIPhy.h"
#include "Perspective.h"
#define   PI   3.1415927
//location
#define AMBIENT          11
//時間模式
#define RUNTIMER 50             //判斷是否跑跑跑
#define JUMPTIMER 51            //一般跳
#define JUMPONWANDTIMER 52      //跳上法杖
#define JUMPTOFLOORTIMER 53     //跳回地板

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 990
//object
using namespace std;
GLuint programID;
SceneVendor* sceneVendor;
ScenePhysicalExpFiled* scenePhysicalExpFiled;
MyRobot* myRobot;
GraphicObj* graphicObj;
mytex* myTex;
Billboard* billboard;
UIPhy* uiphy;
TPPerspective* tpperspective;
FPPerspective* fpperspective;
float   eyeAngy = 90.0;
float   eye[3] = { 0 };
float   eyeDis = 0;
double zNear = 0.1, zFar = 2000, aspect = WINDOW_WIDTH / (double)WINDOW_HEIGHT, fovy;

float   cv = cos(5.0 * PI / 180.0), sv = sin(5.0 * PI / 180.0); /* cos(5.0) and sin(5.0) */

int     preKey = -1;
float   eyeMtx[16] = {0};

void timerFunc(int nTimerID) {
    switch (nTimerID) {
    case RUNTIMER:                //偵測跑
        preKey = -1;
        break;
    case JUMPTIMER:               //跳躍
        if (!myRobot->jump()) {
            glutTimerFunc(100, timerFunc, JUMPTIMER);
        }
        else {
            myRobot->stand();
        }
        glutPostRedisplay();
        break;
    case JUMPONWANDTIMER:           //跳上法杖
        if (!myRobot->jumpOnWand()) {
            glutTimerFunc(100, timerFunc, JUMPONWANDTIMER);
        }
        glutPostRedisplay();
        break;
    case JUMPTOFLOORTIMER:          //跳回地面
        if (!myRobot->jumpToFloor()) {
            glutTimerFunc(100, timerFunc, JUMPTOFLOORTIMER);
        }
        else {
            myRobot->stand();
        }
        glutPostRedisplay();
        break;
    case ANIMATION_EEVEE:                //搖椅擺擺擺
        sceneVendor -> animation(ANIMATION_EEVEE);
        glutTimerFunc(100, timerFunc, ANIMATION_EEVEE);
        break;

    case ANIMATION_SPOTLIGHT_ELF:
        sceneVendor -> animation(ANIMATION_SPOTLIGHT_ELF);
        glutTimerFunc(50, timerFunc, ANIMATION_SPOTLIGHT_ELF);
    }
}
void reset_camera() {
    eyeDis = 30;
    fovy = 100;
    eye[0] = 0;
    eye[1] = 20;
    eye[2] = eyeDis;
    eyeAngy = 90;
    myRobot->angle_y = 0;
}
void keyboardUp_func(unsigned char key, int x, int y) {
    if (key == ' ') {                   //跳
        if (myRobot->isMagician) {
            if (myRobot->isOnWand) {
                glutTimerFunc(100, timerFunc, JUMPTOFLOORTIMER);
                myRobot->change_moveMode(ROBOT_WALK);
            }
            else {
                myRobot->jump_ready();
                glutTimerFunc(100, timerFunc, JUMPONWANDTIMER);
                myRobot->change_moveMode(ROBOT_FLY);
            }
        }
        else {
            myRobot->jump_ready();
            glutTimerFunc(100, timerFunc, JUMPTIMER);
        }
    }
}
void my_move_order(unsigned char key) {        //跟移動相關的判斷

//     float tpPos[3] = { myRobot->pos[0], myRobot->pos[1], myRobot->pos[2] };
//     float w[3] = { myRobot->pos[0] - eye[0] ,myRobot->pos[1] - eye[1], myRobot->pos[2] - eye[2] };
//     float s[3] = { -myRobot->pos[0] + eye[0] , -myRobot->pos[1] + eye[1], -myRobot->pos[2] + eye[2] };
//     float d[3] = { -w[2] , 0 , w[0] };
//     float a[3] = { w[2] , 0 , -w[0] };
//     w[0] = w[0] / sqrt(w[0] * w[0] + w[1] * w[1] + w[2] * w[2]);
//     w[1] = w[1] / sqrt(w[0] * w[0] + w[1] * w[1] + w[2] * w[2]);
//     w[2] = w[2] / sqrt(w[0] * w[0] + w[1] * w[1] + w[2] * w[2]);

//     s[0] = s[0] / sqrt(s[0] * s[0] + s[1] * s[1] + s[2] * s[2]);
//     s[1] = s[1] / sqrt(s[0] * s[0] + s[1] * s[1] + s[2] * s[2]);
//     s[2] = s[2] / sqrt(s[0] * s[0] + s[1] * s[1] + s[2] * s[2]);

//     d[0] = d[0] / sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
//     d[1] = d[1] / sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
//     d[2] = d[2] / sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);

//     a[0] = a[0] / sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
//     a[1] = a[1] / sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
//     a[2] = a[2] / sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);

//     d[0] = (w[0] + d[0])/2.0;
//     d[2] = (w[2] + d[2])/2.0;

//     a[0] = (w[0] + a[0])/2.0;
//     a[2] = (w[2] + a[2])/2.0;

//     if (key == 'S' || key == 's') {
//         myRobot->angle_y = (90 - eyeAngy);
//         myRobot->move();
        
//         tpPos[0] += myRobot->moveOffset * s[0];
//         tpPos[2] += myRobot->moveOffset * s[2];
//     }
//     else if (key == 'W' || key == 'w') {
//         if (myRobot->isOnWand) {
//             myRobot->angle_y = 90;
//         }
//         else {
//             myRobot->angle_y = 270 - eyeAngy;
//             myRobot->move();
//         }
//         tpPos[0] += myRobot->moveOffset * w[0];
//         tpPos[2] += myRobot->moveOffset * w[2];
//     }
//     else if (key == 'A' || key == 'a') {
//         if (myRobot->isOnWand) {
//             myRobot->angle_y = 0;
//         }
//         else {
//             myRobot->angle_y = 360 - eyeAngy - 45;
//             myRobot->move();
//         }
//         tpPos[0] += myRobot->moveOffset * a[0];
//         tpPos[2] += myRobot->moveOffset * a[2];
//     }
//     else if (key == 'D' || key == 'd') {
//         if (myRobot->isOnWand) {
//             myRobot->angle_y = 0;
//         }
//         else {
//             myRobot->angle_y = 180 - eyeAngy + 45;
//             myRobot->move();
//         }
//         tpPos[0] += myRobot->moveOffset * d[0];
//         tpPos[2] += myRobot->moveOffset * d[2];
//     }
//     else if (key == 'r' || key == 'R') {            //轉圈圈
//         myRobot->angle_y += 5;  
//         myRobot->change_moveMode(ROBOT_TURN);
//         myRobot->move();    //在地板才要動腳
        
//     }
//     if (sceneVendor->detectCollision(tpPos[0], tpPos[1], tpPos[2],ROBOT)) return;
//     for (int i = 0; i < 3; i++) myRobot->pos[i] = tpPos[i];
}
float getDis(float x1, float y1, float x2, float y2) {           //算距離
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}
void keybaord_fun(unsigned char key, int X, int Y) {
    //cout << int(key) << "\n";
    //my_move_order(key);
    myRobot->keyEvent(key);
    //scenePhysicalExpFiled->keyEvent(key);
    //sceneVendor->keyEvent(key);
    //fpperspective->keyEvent(key);

    if (key == 127) { //ctrl + backspace
        reset_camera();
    }
}
void myInit() {
    
    myTex = new mytex(programID);
    graphicObj = new GraphicObj(programID);
    
    myRobot = new MyRobot(programID,116,0,165);

    myRobot->setColor(myTex->robot_blue_main, myTex->robot_blue_sub);

    myRobot->setOffset(0.5,1.5,3); 
    myRobot->change_moveMode(ROBOT_RUN);

    glutTimerFunc(1, timerFunc, ANIMATION_SPOTLIGHT_ELF);
    glutTimerFunc(1, timerFunc, ANIMATION_EEVEE);

    sceneVendor = new SceneVendor();
    scenePhysicalExpFiled = new ScenePhysicalExpFiled();
    
    fpperspective = new FPPerspective(aspect);
    tpperspective = new TPPerspective(aspect);

    // eyeDis = 30;
    // fovy = 100;
    // eye[0] = 0;
    // eye[1] = 20;
    // eye[2] = eyeDis;

    uiphy = new UIPhy(aspect);
}
void myDisplay(void)
{
    glUseProgram(programID);
    glEnable(GL_DEPTH_TEST);

    //fpperspective->update();    //更新眼睛位置和看的方向
    tpperspective->update();
    // eye[0] = myRobot->pos[0] + eyeDis * cos(eyeAngy * PI / 180.0);
    // eye[2] = myRobot->pos[2] + eyeDis * sin(eyeAngy * PI / 180.0);
    
    glClearColor(0.70, 0.70, 0.70, 1.0);  //Dark grey background
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
    {
        sceneVendor->useLight();
        glUniform1f(AMBIENT, 0.2);
    }
    //fpperspective->use();
    tpperspective->use();
    // {   //projection
    //     glMatrixMode(GL_PROJECTION);
    //     glLoadIdentity();
    //     //aspect = W/(double)H
    //     //glOrtho(-60, 60, -60, 60,-1000, 1000);
    //     gluPerspective(fovy, aspect, zNear, zFar);
    //     float gluPers[16];
    //     glGetFloatv(GL_PROJECTION_MATRIX, gluPers);
    //     glUniformMatrix4fv(1, 1, GL_FALSE, gluPers);
    //     glLoadIdentity();
    // }
    // {   //lookat
    //     glMatrixMode(GL_MODELVIEW);
    //     glLoadIdentity();
    //     gluLookAt(eye[0], eye[1] , eye[2], myRobot->pos[0], myRobot->pos[1] + 20, myRobot->pos[2], 0,1,0);
    //     glGetFloatv(GL_MODELVIEW_MATRIX, eyeMtx);
    //     glUniformMatrix4fv(0, 1, GL_FALSE, eyeMtx);
    // }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    float objMtx[16];
    //robot
    {
        glPushMatrix();
        glTranslatef(myRobot->pos[0], myRobot->pos[1], myRobot->pos[2]);
        myRobot->draw(programID);
        glPopMatrix();
    }

    {
        sceneVendor -> draw(eyeMtx,programID);
        //scenePhysicalExpFiled->draw(eyeMtx,programID);
        //sceneVendor -> draw(fpperspective->eyeMtx,programID);
        //scenePhysicalExpFiled->draw(fpperspective->eyeMtx,programID);
    }

    //uiphy
    // float dir_[3] = {fpperspective->dir[0],fpperspective->dir[1],fpperspective->dir[2]};
    // float pos_[3] = {fpperspective->pos[0],fpperspective->pos[1],fpperspective->pos[2]};
    // uiphy->draw(*(scenePhysicalExpFiled->chooseObject),pos_,dir_,scenePhysicalExpFiled->force);

    glutSwapBuffers();
    glutPostRedisplay();
    usleep(1000); //micro sleep
    
}
int mouseX = 0, mouseY = 0,mouseBtn = 0;
void passive_motion_func(int x,int y){

    //fpperspective->passiveMotionEvent(x,y);

    // float dir_[3] = {fpperspective->dir[0],fpperspective->dir[1],fpperspective->dir[2]};
    // for(int i=0;i<3;i++){
    //     if(scenePhysicalExpFiled->object[i]->isChoose({fpperspective->pos[0],fpperspective->pos[1],fpperspective->pos[2]},{dir_[0],dir_[1],dir_[2]}))
    //         *(scenePhysicalExpFiled->chooseObject) = *(scenePhysicalExpFiled->object[i]);
    // }
}
void motion_func(int  x, int y) {
    tpperspective->motionEvent(x,y);
    // if (mouseBtn == GLUT_RIGHT_BUTTON) {
    //     if (x > mouseX) eyeAngy += 1;
    //     else eyeAngy -= 1;

    //     if (eyeAngy >= 360) eyeAngy -= 360;
    //     if (eyeAngy <= 0) eyeAngy += 360;

    //     if (y > mouseY)  eye[1] = fmin(eye[1] + 0.5, 80.0);
    //     else eye[1] = fmax(eye[1] - 0.5, 10.0);
    // }
    // else if (mouseBtn == GLUT_LEFT_BUTTON) {

    // }
    // mouseX = x;
    // mouseY = y;
};
void mouseWheel_fun(int button, int dir, int x, int y) {
    tpperspective->mouseWheelEvent(button,dir,x,y);
    // if (dir > 0) fovy = fmax(fovy - 2, 70);
    // else fovy = fmin(fovy + 2, 150);
}
void mouseClick_fun(int btn, int state, int x, int y) {
    tpperspective->mouseClickEvent(btn,state,x,y);
    // if (state == GLUT_DOWN) {
    //     //cout << btn << "\n";
    //     if (btn == GLUT_RIGHT_BUTTON) {
    //         mouseBtn = GLUT_RIGHT_BUTTON;
    //     }
    //     else if(btn == 3){
    //         fovy = fmax(fovy - 2, 70);
    //     }else if(btn == 4){
    //         fovy = fmin(fovy + 2, 150);
    //     }else if(btn == 0){
            
    //         // scenePhysicalExpFiled->object[0]->shoot(scenePhysicalExpFiled->force,fpperspective->pos,fpperspective->dir);
    //         // scenePhysicalExpFiled->object[2]->shoot(scenePhysicalExpFiled->force,fpperspective->pos,fpperspective->dir);
    //         // scenePhysicalExpFiled->object[1]->shoot(scenePhysicalExpFiled->force,fpperspective->pos,fpperspective->dir);
    //     }
    // }
    // else if (state == GLUT_UP) {
    //     mouseBtn = -1;
    // }
}
int main(int argc, char** argv) {
    srand(time(NULL));
    std::cout << "Hello World!\n";
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    //glutInitWindowPosition(100, 100);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
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
    glutSetCursor(GLUT_CURSOR_NONE);
    glutPassiveMotionFunc(passive_motion_func);
    glutKeyboardFunc(keybaord_fun);
    glutKeyboardUpFunc(keyboardUp_func);
    glutMotionFunc(motion_func);/* Mouse motion event callback func */
    glutMouseWheelFunc(mouseWheel_fun);
    glutMouseFunc(mouseClick_fun);
    glutMainLoop();
    return 0;
}