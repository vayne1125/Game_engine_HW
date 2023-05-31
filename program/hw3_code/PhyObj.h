#pragma once
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <bits/stdc++.h>
#define YU_PHYSICS_SPHERE    0
#define YU_PHYSICS_CUBE      1
#define YU_PHYSICS_IRREGULAR 2
class PhyObj
{
public:
    glm::mat3 I_inv;
    glm::quat rot{1, 0, 0, 0};
    glm::vec3 v{0, 0, 0};
    glm::vec3 w{0, 0, 0};
    glm::vec3 pos{0,0,0};
    glm::vec3 lin_a{0,0,0};
    glm::vec3 rot_a{0,0,0};
    float m, k;
    PhyObj(float _m, float _k);
    PhyObj(float _m);
    void applyLinearForce(const glm::vec3 &F);
    void applyRotJ(const glm::vec3 &J);
    virtual void update(float dt);
    bool isOpenDragForce = 1;
    bool isOpenGravity = 1;
};
class Sphere : public PhyObj
{
public:
    float rad;
    Sphere(float r, float _m, float _k);
    Sphere(float r, float _m);
    void update(float dt) override;

private:
} ;

class Cube : public PhyObj
{
public:
    glm::vec3 sz;
    std::array<glm::vec3, 8> corner;
    Cube(const glm::vec3 &_sz, float _m, float _k);
    Cube(const glm::vec3 &_sz, float _m);
    static bool cmp(glm::vec3 a, glm::vec3 b)
    {
        return a.y < b.y;
    }
    void update(float dt) override;

private:
};

class Irregular : public PhyObj{
public:    
    Irregular(float _m);
};