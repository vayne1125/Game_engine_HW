#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#include"../glad/glad.h"
#include "mesh.h"
#define YU_GRAPHICS_SPHERE  0
#define YU_GRAPHICS_CUBE    1
#define YU_GRAPHICS_CLOUD   2
using namespace std;
class GraphicObj {
public:
	mesh* cube, * solidsphere, * cylinder, * solidtorus_5_025, *fountain, *blacksmith, * fountain_base , * fountain_water,* vendor_base, * vendor_col,* vendor_roof;
	mesh* square, *vendor_desk, *tree_round_up, *tree_round_btn,* tree_conical_btn,* tree_conical_up, *cloud1, *vendor_crate, * apple, * cheese, *bread2, *plate, *sword, *bow, *lantern1, * lantern2, * lantern3;
	GraphicObj(unsigned int programID);		
	void drawByID(int graID,unsigned int programID);	
	const glm::mat3 &getIByID(int graID);	
	const vector<glm::vec3>& getVerticesByID(int graID);																  
	mesh* phy_sphere, *phy_cube, *phy_cloud;
private:																												  
	mesh* getMesh(string fname,unsigned int programID,bool isphy);
};
