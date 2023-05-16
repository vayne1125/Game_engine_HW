#pragma once
#include<bits/stdc++.h>
#include <vector>
#include<GL/glew.h>
#include<GL/glut.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include"../glad/glad.h"
using namespace std;

class mesh {
public:
	
	mesh(int programID,const vector<float>& vec);
	void draw(int programID);
private:
	unsigned int VAOID;
	int vertex_count = 0;
	glm::vec3 mc = {0,0,0};
	glm::mat3 I_inv{0}; 
};
