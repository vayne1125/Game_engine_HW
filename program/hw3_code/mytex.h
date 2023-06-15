#pragma once
#include <bits/stdc++.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include"../glad/glad.h"
#include "mesh.h"
#define YU_RED    0
#define YU_CHEESE 1
#define YU_SLIME_CUBE 2
#define YU_SLIME_SPHERE_PHY 3
#define YU_SLIME_FIRE 4
#define YU_SLIME_WATER 5
#define YU_SLIME_LIGHT 6
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
	void useByID(int texID,unsigned int programID);
	
	texture* red, *robot_blue_main,* robot_blue_sub, *black, *white,*robot_blue_eye, *robot_pink_eye, * robot_gray, *robot_gray_dark, * magic_wand_wood;
	//vector<texture> eevee;
	texture* grass_dark, * flower, * grass_light,* coffee_dark,* coffee_light;
	texture* eevee[56],* fountain_base,* fountain_water,* elf_red;
	texture* yellow_light, * yellow_dark, * orange_dark, * orange_light,*red_dark,*orgred_dark,*orgred_light, *stone_floor;
	texture* wood, * wood2, *wood3, *wood4, * wood5, *tablecloth, *tent, *pear, *bread, *cheese, *red_apple, *green_apple, *lemon, *silver, * blue_fabric, * green_fabric, * red_lantern, * red_lantern2;
	texture* slime_cube,*slime_sphere_phy;
	texture* slime_water,*slime_light,*slime_fire,*magic_circle;
	texture* orange_dark_robot, * orange_light_robot,*yellow_light_robot,*yellow_dark_robot;
private:
};


