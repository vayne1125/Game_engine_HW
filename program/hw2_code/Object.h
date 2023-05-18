#pragma once
#include <bits/stdc++.h>
#include "Phyobj.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
class Object{
public:
    Phyobj* phyobj;
    Object(std::string GraphicObjName,std::string PhyObjName,float r,float m,float k);
    //Object(std::string GraphicObjName,std::string PhyObjName,float r,float m);
    //Object(std::string GraphicObjName,std::string PhyObjName,const glm::vec3& sz,float m,float k);
    //Object(std::string GraphicObjName,std::string PhyObjName,const glm::vec3& sz,float m);
};
