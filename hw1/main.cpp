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
#include "myobj.h"
#include"robot.h"
#define   PI   3.1415927

//object
#define CUBE 0

using namespace std;
//read file
//struct vertex {
//    float a, b, c;
//    vertex(float a_ = 0, float b_ = 0, float c_ = 0) {
//        a = a_;
//        b = b_;
//        c = c_;
//    }
//};
GLuint programID;
//mesh* cube,* solidsphere, *cylinder, *solidtorus_5_025;
float   eyeAngx = 0.0, eyeAngy = 0.0, eyeAngz = 0.0;
float   u[3][3] = { {1.0,0.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0} };
float   eye[3] = { 30.0, 10.0, 80.0 };
float   cv = cos(5.0 * PI / 180.0), sv = sin(5.0 * PI / 180.0); /* cos(5.0) and sin(5.0) */
void keybaord_fun(unsigned char key, int X, int Y) {
    cout << (int)key << "\n";
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
    //glutPostRedisplay();
}
//mesh* getMesh(string fname) {
//    //cout << fname << "\n";
//    freopen(fname.c_str(), "r", stdin);
//    vector<float>tp;
//    string s;
//    float x, y, z;
//    vector<vertex> v(1), vt(1), vn(1);
//    while (cin >> s) {
//        cout << fname << "\n";
//        if (s == "v") {
//            cin >> x >> y >> z;
//            v.push_back(vertex(x, y, z));
//        }
//        else if (s == "vt") {
//            cin >> x >> y;
//            vt.push_back(vertex(x, y));
//        }
//        else if (s == "vn") {
//            cin >> x >> y >> z;
//            vn.push_back(vertex(x, y, z));
//        }
//        else if (s == "f") {
//            char x;
//            int a,b,c;
//            for (int i = 0; i < 9; i += 3) {
//                cin >> a >> x >> b >> x >> c;
//                tp.push_back(v[a].a); tp.push_back(v[a].b); tp.push_back(v[a].c);
//                tp.push_back(vt[b].a); tp.push_back(vt[b].b);
//                tp.push_back(vn[c].a); tp.push_back(vn[c].b); tp.push_back(vn[c].c);
//            }
//        }
//    }
//    fclose(stdin);
//    cin.clear();
//    mesh* m = new mesh(programID, tp);
//    cout << tp.size() << "\n";
//    return m;
//}
robot* myRobot;
myobj* myObjj;
void myInit() {
    myObjj = new myobj(programID);
    myRobot = new robot(programID);
    //cube = getMesh(".\\model\\cube.obj");
    //cube = getMesh(".\\model\\cube.obj");
    //solidsphere = getMesh(".\\model\\solidsphere.obj");
    //cube = getMesh(".\\model\\cube.obj");
    //cylinder = getMesh(".\\model\\cylinder.obj");
    //solidtorus_5_025 = getMesh(".\\model\\solidtorus_0.5_0.025.obj");
    
}
void myDisplay(void)
{
    //cout << eye[0] << " " << eye[1] << " " << eye[2] << "\n";
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.10, 0.10, 0.10, 1.0);  //Dark grey background
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

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
    float eyeMtx[16];
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
    glTranslatef(-100, 10, 0);
    glScalef(100, 100, 100);
    //float objMtx[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
    glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);

    //myObjj->solidtorus_5_025->draw(programID);
   
    myRobot->draw(programID);
    /*
    glBegin(GL_TRIANGLES);
        glVertex3f(1,0,0);
        glVertex3f(0,1,0);
        glVertex3f(1,1,0);
    glEnd();
    */
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
    glutMainLoop();
    return 0;
}