#pragma once
#include <bits/stdc++.h>
#include "PhyObj.h"
#include "GraphicObj.h"
#include "mytex.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>

class Object
{
public:
    PhyObj *phyObj;
    Object(int GraphicObjID, int PhyObjID, int textureID,float r, float m, float k);
    void draw(unsigned int programID);
    void setPos(float x,float y,float z);
    void init();
    void update(float dt);
    Object(int GraphicObjID, int PhyObjID, int textureID,float r,float m);
    Object(int GraphicObjID, int PhyObjID, int textureID,const glm::vec3& sz,float m,float k);
    Object(int GraphicObjID, int PhyObjID, int textureID,const glm::vec3& sz,float m);
private:
    glm::vec3 sz;
    int GraphicObjID,PhyObjID,textureID;
};
