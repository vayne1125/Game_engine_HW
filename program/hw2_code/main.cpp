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
#include "SceneVendor.h"
#include "ScenePhysicalExpFiled.h"
#include "Billboard.h"
#include "PhyObj.h"
#include "Object.h"
#include "UI.h"
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
//時間模式
#define RUNTIMER 50             //判斷是否跑跑跑
#define JUMPTIMER 51            //一般跳
#define JUMPONWANDTIMER 52      //跳上法杖
#define JUMPTOFLOORTIMER 53     //跳回地板
#define ANIMATION        54
#define LIGHT_ELF        55

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 990
//object
using namespace std;
GLuint programID;
SceneVendor* sceneVendor;
ScenePhysicalExpFiled* scenePhysicalExpFiled;
Robot* myRobot;
GraphicObj* graphicObj;
mytex* myTex;
Billboard* billboard;
magicwand* uiui;

Object* object[3];
Object* chooseObject;
UI* ui;
float   eyeAngx = 0.0, eyeAngy = 90.0, eyeAngz = 0.0;
float   u[3][3] = { {1.0,0.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0} };
float   eye[3] = { 0 };
float   eyeDis = 0;
double zNear = 0.1, zFar = 2000, aspect = WINDOW_WIDTH / (double)WINDOW_HEIGHT, fovy;

float   cv = cos(5.0 * PI / 180.0), sv = sin(5.0 * PI / 180.0); /* cos(5.0) and sin(5.0) */


int     preKey = -1;
float   eyeMtx[16] = {0};

int dirLightStr = 2.3;
bool isDirLightOpen = 1;
int pretime = 0;

bool firstClick = 0;
float firstSightFovy = 40;
float firstSightEyePos[3] = {116,30,150}; //攝影機
float firstSightSeePoint[3] = {116,30,155}; //看向的點：準心
float firstSightEyeAngY = 90;
void first_sight_move_order(unsigned int key);

