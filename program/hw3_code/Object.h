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
    string name = "object";
    PhyObj *phyObj;
    Object(int GraphicObjID, int PhyObjID, int textureID,float r, float m, float k);
    void draw(unsigned int programID);
    void setPos(float x,float y,float z);
    void init();
    void update(float dt);
    void applyForce(const glm::vec3 &F,const glm::vec3 &impactPoint);
    void shoot(const float& F_,const glm::vec3& start, const glm::vec3& dir);
    bool isChoose(const glm::vec3& start, const glm::vec3& dir);
    void setName(const string& n_);
    void switchDragforce();
    void switchGravity();
    void stopMove();
    void reset();
    Object();
    Object(int GraphicObjID, int PhyObjID, int textureID,float r,float m);
    Object(int GraphicObjID, int PhyObjID, int textureID,const glm::vec3& sz,float m,float k);
    Object(int GraphicObjID, int PhyObjID, int textureID,const glm::vec3& sz,float m);
    Object(int GraphicObjID, int PhyObjID, int textureID,float m);
private:
    glm::vec3 initpos;
    glm::vec3 sz = {1,1,1};
    int GraphicObjID,PhyObjID,textureID;
};
