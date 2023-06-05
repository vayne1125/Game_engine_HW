#pragma once
#include <vector>
#include<GL/glew.h>
#include<GL/glut.h>
#include"../glad/glad.h"
#include"GraphicObj.h"
#include"mytex.h"
#include "Elf.h"
#include "Billboard.h"
#include "Light.h"
#include "Robot.h"
#define ROBOT 0
#define SPOTLIGHT_ELF 1

#define ANIMATION_SPOTLIGHT_ELF 6300
#define ANIMATION_EEVEE 6301

class SceneVendor {
public:
	void animation(int t);  //0~60*1000 毫秒
	SceneVendor(int programID);
	void draw(float* eyeMtx,int programID);
	int eevee_ani = 0;
	bool detectCollision(float x,float y,float z,int ITEM);
	void useLight();
	void keyEvent(unsigned char key);
private:
	Elf* spotLightElf;
	Light* dirLight,*spotLight;
	float fountainPos[3] = { 116.0f,0.0f,124.0f }; 
	vector<vector<float>> cloud = { {100,40,96},{180,40,150},{55,40,170} };
	bool isDirLightOpen = 1;
	float getDis(float x1, float y1, float x2, float y2);	
	void elfMove();
	float floorX = 232;
	float floorY = 10;
	float floorZ = 203;
	Robot *chiefOfVillage,*fruitMerchant,*weaponDealer;  
};