bool detectCollision(int x, int y, int z,int tar);
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
    case ANIMATION:                //搖椅擺擺擺
        //myBig_chair.move();
        //poolAng++;
        sceneVendor->eevee_ani++;
        if (sceneVendor->eevee_ani >= 56) sceneVendor->eevee_ani = 0;
        //if (scene == GRASSLAND) {
        glutTimerFunc(100, timerFunc, ANIMATION);
        //}
        //glutPostRedisplay();
        break;
    case LIGHT_ELF:
        float tpPos[3];
        for (int i = 0; i < 3; i++) tpPos[i] = sceneVendor->spotLightElf->pos[i];
        if (sceneVendor->spotLightElf->dir == 2) { //前
            tpPos[2]--;
        }
        else if (sceneVendor->spotLightElf->dir == 3) { //左
            tpPos[0]--;
        }else if (sceneVendor->spotLightElf->dir == 0) { //後
            tpPos[2]++;
        }
        else if (sceneVendor->spotLightElf->dir == 1) { //右
            tpPos[0]++;
        }
        if (detectCollision(tpPos[0], tpPos[1], tpPos[2],1)) {
            //cout << tpPos[0] << " " << tpPos[2] << "\n";
            int tp = 1 + rand() % 4;
            //tp += 2;
            sceneVendor->spotLightElf->dir = (sceneVendor->spotLightElf->dir + tp) % 4;
            sceneVendor->spotLightElf->angleY = 90 * (sceneVendor->spotLightElf->dir);
        }
        else {
            for (int i = 0; i < 3; i++) sceneVendor->spotLightElf->pos[i] = tpPos[i];
        }

        glutTimerFunc(50, timerFunc, LIGHT_ELF);
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
                //glDisable(GL_LIGHT2);
                //isLitspotOpen = 0;
            }
        }
        else {
            myRobot->jump_ready();
            glutTimerFunc(100, timerFunc, JUMPTIMER);
        }
    }
}
void my_move_order(unsigned char key) {        //跟移動相關的判斷

    float tpPos[3] = { myRobot->pos[0], myRobot->pos[1], myRobot->pos[2] };
    float w[3] = { myRobot->pos[0] - eye[0] ,myRobot->pos[1] - eye[1], myRobot->pos[2] - eye[2] };
    float s[3] = { -myRobot->pos[0] + eye[0] , -myRobot->pos[1] + eye[1], -myRobot->pos[2] + eye[2] };
    float d[3] = { -w[2] , 0 , w[0] };
    float a[3] = { w[2] , 0 , -w[0] };
    w[0] = w[0] / sqrt(w[0] * w[0] + w[1] * w[1] + w[2] * w[2]);
    w[1] = w[1] / sqrt(w[0] * w[0] + w[1] * w[1] + w[2] * w[2]);
    w[2] = w[2] / sqrt(w[0] * w[0] + w[1] * w[1] + w[2] * w[2]);

    s[0] = s[0] / sqrt(s[0] * s[0] + s[1] * s[1] + s[2] * s[2]);
    s[1] = s[1] / sqrt(s[0] * s[0] + s[1] * s[1] + s[2] * s[2]);
    s[2] = s[2] / sqrt(s[0] * s[0] + s[1] * s[1] + s[2] * s[2]);

    d[0] = d[0] / sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
    d[1] = d[1] / sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
    d[2] = d[2] / sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);

    a[0] = a[0] / sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    a[1] = a[1] / sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    a[2] = a[2] / sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);

    d[0] = (w[0] + d[0])/2.0;
    d[2] = (w[2] + d[2])/2.0;

    a[0] = (w[0] + a[0])/2.0;
    a[2] = (w[2] + a[2])/2.0;
    //if (myRobot->getMoveMode() == ROBOT_WALK && preKey == key && (key == 'W' || key == 'w' || key == 'A' || key == 'a' || key == 'S' || key == 's' || key == 'D' || key == 'd')){
    //    myRobot->change_moveMode(ROBOT_RUN);                       //0.3秒內連續按 就變成跑跑
    //}
    if (key == 'S' || key == 's') {
        if (myRobot->isOnWand) {        //飛行模式
            myRobot->angle_y = 90;
        }
        else {                         //一般走路模式
            //myRobot->angle_y = 0;
            myRobot->angle_y = (90 - eyeAngy);
            myRobot->move();
        }
        tpPos[0] += myRobot->moveOffset * s[0];
        tpPos[2] += myRobot->moveOffset * s[2];
        //cout << "here\n";
        //tpPos[2] += myRobot->moveOffset;
    }
    else if (key == 'W' || key == 'w') {
        if (myRobot->isOnWand) {
            myRobot->angle_y = 90;
        }
        else {
            //myRobot->angle_y = 180;
            myRobot->angle_y = 270 - eyeAngy;
            myRobot->move();
        }
        tpPos[0] += myRobot->moveOffset * w[0];
        tpPos[2] += myRobot->moveOffset * w[2];
        //tpPos[2] -= myRobot->moveOffset;
        //pupu
    }
    else if (key == 'A' || key == 'a') {
        if (myRobot->isOnWand) {
            myRobot->angle_y = 0;
        }
        else {
            myRobot->angle_y = 360 - eyeAngy - 45;
            //myRobot->angle_y = 270;
            myRobot->move();
        }
        tpPos[0] += myRobot->moveOffset * a[0];
        tpPos[2] += myRobot->moveOffset * a[2];
    }
    else if (key == 'D' || key == 'd') {
        if (myRobot->isOnWand) {
            myRobot->angle_y = 0;
        }
        else {
            myRobot->angle_y = 180 - eyeAngy + 45;
            //myRobot->angle_y = 90;
            myRobot->move();
        }
        tpPos[0] += myRobot->moveOffset * d[0];
        tpPos[2] += myRobot->moveOffset * d[2];
    }
    else if (key == 'r' || key == 'R') {            //轉圈圈
        myRobot->angle_y += 5;
        if (!myRobot->isOnWand) {
            myRobot->change_moveMode(ROBOT_TURN);
            myRobot->move();    //在地板才要動腳
        }
    }
    //if (detectCollision(tpPos[0], tpPos[1], tpPos[2],0)) return;
    for (int i = 0; i < 3; i++) myRobot->pos[i] = tpPos[i];
}
float getDis(float x1, float y1, float x2, float y2) {           //算距離
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}
void keybaord_fun(unsigned char key, int X, int Y) {

    my_move_order(key);
    first_sight_move_order(key);
    if (key == '0') {
        isDirLightOpen ^= 1;
        if(isDirLightOpen) dirLightStr = 2.3;
        else dirLightStr = 0;
    }   
    if (key == 127) { //ctrl + backspace
        reset_camera();
    }
    if (key == 'p' || key == 'P')
    {
        glm::vec3 F{0, 60, 0};
        glm::vec3 impactPoint{0.01, -1, 0};  //在地座標系

        object[0]->applyForce(F,impactPoint);
        object[1]->applyForce(F,impactPoint);
        object[2]->applyForce(F,impactPoint);
    }
}
void first_sight_move_order(unsigned int key){
    float tpPos[3] = { firstSightEyePos[0], firstSightEyePos[1], firstSightEyePos[2] };
    float w[3] = { firstSightSeePoint[0] - firstSightEyePos[0] ,firstSightSeePoint[1] - firstSightEyePos[1], firstSightSeePoint[2] - firstSightEyePos[2] };
    float s[3] = { -firstSightSeePoint[0] + firstSightEyePos[0] , -firstSightSeePoint[1] + firstSightEyePos[1], -firstSightSeePoint[2] + firstSightEyePos[2] };
    float d[3] = { -w[2] , 0 , w[0] };
    float a[3] = { w[2] , 0 , -w[0] };
    w[0] = w[0] / sqrt(w[0] * w[0] + w[1] * w[1] + w[2] * w[2]);
    w[1] = w[1] / sqrt(w[0] * w[0] + w[1] * w[1] + w[2] * w[2]);
    w[2] = w[2] / sqrt(w[0] * w[0] + w[1] * w[1] + w[2] * w[2]);

    s[0] = s[0] / sqrt(s[0] * s[0] + s[1] * s[1] + s[2] * s[2]);
    s[1] = s[1] / sqrt(s[0] * s[0] + s[1] * s[1] + s[2] * s[2]);
    s[2] = s[2] / sqrt(s[0] * s[0] + s[1] * s[1] + s[2] * s[2]);

    d[0] = d[0] / sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
    d[1] = d[1] / sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
    d[2] = d[2] / sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);

    a[0] = a[0] / sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    a[1] = a[1] / sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    a[2] = a[2] / sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    if(key == 's' || key == 'S'){
        firstSightEyePos[0] += s[0];
        firstSightEyePos[2] += s[2];
    }else if(key == 'w' || key == 'W'){
        firstSightEyePos[0] += w[0];
        firstSightEyePos[2] += w[2];
    }else if(key == 'a' || key == 'A'){
        firstSightEyePos[0] += a[0];
        firstSightEyePos[2] += a[2];
    }else if(key == 'd' || key == 'D'){
        firstSightEyePos[0] += d[0];
        firstSightEyePos[2] += d[2];
    }
}
void myInit() {
    
    myTex = new mytex(programID);
    graphicObj = new GraphicObj(programID);
    
    myRobot = new Robot(programID,116,0,165);

    myRobot->setColor(myTex->robot_blue_main, myTex->robot_blue_sub);

    myRobot->setOffset(0.5,1.5,3); 
    myRobot->change_moveMode(ROBOT_RUN);

    glutTimerFunc(100, timerFunc, ANIMATION);
    glutTimerFunc(100, timerFunc, LIGHT_ELF);

    sceneVendor = new SceneVendor();
    scenePhysicalExpFiled = new ScenePhysicalExpFiled();
    

    eyeDis = 30;
    fovy = 100;
    eye[0] = 0;
    eye[1] = 20;
    eye[2] = eyeDis;

    pretime = clock();
    
    //object.resize(3);
    // = 
    object[0] = new Object(YU_GRAPHICS_CUBE,YU_PHYSICS_CUBE,YU_CHEESE,{5, 10, 2.5}, 1, 0.2);
    object[0] -> setPos(100,10,250);
    object[0] -> setName("cube");
    
    object[1] = new Object(YU_GRAPHICS_SPHERE,YU_PHYSICS_SPHERE,YU_RED,10,1,0.01f);
    object[1] -> setPos(200,100,250);
    object[1] -> setName("sphere");

    object[2] = new Object(YU_GRAPHICS_CLOUD,YU_PHYSICS_IRREGULAR,YU_CHEESE,{4,4,4},1);
    object[2] -> setPos(200,10,250);
    object[2] -> setName("cloud");
    
    chooseObject = new Object();

    ui = new UI(aspect);
}
bool detectCollision(int x, int y, int z,int tar) {
    if (z <= 48 + 6) return 1;   //第一行攤販
    if ((z <= 95 && (x <= 48 + 6 || x >= 187)) || (z <= 95 && (x <= 48 - 6 || x >= 187 - 7))) return 1;     //第二條攤販
    if (x < 0 || x > 232 || z < 0 || z > 203) return 1; //邊界
    if (tar == 0) {
        if (getDis(x, z, sceneVendor->fountainPos[0], sceneVendor->fountainPos[2]) <= 29) return 1;   //噴水池
    }
    else if (tar == 1) {
        if (getDis(x, z, sceneVendor->fountainPos[0], sceneVendor->fountainPos[2]) <= 5) return 1;   //噴水池
        for (int i = 0; i < sceneVendor->cloud.size(); i++) {
            if (getDis(x, z, sceneVendor->cloud[i][0], sceneVendor->cloud[i][2]) <= 5) return 1;
        }
    }
    return 0;
}
void myDisplay(void)
{
    glUseProgram(programID);
    glEnable(GL_DEPTH_TEST);

    firstSightSeePoint[0] = firstSightEyePos[0] + 15 * cos(firstSightEyeAngY * PI / 180.0);
    firstSightSeePoint[2] = firstSightEyePos[2] + 15 * sin(firstSightEyeAngY * PI / 180.0);

    eye[0] = myRobot->pos[0] + eyeDis * cos(eyeAngy * PI / 180.0);
    eye[2] = myRobot->pos[2] + eyeDis * sin(eyeAngy * PI / 180.0);
    glClearColor(0.70, 0.70, 0.70, 1.0);  //Dark grey background
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
    {
        glUniform4f(DIR_LIGHT_POS, -100, 50, 0, 0);
        glUniform3f(DIR_LIGHT_DIR, -1, -1, -1);
        glUniform3f(DIR_LIGHT_COLOR, 1, 1, 1);
        glUniform1f(DIR_LIGHT_EXPO, 1);
        glUniform1f(DIR_LIGHT_STR, dirLightStr); //1.7

        glUniform4f(SPOT_LIGHT_POS, sceneVendor->spotLightElf->pos[0], sceneVendor->spotLightElf->pos[1], sceneVendor->spotLightElf->pos[2], 1);  // 0 平行
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
        gluPerspective(firstSightFovy, aspect, zNear, zFar);
        float gluPers[16];
        glGetFloatv(GL_PROJECTION_MATRIX, gluPers);
        glUniformMatrix4fv(1, 1, GL_FALSE, gluPers);
        glLoadIdentity();
    }
    {   //lookat
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        //gluLookAt(eye[0], eye[1] , eye[2], myRobot->pos[0], myRobot->pos[1] + 20, myRobot->pos[2], 0,1,0);
        
        gluLookAt(firstSightEyePos[0], firstSightEyePos[1] , firstSightEyePos[2], firstSightSeePoint[0], firstSightSeePoint[1], firstSightSeePoint[2], 0,1,0);

        //gluLookAt(eye[0], eye[1], eye[2], eye[0] - u[2][0], eye[1] - u[2][1], eye[2] - u[2][2], u[1][0], u[1][1], u[1][2]);
        glGetFloatv(GL_MODELVIEW_MATRIX, eyeMtx);
        glUniformMatrix4fv(0, 1, GL_FALSE, eyeMtx);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    float objMtx[16];
    //robot
    {
        glPushMatrix();
        //glTranslatef(myRobot->pos[0], myRobot->pos[1], myRobot->pos[2]);
        glTranslatef(firstSightEyePos[0],firstSightEyePos[1],firstSightEyePos[2]);
        myRobot->draw(programID);
        glPopMatrix();
    }

    {
        sceneVendor -> draw(eyeMtx,programID);
        scenePhysicalExpFiled->draw(eyeMtx,programID);
    }

    int pt = clock();
    float dt = std::min((pt - pretime) / 1000.f, 0.01f);
    pretime = pt;
    
    object[0] -> update(dt);
    object[1] -> update(dt);
    object[2] -> update(dt);

    object[0] -> draw(programID);
    object[1] -> draw(programID);
    object[2] -> draw(programID);
    

    //ui
    

    float dir_[3] = {firstSightSeePoint[0]-firstSightEyePos[0],firstSightSeePoint[1]-firstSightEyePos[1],firstSightSeePoint[2]-firstSightEyePos[2]};
    
    ui->draw(*chooseObject,firstSightEyePos,dir_,100);

    glutSwapBuffers();
    glutPostRedisplay();
    usleep(1000); //micro sleep
    
}
int mouseX = 0, mouseY = 0,mouseBtn = 0;
void passive_motion_func(int x,int y){
    if(firstClick == 0) return;

    firstSightEyeAngY += 0.8*(x-mouseX);
    if (firstSightEyeAngY >= 360) firstSightEyeAngY -= 360;
    if (firstSightEyeAngY <= 0) firstSightEyeAngY += 360;

    if (y < mouseY)  firstSightSeePoint[1] = fmin(firstSightSeePoint[1] - 0.2*(y-mouseY), 50.0);
    else firstSightSeePoint[1] = fmax(firstSightSeePoint[1] - 0.2*(y-mouseY), 0.0);    

    //cout << "firstSightSeePoint: " << firstSightSeePoint[1] << "\n";
    mouseX = x;
    mouseY = y;
    float dir_[3] = {firstSightSeePoint[0]-firstSightEyePos[0],firstSightSeePoint[1]-firstSightEyePos[1],firstSightSeePoint[2]-firstSightEyePos[2]};
    for(int i=0;i<3;i++){
        if(object[i]->isChoose({firstSightEyePos[0],firstSightEyePos[1],firstSightEyePos[2]},{dir_[0],dir_[1],dir_[2]}))
            *chooseObject = *object[i];
    }
}
void motion_func(int  x, int y) {
    
    //cout << x << " " << y << "\n";
    if (mouseBtn == GLUT_RIGHT_BUTTON) {
        if (x > mouseX) eyeAngy += 1;
        else eyeAngy -= 1;

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
        }else if(btn == 4){
            fovy = fmin(fovy + 2, 150);
        }else if(btn == 0){
            firstClick = 1;
            //cout << "pos: " << myRobot->pos[0] << " " << myRobot->pos[2] << "\n";
            object[0]->shoot(50,{firstSightEyePos[0],firstSightEyePos[1],firstSightEyePos[2]},{firstSightSeePoint[0]-firstSightEyePos[0],firstSightSeePoint[1]-firstSightEyePos[1],firstSightSeePoint[2]-firstSightEyePos[2]});
            object[2]->shoot(50,{firstSightEyePos[0],firstSightEyePos[1],firstSightEyePos[2]},{firstSightSeePoint[0]-firstSightEyePos[0],firstSightSeePoint[1]-firstSightEyePos[1],firstSightSeePoint[2]-firstSightEyePos[2]});
            object[1]->shoot(50,{firstSightEyePos[0],firstSightEyePos[1],firstSightEyePos[2]},{firstSightSeePoint[0]-firstSightEyePos[0],firstSightSeePoint[1]-firstSightEyePos[1],firstSightSeePoint[2]-firstSightEyePos[2]});
        }
    }
    else if (state == GLUT_UP) {
        mouseBtn = -1;
    }
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