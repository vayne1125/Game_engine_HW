#pragma once
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <bits/stdc++.h>
class Phyobj
{
public:
    glm::mat3 I_inv;
    glm::quat rot{1, 0, 0, 0};
    glm::vec3 v{0, 0, 0};
    glm::vec3 w{0, 0, 0};
    glm::vec3 pos;
    glm::vec3 lin_a;
    glm::vec3 rot_a;
    float m, k;
    Phyobj(float _m, float _k);
    Phyobj(float _m);
    void applyLinearForce(const glm::vec3 &F);
    void applyRotJ(const glm::vec3 &J);
    virtual void update(float dt);
};
class Sphere : public Phyobj
{
public:
    float rad;
    Sphere(float r, float _m, float _k);
    Sphere(float r, float _m);
    void update(float dt) override;

private:
} ;
//sphere(2, 1, 0.4);
class Cube : public Phyobj
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
//cube({1, 2, 0.5}, 1, 0.2);