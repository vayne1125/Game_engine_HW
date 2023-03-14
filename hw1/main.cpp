#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<set>
#include<functional>
#include<fstream>
#include<string>
#include<GL\freeglut.h>
#include"glad\glad.h"
#include"mesh.h"
#include"myobj.h"
#include"robot.h"
#define   PI   3.1415927
//時間模式
#define RUNTIMER 50             //判斷是否跑跑跑
#define JUMPTIMER 51            //一般跳
#define JUMPONWANDTIMER 52      //跳上法杖
#define JUMPTOFLOORTIMER 53     //跳回地板
#define ANIMATION        54
//object
using namespace std;
GLuint programID;
robot* myRobot;
myobj* myObjj;
mytex* myTexx;
mesh* billboard;
magicwand* uiui;
float   eyeAngx = 0.0, eyeAngy = 0.0, eyeAngz = 0.0;
float   u[3][3] = { {1.0,0.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0} };
float   eye[3] = { 30.0, 10.0, 80.0 };
float   cv = cos(5.0 * PI / 180.0), sv = sin(5.0 * PI / 180.0); /* cos(5.0) and sin(5.0) */
float   pos[3] = { 0,0,0 };
int     preKey = 0;
float   eyeMtx[16] = {0};
int eevee_ani = 0;
void draw_billboard(float x, float z, float w, float h, texture* tex)
{   
    float  a[3], b[3];
    float  w0, w2;
    double len;
    float objMtx[16];
    /*----Get w0 and w2 from the modelview matrix mtx[] ---*/
    w0 = eyeMtx[2]; w2 = eyeMtx[10];

    len = sqrt(w0 * w0 + w2 * w2);
    /*---- Define the a and b axes for billboards ----*/
    b[0] = 0.0; b[1] = 1.0; b[2] = 0.0;
    a[0] = w2 / len; a[1] = 0.0; a[2] = -w0 / len;

    float  v0[3], v1[3], v2[3], v3[3];
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    /*----Compute the 4 vertices of the billboard ----*/
    v0[0] = x - (w / 2) * a[0]; v0[1] = 0.0; v0[2] = z - (w / 2) * a[2];
    v1[0] = x + (w / 2) * a[0]; v1[1] = 0.0; v1[2] = z + (w / 2) * a[2];
    v2[0] = x + (w / 2) * a[0]; v2[1] = h; v2[2] = z + (w / 2) * a[2];
    v3[0] = x - (w / 2) * a[0]; v3[1] = h; v3[2] = z - (w / 2) * a[2];
    
    billboard = new mesh(programID, { v0[0],v0[1],v0[2],0,1,0,0,1,v1[0],v1[1],v1[2],1,1,0,0,1,v2[0],v2[1],v2[2],1,0,0,0,1,
        v2[0],v2[1],v2[2],1,0,0,0,1,v3[0],v3[1],v3[2],0,0,0,0,1,v0[0],v0[1],v0[2],0,1,0,0,1});

    glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
    glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
    tex->use(programID);
    billboard->draw(programID);

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
}
void timerFunc(int nTimerID) {
    switch (nTimerID) {
    case RUNTIMER:                //偵測跑
        preKey = -1;
        break;
    case JUMPTIMER:               //跳躍
        if (!myRobot->jump()) {
            //isLock = LOCK;        //跳時不可以按
            glutTimerFunc(100, timerFunc, JUMPTIMER);
        }
        else {
            //if (isLitspotOpen)  myRobot.carryLight();
            //else 
            myRobot->stand();
            //isLock = UNLOCK;
        }
        glutPostRedisplay();
        break;
    case JUMPONWANDTIMER:           //跳上法杖
        if (!myRobot->jumpOnWand()) {
            //isLock = LOCK;
            glutTimerFunc(100, timerFunc, JUMPONWANDTIMER);
        }
        else {
            //isLock = UNLOCK;
        }
        glutPostRedisplay();
        break;
    case JUMPTOFLOORTIMER:          //跳回地面
        if (!myRobot->jumpToFloor()) {
            //isLock = LOCK;
            glutTimerFunc(100, timerFunc, JUMPTOFLOORTIMER);
        }
        else {
            //isLock = UNLOCK;
            //if (isLitspotOpen)  myRobot.carryLight();
            //else 
            myRobot->stand();
        }
        glutPostRedisplay();
        break;
    case ANIMATION:                //搖椅擺擺擺
        //myBig_chair.move();
        //poolAng++;
        eevee_ani++;
        if (eevee_ani >= 56) eevee_ani = 0;
        //if (scene == GRASSLAND) {
        glutTimerFunc(100, timerFunc, ANIMATION);
        //}
        //glutPostRedisplay();
        break;
    }
}
void keyboardUp_func(unsigned char key, int x, int y) {
    //if (isLock == LOCK) return;
    glutTimerFunc(200, timerFunc, RUNTIMER);
    if (myRobot->getMoveMode() != ROBOT_FLY && preKey != key)  myRobot->change_moveMode(ROBOT_WALK);
    preKey = key;
    if (myRobot->getMoveMode() != ROBOT_FLY && (key == 'W' || key == 'w' || key == 'A' || key == 'a' || key == 'S' || key == 's' || key == 'D' || key == 'd' || key == 'r' || key == 'R')) {
        //if (isLitspotOpen)  myRobot.carryLight();
        //else 
        myRobot->stand();
    }
    cout << "kokoko\n";
    if (key == ' ') {                   //跳
        if (myRobot->isMagician) {
            if (myRobot->isOnWand) {
                glutTimerFunc(100, timerFunc, JUMPTOFLOORTIMER);
                myRobot->change_moveMode(ROBOT_WALK);
            }
            else {
                myRobot->jump_ready();
                glutTimerFunc(100, timerFunc, JUMPONWANDTIMER);
                myRobot->change_moveMode(ROBOT_FLY);
                //glDisable(GL_LIGHT2);
                //isLitspotOpen = 0;
            }
        }
        else {
            myRobot->jump_ready();
            glutTimerFunc(100, timerFunc, JUMPTIMER);
        }
    }
}
void my_move_order(unsigned char key) {        //跟移動相關的判斷
    if (myRobot-> isSitOnChair) return;        //如果坐在椅子上就不能動
    float tpPos[3] = { pos[0], pos[1], pos[2] };
    if (myRobot->getMoveMode() == ROBOT_WALK && preKey == key && (key == 'W' || key == 'w' || key == 'A' || key == 'a' || key == 'S' || key == 's' || key == 'D' || key == 'd'))
        myRobot->change_moveMode(ROBOT_RUN);                       //0.3秒內連續按 就變成跑跑

    if (key == 'S' || key == 's') {
        if (myRobot->isOnWand) {        //飛行模式
            myRobot->angle_y = 90;
        }
        else {                         //一般走路模式
            myRobot->angle_y = 0;
            myRobot->move();
        }
        tpPos[2] += myRobot->moveOffset;
    }
    else if (key == 'W' || key == 'w') {
        if (myRobot->isOnWand) {
            myRobot->angle_y = 90;
        }
        else {
            myRobot->angle_y = 180;
            myRobot->move();
        }
        tpPos[2] -= myRobot->moveOffset;
    }
    else if (key == 'A' || key == 'a') {
        if (myRobot->isOnWand) {
            myRobot->angle_y = 0;
        }
        else {
            myRobot->angle_y = 270;
            myRobot->move();
        }
        tpPos[0] -= myRobot->moveOffset;
    }
    else if (key == 'D' || key == 'd') {
        if (myRobot->isOnWand) {
            myRobot->angle_y = 0;
        }
        else {
            myRobot->angle_y = 90;
            myRobot->move();
        }
        tpPos[0] += myRobot->moveOffset;
    }
    else if (key == 'r' || key == 'R') {            //轉圈圈
        myRobot->angle_y += 5;
        if (!myRobot->isOnWand) {
            myRobot->change_moveMode(ROBOT_TURN);
            myRobot->move();    //在地板才要動腳
        }
    }
    //if (detectCollision(tpPos[0], tpPos[1], tpPos[2])) return;
    //cout << tpPos[0];
    for (int i = 0; i < 3; i++) pos[i] = tpPos[i];
    //cout << myRobot->moveOffset << "   jj\n";
    //cout << pos[0];
}
void keybaord_fun(unsigned char key, int X, int Y) {
    my_move_order(key);
    float  x[3], y[3], z[3];
    int i;
    if (key == 19) {       //上 ctrl + w
        for (int i = 0; i < 3; i++) eye[i] -= 1 * u[1][i];
    }
    else if (key == 23) {   //下 ctrl + s    
        for (int i = 0; i < 3; i++) eye[i] += 1 * u[1][i];
    }
    else if (key == 4) {   //右 ctrl + d     
        for (int i = 0; i < 3; i++) eye[i] += 1 * u[0][i];
    }
    else if (key == 1) {   //左 ctrl + a   
        for (int i = 0; i < 3; i++) eye[i] -= 1 * u[0][i];
    }
    if (key == 17) {       //往前 ctrl + q
        for (i = 0; i < 3; i++) eye[i] -= 1 * u[2][i];
    }
    else if (key == 5) {   //往後 ctrl + e
        for (i = 0; i < 3; i++) eye[i] += 1 * u[2][i];
    }
    else if (key == 24) {             //ctrl + x pitching 
        eyeAngx += 5.0;
        if (eyeAngx > 360.0) eyeAngx -= 360.0;
        for (i = 0; i < 3; i++) {
            z[i] = cv * u[2][i] - sv * u[1][i];
            y[i] = sv * u[2][i] + cv * u[1][i];
        }
        /*
        y[0] = u[1][0] * cv - u[2][0] * sv;
        y[1] = u[1][1] * cv - u[2][1] * sv;
        y[2] = u[1][2] * cv - u[2][2] * sv;

        z[0] = u[2][0] * cv + u[1][0] * sv;
        z[1] = u[2][1] * cv + u[1][1] * sv;
        z[2] = u[2][2] * cv + u[1][2] * sv;
        */
        for (i = 0; i < 3; i++) {
            u[1][i] = y[i];
            u[2][i] = z[i];
        }
    }
    else if (key == 25) {            // heading ctrl + y
        eyeAngy += 5.0;
        if (eyeAngy > 360.0) eyeAngy -= 360.0;
        for (i = 0; i < 3; i++) {
            x[i] = cv * u[0][i] - sv * u[2][i];
            z[i] = sv * u[0][i] + cv * u[2][i];
        }
        for (i = 0; i < 3; i++) {
            u[0][i] = x[i];
            u[2][i] = z[i];
        }
    }
    else if (key == 26) {            //ctrl + z rolling
        eyeAngz += 5.0;
        if (eyeAngz > 360.0) eyeAngz -= 360.0;
        for (i = 0; i < 3; i++) {
            x[i] = cv * u[0][i] - sv * u[1][i];
            y[i] = sv * u[0][i] + cv * u[1][i];
        }
        for (i = 0; i < 3; i++) {
            u[0][i] = x[i];
            u[1][i] = y[i];
        }
    }
}
void myInit() {
    myTexx = new mytex(programID);
    myObjj = new myobj(programID);
    myRobot = new robot(programID);
    myRobot->setOffset(0.5,2,3); 
    glutTimerFunc(100, timerFunc, ANIMATION);

    uiui = new magicwand(programID);
}
void myDisplay(void)
{
    //cout << eye[0] << " " << eye[1] << " " << eye[2] << "\n";
    glEnable(GL_DEPTH_TEST);
    //glClearColor(1, 1, 1, 1.0);  //Dark grey background
    glClearColor(0.10, 0.10, 0.10, 1.0);  //Dark grey background
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    //layout(location = 4) uniform vec4  lightInfopos; //0: 平行光
    //layout(location = 5) uniform vec3  lightInfodir;
    //layout(location = 6) uniform vec3  lightInfocolor;
    //layout(location = 7) uniform float  lightInfoexpo;   //should up to 1
    //layout(location = 8) uniform float lightInfostrength;
    //layout(location = 10) uniform vec3 cameraPos;
    //layout(location = 11) uniform float ambient;

    glUniform4f(4, 0, -70, 0, 1.0);
    glUniform3f(5, 1,1,1);
    glUniform3f(6, 1,1,1);
    glUniform1f(7, 0);
    glUniform1f(8, 50);
    glUniform3f(10, eye[0],eye[1],eye[2]);
    glUniform1f(11, 0.2);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double zNear = 0.1, zFar = 500, aspect = 800 / (double)800, fovy = 105;
    //aspect = W/(double)H
    gluPerspective(fovy, aspect, zNear, zFar);
    float gluPers[16];
    glGetFloatv(GL_PROJECTION_MATRIX,gluPers);
    glUniformMatrix4fv(1, 1, GL_FALSE, gluPers);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2], eye[0] - u[2][0], eye[1] - u[2][1], eye[2] - u[2][2], u[1][0], u[1][1], u[1][2]);
    glGetFloatv(GL_MODELVIEW_MATRIX, eyeMtx);
    glUniformMatrix4fv(0,1,GL_FALSE, eyeMtx);

    //glLoadIdentity();
    ////glRotatef(30,0,0,1);
    //glTranslatef(100,10,0);
    ////glScalef(10,10,10);
    float objMtx[16];
    //glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
    //glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
    //
    ////myObj->cylinder->draw(programID);
    //
    //glLoadIdentity();
    //glRotatef(30,0,0,1);
    //glTranslatef(-100, 10, 0);
    //glScalef(100, 100, 100);
    //float objMtx[16];
    //glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
    //glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);

    //myObjj->house_1->draw(programID);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]);
    //glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
    //glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
    myRobot->draw(programID);
    glPopMatrix();

    
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    draw_billboard(0, 0, 37.5, 20,myTexx->eevee[eevee_ani]);

glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
myTexx->red-> use(programID);
    myObjj->cube->draw(programID);


    /*
    glBegin(GL_TRIANGLES);
        glVertex3f(1,0,0);
        glVertex3f(0,1,0);
        glVertex3f(1,1,0);
    glEnd();
    */
    glPushMatrix();
    glTranslatef(60,8.75,0);
    uiui->draw(programID);
    glPopMatrix();

    glutSwapBuffers();
    glutPostRedisplay();
    Sleep(1);
}
int main(int argc, char** argv) {
    std::cout << "Hello World!\n";
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Shader Sample Program"); //Window title bar
    glutDisplayFunc(myDisplay); //Display callback function

    //Initiate glad by calling gladLoadGL(void)
    //We have to initialize OpenGL before starting glad. Error checking
    if (!gladLoadGL()) {
        printf("Something went wrong!\n");
        exit(-1);
    }

    //print out OpenGL and GLSL versions
    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
        glGetString(GL_SHADING_LANGUAGE_VERSION));
    //Setup the shaders and create my program object
    //Program objects and shaders must be created after glad has been initiated!!!
    
    programID = glCreateProgram(); //此程式的id(gpu)
    GLuint vertID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragID = glCreateShader(GL_FRAGMENT_SHADER);
    
    //開檔讀黨
    fstream file("Phong.frag",std::fstream::in | std::fstream::ate); //ate -> 移到最後面
    int ffsz = file.tellg();  //回傳當前指針位置( ate在尾巴 -> 檔案大小 )
    file.seekg(0); //移到頭
    string fragS;
    fragS.resize(ffsz);
    file.read((char*)fragS.data(), ffsz);
    file.close();

    file = fstream("Phong.vert", std::fstream::in | std::fstream::ate);
    int vfsz = file.tellg();
    file.seekg(0); //移到頭
    string vertS;
    vertS.resize(vfsz);
    file.read((char*)vertS.data(), vfsz);
    file.close();

    char* vertS_ = (char*)vertS.data();
    glShaderSource(vertID, 1, &vertS_, &vfsz);
    glCompileShader(vertID);

    char* fragS_ = (char*)fragS.data();
    glShaderSource(fragID, 1, &fragS_, &ffsz);
    glCompileShader(fragID);

    glAttachShader(programID, vertID);
    glAttachShader(programID, fragID);

    glLinkProgram(programID);

    glUseProgram(programID);
    //The main loop.
    myInit();
    glutKeyboardFunc(keybaord_fun);
    glutKeyboardUpFunc(keyboardUp_func);
    glutMainLoop();
    return 0;
}