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
    int moveMode = 0; //���ʼҦ� 0->walk  1->run  2->turn  3->fly
    float angle_x = 0.0, angle_y = 0.0;                         //���ਤ��
    bool carry_mw = 0;                                          //�ثe�����]�k�ζ�
    bool isMagician = 0;                                        //�O���O�]�k�v
    bool isJump_ready = 0;                                      //�ǳƸ������ն�
    float x = 0, y = 0, z = 0;                                  //��������H�y��
	//unsigned int VAOID;
	//int vertex_count = 0;
};
