#pragma once
#include <bits/stdc++.h>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include "../glad/glad.h"
#include "GraphicObj.h"
#include "mytex.h"
#include "magicwand.h"
//移動方式
#define ROBOT_WALK 0
#define ROBOT_RUN  1
#define ROBOT_TURN 2   //轉
#define ROBOT_FLY  3   //飛行
using namespace std;
class Foot {
public:
    void draw(int programID);
    float hipJointAng_x = 180, hipJointAng_y = 0, hipJointAng_z = 0;   //髖關節
    float kneeAng_x = 0, kneeAng_y = 0, kneeAng_z = 0;
    float ankle_x = 0, ankle_y = 0, ankle_z = 0;
    void setColor(texture* m, texture* s);
private:
    texture* main;
    texture* sub;
};
class Hand {
public:
    void draw(int programID);
    float shoulderAng_x = 180, shoulderAng_y = 0, shoulderAng_z = 35;    //肩膀
    float elbowAng_x = 0, elbowAng_y = 0, elbowAng_z = 0;                //手肘
    float fingerAng_x = 0, fingerAng_y = 0, fingerAng_z = 0;             //手指
    void setColor(texture* m, texture* s);
private:
    texture* main;
    texture* sub;
};
class Robot {
public:
	Robot(unsigned int programID,float pos_x,float pos_y,float pos_z);
    void change_moveMode(int mode);
	void sit();
	void draw(unsigned int programID);
    void stand();
    void move();
    void jump_ready();
    bool jump();
    bool jumpOnWand();
    bool jumpToFloor();
    bool isSitOnChair = 0;
    float angle_x = 0.0, angle_y = 0.0;                         //旋轉角度
    bool carry_mw = 0;                                          //目前有拿魔法棒嗎
    bool isMagician = 0;                                        //是不是魔法師
    bool isJump_ready = 0;                                      //準備跳的姿勢嗎
    bool isOnWand = 0;                                          //是否坐在法杖上
    bool flag = 0, flag2 = 0;
    void setOffset(float walk, float run, float fly);
    int getMoveMode();
    float moveOffset = 0;
    void setColor(texture* m,texture* s);
                        
    float pos[3] = {0,0,0};        //控制機器人在世界座標系的座標
        
private:
    float x = 0, y = 0, z = 0;       //控制機器人座標                            
    Hand* right_h;
    Hand* left_h;
    Foot* left_f;
    Foot* right_f;
    int moveMode = ROBOT_RUN; //move model: 0->walk  1->run  2->turn  3->fly
    float runOffset = 0;
    float flyOffset = 0;
    float walkOffset = 0;
    texture* main;
    texture* sub;
};
