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

void Object::applyForce(const glm::vec3 &F,const glm::vec3 &impactPoint)
{
    
    // glm::vec3 F{0, 60, 0};
    // glm::vec3 impactPoint{0.001, -1, 0};  //在地座標系
    // glm::vec3 J = cross(impactPoint, F);

    // cube -> phyObj -> applyLinearForce(dot(F, impactPoint) * impactPoint / dot(impactPoint, impactPoint));
    // cube -> phyObj -> applyRotJ(J);
    
    glm::vec3 J = cross(impactPoint, F/100.0f);

    phyObj -> applyLinearForce(dot(F, impactPoint) * impactPoint / dot(impactPoint, impactPoint));
    phyObj -> applyRotJ(J);
}
bool Object::isChoose(const glm::vec3& start, const glm::vec3& dir){
    //因為是正交矩陣，所以 inverse = transpose
    glm::mat4 tp = glm::translate(glm::mat4(1),phyObj->pos)*glm::toMat4(phyObj->rot)*glm::scale(glm::mat4(1),sz);

    float min_pnt = 1e9;
    const vector<glm::vec3>& tpvec = graphicObj->getVerticesByID(GraphicObjID);
    for(int i = 0; i < tpvec.size(); i += 3){
        glm::vec3 a = tp*glm::vec4{tpvec[i],1};
        glm::vec3 b = tp*glm::vec4{tpvec[i+1],1};
        glm::vec3 c = tp*glm::vec4{tpvec[i+2],1};
        glm::vec3 U = b - a;
        glm::vec3 V = c - a;
        float det_mtx = dot(V, cross(U, - dir));
        float s = dot(start - a, cross(U, -dir)) / det_mtx;
        float t = dot(V, cross(start - a, -dir)) / det_mtx;
        float r = dot(V, cross(U, start - a)) / det_mtx;

        if(s >= 0 && t >= 0 && s + t <= 1 && r >= 0){
            return 1;
        }
    }
    return 0;
}
void Object::shoot(const float& F_,const glm::vec3& start, const glm::vec3& dir)
{
    //因為是正交矩陣，所以 inverse = transpose
    glm::mat4 tp = glm::translate(glm::mat4(1),phyObj->pos)*glm::toMat4(phyObj->rot)*glm::scale(glm::mat4(1),sz);

    float min_pnt = 1e9;
    const vector<glm::vec3>& tpvec = graphicObj->getVerticesByID(GraphicObjID);
    for(int i = 0; i < tpvec.size(); i += 3){
        glm::vec3 a = tp*glm::vec4{tpvec[i],1};
        glm::vec3 b = tp*glm::vec4{tpvec[i+1],1};
        glm::vec3 c = tp*glm::vec4{tpvec[i+2],1};
        glm::vec3 U = b - a;
        glm::vec3 V = c - a;
        float det_mtx = dot(V, cross(U, - dir));
        float s = dot(start - a, cross(U, -dir)) / det_mtx;
        float t = dot(V, cross(start - a, -dir)) / det_mtx;
        float r = dot(V, cross(U, start - a)) / det_mtx;

        if(s >= 0 && t >= 0 && s + t <= 1 && r >= 0){
            min_pnt = fmin(min_pnt, r);
        }
    }
    if(min_pnt != 1e9){
        glm::vec3 impactPoint = (start + dir * min_pnt)- phyObj->pos;
        //cout << impactPoint.x  << " " << impactPoint.y << " " << impactPoint.z << "\n";
        glm::vec3 F = dir * F_;
        applyForce(F,impactPoint);
    }
}

void Object::setName(const string &n_)
{
    name = n_;
}
void Object::switchDragforce()
{
    phyObj->isOpenDragForce ^= 1;
}
void Object::switchGravity()
{
    if(PhyObjID == YU_PHYSICS_IRREGULAR) return;   //不規則物體沒有重力
    phyObj->isOpenGravity ^= 1;
}
void Object::reset()
{
    phyObj->v = {0,0,0};
    phyObj->w = {0,0,0};
    phyObj->rot = {1,0,0,0};
}
void Object::reset(float x, float y, float z)
{
    phyObj->pos = {x,y,z};
    phyObj->v = {0,0,0};
    phyObj->w = {0,0,0};
    phyObj->rot = {1,0,0,0};
}
Object::Object(int GraphicObjID, int PhyObjID, int textureID, float r, float m) : GraphicObjID(GraphicObjID), PhyObjID(PhyObjID), textureID(textureID)
{
    if(PhyObjID == YU_PHYSICS_SPHERE){
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
    sz = sz_;
    switch(PhyObjID){
        case YU_PHYSICS_CUBE:
            phyObj = new Cube(sz,m);
            break;
        case YU_PHYSICS_IRREGULAR:
            phyObj = new Irregular(m);
            phyObj->I_inv = glm::inverse(graphicObj->getIByID(GraphicObjID));
            //cout << "come2\n";
            //cout << phyObj->I_inv[0][0] << "\n";
            break;
        default:
            cout << "Object constructor(int GraphicObjID, int PhyObjID, int textureID, const glm::vec3 &sz_, float m) is wrong!!\n";
    }

}
Object::Object(int GraphicObjID, int PhyObjID, int textureID, float m):GraphicObjID(GraphicObjID),PhyObjID(PhyObjID),textureID(textureID)
{
    switch(PhyObjID){
        case YU_PHYSICS_IRREGULAR:
            phyObj = new Irregular(m);
            phyObj->isOpenGravity = 0;
            phyObj->I_inv = glm::inverse(graphicObj->getIByID(GraphicObjID));
            break;
        default:
            cout << "Object constructor(int GraphicObjID, int PhyObjID, int textureID, float m) is wrong!!\n";
    }

}
Object::Object(){
    phyObj = new PhyObj(0);
};