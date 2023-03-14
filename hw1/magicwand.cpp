#include "magicwand.h"
myobj* mgObj;
mytex* mgTex;
magicwand::magicwand(unsigned int programID)
{
    mgObj = new myobj(programID);
    mgTex = new mytex(programID);
}
void magicwand::draw(int programID)
{
    float objMtx[16];

    glScalef(2.5, 2.5, 2.5);

    glPushMatrix();
    
    glScalef(0.8, 6, 0.8);                              
    glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
    glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
    mgTex->magic_wand_wood->use(programID);
    mgObj->cylinder->draw(programID);
    glPopMatrix();
    //draw_cylinder(0.5, 0.8, 14);             //長度為14 寬為0.8

}
