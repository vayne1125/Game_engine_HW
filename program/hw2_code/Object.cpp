#include "Object.h"
extern GraphicObj* graphicObj;
extern mytex* myTex;
Object::Object(int GraphicObjID, int PhyObjID, int textureID,float r, float m, float k):GraphicObjID(GraphicObjID),PhyObjID(PhyObjID),textureID(textureID)
{
    if(PhyObjID == YU_PHYSICS_SPHERE){
        phyObj = new Sphere(r,m,k);
        sz = {r,r,r};
    }
}

void Object::draw(unsigned int programID)
{
    glm::mat4 tp;
    tp = glm::translate(glm::mat4(1), phyObj->pos) * glm::toMat4(phyObj->rot) * glm::scale(glm::mat4(1), sz);
    glUniformMatrix4fv(2, 1, GL_FALSE, &tp[0][0]);

    //std::cout << phyObj->pos[0] << "\n" << phyObj->rot[0] << "\n" <<sz[0] << "\n";

    myTex->useByID(textureID,programID);
    graphicObj->drawByID(GraphicObjID,programID);  

    init();
}

void Object::setPos(float x, float y, float z)
{
    phyObj->pos = {x,y,z};
}

void Object::init()
{
    phyObj -> lin_a = {0, 0, 0};
    phyObj -> rot_a = {0, 0, 0};
}

void Object::update(float dt)
{
    phyObj->update(dt);
}

Object::Object(int GraphicObjID, int PhyObjID, int textureID, float r, float m):GraphicObjID(GraphicObjID),PhyObjID(PhyObjID),textureID(textureID)
{
    if(PhyObjID == YU_PHYSICS_SPHERE){
        //cout << "in\n";
        phyObj = new Sphere(r,m);
        sz = {r,r,r};
    }
}

Object::Object(int GraphicObjID, int PhyObjID, int textureID, const glm::vec3 &sz_, float m, float k):GraphicObjID(GraphicObjID),PhyObjID(PhyObjID),textureID(textureID)
{
    if(PhyObjID == YU_PHYSICS_CUBE){
        //cout << "in\n";
        sz = sz_;
        phyObj = new Cube(sz,m,k);
    }
}
Object::Object(int GraphicObjID, int PhyObjID, int textureID, const glm::vec3 &sz_, float m):GraphicObjID(GraphicObjID),PhyObjID(PhyObjID),textureID(textureID)
{
    if(PhyObjID == YU_PHYSICS_CUBE){
        //cout << "in\n";
        sz = sz_;
        phyObj = new Cube(sz,m);
    }
}