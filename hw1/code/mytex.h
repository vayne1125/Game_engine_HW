#pragma once
#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#include "glad/glad.h"
#include "mesh.h"
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
	texture* red, *robot_blue_main,* robot_blue_sub, *black, *white,*robot_blue_eye, *robot_pink_eye, * robot_gray, *robot_gray_dark, * magic_wand_wood, *blacksmith;
	//vector<texture> eevee;
	texture* grass_dark, * flower, * grass_light,* coffee_dark,* coffee_light;
	texture* eevee[56],*a,* fountain_base,* fountain_water,* elf_red;
	texture* yellow_light, * yellow_dark, * orange_dark, * orange_light,*red_dark,*orgred_dark,*orgred_light, *stone_floor;
	texture* wood2, *wood3, *wood4, * wood5, *tablecloth, *tent, *pear, *bread, *cheese, *red_apple, *green_apple, *lemon;
private:
};


