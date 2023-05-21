#pragma once
#include <bits/stdc++.h>
#include<GL/glew.h>
#include<GL/glut.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include"../glad/glad.h"
using namespace std;
class mesh {
public:
mesh();
	mesh(unsigned int programID,vector<float>& vec,bool phy);
	void draw(int programID);
	const glm::mat3 &getI(){return I;}
private:
	unsigned int VAOID;
	int vertex_count = 0;
	glm::vec3 mc = {0,0,0};
	glm::mat3 I{0};
	vector<glm::vec3> vertices;
};
