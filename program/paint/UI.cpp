#include "UI.h"
std::string UI::fixed2(float val)
{
    // std::cout << val << "\n";
    //-0.05
    //-5
    std::string rt ="";
    int tp = val*100;
    if(tp<0) rt += '-';
    tp = abs(tp);
    rt += std::to_string(tp / 100) + '.' + std::to_string(tp % 100);
    return rt;
}
void UI::changeInfo(const Object &object, const std::vector<float> &pos, const std::vector<float> &dir, float F)
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
    vecs[1] = modelForObject[1] + object.id;
    // std::cout << vecs[1] << "\n";
    vecs[2] = modelForObject[2] + '(' + fixed2(object.pos[0]) + ", " + fixed2(object.pos[1]) + ", " + fixed2(object.pos[2]) + ')';
    vecs[3] = modelForObject[3] + fixed2(object.m) + modelForObject[4];
    vecs[4] = modelForObject[5] + '(' + fixed2(object.v[0]) + ", " + fixed2(object.v[1]) + ", " + fixed2(object.v[2]) + ')' + modelForObject[6];
    vecs[5] = modelForObject[7] + '(' + fixed2(object.w[0]) + ", " + fixed2(object.w[1]) + ", " + fixed2(object.w[2]) + ')' + modelForObject[8];
    vecs[6] = modelForObject[9] + std::to_string(object.isOpenDragForce) + modelForObject[10];
    vecs[7] = modelForObject[11] + std::to_string(object.isOpenGravity) + modelForObject[12];

    /*
    --bullet info--,
    pos:
    dir:
    force: 100 N, press +/- to change
    */
    vecs2[0] = modelForBullet[0];
    vecs2[1] = modelForBullet[1] + '(' + fixed2(pos[0]) + ", " + fixed2(pos[1]) + ", " + fixed2(pos[2]) + ')';
    vecs2[2] = modelForBullet[2] + '(' + fixed2(object.pos[0]) + ", " + fixed2(object.pos[1]) + ", " + fixed2(object.pos[2]) + ')';
    vecs2[3] = modelForBullet[3] + std::to_string(int(F)) + modelForBullet[4];
    // for(std::string s:vecs){
    //     std::cout << s << "\n";
    // }
}
void UI::draw(const Object &object, const std::vector<float> &pos, const std::vector<float> &dir, float F)
{
    changeInfo(object, pos, dir, F);
    // std::cout << "kk\n";
    glDisable(GL_DEPTH_TEST);
    //glClearColor(0.70, 0.70, 0.70, 1.0); // Dark grey background
    //glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    int windowWidth = 1920;
    int windowHeight = 990;
    float f = 0.1 / (windowWidth / (double)windowHeight);
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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(208 / 255.0, 159 / 255.0, 159 / 255.0, 0.5);
    glBegin(GL_QUADS);
    glVertex2f(0.95, -0.3);
    glVertex2f(0.5, -0.3);
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
        glRasterPos2f(0.52, 0.04 - i * 0.1);
        for (int j = 0; j < s.size(); j++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, int(s[j]));
        }
    }
}