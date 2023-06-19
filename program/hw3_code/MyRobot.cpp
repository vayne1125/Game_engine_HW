#include "MyRobot.h"
#include "SceneVendor.h"
#include "ScenePhysicalExpFiled.h"
#include "SceneJungle.h"
#include "Perspective.h"
#include "Define.h"
#include "UI.h"
extern SceneVendor* sceneVendor;
extern SceneJungle* sceneJungle;
extern TPPerspective* tpperspective;
extern FPPerspective* fpperspective;
extern int perspective;
extern int scene;
extern UI* ui;
MyRobot::MyRobot(unsigned int programID, float pos_x, float pos_y, float pos_z):Robot(programID,pos_x,pos_y,pos_z)
{

}
void MyRobot::keyEvent(unsigned char key)
{
    if(perspective == FPPERSPECTIVE) {
        // pos[0] = fpperspective->pos[0];
        // pos[2] = fpperspective->pos[2];
        return;
    }
    float tpPos[3] = { pos[0], pos[1], pos[2] };
    float w[3] = { pos[0] - tpperspective->pos[0] ,pos[1] - tpperspective->pos[1], pos[2] - tpperspective->pos[2] };
    float s[3] = { -pos[0] + tpperspective->pos[0] , -pos[1] + tpperspective->pos[1], -pos[2] + tpperspective->pos[2] };

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

    if (key == 'S' || key == 's') {
        angle_y = (90 - tpperspective->eyeAngY);
        move();
        
        tpPos[0] += moveOffset * s[0];
        tpPos[2] += moveOffset * s[2];
    }
    else if (key == 'W' || key == 'w') {
        if (isOnWand) {
            angle_y = 90;
        }
        else {
            angle_y = 270 - tpperspective->eyeAngY;
            move();
        }
        tpPos[0] += moveOffset * w[0];
        tpPos[2] += moveOffset * w[2];
    }
    else if (key == 'A' || key == 'a') {
        if (isOnWand) {
            angle_y = 0;
        }
        else {
            angle_y = 360 - tpperspective->eyeAngY - 45;
            move();
        }
        tpPos[0] += moveOffset * a[0];
        tpPos[2] += moveOffset * a[2];
    }
    else if (key == 'D' || key == 'd') {
        if (isOnWand) {
            angle_y = 0;
        }
        else {
            angle_y = 180 - tpperspective->eyeAngY + 45;
            move();
        }
        tpPos[0] += moveOffset * d[0];
        tpPos[2] += moveOffset * d[2];
    }
    else if (key == 'r' || key == 'R') {            //轉圈圈
        angle_y += 5;  
        change_moveMode(ROBOT_TURN);
        move();    //在地板才要動腳
        
    }
    if (scene == SCENE_VENDOR && sceneVendor->detectCollision(tpPos[0], tpPos[1], tpPos[2],SV_ROBOT)) return;
    if (scene == SCENE_JUNGLE && sceneJungle->detectCollision(tpPos[0], tpPos[2],SJ_ROBOT)) return;
    for (int i = 0; i < 3; i++) pos[i] = tpPos[i];
    dir = {w[0],w[1],w[2]};
}

void MyRobot::addExp(int e)
{
    exp += e;
    if(exp >= level*10){
        exp -= level*10;
        level++;
        blood = MAX_BLOOD;
        ui->addMsg(LEVEL_UP);
    }
}
