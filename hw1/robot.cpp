#include "robot.h"
#include "mesh.h"
myobj* myObj;
robot::robot(unsigned int programID)
{
    myObj = new myobj(programID);
}
struct node {   //�w�q���y�Ъ��I
    double x = 0, y = 0, z = 0;
};
node ball_cor(double r, int A, int B) {          //���y���ഫ
    node rt;
    rt.x = r * sin(A * 0.01745) * cos(B * 0.01745);
    rt.y = r * sin(A * 0.01745) * sin(B * 0.01745);
    rt.z = r * cos(A * 0.01745);
    return rt;
}
void robot::sit(){
    //left_f->kneeAng_x = 80;
    //right_f->kneeAng_x = 80;
    //left_f->hipJointAng_x = 100;
    //right_f->hipJointAng_x = 100;
    //left_h->shoulderAng_x = 180;
    //right_h->shoulderAng_x = 180;
    //left_h->shoulderAng_z = -15;
    //right_h->shoulderAng_z = 15;
}

void robot::draw(unsigned int programID)
{
    
    float objMtx[16];
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    glScalef(2.5, 2.5, 2.5);
    glTranslatef(x, y, z);                      //�㰦���y��

    glRotatef(angle_y, 0, 1, 0);
    glRotatef(angle_x, 1, 0, 0);

    glTranslatef(0, 4.75, 0);                   //����{�l
    
    {
    glPushMatrix();
    glScalef(4, 4, 4);
    glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
    glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
    myObj->solidsphere->draw(programID);                //�e�{�l ���|4
    glPopMatrix();
    }                

    //setMaterial(0.4, 0.4, 0.4, 0, 0, 0, 120);
    //node tp = ball_cor(2, 90, 30);
    //glPushMatrix();
    //glTranslatef(tp.x, tp.y, tp.z);             //����k�ӻH
    //right_h->draw();
    //
    //glPushMatrix();                             //push3
    //glTranslatef(0, 0.3, 0);
    //if (carry_mw && isMagician) magic_wand_carry->draw();
    //glPopMatrix();                              //pop3
    //
    //glPopMatrix();                              //pop2
    //
    ////�k��������� �n���s��
    //setMaterial(0.4, 0.4, 0.4, 0, 0, 0, 120);
    ////���ӻH
    //tp = ball_cor(2, 270, 330);
    //glPushMatrix();
    //glTranslatef(tp.x, tp.y, tp.z);       //���쥪�ӻH
    //left_h->draw();
    //glPopMatrix();
    //
    //glPushMatrix();                       //���b�k���W 
    //glTranslatef(0, -2, 0);
    //if (isOnWand) magic_wand_sit->draw();
    //glPopMatrix();
    //
    ////�k��������� �n���s��
    //setMaterial(0.4, 0.4, 0.4, 0, 0, 0, 120);
    ////���j�L�W�������`  �I�b�����
    //glPushMatrix();
    //glTranslatef(-0.4, -1.75, 0);
    //left_f->draw();
    //glPopMatrix();
    //
    ////�k�j�L�W�������`
    //glPushMatrix();
    //glTranslatef(0.4, -1.75, 0);
    //right_f->draw();
    //glPopMatrix();
    //
    ////�Y
    //if (isLighting)setMaterial(0, 0, 0, 0.0f, 0.0f, 0.0f, 194 / 255.0, 223 / 255.0, 1.0, 1, 1, 1, 12.8f);
    //else change_color(mainColor);
    //
    ////change_color(mainColor);
    glTranslatef(0, 3, 0);               //�b�����Y  �M���魫�|0.5
    glPushMatrix();
    
    {
    glPushMatrix();
    glScalef(3, 3, 3);                    //���|3
    glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
    glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
    myObj->solidsphere->draw(programID);
    glPopMatrix();  
    }
    glPopMatrix();
    glPopMatrix();
    ////����
    //setMaterial(0, 0, 0, 0, 0, 0, 0);
    //if (isMagician)
    //    glColor3f(128 / 255.0, 128 / 255.0, 1);  //��
    //else
    //    glColor3f(0, 0, 0);
    //glPushMatrix();                          //����  push3
    //glTranslatef(0.6, 0, 1.3);
    //glScalef(0.4, 0.8, 0.4);
    //glutSolidSphere(0.5, 10, 10);
    //glColor3f(1, 1, 1);                      //����
    //glTranslatef(0, 0.15, 0.15);
    //glutSolidSphere(0.33, 10, 10);
    //glPopMatrix();                           //pop3
    //
    //setMaterial(0, 0, 0, 0, 0, 0, 0);
    //if (isMagician)
    //    //glColor3f(128 / 255.0, 128 / 255.0, 1);
    //    glColor3f(1, 122 / 255.0, 189 / 255.0);      //��
    //else
    //    glColor3f(0, 0, 0);
    //glPushMatrix();                          //push3
    //glTranslatef(-0.6, 0, 1.3);
    //glScalef(0.4, 0.8, 0.4);
    //glutSolidSphere(0.5, 10, 10);
    //glColor3f(1, 1, 1);                      //����
    //glTranslatef(0, 0.15, 0.15);
    //glutSolidSphere(0.33, 10, 10);
    //glPopMatrix();                           //pop3
    //
    //glColor3f(1, 0, 0);
    //glPushMatrix();                           //push3
    //glTranslatef(0, -0.6, 1.5);              //�L��
    //glLineWidth(1);
    //glBegin(GL_LINES);
    //glVertex3f(-0.2, 0, 0);
    //glVertex3f(0.2, 0, 0);
    //glEnd();
    //glPopMatrix();                          //pop3
    //
    //glTranslatef(0, 0.5, 0);                //�U�l���Шt
    //if (isMagician) draw_hat();
    //
    //glPopMatrix();                       //���}�Y pop2
    //
    //glPopMatrix();                       //���}�{�l���Шt pop1

}
