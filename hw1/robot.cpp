#include "robot.h"
myobj* myObj;
mytex* myTex;
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
void hand::draw(int programID){

    float objMtx[16];
    glRotatef(shoulderAng_z, 0, 0, 1);             //-35��b������� ��z�౱��k
    glRotatef(shoulderAng_x, 1, 0, 0);

    {   //�b�|�� 0.5 ���ӻH
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->robot_blue_sub->use(programID);
        myObj->solidsphere->draw(programID);
    }
    //forarms
    glTranslatef(0, 0.75, 0);              //���� �ӻH�W��0.25 + �ꤤ��0.75(�e1.5�����u) - 0.25���|
    {   //���|��1.5�����u
        glPushMatrix();
        glScalef(0.7, 1.5, 0.7);           
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->robot_gray->use(programID);
        myObj->solidsphere->draw(programID);
        glPopMatrix();
    }
    glTranslatef(0, 0.75, 0);                 //���u�e�ݤ���
    
    glRotatef(elbowAng_x, 1, 0, 0);           //�����y
    glRotatef(elbowAng_y, 0, 1, 0);
    glRotatef(elbowAng_z, 0, 0, 1);

    {   //���|�� 0.5 ����y      0.25���|
        glPushMatrix();
        glScalef(0.5, 0.5, 0.5);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->robot_blue_sub->use(programID);
        myObj->solidsphere->draw(programID);
        glPopMatrix();
    }
    
    glTranslatef(0, 0.75, 0);                //���� ��b�e��0.25 + �ꤤ��0.75(�e1.5����e�u) - 0.25���|
    
    //��e�u
    {   //���|��1.5����e�u
        glPushMatrix();
        glScalef(0.55, 1.5, 0.55);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->robot_gray->use(programID);
        myObj->solidsphere->draw(programID);
        glPopMatrix();
    }

    glTranslatef(0, 0.75, 0);               //��e�u�e�� 

    //�������V���Ӧb�o��
    glRotatef(fingerAng_y, 0, 1, 0);

    {   //������Y
        glPushMatrix();
        glTranslatef(-0.25, 0.2, 0);
        glRotatef(15, 0, 0, 1);               //�i�}����
        glScalef(0.3, 0.7, 0.3);              //�����: 0.7
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->robot_blue_sub->use(programID);
        myObj->solidsphere->draw(programID);
        glPopMatrix();
    }
    {   //�k����Y
        glPushMatrix();
        glTranslatef(0.25, 0.2, 0);
        glRotatef(-15, 0, 0, 1);              //�i�}����
        glScalef(0.3, 0.7, 0.3);              //�����: 0.7
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->robot_blue_sub->use(programID);
        myObj->solidsphere->draw(programID);
        glPopMatrix();
    }

    glRotatef(-shoulderAng_z, 0, 0, 1);   //�ܦ^���`���y�Шt��
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
        myTex->robot_blue_main->use(programID);
        myObj->solidsphere->draw(programID);                //�e�{�l ���|4
        glPopMatrix();
    }                

    //setMaterial(0.4, 0.4, 0.4, 0, 0, 0, 120);
    {   //�e�L���k��
        node tp = ball_cor(2, 90, 30);
        glPushMatrix();
        glTranslatef(tp.x, tp.y, tp.z);             //����k�ӻH
        right_h->draw(programID);
        //
        //glPushMatrix();                             //push3
        //glTranslatef(0, 0.3, 0);
        //if (carry_mw && isMagician) magic_wand_carry->draw();
        //glPopMatrix();                              //pop3
        //
        glPopMatrix();
    }
    
    ////�k��������� �n���s��
    {   //���ӻH
        node tp = ball_cor(2, 270, 330);
        glPushMatrix();
        glTranslatef(tp.x, tp.y, tp.z);       //���쥪�ӻH
        left_h->draw(programID);
        glPopMatrix();
    }
    

    //glPushMatrix();                       //���b�k���W 
    //glTranslatef(0, -2, 0);
    //if (isOnWand) magic_wand_sit->draw();
    //glPopMatrix();
    //
    ////�k��������� �n���s��
    //setMaterial(0.4, 0.4, 0.4, 0, 0, 0, 120);
    
    {   //���j�L�W�������`  �I�b�����
        glPushMatrix();
        glTranslatef(-0.4, -1.75, 0);
        left_f->draw(programID);
        glPopMatrix();
    }

    {   //�k�j�L�W�������`
        glPushMatrix();
        glTranslatef(0.4, -1.75, 0);
        right_f->draw(programID);
        glPopMatrix();
    }
    //�Y
    //if (isLighting)setMaterial(0, 0, 0, 0.0f, 0.0f, 0.0f, 194 / 255.0, 223 / 255.0, 1.0, 1, 1, 1, 12.8f);
    //else change_color(mainColor);

    glTranslatef(0, 3, 0);               //�b�����Y  �M���魫�|0.5
    glPushMatrix();
    {
        glPushMatrix();
        glScalef(3, 3, 3);                    //���|3
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->robot_blue_main->use(programID);
        myObj->solidsphere->draw(programID);
        glPopMatrix();  
    }

    //����
    {   //�����H����
        glPushMatrix();                          //����  push3
        glTranslatef(0.6, 0, 1.3);
        glScalef(0.4, 0.8, 0.4);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        if(isMagician) myTex->robot_blue_eye->use(programID);
        else myTex->black->use(programID);
        myObj->solidsphere->draw(programID);        //����
        glTranslatef(0, 0.15, 0.15);
        glScalef(0.66, 0.66, 0.66);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->white->use(programID);
        myObj->solidsphere->draw(programID);
        //glutSolidSphere(0.33, 10, 10);
        glPopMatrix();                           //pop3
    }
    {   //�����H�k��
        glPushMatrix();                          //push3
        glTranslatef(-0.6, 0, 1.3);
        glScalef(0.4, 0.8, 0.4);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        if (isMagician) myTex->robot_pink_eye->use(programID);
        else myTex->black->use(programID);
        myObj->solidsphere->draw(programID);       
        glTranslatef(0, 0.15, 0.15);         //����
        glScalef(0.66, 0.66, 0.66);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->white->use(programID);
        myObj->solidsphere->draw(programID);
        glPopMatrix();                          
    }
    //glColor3f(1, 0, 0);
    {
        //glPushMatrix();                           //push3
        //glTranslatef(0, -0.6, 1.5);              //�L��
        //glLineWidth(1);
        //glBegin(GL_LINES);
        //glVertex3f(-0.2, 0, 0);
        //glVertex3f(0.2, 0, 0);
        //glEnd();
        //glPopMatrix();                          //pop3
    }
    //glTranslatef(0, 0.5, 0);                //�U�l���Шt
    //if (isMagician) draw_hat();

    glPopMatrix();                       //���}�Y 
    
    glPopMatrix();                       //���}�{�l���Шt 

}
void robot::stand()
{
    y = 0;
    x = 0;
    y = 0;
    angle_x = 0;
    //right_f->hipJointAng_x = 180;
    //right_f->kneeAng_x = 0;
    //left_f->hipJointAng_x = 180;
    //left_f->kneeAng_x = 0;
    left_h->shoulderAng_x = 180;
    right_h->shoulderAng_x = 180;
    left_h->elbowAng_x = 0;
    right_h->elbowAng_x = 0;
    left_h->elbowAng_y = 0;
    right_h->elbowAng_y = 0;
    left_h->elbowAng_z = 0;
    right_h->elbowAng_z = 0;
    left_h->shoulderAng_z = -35;
    right_h->shoulderAng_z = 35;
}
robot::robot(unsigned int programID)
{
    myObj = new myobj(programID);
    myTex = new mytex(programID);
    isMagician = 1;
    right_h = new hand;
    left_h = new hand;
    left_f = new foot;
    right_f = new foot;
    stand();
}

