#pragma once
#include<GL/glew.h>
#include<GL/glut.h>
#include"../glad/glad.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include"myobj.h"
#include"mytex.h"
class Elf {
public:
	Elf(float x,float y,float z);
	void draw(unsigned programID);
	int dir = 0; //0後  1右  2前  3左
	float angleY = 0;
	glm::vec3 pos = {0,0,0};
private:
};
