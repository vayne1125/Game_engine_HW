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
} sphere(2, 1, 0.4);
class Cube : public Phyobj
{
public:
    glm::vec3 sz;
    std::array<glm::vec3, 8> corner;
    Cube(const glm::vec3 &_sz, float _m, float _k) : sz(_sz), Phyobj(_m, _k)
    {
        I_inv = glm::mat3{
            12 / (m * (sz.y * sz.y + sz.z * sz.z)), 0, 0,
            0, 12 / (m * (sz.x * sz.x + sz.z * sz.z)), 0,
            0, 0, 12 / (m * (sz.x * sz.x + sz.y * sz.y))};
        // for(int i = 0; i < 8; ++i)
        //     corner[i] = {(i % 2? -1: 1) * sz.x / 2, (i / 2 % 2? -1: 1) * sz.y /2, (i % 4? -1: 1) * sz.z /2};

        corner[0] = {sz.x / 2, sz.y / 2, sz.z / 2};
        corner[1] = {-sz.x / 2, sz.y / 2, sz.z / 2};
        corner[2] = {sz.x / 2, -sz.y / 2, sz.z / 2};
        corner[3] = {-sz.x / 2, -sz.y / 2, sz.z / 2};
        corner[4] = {sz.x / 2, sz.y / 2, -sz.z / 2};
        corner[5] = {-sz.x / 2, sz.y / 2, -sz.z / 2};
        corner[6] = {sz.x / 2, -sz.y / 2, -sz.z / 2};
        corner[7] = {-sz.x / 2, -sz.y / 2, -sz.z / 2};
    }
    Cube(const glm::vec3 &_sz, float _m) : sz(_sz), Phyobj(_m)
    {
        I_inv = glm::mat3{
            12 / (m * (sz.y * sz.y + sz.z * sz.z)), 0, 0,
            0, 12 / (m * (sz.x * sz.x + sz.z * sz.z)), 0,
            0, 0, 12 / (m * (sz.x * sz.x + sz.y * sz.y))};
    }
    static bool cmp(glm::vec3 a, glm::vec3 b)
    {
        return a.y < b.y;
    }
    void update(float dt) override
    {
        // collision
        // v.y-=9.8*dt;
        applyLinearForce({0, -9.8 * m, 0});
        glm::vec3 mny = {1e9, 1e9, 1e9};
        std::array<glm::vec3, 8> tmp;
        for (int i = 0; i < 8; ++i)
        {
            tmp[i] = toMat3(rot) * corner[i] + pos;
        }
        std::sort(tmp.begin(), tmp.end(), cmp);
        bool flag = false, flag2 = false;

        mny = tmp[0];
        if (mny.y < 0)
        {
            float kk = 21.5;
            glm::vec3 r = mny - pos;
            glm::vec3 cf = {0, -mny.y * kk, 0};
            applyLinearForce(glm::dot(cf, r) * r / glm::dot(r, r));
            flag = true;
            w *= 0.99;
            pos.y -= mny.y;
        }

        if (fabs(tmp[0].y - tmp[2].y) <= 0.15 && fabs(tmp[0].y) <= 0.1)
            flag2 = true;
        for (int i = 0; i < 8 && !flag2; ++i)
        {
            mny = tmp[i];
            if (mny.y <= 0)
            {

                // cout << "\ncf: " << cf.x << " " << cf.y << " " << cf.z << "\n";
                glm::vec3 r = mny - pos;
                glm::vec3 J = glm::cross(r, {0, 0.08 * m, 0});
                // cout << "\nJ: " << J.x << " " << J.y << " " << J.z << "\n";
                applyRotJ(J);
                // cout << "\nmny: " << mny.x << " " << mny.y << " " << mny.z << "\n";
            }
        }

        Phyobj::update(dt);
        if (flag)
        {
            v.y = fmax(v.y, 0.0);
        }
        std::cout << "\npos: " << pos.x << " " << pos.y << " " << pos.z << "\n";
        std::cout << "\nv: " << v.x << " " << v.y << " " << v.z << "\n";
    }

private:
} cube({1, 2, 0.5}, 1, 0.2);