void foot::draw(int programID)
{
    float objMtx[16];
    glRotatef(hipJointAng_x, 1, 0, 0);          //�������`���� 180��U
    
    {   //�j�L�W�����` 0.5���|
        glPushMatrix();
        glScalef(0.5,0.5,0.5);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->robot_blue_sub->use(programID);
        myObj->solidsphere->draw(programID);
        glPopMatrix();
    }

    glTranslatef(0, 0.625, 0);           //���� ���\0.125 + �ꤤ��0.5(�e1����)
    
    {   //�j�L
        glPushMatrix();
        glScalef(0.7, 1, 0.7);               //�j�L��1
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->robot_gray->use(programID);
        myObj->solidsphere->draw(programID);
        glPopMatrix();
    }
    
    glTranslatef(0, 0.5, 0);             //�L�e�ݤ���  �M���\���|0.25
    glRotatef(kneeAng_x, 1, 0, 0);       //���\����
    
    {   //���\
        glPushMatrix();
        glScalef(0.5, 0.5, 0.5);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->robot_blue_sub->use(programID);
        myObj->solidsphere->draw(programID);
        glPopMatrix();    
    }

    glTranslatef(0, 0.5, 0);             //���\�e�� 0.25 + �L����0.5(�L��1) - 0.25(���|�a��)
    
    {
        glPushMatrix();
        glScalef(0.5, 1, 0.5);               //�p�L��1
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->robot_gray->use(programID);
        myObj->solidsphere->draw(programID);
        glPopMatrix();
    }

    glTranslatef(0, 0.5, 0);             //�p�L�e�ݤ��� 0.5 �M�L���|0.25
    
    {   //�} ���|1
        glPushMatrix();
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->robot_gray_dark->use(programID);
        myObj->solidsphere->draw(programID);
        glPopMatrix();
    }       
}
