#pragma once
#include<GL/glew.h>
#include<GL/glut.h>
#include"../glad/glad.h"
#include"myobj.h"
#include"mytex.h"
class elf {
public:
	void draw(unsigned programID);
	int dir = 0; //0後  1右  2前  3左
	float angleY = 0;
private:
};
