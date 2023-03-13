#include "robot.h"
myobj* myObj;
mytex* myTex;
struct node {   //定義極座標的點
    double x = 0, y = 0, z = 0;
};
node ball_cor(double r, int A, int B) {          //極座標轉換
    node rt;
    rt.x = r * sin(A * 0.01745) * cos(B * 0.01745);
    rt.y = r * sin(A * 0.01745) * sin(B * 0.01745);
    rt.z = r * cos(A * 0.01745);
    return rt;
}
void hand::draw(int programID){

    float objMtx[16];
    glRotatef(shoulderAng_z, 0, 0, 1);             //-35放在身體旁邊 對z轉控制左右
    glRotatef(shoulderAng_x, 1, 0, 0);

    {   //半徑為 0.5 的肩膀
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->robot_blue_sub->use(programID);
        myObj->solidsphere->draw(programID);
    }
    //forarms
    glTranslatef(0, 0.75, 0);              //走到 肩膀上方0.25 + 圓中心0.75(畫1.5的手臂) - 0.25重疊
    {   //直徑為1.5的手臂
        glPushMatrix();
        glScalef(0.7, 1.5, 0.7);           
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->robot_gray->use(programID);
        myObj->solidsphere->draw(programID);
        glPopMatrix();
    }
    glTranslatef(0, 0.75, 0);                 //手臂前端中心
    
    glRotatef(elbowAng_x, 1, 0, 0);           //旋轉手肘
    glRotatef(elbowAng_y, 0, 1, 0);
    glRotatef(elbowAng_z, 0, 0, 1);

    {   //直徑為 0.5 的手肘      0.25重疊
        glPushMatrix();
        glScalef(0.5, 0.5, 0.5);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->robot_blue_sub->use(programID);
        myObj->solidsphere->draw(programID);
        glPopMatrix();
    }
    
    glTranslatef(0, 0.75, 0);                //走到 手軸前端0.25 + 圓中心0.75(畫1.5的手前臂) - 0.25重疊
    
    //手前臂
    {   //直徑為1.5的手前臂
        glPushMatrix();
        glScalef(0.55, 1.5, 0.55);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->robot_gray->use(programID);
        myObj->solidsphere->draw(programID);
        glPopMatrix();
    }

    glTranslatef(0, 0.75, 0);               //手前臂前端 

    //換手指方向應該在這轉
    glRotatef(fingerAng_y, 0, 1, 0);

    {   //左手指頭
        glPushMatrix();
        glTranslatef(-0.25, 0.2, 0);
        glRotatef(15, 0, 0, 1);               //張開角度
        glScalef(0.3, 0.7, 0.3);              //手指長: 0.7
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->robot_blue_sub->use(programID);
        myObj->solidsphere->draw(programID);
        glPopMatrix();
    }
    {   //右手指頭
        glPushMatrix();
        glTranslatef(0.25, 0.2, 0);
        glRotatef(-15, 0, 0, 1);              //張開角度
        glScalef(0.3, 0.7, 0.3);              //手指長: 0.7
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->robot_blue_sub->use(programID);
        myObj->solidsphere->draw(programID);
        glPopMatrix();
    }

    glRotatef(-shoulderAng_z, 0, 0, 1);   //變回正常的座標系統
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
    glTranslatef(x, y, z);                      //整隻的座標

    glRotatef(angle_y, 0, 1, 0);
    glRotatef(angle_x, 1, 0, 0);

    glTranslatef(0, 4.75, 0);                   //移到肚子
    {
        glPushMatrix();
        glScalef(4, 4, 4);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->robot_blue_main->use(programID);
        myObj->solidsphere->draw(programID);                //畫肚子 直徑4
        glPopMatrix();
    }                

    //setMaterial(0.4, 0.4, 0.4, 0, 0, 0, 120);
    {   //畫他的右手
        node tp = ball_cor(2, 90, 30);
        glPushMatrix();
        glTranslatef(tp.x, tp.y, tp.z);             //走到右肩膀
        right_h->draw(programID);
        //
        //glPushMatrix();                             //push3
        //glTranslatef(0, 0.3, 0);
        //if (carry_mw && isMagician) magic_wand_carry->draw();
        //glPopMatrix();                              //pop3
        //
        glPopMatrix();
    }
    
    ////法杖有改材質 要重新調
    {   //左肩膀
        node tp = ball_cor(2, 270, 330);
        glPushMatrix();
        glTranslatef(tp.x, tp.y, tp.z);       //走到左肩膀
        left_h->draw(programID);
        glPopMatrix();
    }
    

    //glPushMatrix();                       //坐在法杖上 
    //glTranslatef(0, -2, 0);
    //if (isOnWand) magic_wand_sit->draw();
    //glPopMatrix();
    //
    ////法杖有改材質 要重新調
    //setMaterial(0.4, 0.4, 0.4, 0, 0, 0, 120);
    
    {   //左大腿上面的關節  埋在身體裡
        glPushMatrix();
        glTranslatef(-0.4, -1.75, 0);
        left_f->draw(programID);
        glPopMatrix();
    }

    {   //右大腿上面的關節
        glPushMatrix();
        glTranslatef(0.4, -1.75, 0);
        right_f->draw(programID);
        glPopMatrix();
    }
    //頭
    //if (isLighting)setMaterial(0, 0, 0, 0.0f, 0.0f, 0.0f, 194 / 255.0, 223 / 255.0, 1.0, 1, 1, 1, 12.8f);
    //else change_color(mainColor);

    glTranslatef(0, 3, 0);               //在走到頭  和身體重疊0.5
    glPushMatrix();
    {
        glPushMatrix();
        glScalef(3, 3, 3);                    //直徑3
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->robot_blue_main->use(programID);
        myObj->solidsphere->draw(programID);
        glPopMatrix();  
    }

    //眼睛
    {   //機器人左眼
        glPushMatrix();                          //眼睛  push3
        glTranslatef(0.6, 0, 1.3);
        glScalef(0.4, 0.8, 0.4);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        if(isMagician) myTex->robot_blue_eye->use(programID);
        else myTex->black->use(programID);
        myObj->solidsphere->draw(programID);        //眼白
        glTranslatef(0, 0.15, 0.15);
        glScalef(0.66, 0.66, 0.66);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->white->use(programID);
        myObj->solidsphere->draw(programID);
        //glutSolidSphere(0.33, 10, 10);
        glPopMatrix();                           //pop3
    }
    {   //機器人右眼
        glPushMatrix();                          //push3
        glTranslatef(-0.6, 0, 1.3);
        glScalef(0.4, 0.8, 0.4);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        if (isMagician) myTex->robot_pink_eye->use(programID);
        else myTex->black->use(programID);
        myObj->solidsphere->draw(programID);       
        glTranslatef(0, 0.15, 0.15);         //眼白
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
        //glTranslatef(0, -0.6, 1.5);              //嘴巴
        //glLineWidth(1);
        //glBegin(GL_LINES);
        //glVertex3f(-0.2, 0, 0);
        //glVertex3f(0.2, 0, 0);
        //glEnd();
        //glPopMatrix();                          //pop3
    }
    //glTranslatef(0, 0.5, 0);                //帽子坐標系
    //if (isMagician) draw_hat();

    glPopMatrix();                       //離開頭 
    
    glPopMatrix();                       //離開肚子坐標系 

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
    glRotatef(hipJointAng_x, 1, 0, 0);          //移動關節角度 180放下
    
    {   //大腿上的關節 0.5直徑
        glPushMatrix();
        glScalef(0.5,0.5,0.5);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->robot_blue_sub->use(programID);
        myObj->solidsphere->draw(programID);
        glPopMatrix();
    }

    glTranslatef(0, 0.625, 0);           //走到 膝蓋0.125 + 圓中心0.5(畫1的圓)
    
    {   //大腿
        glPushMatrix();
        glScalef(0.7, 1, 0.7);               //大腿長1
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->robot_gray->use(programID);
        myObj->solidsphere->draw(programID);
        glPopMatrix();
    }
    
    glTranslatef(0, 0.5, 0);             //腿前端中心  和膝蓋重疊0.25
    glRotatef(kneeAng_x, 1, 0, 0);       //膝蓋角度
    
    {   //膝蓋
        glPushMatrix();
        glScalef(0.5, 0.5, 0.5);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->robot_blue_sub->use(programID);
        myObj->solidsphere->draw(programID);
        glPopMatrix();    
    }

    glTranslatef(0, 0.5, 0);             //膝蓋前端 0.25 + 腿中間0.5(腿長1) - 0.25(重疊地方)
    
    {
        glPushMatrix();
        glScalef(0.5, 1, 0.5);               //小腿長1
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->robot_gray->use(programID);
        myObj->solidsphere->draw(programID);
        glPopMatrix();
    }

    glTranslatef(0, 0.5, 0);             //小腿前端中心 0.5 和腿重疊0.25
    
    {   //腳 直徑1
        glPushMatrix();
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->robot_gray_dark->use(programID);
        myObj->solidsphere->draw(programID);
        glPopMatrix();
    }       
}
