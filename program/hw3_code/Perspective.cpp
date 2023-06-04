#include "Perspective.h"
#include "MyRobot.h"
extern MyRobot* myRobot;
Perspective::Perspective(float aspect_)
{
    aspect = aspect_;
}
void Perspective::update(){

}
void Perspective::use()
{
    {   //projection
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(fovy, aspect, zNear, zFar);
        float gluPers[16];
        glGetFloatv(GL_PROJECTION_MATRIX, gluPers);
        glUniformMatrix4fv(1, 1, GL_FALSE, gluPers);
        glLoadIdentity();
    }
    {   //lookat
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(pos[0], pos[1] , pos[2], seePoint[0], seePoint[1], seePoint[2], 0,1,0);
        glGetFloatv(GL_MODELVIEW_MATRIX, eyeMtx);
        glUniformMatrix4fv(0, 1, GL_FALSE, eyeMtx);
    }
    glUniform3f(CAMERA_POS, pos[0], pos[1], pos[2]);
}

FPPerspective::FPPerspective(float aspect_):Perspective(aspect_)
{
    pos = {116,30,150};
    seePoint = {116,30,155};
    fovy = 40;
    eyeAngY = 90;
}

void FPPerspective::keyEvent(unsigned char key)
{
    float tpPos[3] = { pos[0], pos[1], pos[2] };
    float w[3] = { seePoint[0] - pos[0] ,seePoint[1] - pos[1], seePoint[2] - pos[2] };
    float s[3] = { -seePoint[0] + pos[0] , -seePoint[1] + pos[1], -seePoint[2] + pos[2] };
    float d[3] = { -w[2] , 0 , w[0] };
    float a[3] = { w[2] , 0 , -w[0] };
    w[0] = w[0] / sqrt(w[0] * w[0] + w[1] * w[1] + w[2] * w[2]);
    w[1] = w[1] / sqrt(w[0] * w[0] + w[1] * w[1] + w[2] * w[2]);
    w[2] = w[2] / sqrt(w[0] * w[0] + w[1] * w[1] + w[2] * w[2]);

    s[0] = s[0] / sqrt(s[0] * s[0] + s[1] * s[1] + s[2] * s[2]);
    s[1] = s[1] / sqrt(s[0] * s[0] + s[1] * s[1] + s[2] * s[2]);
    s[2] = s[2] / sqrt(s[0] * s[0] + s[1] * s[1] + s[2] * s[2]);

    d[0] = d[0] / sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
    d[1] = d[1] / sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
    d[2] = d[2] / sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);

    a[0] = a[0] / sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    a[1] = a[1] / sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    a[2] = a[2] / sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    if(key == 's' || key == 'S'){
        pos[0] += 2*s[0];
        pos[2] += 2*s[2];
    }else if(key == 'w' || key == 'W'){
        pos[0] += 2*w[0];
        pos[2] += 2*w[2];
    }else if(key == 'a' || key == 'A'){
        pos[0] += 2*a[0];
        pos[2] += 2*a[2];
    }else if(key == 'd' || key == 'D'){
        pos[0] += 2*d[0];
        pos[2] += 2*d[2];
    }
}
void FPPerspective::passiveMotionEvent(int x, int y)
{
    if(first){
        mouseX = x;
        mouseY = y;
        first = 0; 
    }

    eyeAngY += 0.8*(x-mouseX);
    if (eyeAngY >= 360) eyeAngY -= 360;
    if (eyeAngY <= 0) eyeAngY += 360;

    if (y < mouseY)  seePoint[1] = fmin(seePoint[1] - 0.2*(y-mouseY), 50.0);
    else seePoint[1] = fmax(seePoint[1] - 0.2*(y-mouseY), 0.0);    

    mouseX = x;                                                                                                                                                                                                                                                
    mouseY = y;
}

void FPPerspective::update()
{
    seePoint[0] = pos[0] + 15 * cos(eyeAngY * PI / 180.0);
    seePoint[2] = pos[2] + 15 * sin(eyeAngY * PI / 180.0);
    dir = seePoint - pos;
}
TPPerspective::TPPerspective(float aspect_):Perspective(aspect_)
{
    pos = {116,20,150};
    seePoint = {116,30,155};
    eyeAngY = 90;
    eyeDis = 30;
    fovy = 100;
}

void TPPerspective::update()
{
    pos[0] = myRobot->pos[0] + eyeDis * cos(eyeAngY * PI / 180.0);
    pos[2] = myRobot->pos[2] + eyeDis * sin(eyeAngY * PI / 180.0);

    seePoint = {myRobot->pos[0],myRobot->pos[1]+20,myRobot->pos[2]};
}

void TPPerspective::motionEvent(int x, int y)
{
    if (mouseBtn == GLUT_RIGHT_BUTTON) {
        if (x > mouseX) eyeAngY += 1;
        else eyeAngY -= 1;

        if (eyeAngY >= 360) eyeAngY -= 360;
        if (eyeAngY <= 0) eyeAngY += 360;

        if (y > mouseY)  pos[1] = fmin(pos[1] + 0.5, 80.0);
        else pos[1] = fmax(pos[1] - 0.5, 10.0);
    }
    else if (mouseBtn == GLUT_LEFT_BUTTON) {

    }
    mouseX = x;
    mouseY = y;
}

void TPPerspective::mouseWheelEvent(int button, int dir, int x, int y)
{
    if (dir > 0) fovy = fmax(fovy - 2, 70);
    else fovy = fmin(fovy + 2, 150);
}

void TPPerspective::mouseClickEvent(int btn, int state, int x, int y)
{
    if (state == GLUT_DOWN) {
        //cout << btn << "\n";
        if (btn == GLUT_RIGHT_BUTTON) {
            mouseBtn = GLUT_RIGHT_BUTTON;
        }
        else if(btn == 3){
            fovy = fmax(fovy - 2, 70);
        }else if(btn == 4){
            fovy = fmin(fovy + 2, 150);
        }else if(btn == 0){   //left
            
        }
    }
    else if (state == GLUT_UP) {
        mouseBtn = -1;
    }
}
