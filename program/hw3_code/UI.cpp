#include "UI.h"
void UI::draw()
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


    //機器人
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(208 / 255.0, 159 / 255.0, 159 / 255.0, 0.5);
    glBegin(GL_QUADS);
    glVertex2f(-0.98, -0.95);
    glVertex2f(-0.98, -0.6);
    glVertex2f(-0.6, -0.6);
    glVertex2f(-0.6, -0.95);
    glEnd();
    

    glColor4f(1,1,1,0.8);
    glBegin(GL_QUADS);
    glVertex2f(-0.96, -0.91);
    glVertex2f(-0.96, -0.64);
    glVertex2f(-0.84, -0.64);
    glVertex2f(-0.84, -0.91);
    glEnd();
    glDisable(GL_BLEND);

    glColor3f(0,0,0);
    std::string s = "A l l u k a";
    glRasterPos2f(-0.83, -0.69);
    for (int j = 0; j < s.size(); j++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, int(s[j]));
    }

    s = "blood";
    glRasterPos2f(-0.83, -0.76);
    for (int j = 0; j < s.size(); j++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, int(s[j]));
    }    

    s = "money";
    glRasterPos2f(-0.83, -0.82);
    for (int j = 0; j < s.size(); j++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, int(s[j]));
    }   

    s = "exp";
    glRasterPos2f(-0.83, -0.88);
    for (int j = 0; j < s.size(); j++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, int(s[j]));
    }   
    // for (int i = 0; i < vecs.size(); i++)
    // {
    //     s = vecs[i];
    //     glRasterPos2f(0.52, 0.88 - i * 0.1);
    //     for (int j = 0; j < s.size(); j++)
    //     {
    //         glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, int(s[j]));
    //     }
    // }

}