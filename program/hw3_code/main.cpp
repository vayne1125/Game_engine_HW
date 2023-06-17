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
#include "SceneJungle.h"
#include "Billboard.h"
#include "PhyObj.h"
#include "Object.h"
#include "UIPhy.h"
#include "UI.h"
#include "Perspective.h"
#include "Define.h"
#define   PI   3.1415927
//location
#define AMBIENT          11
//時間模式
#define RUNTIMER 50             //判斷是否跑跑跑
#define JUMPTIMER 51            //一般跳

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 990

//object
using namespace std;
GLuint programID;
SceneVendor* sceneVendor;
ScenePhysicalExpFiled* scenePhysicalExpFiled;
SceneJungle* sceneJungle;
MyRobot* myRobot;
GraphicObj* graphicObj;
mytex* myTex;
Billboard* billboard;
UIPhy* uiphy;
UI* ui;
TPPerspective* tpperspective;
FPPerspective* fpperspective;

int perspective = TPPERSPECTIVE;
int scene = SCENE_JUNGLE;

double aspect = WINDOW_WIDTH / (double)WINDOW_HEIGHT;
float   cv = cos(5.0 * PI / 180.0), sv = sin(5.0 * PI / 180.0); /* cos(5.0) and sin(5.0) */

void timerFunc(int nTimerID) {
    switch (nTimerID) {
    case JUMPTIMER:               //跳躍
        if (!myRobot->jump()) {
            glutTimerFunc(100, timerFunc, JUMPTIMER);
        }
        else {
            myRobot->stand();
        }
        glutPostRedisplay();
        break;
    case ANIMATION_EEVEE:               
        sceneVendor -> animation(ANIMATION_EEVEE);
        glutTimerFunc(100, timerFunc, ANIMATION_EEVEE);
        break;

    case ANIMATION_SPOTLIGHT_ELF:
        sceneVendor -> animation(ANIMATION_SPOTLIGHT_ELF);
        glutTimerFunc(50, timerFunc, ANIMATION_SPOTLIGHT_ELF);
    }
}
void keyboardUp_func(unsigned char key, int x, int y) {
    if (key == ' ') {                   //跳
        myRobot->jump_ready();
        glutTimerFunc(100, timerFunc, JUMPTIMER);
    }
}
float getDis(float x1, float y1, float x2, float y2) {           //算距離
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}
void keybaord_fun(unsigned char key, int X, int Y) {
    myRobot->keyEvent(key);
    //scenePhysicalExpFiled->keyEvent(key);
    switch(scene){
    case SCENE_VENDOR:   
        sceneVendor->keyEvent(key);
        break;
    case SCENE_JUNGLE:
        break;
    case SCENE_PHYSICALEXPFILED:
        break;
    }
    if(perspective == FPPERSPECTIVE) fpperspective->keyEvent(key);

    if(key == 'Y' || key == 'y') {
        perspective^=1;
        if(perspective == TPPERSPECTIVE){
            glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
        }else if(perspective == FPPERSPECTIVE){
            glutSetCursor(GLUT_CURSOR_NONE);
        }
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

    sceneVendor = new SceneVendor(programID);
    scenePhysicalExpFiled = new ScenePhysicalExpFiled();
    sceneJungle = new SceneJungle();
    fpperspective = new FPPerspective(aspect);
    tpperspective = new TPPerspective(aspect);

    uiphy = new UIPhy(aspect);
    ui = new UI(aspect);
}
void myDisplay(void)
{
    glUseProgram(programID);
    glEnable(GL_DEPTH_TEST);

    if(perspective == FPPERSPECTIVE) fpperspective->update();    //更新眼睛位置和看的方向
    else if(perspective == TPPERSPECTIVE) tpperspective->update();
    
    glClearColor(0.70, 0.70, 0.70, 1.0);  //Dark grey background
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
    switch(scene){
        case SCENE_VENDOR:
            sceneVendor->useLight();
            break;
        case SCENE_PHYSICALEXPFILED:
            break;
        case SCENE_JUNGLE:
            //cout<<"O\n";
            sceneJungle->useLight();
            break;
    }
    
    glUniform1f(AMBIENT, 0.2);

    if(perspective == FPPERSPECTIVE) fpperspective->use();    //更新眼睛位置和看的方向
    else if(perspective == TPPERSPECTIVE) tpperspective->use();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    float objMtx[16];
    //robot
    {
        myRobot->draw(programID);
    }
    
    
    {
        float eyeM[16];

        if(perspective == FPPERSPECTIVE)
            for(int i=0;i<16;i++) eyeM[i] = fpperspective->eyeMtx[i];
        else if(perspective == TPPERSPECTIVE) 
            for(int i=0;i<16;i++) eyeM[i] = tpperspective->eyeMtx[i];
        
        switch(scene){
            case SCENE_VENDOR:
                sceneVendor -> draw(eyeM,programID);
                break;
            case SCENE_PHYSICALEXPFILED:
                scenePhysicalExpFiled->draw(eyeM,programID);
                break;
            case SCENE_JUNGLE:
                sceneJungle->draw(eyeM,programID);
                break;
        }
        if(perspective == FPPERSPECTIVE) ui->draw();
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
    if(perspective == FPPERSPECTIVE) fpperspective->passiveMotionEvent(x,y);
    if(scene == SCENE_JUNGLE) sceneJungle->passiveMotionEvent(x,y);
    // float dir_[3] = {fpperspective->dir[0],fpperspective->dir[1],fpperspective->dir[2]};
    // for(int i=0;i<3;i++){
    //     if(scenePhysicalExpFiled->object[i]->isChoose({fpperspective->pos[0],fpperspective->pos[1],fpperspective->pos[2]},{dir_[0],dir_[1],dir_[2]}))
    //         *(scenePhysicalExpFiled->chooseObject) = *(scenePhysicalExpFiled->object[i]);
    // }
}
void motion_func(int  x, int y) {
    if(perspective == TPPERSPECTIVE) tpperspective->motionEvent(x,y);
    // if (mouseBtn == GLUT_RIGHT_BUTTON) { 
    // }
    // else if (mouseBtn == GLUT_LEFT_BUTTON) {
    // }
    // mouseX = x;
    // mouseY = y;
};
void mouseWheel_fun(int button, int dir, int x, int y) {
    if(perspective == TPPERSPECTIVE) tpperspective->mouseWheelEvent(button,dir,x,y);
    // if (dir > 0) {}
    // else {}
}
void mouseClick_fun(int btn, int state, int x, int y) {
    if(perspective == TPPERSPECTIVE) tpperspective->mouseClickEvent(btn,state,x,y);
    if(scene == SCENE_JUNGLE) sceneJungle->mouseClickEvent(btn,state,x,y);
    // if (state == GLUT_DOWN) {
    //     if (btn == GLUT_RIGHT_BUTTON) {
    //         mouseBtn = GLUT_RIGHT_BUTTON;
    //     }
    //     else if(btn == 3){
    //     }else if(btn == 4){
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
    glutPassiveMotionFunc(passive_motion_func);
    glutKeyboardFunc(keybaord_fun);
    glutKeyboardUpFunc(keyboardUp_func);
    glutMotionFunc(motion_func);/* Mouse motion event callback func */
    glutMouseWheelFunc(mouseWheel_fun);
    glutMouseFunc(mouseClick_fun);
    glutMainLoop();
    return 0;
}