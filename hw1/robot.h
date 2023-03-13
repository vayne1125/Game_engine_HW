#pragma once
#include <vector>
#include<GL\freeglut.h>
#include"glad\glad.h"
#include"myobj.h"
#include "mytex.h"
using namespace std;
class foot {
public:
    void draw(int programID);
    float hipJointAng_x = 180, hipJointAng_y = 0, hipJointAng_z = 0;   //�b���`
    float kneeAng_x = 0, kneeAng_y = 0, kneeAng_z = 0;
    float ankle_x = 0, ankle_y = 0, ankle_z = 0;
    int subColor = 0;
private:
};
class hand {
public:
    void draw(int programID);
    float shoulderAng_x = 180, shoulderAng_y = 0, shoulderAng_z = 35;    //�ӻH
    float elbowAng_x = 0, elbowAng_y = 0, elbowAng_z = 0;                //��y
    float fingerAng_x = 0, fingerAng_y = 0, fingerAng_z = 0;             //���
    int subColor = 0;
private:
};
class robot {
public:
	robot(unsigned int programID);
	void sit();
	void draw(unsigned int programID);
    void stand();
private:
    int moveMode = 0; //���ʼҦ� 0->walk  1->run  2->turn  3->fly
    float angle_x = 0.0, angle_y = 0.0;                         //���ਤ��
    bool carry_mw = 0;                                          //�ثe�����]�k�ζ�
    bool isMagician = 0;                                        //�O���O�]�k�v
    bool isJump_ready = 0;                                      //�ǳƸ������ն�
    float x = 0, y = 0, z = 0;                                  //��������H�y��
    hand* right_h;
    hand* left_h;
    foot* left_f;
    foot* right_f;
};
