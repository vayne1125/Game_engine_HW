#include "Phyobj.h"
Phyobj::Phyobj(float _m, float _k): m(_m), k(_k){
}
Phyobj::Phyobj(float _m): m(_m), k(0.5){
}
void Phyobj::applyLinearForce(const glm::vec3 &F)
{
    lin_a += F / m;
}

void Phyobj::applyRotJ(const glm::vec3 &J)
{
    // J ＝ I x w = r x F
    // 求 I , F
    glm::mat3 Rot = toMat3(rot);
    glm::mat3 Rot_inv = transpose(Rot);
    rot_a += Rot_inv * I_inv * Rot * J;
}

void Phyobj::update(float dt)
{
    // if (pos[1] > 0)
    //     applyLinearForce({0, -9.8 * m, 0});
    glm::vec3 dragforce = -v * k;
    applyLinearForce(dragforce);
    w *= 0.995;

    v += lin_a * dt;
    w += rot_a * dt;

    pos += (v * dt);
    if (glm::length(w) != 0)
        rot = glm::angleAxis(glm::length(w), glm::normalize(w)) * rot;
}
Sphere::Sphere(float r, float _m, float _k) : rad(r), Phyobj(_m, _k){
    I_inv = glm::mat3{1 / (2.0 / 5 * m * rad * rad)};
}
Sphere::Sphere(float r, float _m) : rad(r), Phyobj(_m)
{
    I_inv = glm::mat3{1 / (2.0 / 5 * m * rad * rad)};
}
void Sphere::update(float dt){
    applyLinearForce({0, -9.8 * m, 0});
    // collision
    if (pos[1] <= 0 - 0.00001)
    {
        v[1] = -v[1] * k;
        if (fabs(v[1]) < 5)
            v[1] = 0;
        pos[1] = 0;
    }
    Phyobj::update(dt);
}