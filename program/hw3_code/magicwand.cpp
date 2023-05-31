#include "magicwand.h"
extern GraphicObj* graphicObj;
extern mytex* myTex;
magicwand::magicwand(unsigned int programID)
{

}
void magicwand::draw(int programID)
{
    float objMtx[16];

    glScalef(2.5, 2.5, 2.5);

    glPushMatrix();
    
    glScalef(0.8, 6, 0.8);                              
    glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
    glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
    myTex->magic_wand_wood->use(programID);
    graphicObj->cylinder->draw(programID);
    glPopMatrix();
    //draw_cylinder(0.5, 0.8, 14);             //長度為14 寬為0.8

}
