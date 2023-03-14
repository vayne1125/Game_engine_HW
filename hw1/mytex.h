#pragma once
#include<iostream>
#include <vector>
#include <string>
#include <GL\freeglut.h>
#include"glad\glad.h"
#include"mesh.h"
using namespace std;
class texture{
public:
	void use(unsigned int programID);
	void use(unsigned int programID,float sper, float speg, float speb, float shine);
	texture(const string& fname, unsigned int programID);
	texture(int r,int g,int b,int a, unsigned int programID);
private:
	unsigned int textureID;
};
class mytex {
public:
	mytex(unsigned int programID);
	texture* red, *robot_blue_main,* robot_blue_sub, *black, *white,*robot_blue_eye, *robot_pink_eye, * robot_gray, *robot_gray_dark, * magic_wand_wood;
	//vector<texture> eevee;
	texture* eevee[56];
private:
};

