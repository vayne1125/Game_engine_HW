#pragma once
#include <vector>
#include<GL\freeglut.h>
#include"glad\glad.h"
#include"myobj.h"
using namespace std;
class robot {
public:
	robot(unsigned int programID);
	void sit();
	void draw(unsigned int programID);
private:
    int moveMode = 0; //移動模式 0->walk  1->run  2->turn  3->fly
    float angle_x = 0.0, angle_y = 0.0;                         //旋轉角度
    bool carry_mw = 0;                                          //目前有拿魔法棒嗎
    bool isMagician = 0;                                        //是不是魔法師
    bool isJump_ready = 0;                                      //準備跳的姿勢嗎
    float x = 0, y = 0, z = 0;                                  //控制機器人座標
	//unsigned int VAOID;
	//int vertex_count = 0;
};
