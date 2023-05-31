#include "PhyObj.h"
PhyObj::PhyObj(float _m, float _k): m(_m), k(_k){
}
PhyObj::PhyObj(float _m): m(_m), k(0.5){
}
void PhyObj::applyLinearForce(const glm::vec3 &F)
{
    lin_a += F / m;
}

void PhyObj::applyRotJ(const glm::vec3 &J)
{
    // J ＝ I x w = r x F
    // 求 I , F
    glm::mat3 Rot = toMat3(rot);
    glm::mat3 Rot_inv = transpose(Rot);
    rot_a += Rot_inv * I_inv * Rot * J;
    //std::cout << rot_a[1] << "\n";
}

void PhyObj::update(float dt)
{
    //給空氣阻力 （平移 ＋ 旋轉）
    if(isOpenDragForce){
        glm::vec3 dragforce = -v * k;
        applyLinearForce(dragforce);
        w *= 0.995;
    }
    v += lin_a * dt;
    w += rot_a * dt;

    pos += (v * dt);
    if (glm::length(w) != 0)
        rot = glm::angleAxis(glm::length(w), glm::normalize(w)) * rot;

}
Sphere::Sphere(float r, float _m, float _k) : rad(r), PhyObj(_m, _k){
    I_inv = glm::mat3{1 / (2.0 / 5 * m * rad * rad)};
}
Sphere::Sphere(float r, float _m) : rad(r), PhyObj(_m)
{
    I_inv = glm::mat3{1 / (2.0 / 5 * m * rad * rad)};
}
void Sphere::update(float dt)
{
    //給重力
    if(isOpenGravity) applyLinearForce({0, -9.8 * m, 0});
    // collision
    if (pos[1]<= (0 + 0.00001) + rad/2.0)
    {
        v[1] = -v[1] * 0.6;
        if (fabs(v[1]) < 5)
            v[1] = 0;
        pos[1] = rad/2.0;
    }
    PhyObj::update(dt);
}
Cube :: Cube(const glm::vec3 &_sz, float _m, float _k) : sz(_sz), PhyObj(_m, _k)
{
    I_inv = glm::mat3{
        12 / (m * (sz.y * sz.y + sz.z * sz.z)), 0, 0,
        0, 12 / (m * (sz.x * sz.x + sz.z * sz.z)), 0,
        0, 0, 12 / (m * (sz.x * sz.x + sz.y * sz.y))};

    corner[0] = {sz.x / 2, sz.y / 2, sz.z / 2};
    corner[1] = {-sz.x / 2, sz.y / 2, sz.z / 2};
    corner[2] = {sz.x / 2, -sz.y / 2, sz.z / 2};
    corner[3] = {-sz.x / 2, -sz.y / 2, sz.z / 2};
    corner[4] = {sz.x / 2, sz.y / 2, -sz.z / 2};
    corner[5] = {-sz.x / 2, sz.y / 2, -sz.z / 2};
    corner[6] = {sz.x / 2, -sz.y / 2, -sz.z / 2};
    corner[7] = {-sz.x / 2, -sz.y / 2, -sz.z / 2};
}
Cube::Cube(const glm::vec3 &_sz, float _m) : sz(_sz), PhyObj(_m)
{
    I_inv = glm::mat3{
        12 / (m * (sz.y * sz.y + sz.z * sz.z)), 0, 0,
        0, 12 / (m * (sz.x * sz.x + sz.z * sz.z)), 0,
        0, 0, 12 / (m * (sz.x * sz.x + sz.y * sz.y))};

    corner[0] = {sz.x / 2, sz.y / 2, sz.z / 2};
    corner[1] = {-sz.x / 2, sz.y / 2, sz.z / 2};
    corner[2] = {sz.x / 2, -sz.y / 2, sz.z / 2};
    corner[3] = {-sz.x / 2, -sz.y / 2, sz.z / 2};
    corner[4] = {sz.x / 2, sz.y / 2, -sz.z / 2};
    corner[5] = {-sz.x / 2, sz.y / 2, -sz.z / 2};
    corner[6] = {sz.x / 2, -sz.y / 2, -sz.z / 2};
    corner[7] = {-sz.x / 2, -sz.y / 2, -sz.z / 2};
}
void Cube :: update(float dt){
    // collision
    // v.y-=9.8*dt;
    if(isOpenGravity)applyLinearForce({0, -9.8 * m, 0});
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
            glm::vec3 r = mny - pos;
            glm::vec3 J = glm::cross(r, {0, 0.08 * m, 0});
            applyRotJ(J);
        }
    }
    PhyObj::update(dt);
    if (flag)
    {
        v.y = fmax(v.y, 0.0);
    }
}
Irregular :: Irregular(float _m):PhyObj(_m){
    std::cout << "irr con\n";
}