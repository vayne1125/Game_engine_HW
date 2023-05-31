#include "UIPhy.h"
std::string UIPhy::fixed2(float val)
{
    // std::cout << val << "\n";
    //-0.05
    //-5
    string rt ="";
    int tp = val*100;
    if(tp<0) rt += '-';
    tp = abs(tp);
    rt += std::to_string(tp / 100) + '.' + std::to_string(tp % 100);
    return rt;
}
void UIPhy::changeInfo(const Object &object, float pos[3], float dir[3], float F)
{
    /*
    --object info--
    ID:
    pos:
    weight:
    velocity:
    angular velocity:
    drag force (0), press Z to switch,
    gravity (0), press X to switch
    */
    vecs[0] = modelForObject[0];
    vecs[1] = modelForObject[1] + object.name;
    // std::cout << vecs[1] << "\n";
    vecs[2] = modelForObject[2] + '(' + fixed2(object.phyObj->pos[0]) + ", " + fixed2(object.phyObj->pos[1]) + ", " + fixed2(object.phyObj->pos[2]) + ')';
    vecs[3] = modelForObject[3] + fixed2(object.phyObj->m) + modelForObject[4];
    vecs[4] = modelForObject[5] + '(' + fixed2(object.phyObj->v[0]) + ", " + fixed2(object.phyObj->v[1]) + ", " + fixed2(object.phyObj->v[2]) + ')' + modelForObject[6];
    //vecs[5] = modelForObject[7] + '(' + std::to_string(object.phyObj->w[0]) + ", " + std::to_string(object.phyObj->w[1]) + ", " + std::to_string(object.phyObj->w[2]) + ')' + modelForObject[8];
    vecs[5] = modelForObject[9] + std::to_string(object.phyObj->isOpenDragForce) + modelForObject[10];
    vecs[6] = modelForObject[11] + std::to_string(object.phyObj->isOpenGravity) + modelForObject[12];

    /*
    --bullet info--,
    pos:
    dir:
    force: 100 N, press +/- to change
    */
    vecs2[0] = modelForBullet[0];
    vecs2[1] = modelForBullet[1] + '(' + fixed2(pos[0]) + ", " + fixed2(pos[1]) + ", " + fixed2(pos[2]) + ')';
    
    glm::vec3 dir_ = {dir[0],dir[1],dir[2]};
    dir_ = glm::normalize(dir_);
    
    vecs2[2] = modelForBullet[2] + '(' + fixed2(dir_[0]) + ", " + fixed2(dir_[1]) + ", " + fixed2(dir_[2]) + ')';
    vecs2[3] = modelForBullet[3] + std::to_string(int(F)) + modelForBullet[4];
    // for(std::string s:vecs){
    //     std::cout << s << "\n";
    // }
}
void UIPhy::draw()
{
    glDisable(GL_DEPTH_TEST);
    float f = 0.1 / aspect;
    glColor3f(1, 1, 1);
    glDisable(GL_DEPTH_TEST);
    glUseProgram(0);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(-f / 2.0, 0);
    glVertex2f(f / 2.0, 0);
    glVertex2f(0, -0.05);
    glVertex2f(0, 0.05);
    glEnd();
}
void UIPhy::draw(const Object &object, float pos[3], float dir[3], float F)
{
    changeInfo(object, pos, dir, F);
    // std::cout << "kk\n";
    glDisable(GL_DEPTH_TEST);
    float f = 0.1 / aspect;
    glColor3f(1, 1, 1);
    glUseProgram(0);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(-f / 2.0, 0);
    glVertex2f(f / 2.0, 0);
    glVertex2f(0, -0.05);
    glVertex2f(0, 0.05);
    glEnd();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(208 / 255.0, 159 / 255.0, 159 / 255.0, 0.5);
    glBegin(GL_QUADS);
    glVertex2f(0.95, -0.55);
    glVertex2f(0.5, -0.55);
    glVertex2f(0.5, 0.95);
    glVertex2f(0.95, 0.95);
    glEnd();
    glDisable(GL_BLEND);

    glColor3f(0, 0, 0);

    std::string s;
    for (int i = 0; i < vecs.size(); i++)
    {
        s = vecs[i];
        glRasterPos2f(0.52, 0.88 - i * 0.1);
        for (int j = 0; j < s.size(); j++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, int(s[j]));
        }
    }

    for (int i = 0; i < vecs2.size(); i++)
    {
        s = vecs2[i];
        glRasterPos2f(0.52, 0.14 - i * 0.1);
        for (int j = 0; j < s.size(); j++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, int(s[j]));
        }
    }

    for (int i = 0; i < modelForOthers.size(); i++)
    {
        s = modelForOthers[i];
        glRasterPos2f(0.52, -0.3 - i * 0.1);
        for (int j = 0; j < s.size(); j++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, int(s[j]));
        }
    }

}
