#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GL/glut.h>

#include <vector>
#include <cstdlib> /* �üƬ������ */
#include <ctime>   /* �ɶ�������� */
#include<stb_image.h>

#define STB_IMAGE_IMPLEMENTATION

#define   PI   3.1415927
//�w�q�C��
#define ICE_COLOR       100
#define WAND_WOOD_COLOR 102
#define ROBOT_BLUE_MAIN 103
#define ROBOT_BLUE_SUB  104
#define ROBOT_PINK_MAIN 105
#define ROBOT_PINK_SUB  106
#define HOME_COLOR      107

//�w�q����
#define WHITE           0
#define GOLD            150
#define SILVER          151
#define PERL            152
#define CYAN_RUBBER     153
#define RED_PLASTIC     154
#define COPPER          155
#define BLACK_PLACTIC   156

//���ʤ覡
#define WALK 0
#define RUN  1
#define TURN 2   //��
#define FLY  3   //����

//�ɶ��Ҧ�
#define RUNTIMER 50             //�P�_�O�_�]�]�]
#define JUMPTIMER 51            //�@���
#define JUMPONWANDTIMER 52      //���W�k��
#define JUMPTOFLOORTIMER 53     //���^�a�O
#define GRASSLAND_ANIMATION 54           //�Ȥl�\��
#define DEBUG_MODE 55           //debug�}�Ұʵe
#define OUT_LINE_FRONT   56           //debug mode �I����� �e
#define OUT_LINE_BACK    57           //debug mode �I����� ��
#define OUT_LINE_LEFT    58           //debug mode �I����� ��
#define OUT_LINE_RIGHT   59           //debug mode �I����� �k
#define TREE_LIGHT       60
#define FIREWORKLIGHT    61
#define FIREWORK0         70
#define FIREWORK1         71
#define FIREWORK2         72
#define FIREWORK3         73
#define FIREWORK4         74
#define FIREWORK5         75

//����� todo:�٦��pbug q
#define LOCK true
#define UNLOCK false

//�����Ҧ���� 
#define MAGICFIELD 0
#define GRASSLAND 1

//define texture dimension
#define   TSIZE0  64 
#define   TSIZE1  64

//pattern
#define NONE 0x3f3f3f3f3f
#define CHECKBOARD_BLUE 0
#define HEART_PINK 1
#define FLOWER 2
#define WOOD_FLOOR 3
#define POOL 4
#define FOREST 5
#define STARSKY 6
#define ELF 7
#define EEVEE_1 8
#define EEVEE_2 9
#define EEVEE_3 10
#define EEVEE_4 11
#define EEVEE_5 12
#define EEVEE_6 13


using namespace std;
//�w�qcube
float  points[][3] = { {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0},       //coordinates of the 8 corners.(0,0,0)-(1,1,1)
                      {1.0, 0.0, 1.0}, {0.0, 0.0, 1.0},
                      {0.0, 1.0, 0.0}, {1.0, 1.0, 0.0},
{1.0, 1, 1.0}, {0.0, 1.0, 1.0} };

int    face[][4] = { {0, 1, 2, 3}, {7, 6, 5, 4}, {0, 4, 5, 1},  //Define 6 faces using the 8 corners (vertices)
                    {1, 5, 6, 2}, {3, 2, 6, 7}, {0, 3, 7, 4} };
int    cube[6] = { 0, 1, 2, 3, 4, 5 };                          //Define the cube using the 6 faces.
int    width = 700, height = 700;                               //window shape 
float  pos[3] = { 0.0, 0.0, 0.0 };                              //��m
float  anglex = 0.0, angley = 0.0;
bool tree_rand = 0;

/*-----Translation and rotations of eye coordinate system---*/
#define _l 0
#define _r 1
#define _b 2
#define _t 3
#define _n 4
#define _f 5

float   eyeDx = 0.0, eyeDy = 0.0, eyeDz = 0.0;
float   eyeAngx = 0.0, eyeAngy = 0.0, eyeAngz = 0.0;
double  Eye[3] = { 30.0, 10.0, 80.0 }, Focus[3] = { 0.0, 0.0, 0.0 },
Vup[3] = { 0.0, 1.0, 0.0 };
double mtx[16] = { 0 };
float eyeMtx[16] = { 0 };
float   u[3][3] = { {1.0,0.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0} };
float   eye[3];
float   cv, sv; /* cos(5.0) and sin(5.0) */
int viewStyle = 5;    //viw change: 
                      //magic - 0x/1y/2z/3perspective/4all/5my
                      //grass - 0per/1my
bool camera_show = 0, view_volume_show = 0;
double clippingWindowPerspective[6] = { 0 };   //�z�g��v window�j�p
double clippingWindowOrtho[6] = { 0 };         //�����v window�j�p
/*----------------------------------------------------------*/

/*-----���Ѽ�-----*/
bool isLighting = 1;
/*----Define normals of faces ----*/
float  normal[][4] = { {0.0, -1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, -1.0},
              {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, {-1.0, 0.0, 0.0} };

/*----Define light source properties -------*/
float  litdir_init[] = {30,0,30,0};  //�쥻/�Ӫ���m
float  litdir_position[] = { 80.0, 0.0, 30.0, 0.0 }; 
float  litdir_direction[] = { 50,0,0,0 };              //pos - init
float  litdir_diffuse[] = { 1,1,1,1 };
float  litdir_specular[] = { 1,1,1,1 };
float  litdirColor[] = { 1,69 / 255.0,0};
float  litdirAng = 0;
float  litdirR = 50;
float  litdirIntensity = 0.7;
bool   isLitdirOpen = 1;
bool   isLitdirColorOpen = 0;

float  litpos_position[] = { 30,70,30,1.0 };
float  litpos_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
float  litpos_specular[] = { 0.7, 0.7, 0.7, 1.0 };
float  litposColor[] = { 25 / 255.0,25 / 255.0,112 / 255.0 };
float  litposIntensity = 0.7;
bool   isLitposOpen = 0;
bool   isLitposColorOpen = 0;

float  litspot_position[] = { 0,0,0,1.0 };
float  litspot_direction[] = { 0,0,3,0 };      //0,0,0 -> 0,-1,1
float  litspot_diffuse[] = { 1, 1, 1, 1.0 };
float  litspot_specular[] = { 1, 1, 1, 1.0 };
float  litspotIntensity = 0.7;
float  litspotCutoffAng = 45;
bool   isLitspotOpen = 0;
float  litspotAng = 0;

float  litfire_position[] = { 100,0,0,1.0 };
float  litfire_diffuse[] = { 0, 0, 0, 1.0 };
float  litfire_specular[] = { 0, 0, 0, 1.0 };
//float  litfireColor[] = { 25 / 255.0,25 / 255.0,112 / 255.0 };
float  litfireIntensity = 0;
bool   isLitfireOpen = 0;
bool   isLitfireColorOpen = 0;
int    litfireCnt = 0;

//float  global_ambient[] = { 0, 0, 0, 1.0 };
float  global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
/*----------------------------------------------------------*/

/*-----textures �Ѽ�-----*/
unsigned int   textName[100];                   /* declare many texture maps*/
unsigned char  checkboard[TSIZE0][TSIZE0][4];   /* checkboard textures */
unsigned char  dot[TSIZE1][TSIZE1][4];        /* brick wall textures */

int image_width, image_height, nrChannels;
unsigned char* image_data;
int poolAng = 0;
bool isFogOpen = 0;
int fogMode = 0;
float fogOpacity = 0.1;
int fogColorMode = 0;
float fogColor[4] = { 1,1,1,0.1 };
int eevee_ani = 14;
/*---- the axes of billboard ----*/
float  a[3], b[3];

//Define GLU quadric objects, a sphere and a cylinder
GLUquadricObj* sphere = NULL, * cylind = NULL, * mycircle = NULL;
int see = 0;             //��������(�}�o�a�ϥ� ��ڵL���\��)                                     
int preKey = 0;          //�W�@�ӫ���׽�
int scene = MAGICFIELD;  //��l�I�����]�k�}
bool isLock = 0;         //����O�_��F
bool sitOnChair = 0;
bool debugMode = 0;
int debugModeCmd = 0;
void draw_magic_field();
void draw_cube();
void draw_cylinder(double up, double down, double height);
void change_color(int value);
void change_color_material(int value);
void draw_circle(double size, int wid);
void draw_square(int hei, int wid, int sz);
void draw_view_volume();
void draw_tree(int button, int height);

/*-------------------------------------------------------
 * Procedure to compute the a[] and b[] axes of billboard
 * after eye parameters are specified.
 */
void compute_ab_axes(void)
{
    float  w0, w2;
    double len;

    /*----Get w0 and w2 from the modelview matrix mtx[] ---*/
    w0 = eyeMtx[2]; w2 = eyeMtx[10];

    len = sqrt(w0 * w0 + w2 * w2);
    /*---- Define the a and b axes for billboards ----*/
    b[0] = 0.0; b[1] = 1.0; b[2] = 0.0;
    a[0] = w2 / len; a[1] = 0.0; a[2] = -w0 / len;
}
/*--------------------------------------------------------
 * Procedure to draw a billboard, center=[x,z], width=w,
 * height = h;
 */
void draw_board() {
    glNormal3f(0, 0, 1);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0, 0.0); glVertex3f(0, 1, 0);
    glTexCoord2f(1.0, 1.0); glVertex3f(0, 0, 0);
    glTexCoord2f(0.0, 1.0); glVertex3f(1, 0, 0);
    glTexCoord2f(0.0, 0.0); glVertex3f(1, 1, 0);
    glEnd();
}
void draw_billboard(float x, float z, float w, float h)
{
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
    glColor3f(1,1,1);
    glNormal3f(0,0,1);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1.0); glVertex3fv(v0);
    glTexCoord2f(1.0, 1.0); glVertex3fv(v1);
    glTexCoord2f(1.0, 0.0); glVertex3fv(v2);
    glTexCoord2f(0.0, 0.0); glVertex3fv(v3);
    glEnd();
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
}
void make_checkboard_blue()
{
    //rgb(148, 255, 250)
    int   i, j, r,g,b;
    for (i = 0; i < TSIZE0; i++) {
        for (j = 0; j < TSIZE0; j++) {
            r = g = b = 255;
            if (((i & 16) == 0) ^ ((j & 16) == 0)) {
                r = 148;
                g = 255;
                b = 250;
            }
            checkboard[i][j][0] = r;
            checkboard[i][j][1] = g;
            checkboard[i][j][2] = b;
            checkboard[i][j][3] = 255;
        }
    }
}
void make_heart_pink()
{
    int   i, j, k,kk,r, g, b;
    for (i = 0; i < TSIZE1; i++) {
        for (j = 0; j < TSIZE1; j++) {
            dot[i][j][0] = 255;
            dot[i][j][1] = 255;
            dot[i][j][2] = 255;
            dot[i][j][3] = 255;
        }
    }

    //rgb(255, 148, 201)
    for (i = 0; i < TSIZE1; i++) {
        for (j = 0; j < TSIZE1; j++) {
            if (i % 8 == 4 && j%8 == 4) {
                r = 255;
                g = 148;
                b = 201;
                for (k = -2; k <= 2; k++) {
                    if (k == 0)continue;
                    dot[i + 2][k + j][0] = r;
                    dot[i + 2][k + j][1] = g;
                    dot[i + 2][k + j][2] = b;
                }
    
                for (k = -3; k <= 3; k++) {
                    for ( kk = 1; kk >= 0; kk--) {
                        dot[i + kk][k+j][0] = r;
                        dot[i + kk][k+j][1] = g;
                        dot[i + kk][k+j][2] = b;
                    }
                }

                for (k = -1; k >= -3; k--) {
                    if (k == -1) kk = 2;
                    if (k == -2) kk = 1;
                    if (k == -3) kk = 0;
                    for (int kkk = kk; kkk >= -kk; kkk--) {
                        dot[i + k][kkk + j][0] = r;
                        dot[i + k][kkk + j][1] = g;
                        dot[i + k][kkk + j][2] = b;
                    }
                }
    
            }
        }
    }

}
void create_texture() {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glGenTextures(100, textName);

    make_checkboard_blue();
    glBindTexture(GL_TEXTURE_2D, textName[CHECKBOARD_BLUE]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, TSIZE0, TSIZE0, GL_RGBA, GL_UNSIGNED_BYTE, checkboard);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 1, GL_RGBA, GL_UNSIGNED_BYTE, checkboard);

    make_heart_pink();
    glBindTexture(GL_TEXTURE_2D, textName[HEART_PINK]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, TSIZE1, TSIZE1, GL_RGBA, GL_UNSIGNED_BYTE, dot);


    //image_data = stbi_load("C:\\Users\\WANG\\source\\repos\\cg_test\\x64\\Debug\\grass.jpeg", &image_width, &image_height, &nrChannels, 0);
    image_data = stbi_load("image\\flower.png", &image_width, &image_height, &nrChannels, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, textName[FLOWER]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    }

    //image_data = stbi_load("C:\\Users\\WANG\\source\\repos\\cg_test\\x64\\Debug\\wood.jpg", &image_width, &image_height, &nrChannels, 0);
    image_data = stbi_load("image\\wood.jpg", &image_width, &image_height, &nrChannels, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, textName[WOOD_FLOOR]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    }

    //image_data = stbi_load("C:\\Users\\WANG\\source\\repos\\cg_test\\x64\\Debug\\pool.jpg", &image_width, &image_height, &nrChannels, 0);
    image_data = stbi_load("image\\pool.jpg", &image_width, &image_height, &nrChannels, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, textName[POOL]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    }

    image_data = stbi_load("image\\forest.png", &image_width, &image_height, &nrChannels, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, textName[FOREST]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    }


    image_data = stbi_load("image\\starsky.jpg", &image_width, &image_height, &nrChannels, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, textName[STARSKY]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    }

    image_data = stbi_load("image\\elk.png", &image_width, &image_height, &nrChannels, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, textName[ELF]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    }

    image_data = stbi_load("image\\eevee_1.png", &image_width, &image_height, &nrChannels, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, textName[EEVEE_1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    }

    image_data = stbi_load("image\\eevee_2.png", &image_width, &image_height, &nrChannels, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, textName[EEVEE_2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    }

    image_data = stbi_load("image\\eevee_3.png", &image_width, &image_height, &nrChannels, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, textName[EEVEE_3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    }


    image_data = stbi_load("image\\eevee_4.png", &image_width, &image_height, &nrChannels, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, textName[EEVEE_4]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    }

    image_data = stbi_load("image\\eevee_5.png", &image_width, &image_height, &nrChannels, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, textName[EEVEE_5]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    }

    image_data = stbi_load("image\\eevee_6.png", &image_width, &image_height, &nrChannels, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, textName[EEVEE_6]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    }


    for (int i = 0; i <= 55; i++) {
        string s1 = "image\\eevee_ani_",s2 = "";
        int tp = i;
        while (tp) {
            s2 += (tp % 10) + '0';
            tp /= 10;
        }
        reverse(s2.begin(),s2.end());
        if (i == 0) s2 = "0";
        string s3 = ".png";
        s1 = s1 + s2 + s3;
        image_data = stbi_load(s1.c_str(), &image_width, &image_height, &nrChannels, 0);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glBindTexture(GL_TEXTURE_2D, textName[14+i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        if (nrChannels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        }
        else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
        }
    }

    //image_data = stbi_load("eevee_ani_1.png", &image_width, &image_height, &nrChannels, 0);
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    //glBindTexture(GL_TEXTURE_2D, textName[14]);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

}


float getDis(float x1, float y1, float x2, float y2) {           //��Z��
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}
struct node {   //�w�q���y�Ъ��I
    double x = 0, y = 0, z = 0;
};
struct pp {   //�U�ئ�m���c
    float x1 = 0, x2 = 0, tp1 = 0, tp2 = 0;
    float x = 0, z = 0, r = 0;
    float a1 = 0, a2 = 0;
    pp(float x1_, float tp1_, float x2_, float tp2_) {      //�x��
        x1 = x1_;
        x2 = x2_;
        tp1 = tp1_;
        tp2 = tp2_;
    }
    pp(float x_, float z_, float r_, float a1_, float a2_) {  //�y
        x = x_;
        z = z_;
        r = r_;
        a1 = a1_;
        a2 = a2_;
    }
};
//�e�D
const vector<pp> river = { {-10,120 ,100,0,25},{190,120 ,100,180,220}, {24,9 ,100,-3,30}, {100,70 ,100,75,115}, {130,267 ,100,268,290},{200,80 ,100,93,112} };
const vector<pp> rock = { {-10,120 ,100,0,25},{190,120 ,100,180,220}, {24,9 ,100,-3,30},{100,70 ,100,75,96} ,{100,70 ,100,101,115}, {130,267 ,100,268,290},{200,80 ,100,93,112} };
//�W�l
const vector<pair<float, float>> pillar = { {110,100},{180,100},{110,160},{180,160} };
//�����m���y��
vector<pair<float, float>>limit;
void setMaterial(float spr, float spg, float spb, float emir, float emig, float emib, float shi) {
    glColorMaterial(GL_FRONT, GL_SPECULAR); glColor3f(spr, spg, spb);
    glColorMaterial(GL_FRONT, GL_EMISSION); glColor3f(emir, emig, emib);
    glMaterialf(GL_FRONT, GL_SHININESS, shi);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}
void setMaterial(float spr, float spg, float spb, float emir, float emig, float emib, float ambr,float ambg,float ambb,float difr,float difg,float difb,float shi) {
    glColorMaterial(GL_FRONT, GL_SPECULAR); glColor3f(spr, spg, spb);
    glColorMaterial(GL_FRONT, GL_EMISSION); glColor3f(emir, emig, emib);
    glMaterialf(GL_FRONT, GL_SHININESS, shi);
    glColorMaterial(GL_FRONT, GL_AMBIENT); glColor3f(ambr, ambg, ambb);
    glColorMaterial(GL_FRONT, GL_DIFFUSE); glColor3f(difr, difg, difb);
}
void setMaterial(float spr, float spg, float spb,float spa, float emir, float emig, float emib, float emia, float ambr, float ambg, float ambb, float amba, float difr, float difg, float difb, float difa,float shi) {
    glColorMaterial(GL_FRONT, GL_SPECULAR); glColor4f(spr, spg, spb,spa);
    glColorMaterial(GL_FRONT, GL_EMISSION); glColor4f(emir, emig, emib,emia);
    glMaterialf(GL_FRONT, GL_SHININESS, shi);
    glColorMaterial(GL_FRONT, GL_AMBIENT); glColor4f(ambr, ambg, ambb,amba);
    glColorMaterial(GL_FRONT, GL_DIFFUSE); glColor4f(difr, difg, difb,difa);
}
node ball_cor(double r, int A, int B) {          //���y���ഫ
    node rt;
    rt.x = r * sin(A * 0.01745) * cos(B * 0.01745);
    rt.y = r * sin(A * 0.01745) * sin(B * 0.01745);
    rt.z = r * cos(A * 0.01745);
    return rt;
}
struct elf {
    void draw() {
        glPushMatrix();
        setMaterial(0.4, 0.4, 0.4, 0, 0, 0, 120);
        change_color(ROBOT_BLUE_MAIN);
        glPushMatrix();                 //�Y
        glutSolidSphere(1, 50, 50);
        glPopMatrix();

        change_color(ROBOT_BLUE_SUB);
        glPushMatrix();                 //�~�k��
        glTranslatef(0.5,0.6,-0.2);
        glRotatef(20,0,0,1);
        glScalef(0.8, 0.8, 0.4);
        draw_cube();

        glColor3f(167 / 255.0, 167 / 255.0, 167 / 255.0);            //����
        glPushMatrix();
        glTranslatef(0.2, 0.2, 0.7);
        glScalef(0.6,0.6,0.4);
        draw_cube();
        glPopMatrix();
        glPopMatrix();

        change_color(ROBOT_BLUE_SUB);
        glPushMatrix();
        glTranslatef(-0.5, 0.6, 0.2);
        glRotatef(-20, 0, 0, 1);
        glRotatef(180, 0, 1, 0);
        glScalef(0.8, 0.8, 0.4);
        draw_cube();

        glColor3f(167 / 255.0, 167 / 255.0, 167 / 255.0);            //����
        glPushMatrix();
        glTranslatef(0.2, 0.2, -0.1);
        glScalef(0.6, 0.6, 0.4);
        draw_cube();
        glPopMatrix();
        glPopMatrix();
        setMaterial(0, 0, 0, 0, 0, 0, 0);
        //����
        glColor3f(0,0,0);
        glPushMatrix();
        glTranslatef(0.4,0.15,1);
        glScalef(0.2,0.4,0.2);
        glutSolidSphere(0.5, 10, 10);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-0.4, 0.15, 1);
        glScalef(0.2, 0.4, 0.2);
        glutSolidSphere(0.5, 10, 10);
        glPopMatrix();

        //�|��
        //#FFAAD5
        glColor3f(1, 168/255.0, 212/255.0);
        glPushMatrix();
        glTranslatef(0.7, -0.2, 0.8);
        glScalef(0.4, 0.2, 0.2);
        glutSolidSphere(0.5, 10, 10);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-0.7, -0.2, 0.8);
        glScalef(0.4, 0.2, 0.2);
        glutSolidSphere(0.5, 10, 10);
        glPopMatrix();

        glPopMatrix();
    }
}camera;
struct magic_wand {
    float x = 0, y = 0, z = 0;                    //�ۤv���y��
    float angle_x = 0, angle_y = 0, angle_z = 0;
    float scale = 0;
    bool show = 1;   //�O�_���
    magic_wand(float s_ = 1, float x = 0, float y = 0, float z = 0) {
        scale = s_;
        angle_x = x;
        angle_y = y;
        angle_z = z;
    }
    void setPos(int x_, int y_, int z_) {
        x = x_;
        y = y_;
        z = z_;
    }
    void draw() {
        //x�k y�W z�e �����I:�k��������
        //���Y
        setMaterial(0,0,0,0,0,0,0);
        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
        
        change_color(WAND_WOOD_COLOR);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glRotatef(angle_x, 1, 0, 0);
        glRotatef(angle_y, 0, 1, 0);
        glRotatef(angle_z, 0, 0, 1);
        glScalef(scale, scale, scale);

        glTranslatef(0, 0, -7);
        draw_cylinder(0.5, 0.8, 14);             //���׬�14 �e��0.8

        glColor3f(102 / 255.0, 34 / 255.0, 0);   //�k����������
        glPushMatrix();

        glTranslatef(0, 0, 0);                   //�k����������
        glRotatef(270, 1, 0, 0);                 //��ί���
        draw_circle(0.5, 1);
        glPopMatrix();

        glColor3f(102 / 255.0, 34 / 255.0, 0);   //�k����������
        glPushMatrix();
        glTranslatef(0, 0, 14);
        glRotatef(270, 1, 0, 0);                 //��ί���
        draw_circle(0.8, 1);
        glPopMatrix();

        //�����n�Ϯg �Ϯg�E�J ����o��
        setMaterial(0.316228, 0.316228, 0.316228, 1, 1, 168 / 255.0, 64);
        glColor3f(1, 1, 168 / 255.0);              //����(��)
        glPushMatrix();
        glTranslatef(0, 0, 14);
        glutSolidSphere(1.5, 10, 10);              //9.5

        setMaterial(0, 0, 0, 0, 0, 0, 0);
        glColor3f(168 / 255.0, 1.0, 1);            //�{����(�Ŧ�)
        glPushMatrix();
        glRotatef(30, 0, 1, 0);
        glutSolidTorus(0.1, 2.5, 100, 100);
        glPopMatrix();

        glColor3f(168 / 255.0, 1.0, 1);           //�{����(�Ŧ�)
        glPushMatrix();
        glRotatef(330, 0, 1, 0);
        glutSolidTorus(0.1, 2.5, 100, 100);
        glPopMatrix();
        glPopMatrix();
    }
}myMagic_wand;
struct robot {
    magic_wand* magic_wand_carry = new magic_wand(0.34, 180, 0, 0); //�]�k�Ϊ���(���)
    magic_wand* magic_wand_sit = new magic_wand(0.5, 0, -90, 0);    //�]�k�Ϊ���(����)
    int mainColor = ROBOT_BLUE_MAIN, subColor = ROBOT_BLUE_SUB;    //�C��
    int moveMode = 0; //���ʼҦ� 0->walk  1->run  2->turn  3->fly
    robot() {
        stand();
    }
    float angle_x = 0.0, angle_y = 0.0;                         //���ਤ��
    bool carry_mw = 0;                                          //�ثe�����]�k�ζ�
    bool isMagician = 0;                                        //�O���O�]�k�v
    bool isJump_ready = 0;                                      //�ǳƸ������ն�
    float x = 0, y = 0, z = 0;                                  //��������H�y��
    struct hand {
        node tp;
        float shoulderAng_x = 180, shoulderAng_y = 0, shoulderAng_z = 35;    //�ӻH
        float elbowAng_x = 0, elbowAng_y = 0, elbowAng_z = 0;                //��y
        float fingerAng_x = 0, fingerAng_y = 0, fingerAng_z = 0;             //���
        int subColor = 0;
        void draw() {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            //glRotatef(shoulderAng_x, 1, 0, 0);           //�������`���� 180��U
            glRotatef(shoulderAng_z, 0, 0, 1);             //-35��b������� ��z�౱��k
            glRotatef(shoulderAng_x, 1, 0, 0);
            change_color(subColor);
            glutSolidSphere(0.5, 10, 10);                  //�b�|�� 0.5 ���ӻH

            //forarms
            glColor3f(167 / 255.0, 167 / 255.0, 167 / 255.0);
            glTranslatef(0, 0.75, 0);              //���� �ӻH�W��0.25 + �ꤤ��0.75(�e1.5�����u) - 0.25���|

            glPushMatrix();
            glScalef(0.7, 1.5, 0.7);
            glutSolidSphere(0.5, 10, 10);          //���|��1.5�����u
            glPopMatrix();

            glTranslatef(0, 0.75, 0);               //���u�e�ݤ���

            glRotatef(elbowAng_x, 1, 0, 0);         //�����y
            glRotatef(elbowAng_y, 0, 1, 0);
            glRotatef(elbowAng_z, 0, 0, 1);

            change_color(subColor);

            glutSolidSphere(0.25, 10, 10);          //���|�� 0.5 ����y      0.25���|

            //��e�u
            glColor3f(167 / 255.0, 167 / 255.0, 167 / 255.0);
            glTranslatef(0, 0.75, 0);                //���� ��b�e��0.25 + �ꤤ��0.75(�e1.5����e�u) - 0.25���|

            glPushMatrix();
            glScalef(0.55, 1.5, 0.55);
            glutSolidSphere(0.5, 10, 10);           //���|��1.5����e�u
            glPopMatrix();

            glTranslatef(0, 0.75, 0);               //��e�u�e��   
            //�������V���Ӧb�o��
            glRotatef(fingerAng_y, 0, 1, 0);
            //������Y
            change_color(subColor);
            glPushMatrix();
            glTranslatef(-0.25, 0.2, 0);
            glRotatef(15, 0, 0, 1);               //�i�}����
            glScalef(0.3, 0.7, 0.3);              //�����: 0.7
            glutSolidSphere(0.5, 10, 10);
            glPopMatrix();

            //�k����Y
            glPushMatrix();
            glTranslatef(0.25, 0.2, 0);
            glRotatef(-15, 0, 0, 1);              //�i�}����
            glScalef(0.3, 0.7, 0.3);              //�����: 0.7
            glutSolidSphere(0.5, 10, 10);
            glPopMatrix();
            //glRotatef(-shoulderAng_z, 0, 0, 1);   //�ܦ^���`���y�Шt��
        }
    };
    struct foot {          //�L��2.75
        node tp;
        float hipJointAng_x = 180, hipJointAng_y = 0, hipJointAng_z = 0;   //�b���`
        float kneeAng_x = 0, kneeAng_y = 0, kneeAng_z = 0;
        float ankle_x = 0, ankle_y = 0, ankle_z = 0;
        int subColor = 0;
        void draw() {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glRotatef(hipJointAng_x, 1, 0, 0);          //�������`���� 180��U
            //glRotatef(-30, 0, 0, 1);            
            //glRotatef(90, 1, 0, 0);             //���e  ��x�b�౱���|�_��U
            change_color(subColor);
            glutSolidSphere(0.25, 10, 10);        //�j�L�W�����` 0.5���|

            //�j�L
            glColor3f(167 / 255.0, 167 / 255.0, 167 / 255.0);
            glTranslatef(0, 0.625, 0);           //���� ���\0.125 + �ꤤ��0.5(�e1����)

            glPushMatrix();
            glScalef(0.7, 1, 0.7);               //�j�L��1
            glutSolidSphere(0.5, 10, 10);
            glPopMatrix();

            glTranslatef(0, 0.5, 0);             //�L�e�ݤ���  �M���\���|0.25

            glRotatef(kneeAng_x, 1, 0, 0);       //���\����
            //���\
            change_color(subColor);
            glutSolidSphere(0.25, 10, 10);       //���|0.5�����\

            glTranslatef(0, 0.5, 0);             //���\�e�� 0.25 + �L����0.5(�L��1) - 0.25(���|�a��)

            glColor3f(167 / 255.0, 167 / 255.0, 167 / 255.0);
            glPushMatrix();
            glScalef(0.5, 1, 0.5);               //�p�L��1
            glutSolidSphere(0.5, 10, 10);
            glPopMatrix();

            glTranslatef(0, 0.5, 0);             //�p�L�e�ݤ��� 0.5 �M�L���|0.25
            glColor3f(0.5, 0.5, 0.5);
            glutSolidSphere(0.5, 10, 10);        //�} ���|1
        }
    };
    hand* right_h = new hand;
    hand* left_h = new hand;
    foot* left_f = new foot;
    foot* right_f = new foot;
    void setColor(int m, int s) {
        mainColor = m;
        subColor = s;
    };
    void draw_hat() {
        glPushMatrix();           //push1
        glRotatef(270, 1, 0, 0);  //�ઽ ���W�e
        glRotatef(-10, 1, 0, 0);
        //draw_cylinder(0,3,6); �Ȥl�ĪG
        //glRotatef(80, 1, 0, 0);

        glColor3f(38 / 255.0, 38 / 255.0, 38 / 255.0);   //�U�u
        glPushMatrix();           //push2
        draw_cylinder(3, 0, 1);

        glColor3f(61 / 255.0, 61 / 255.0, 61 / 255.0);  //�U�l
        draw_cylinder(2, 0, 4);

        //�᯾
        glColor3f(1, 1, 168 / 255.0);                    //�e
        glTranslatef(0, 0, 0.5);
        draw_cylinder(2, 1, 0.8);

        change_color(subColor);          //��
        glTranslatef(0, 0, 0.6);
        draw_cylinder(1.5, 0.8, 0.6);
        glPopMatrix();                  //pop2
        glPopMatrix();                  //pop1
    }
    void draw() {
        right_h->subColor = subColor;
        left_h->subColor = subColor;
        left_f->subColor = subColor;
        right_f->subColor = subColor;

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //�{�l

        //setMaterial(0.4, 0.4, 0.4, 0, 0, 0, 120);

        if (isLighting)setMaterial(0, 0, 0, 0.0f, 0.0f, 0.0f, 194 / 255.0, 223 / 255.0, 1.0, 1, 1, 1, 12.8f);
        else change_color(mainColor);

        glPushMatrix();
        glScalef(2.5, 2.5, 2.5);
        glTranslatef(x, y, z);                      //�㰦���y��

        glRotatef(angle_y, 0, 1, 0);
        glRotatef(angle_x, 1, 0, 0);

        glTranslatef(0, 4.75, 0);                   //����{�l
        glutSolidSphere(2, 50, 50);                 //�e�{�l ���|4

        setMaterial(0.4, 0.4, 0.4, 0, 0, 0, 120);
        node tp = ball_cor(2, 90, 30);
        glPushMatrix();
        glTranslatef(tp.x, tp.y, tp.z);             //����k�ӻH
        right_h->draw();

        glPushMatrix();                             //push3
        glTranslatef(0, 0.3, 0);
        if (carry_mw && isMagician) magic_wand_carry->draw();
        glPopMatrix();                              //pop3

        glPopMatrix();                              //pop2

        //�k��������� �n���s��
        setMaterial(0.4, 0.4, 0.4, 0, 0, 0, 120);
        //���ӻH
        tp = ball_cor(2, 270, 330);
        glPushMatrix();
        glTranslatef(tp.x, tp.y, tp.z);       //���쥪�ӻH
        left_h->draw();
        glPopMatrix();

        glPushMatrix();                       //���b�k���W 
        glTranslatef(0, -2, 0);
        if (isOnWand) magic_wand_sit->draw();
        glPopMatrix();

        //�k��������� �n���s��
        setMaterial(0.4, 0.4, 0.4, 0, 0, 0, 120);
        //���j�L�W�������`  �I�b�����
        glPushMatrix();
        glTranslatef(-0.4, -1.75, 0);
        left_f->draw();
        glPopMatrix();

        //�k�j�L�W�������`
        glPushMatrix();
        glTranslatef(0.4, -1.75, 0);
        right_f->draw();
        glPopMatrix();

        //�Y
        if (isLighting)setMaterial(0, 0, 0, 0.0f, 0.0f, 0.0f, 194 / 255.0, 223 / 255.0, 1.0, 1, 1, 1, 12.8f);
        else change_color(mainColor);

        //change_color(mainColor);
        glTranslatef(0, 3, 0);               //�b�����Y  �M���魫�|0.5
        glPushMatrix();
        glutSolidSphere(1.5, 50, 50);        //���|3

        //����
        setMaterial(0, 0, 0, 0, 0, 0, 0);
        if (isMagician)
            glColor3f(128 / 255.0, 128 / 255.0, 1);  //��
        else
            glColor3f(0, 0, 0);
        glPushMatrix();                          //����  push3
        glTranslatef(0.6, 0, 1.3);
        glScalef(0.4, 0.8, 0.4);
        glutSolidSphere(0.5, 10, 10);
        glColor3f(1, 1, 1);                      //����
        glTranslatef(0, 0.15, 0.15);
        glutSolidSphere(0.33, 10, 10);
        glPopMatrix();                           //pop3

        setMaterial(0, 0, 0, 0, 0, 0, 0);
        if (isMagician)
            //glColor3f(128 / 255.0, 128 / 255.0, 1);
            glColor3f(1, 122 / 255.0, 189 / 255.0);      //��
        else
            glColor3f(0, 0, 0);
        glPushMatrix();                          //push3
        glTranslatef(-0.6, 0, 1.3);
        glScalef(0.4, 0.8, 0.4);
        glutSolidSphere(0.5, 10, 10);
        glColor3f(1, 1, 1);                      //����
        glTranslatef(0, 0.15, 0.15);
        glutSolidSphere(0.33, 10, 10);
        glPopMatrix();                           //pop3

        glColor3f(1, 0, 0);
        glPushMatrix();                           //push3
        glTranslatef(0, -0.6, 1.5);              //�L��
        glLineWidth(1);
        glBegin(GL_LINES);
        glVertex3f(-0.2, 0, 0);
        glVertex3f(0.2, 0, 0);
        glEnd();
        glPopMatrix();                          //pop3

        glTranslatef(0, 0.5, 0);                //�U�l���Шt
        if (isMagician) draw_hat();

        glPopMatrix();                       //���}�Y pop2

        glPopMatrix();                       //���}�{�l���Шt pop1
    }
    void stand() {                           //�w�q����(��l��)
        y = 0;
        x = 0;
        y = 0;
        angle_x = 0;
        right_f->hipJointAng_x = 180;
        right_f->kneeAng_x = 0;
        left_f->hipJointAng_x = 180;
        left_f->kneeAng_x = 0;
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
    void carryLight() {
        stand();
        right_h->shoulderAng_x = 60;
        right_h->fingerAng_y = 350;
        right_h->shoulderAng_x = 95;
    }
    bool flag = 0, flag2 = 0;
    void move() {
        //(���\,�b���`)
        //�}��(0,180) ~ (35,200)     35 +7   20 +4
        //�}���e(0,180) ~ (35,130)   35 +7   50 -10
        //(�ӻH)
        //�⩹�e(180) ~ (200)        20 +4   100 +20
        //�⩹��(180) ~ (170)        20 -4
        //moveMode = walk
        int hipJointXLimit = 130;
        int hipJointFrontOffset = -10;
        int kneeFrontOffset = 7;
        int hipJointBackOffset = 4;
        int kneeBackOffset = 7;
        int shoulderOffset = -4;
        left_h->elbowAng_x = -20;
        right_h->elbowAng_x = -20;
        if (moveMode == RUN) {
            angle_x = 5;
            hipJointFrontOffset = -20;
            kneeFrontOffset = 14;
            hipJointBackOffset = 8;
            kneeBackOffset = 25;
            shoulderOffset = -20;
            left_h->elbowAng_x = -40;
            right_h->elbowAng_x = -40;
        }
        if (moveMode == TURN) {
            hipJointXLimit = 160;
        }
        if (flag == 0) {
            if (flag2 == 0) {  //�k�}���e�A���}����             
                right_f->hipJointAng_x += hipJointFrontOffset / 2.0;          //�k�}���e��
                right_f->kneeAng_x += kneeFrontOffset / 2.0;
                left_f->hipJointAng_x += hipJointBackOffset / 2.0;            //���}����
                left_f->kneeAng_x += kneeBackOffset / 2.0;
                left_h->shoulderAng_x += shoulderOffset / 2.0;                //���⩹�e
                right_h->shoulderAng_x -= shoulderOffset / 2.0;               //�k�⩹��
                if (right_f->hipJointAng_x <= hipJointXLimit) {               //��ɱ���
                    flag2 = 1;
                }
            }
            else { //�k�}�������I�A���}���e����I
                right_f->hipJointAng_x -= hipJointFrontOffset / 2.0;
                right_f->kneeAng_x -= kneeFrontOffset / 2.0;
                left_f->hipJointAng_x -= hipJointBackOffset / 2.0;
                left_f->kneeAng_x -= kneeBackOffset / 2.0;
                left_h->shoulderAng_x -= shoulderOffset / 2.0;
                right_h->shoulderAng_x += shoulderOffset / 2.0;
                if (right_f->hipJointAng_x >= 180) {
                    flag2 = 0;
                    flag = 1;
                }
            }
        }
        else {   //���}���e�A�k�}���� 
            if (flag2 == 0) {
                left_f->hipJointAng_x += hipJointFrontOffset / 2.0;
                left_f->kneeAng_x += kneeFrontOffset / 2.0;
                right_f->hipJointAng_x += hipJointBackOffset / 2.0;
                right_f->kneeAng_x += kneeBackOffset / 2.0;
                left_h->shoulderAng_x -= shoulderOffset / 2.0;
                right_h->shoulderAng_x += shoulderOffset / 2.0;
                if (left_f->hipJointAng_x <= hipJointXLimit) {
                    flag2 = 1;
                }
            }
            else { //���}�������I�A�k�}���e����I
                left_f->hipJointAng_x -= hipJointFrontOffset / 2.0;
                left_f->kneeAng_x -= kneeFrontOffset / 2.0;
                right_f->hipJointAng_x -= hipJointBackOffset / 2.0;
                right_f->kneeAng_x -= kneeBackOffset / 2.0;
                left_h->shoulderAng_x += shoulderOffset / 2.0;
                right_h->shoulderAng_x -= shoulderOffset / 2.0;
                if (left_f->hipJointAng_x >= 180) {
                    flag2 = 0;
                    flag = 0;
                }
            }
        }
    }
    void jump_ready() {               //�����w�ưʧ@
        left_f->kneeAng_x = 45;
        right_f->kneeAng_x = 45;

        left_f->hipJointAng_x = 150;
        right_f->hipJointAng_x = 150;

        left_h->shoulderAng_x = 200;
        right_h->shoulderAng_x = 200; //180 160
        isJump_ready = 1;
    }
    int jump_cmd = 0;              //fsm
    bool jump() {
        left_h->elbowAng_x = -15;
        right_h->elbowAng_x = -15;
        switch (jump_cmd) {
        case 0:              //���W��  ���`�ܥ��` �⩹�e
            y += 0.5;
            left_f->kneeAng_x -= 45 / 4.0;
            right_f->kneeAng_x -= 45 / 4.0;
            left_f->hipJointAng_x += 30 / 4.0;
            right_f->hipJointAng_x += 30 / 4.0;
            left_h->shoulderAng_x -= 40 / 4.0;
            right_h->shoulderAng_x -= 40 / 4.0;   //160
            if (y == 2) jump_cmd++;
            break;
        case 1:              //��a�O �s��
            y -= 1;
            left_f->kneeAng_x += 60 / 2.0;
            right_f->kneeAng_x += 60 / 2.0;
            left_f->hipJointAng_x -= 40 / 2.0;
            right_f->hipJointAng_x -= 40 / 2.0;
            left_h->shoulderAng_x += 40 / 2.0;
            right_h->shoulderAng_x += 40 / 2.0;  //200
            if (y == 0) jump_cmd++;
            break;
        case 2:
            y += 0.8;
            left_f->kneeAng_x -= 60 / 5.0;
            right_f->kneeAng_x -= 60 / 5.0;
            left_f->hipJointAng_x += 40 / 5.0;
            right_f->hipJointAng_x += 40 / 5.0;
            left_h->shoulderAng_x -= 40 / 5.0;
            right_h->shoulderAng_x -= 40 / 5.0;
            if (y == 4) jump_cmd++;
            break;
        case 3:
            y -= 1;
            left_f->kneeAng_x += 70 / 4.0;
            right_f->kneeAng_x += 70 / 4.0;
            left_f->hipJointAng_x -= 50 / 4.0;
            right_f->hipJointAng_x -= 50 / 4.0;
            left_h->shoulderAng_x += 40 / 4.0;
            right_h->shoulderAng_x += 40 / 4.0;
            if (y == 0) jump_cmd++;
            break;
        case 4:
        case 5:
            left_f->kneeAng_x -= 70 / 2.0;
            right_f->kneeAng_x -= 70 / 2.0;
            left_f->hipJointAng_x += 50 / 2.0;
            right_f->hipJointAng_x += 50 / 2.0;
            left_h->shoulderAng_x -= 20 / 2.0;
            right_h->shoulderAng_x -= 20 / 2.0;
            jump_cmd++;
            break;
        }
        if (jump_cmd == 6) {
            jump_cmd = 0;
            return 1;
        }
        return 0;
    }
    bool isOnWand = 0;             //�O�_���b�k���W
    bool jumpOnWand() {            //���W�k��
        carry_mw = 0;
        isOnWand = 1;
        left_h->elbowAng_x = -15;
        right_h->elbowAng_x = -15;
        switch (jump_cmd) {
        case 0:              //���W��  ���`�ܥ��` �⩹�e
            y += 0.5;
            left_f->kneeAng_x -= 45 / 4.0;
            right_f->kneeAng_x -= 45 / 4.0;
            left_f->hipJointAng_x += 30 / 4.0;
            right_f->hipJointAng_x += 30 / 4.0;
            left_h->shoulderAng_x -= 40 / 4.0;
            right_h->shoulderAng_x -= 40 / 4.0;   //160
            if (y == 2) jump_cmd++;
            break;
        case 1:
            sit();
            jump_cmd++;
            break;
        }
        if (jump_cmd == 2) {
            jump_cmd = 0;
            //cout << y << " " << jump_cmd << " " << left_f->kneeAng_x << " " << left_f->hipJointAng_x << " " << left_h->shoulderAng_x << "\n";
            return 1;
        }
        return 0;
    }
    bool jumpToFloor() {           //����a�O
        //80 80 100 100 180 180
        left_h->elbowAng_x = -15;
        right_h->elbowAng_x = -15;
        switch (jump_cmd) {
        case 0:                 //��a�O �s��
            y -= 1;
            left_f->kneeAng_x += 60 / 2.0;
            right_f->kneeAng_x += 60 / 2.0;
            left_f->hipJointAng_x -= 40 / 2.0;
            right_f->hipJointAng_x -= 40 / 2.0;
            left_h->shoulderAng_x += 40 / 2.0;
            right_h->shoulderAng_x += 40 / 2.0;
            if (y == 0) jump_cmd++;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            left_f->kneeAng_x -= 140 / 4.0;
            right_f->kneeAng_x -= 140 / 4.0;
            left_f->hipJointAng_x += 120 / 4.0;
            right_f->hipJointAng_x += 120 / 4.0;
            left_h->shoulderAng_x -= 40 / 4.0;
            right_h->shoulderAng_x -= 40 / 4.0;
            jump_cmd++;
            break;
        }
        if (jump_cmd == 5) {
            jump_cmd = 0;
            carry_mw = 1;
            isOnWand = 0;
            return 1;
        }
        return 0;
    }
    void sit() {
        left_f->kneeAng_x = 80;
        right_f->kneeAng_x = 80;
        left_f->hipJointAng_x = 100;
        right_f->hipJointAng_x = 100;
        left_h->shoulderAng_x = 180;
        right_h->shoulderAng_x = 180;
        left_h->shoulderAng_z = -15;
        right_h->shoulderAng_z = 15;
    }
}myRobot, jakao, pupu;
struct big_chair {
    float x = 0, y = 0, z = 0;                    //�ۤv���y��
    float angle_z = 0.0;
    int flag = 0;
    void setPos(int x_, int y_, int z_) {
        x = x_;
        y = y_;
        z = z_;
    }
    void move() {                                 //�n�\
        if (flag == 0) {
            angle_z += 3;
            if (angle_z >= 20) flag = 1;
        }
        else {
            angle_z -= 3;
            if (angle_z <= -20) flag = 0;
        }
    }
    void draw() {
        //�Ȥl����25 �e10 ��26
        setMaterial(0, 0, 0, 0, 0, 0, 0);
        glColor3f(141 / 255.0, 84 / 255.0, 28 / 255.0);
        glPushMatrix();              //2�Ӭ[�l
        glTranslatef(0, 0, -20);
        glRotatef(90, 0, 1, 0);
        glRotatef(75, 1, 0, 0);
        draw_cylinder(0.5, 0.5, 30);
        glRotatef(30, 1, 0, 0);
        draw_cylinder(0.5, 0.5, 30);
        glPopMatrix();              //pop�[�l

        glPushMatrix();
        glTranslatef(0, 0, 20);
        glRotatef(90, 0, 1, 0);
        glRotatef(75, 1, 0, 0);
        draw_cylinder(0.5, 0.5, 30);
        glRotatef(30, 1, 0, 0);
        draw_cylinder(0.5, 0.5, 30);
        glPopMatrix();               //pop�[�l

        glPushMatrix();
        glRotatef(angle_z, 0, 0, 1);       //����Ȥl
        glTranslatef(0, 0, -20);        //���
        draw_cylinder(0.8, 0.8, 40);


        glColor3f(1, 1, 1);
        glPushMatrix();             //�u�u
        glTranslatef(0, 0, 7);
        glRotatef(90, 0, 1, 0);
        glRotatef(90, 1, 0, 0);
        draw_cylinder(0.3, 0.3, 20);
        glPopMatrix();

        glPushMatrix();             //�u�u
        glTranslatef(0, 0, 33);
        glRotatef(90, 0, 1, 0);
        glRotatef(90, 1, 0, 0);
        draw_cylinder(0.3, 0.3, 20);
        glPopMatrix();

        glColor3f(162 / 255.0, 92 / 255.0, 21 / 255.0);
        glTranslatef(0, -20, 7);

        glPushMatrix();               //�Ȥl
        glScalef(10, 2, 26);
        draw_cube();
        glPopMatrix();

        glColor3f(141 / 255.0, 84 / 255.0, 28 / 255.0);
        glPushMatrix();               //�ȭI
        glRotatef(110, 0, 0, 1);
        glScalef(10, 2, 26);
        draw_cube();
        glPopMatrix();

        jakao.sit();
        jakao.left_h->shoulderAng_x -= 20;
        jakao.right_h->shoulderAng_x -= 20;

        glPushMatrix();
        glTranslatef(8, -4.5, 6);   //���e�� ���U ���k
        glRotatef(90, 0, 1, 0);
        jakao.draw();
        glPopMatrix();

        pupu.sit();
        pupu.left_h->shoulderAng_x -= 20;
        pupu.right_h->shoulderAng_x -= 20;

        glPushMatrix();
        glTranslatef(8, -4.5, 19);   //���e�� ���U ���k
        glRotatef(90, 0, 1, 0);
        if (sitOnChair) pupu.draw();
        glPopMatrix();

        glPopMatrix();          //pop���

    }
}myBig_chair;
struct floor {
    int light[6] = { 0 }; //�W�U���k�e��
    void draw() {                  //�e����M�a�O
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //�W
        if (light[0] % 2 == 0)
            change_color(ICE_COLOR);
        else {
            glColor3f(1, 0, 0);
        }
        glPushMatrix();
        glTranslatef(0, 60, 0);
        draw_square(60, 60, 1);              //�e�Ѫ�O
        glPopMatrix();

        //�U
        change_color(ICE_COLOR);
        glPushMatrix();
        draw_square(60, 60, 1);              //�e�a�O
        glPopMatrix();

        //��
        if (light[2] % 2 == 0)
            change_color(ICE_COLOR);
        else
            glColor3f(1, 0, 0);
        glPushMatrix();                   //�O�s0,0
        glRotatef(90, 0, 0, 1);           //��z�b�f�ɰw��270��
        draw_square(60, 60, 1);
        if (light[2] % 2 == 1) {
            setMaterial(0,0,0,1,0,0,0);
            glColor3f(1, 0, 0);
            glPushMatrix();
            for (int i = 0; i < 6; i++) {
                glBegin(GL_LINES);
                glVertex3f(0, 0, 0);
                glVertex3f(0, 0, 60);
                glEnd();
                glTranslated(10, 0, 0);
            }
            glPopMatrix();
            glPushMatrix();
            for (int i = 0; i < 6; i++) {
                glBegin(GL_LINES);
                glVertex3f(0, 0, 0);
                glVertex3f(60, 0, 0);
                glEnd();
                glTranslated(0, 0, 10);
            }
            glPopMatrix();
            setMaterial(0, 0, 0, 0, 0, 0, 0);
        }
        glPopMatrix();                    //�^��0,0

        //�k
        if (light[3] % 2 == 0)
            change_color(ICE_COLOR);
        else
            glColor3f(1, 0, 0);
        glPushMatrix();
        glTranslatef(60, 0, 0);            //�O�s0,0
        glRotatef(90, 0, 0, 1);           //��z�b�f�ɰw��270��
        draw_square(60, 60, 1);
        if (light[3] % 2 == 1) {
            setMaterial(0, 0, 0, 1, 0, 0, 0);
            glColor3f(1, 0, 0);
            glPushMatrix();
            for (int i = 0; i < 6; i++) {
                glBegin(GL_LINES);
                glVertex3f(0, 0, 0);
                glVertex3f(0, 0, 60);
                glEnd();
                glTranslated(10, 0, 0);
            }
            glPopMatrix();
            glPushMatrix();
            for (int i = 0; i < 6; i++) {
                glBegin(GL_LINES);
                glVertex3f(0, 0, 0);
                glVertex3f(60, 0, 0);
                glEnd();
                glTranslated(0, 0, 10);
            }
            glPopMatrix();
            setMaterial(0, 0, 0, 0, 0, 0, 0);
        }
        glPopMatrix();
        

        //�e
        if (light[4] % 2 == 0)
            change_color(ICE_COLOR);
        else
            glColor3f(1, 0, 0);
        glPushMatrix();                   //�O�s0,0
        glTranslatef(0, 0, 60);
        glRotatef(270, 1, 0, 0);          //��x�b�f�ɰw��270��
        draw_square(60, 60, 1);
        if (light[4] % 2 == 1) {
            setMaterial(0, 0, 0, 1, 0, 0, 0);
            glColor3f(1, 0, 0);
            glPushMatrix();
            for (int i = 0; i < 6; i++) {
                glBegin(GL_LINES);
                glVertex3f(0, 0, 0);
                glVertex3f(0, 0, 60);
                glEnd();
                glTranslated(10, 0, 0);
            }
            glPopMatrix();
            glPushMatrix();
            for (int i = 0; i < 6; i++) {
                glBegin(GL_LINES);
                glVertex3f(0, 0, 0);
                glVertex3f(60, 0, 0);
                glEnd();
                glTranslated(0, 0, 10);
            }
            glPopMatrix();
            setMaterial(0, 0, 0, 0, 0, 0, 0);
        }
        glPopMatrix();

        //��
        if (light[5] % 2 == 0)
            change_color(ICE_COLOR);
        else
            glColor3f(1, 0, 0);
        glPushMatrix();                   //�O�s0,0
        glRotatef(270, 1, 0, 0);          //��x�b�f�ɰw��270��
        draw_square(60, 60, 1);
        if (light[5] % 2 == 1) {
            setMaterial(0, 0, 0, 1, 0, 0, 0);
            glColor3f(1, 0, 0);
            glPushMatrix();
            for (int i = 0; i < 6; i++) {
                glBegin(GL_LINES);
                glVertex3f(0, 0, 0);
                glVertex3f(0, 0, 60);
                glEnd();
                glTranslated(10, 0, 0);
            }
            glPopMatrix();
            glPushMatrix();
            for (int i = 0; i < 6; i++) {
                glBegin(GL_LINES);
                glVertex3f(0, 0, 0);
                glVertex3f(60, 0, 0);
                glEnd();
                glTranslated(0, 0, 10);
            }
            glPopMatrix();
            setMaterial(0, 0, 0, 0, 0, 0, 0);
        }
        glPopMatrix();                    //�^��0,0
    }
}myFloor;
struct firework {
    int cnt = 0, ps = 0, dif = 0;
    int lim = 80;
    int changeStatus() {
        cnt++;
        return cnt;
    }
    void resetStatus(){
        cnt = 0;
    }
    void draw(int style) {
        if (style == 1) drawStyle1();
        else if (style == 2) drawStyle2();
        else if (style == 3) drawStyle3();
    }
    void drawBasic1(int x,float len) { //���u ���פ����@
        glPushMatrix();
        glTranslatef(0, x, 0);
        glScalef(0.5, x / len, 0.5);
        glutSolidSphere(1, 10, 10);
        glPopMatrix();
    }
    void drawBasic2(int x,float r,float len) {  //cnt �b�|�����@ ���פ����@
        for (int i = 0; i < 360; i += 40) {
            glPushMatrix();
            glTranslatef(x / r * cos(i * 0.01745), x / r * sin(i * 0.01745), 0);
            glRotatef(i, 0, 0, 1);
            glScalef(x / len, 0.5, 0.5);
            glutSolidSphere(1, 10, 10);
            glPopMatrix();
        }
    }
    void drawBasic3(int x, float total,int r,float sx,float sy,float sz) {
        for (int i = 0; i <= x; i++) {
            glPushMatrix();
            glTranslatef(r * cos(360 / total * i * 0.01745), r * sin(360 / total * i * 0.01745), 0);
            glRotatef(360 / total * i, 0, 0, 1);
            glScalef(sx, sy, sz);
            glutSolidSphere(1, 10, 10);
            glPopMatrix();
        }
    }
    void drawStyle1() {
        if (cnt <= 60) {
            ps = 0;
            dif = cnt;
        }
        else if(cnt <= 80){
            ps = 1;
            dif = cnt - 60;
        }
        else if(cnt <= 90){
            ps = 2;
            dif = cnt - 80;
        }
        switch (ps) {
        case 0:
            setMaterial(1, 0.68, 0.68, 1, 0.68, 0.68, 1, 0.68, 0.68, 1, 0.68, 0.68, 0);
            drawBasic1(dif,20);
            break;
        case 1:
            setMaterial(1, 0.68, 0.68, 1, 0.68, 0.68, 1, 0.68, 0.68, 1, 0.68, 0.68, 0);
            glPushMatrix();
            glTranslatef(0, 60, 0);

            glPushMatrix();
            glScalef(0.6, 0.6 , 0.6);
            glutSolidSphere(1, 10, 10);
            glPopMatrix();

            drawBasic2(dif, 2.5, 5);
            glPopMatrix();
            break;
        case 2:
            setMaterial(1 * (1 - dif / 10.0), 0.68 * (1 - dif / 10.0), 0.68 * (1 - dif / 10.0), 1*(1 - dif / 10.0), 0.68 * (1 - dif / 10.0), 0.68 * (1 - dif / 10.0), 1 * (1 - dif / 10.0), 0.68 * (1 - dif / 10.0), 0.68 * (1 - dif / 10.0), 1 * (1 - dif / 10.0), 0.68 * (1 - dif / 10.0), 0.68 * (1 - dif / 10.0), 0);
            //setMaterial(1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0);
            glPushMatrix();
            glTranslatef(0, 60, 0);

            glPushMatrix();
            glScalef(0.6, 0.6, 0.6);
            glutSolidSphere(1, 10, 10);
            glPopMatrix();

            drawBasic2(20, 2.5, 5);
            glPopMatrix();
            break;
        }
        
    }
    void drawStyle2() {
        if (cnt < 50) {
            ps = 0;
            dif = cnt;
        }
        else if (cnt <= 65) {
            ps = 1;
            dif = cnt - 50;
        }
        else if (cnt <= 85) {
            ps = 2;
            dif = cnt - 65;
        }
        else if (cnt <= 110) {
            ps = 3;
            dif = cnt - 85;
        }
        else if (cnt <= 120) {
            ps = 4;
            dif = cnt - 110;
        }
        switch (ps) {
        case 0:
            setMaterial(0.68, 0.68, 1, 0.68, 0.68, 1, 0.68, 0.68, 1, 0.68, 0.68, 1, 1);
            drawBasic1(dif, 20);
            break;
        case 1:
            setMaterial(0.68, 0.68, 1, 0.68, 0.68, 1, 0.68, 0.68, 1, 0.68, 0.68, 1, 1);
            glPushMatrix();
            glTranslatef(0, 50, 0);
            
            drawBasic3(dif,15,6,0.8,0.8,0.8);
            glPopMatrix();
            break;
        case 2:
            setMaterial(0.68, 0.68, 1, 0.68, 0.68, 1, 0.68, 0.68, 1, 0.68, 0.68, 1, 1);
            glPushMatrix();
            glTranslatef(0, 50, 0);
            drawBasic3(15, 15, 6, 0.8, 0.8, 0.8);
            drawBasic2(dif,15,7.0);
            glPopMatrix();
            break;
        case 3:
            setMaterial(0.68, 0.68, 1, 0.68, 0.68, 1, 0.68, 0.68, 1, 0.68, 0.68, 1, 1);
            glPushMatrix();
            glTranslatef(0, 50, 0);
            drawBasic3(15, 15, 6, 0.8, 0.8, 0.8);
            drawBasic2(20, 15, 7.0);
            drawBasic2(dif,2,4);
            glPopMatrix();
            break;
        case 4:
            setMaterial(0.68*(1 - dif / 10.0), 0.68 * (1 - dif / 10.0), 1 * (1 - dif / 10.0), 0.68 * (1 - dif / 10.0), 0.68 * (1 - dif / 10.0), 1 * (1 - dif / 10.0), 0.68 * (1 - dif / 10.0), 0.68 * (1 - dif / 10.0), 1 * (1 - dif / 10.0), 0.68 * (1 - dif / 10.0), 0.68 * (1 - dif / 10.0), 1 * (1 - dif / 10.0), 1);
            //setMaterial(1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0);
            glPushMatrix();
            glTranslatef(0, 50, 0);
            drawBasic3(15, 15, 6, 0.8, 0.8, 0.8);
            drawBasic2(20, 15, 7.0);
            drawBasic2(25, 2, 4);
            glPopMatrix();
            break;
        }
    }
    void drawStyle3() {
        if (cnt < 40) {
            ps = 0;
            dif = cnt;
        }
        else if (cnt <= 55) {
            ps = 1;
            dif = cnt - 40;
        }
        else if (cnt <= 65) {
            ps = 2;
            dif = cnt - 55;
        }
        else if (cnt <= 80) {
            ps = 3;
            dif = cnt - 55;
        }
        else if (cnt <= 90) {
            ps = 4;
            dif = cnt - 80;
        }
        switch (ps) {
        case 0:
            setMaterial(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
            drawBasic1(dif, 15);
            break;
        case 1:
            setMaterial(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
            glPushMatrix();
            glTranslatef(0, 40, 0);
            drawBasic3(dif, 10, 9, 0.8, 0.4, 0.4);
            glPopMatrix();
            break;
        case 2:
            setMaterial(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
            glPushMatrix();
            glTranslatef(0, 40, 0);
            drawBasic3(10, 10, 9, 0.8, 0.4, 0.4);
            drawBasic3(dif, 20, 6, 0.5, 0.4, 0.4);
            glPopMatrix();
            break;
        case 3:
            setMaterial(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
            glPushMatrix();
            glTranslatef(0, 40, 0);
            drawBasic3(15, 15, 6, 0.8, 0.8, 0.8);
            drawBasic3(20, 20, 6, 0.5, 0.4, 0.4);
            drawBasic2(dif, 1.5, 3.5);
            glPopMatrix();
            break;
        case 4:
            setMaterial(1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0, 1 - dif / 10.0);
            glPushMatrix();
            glTranslatef(0, 40, 0);
            drawBasic3(15, 15, 6, 0.8, 0.8, 0.8);
            drawBasic3(20, 20, 6, 0.5, 0.4, 0.4);
            drawBasic2(25, 1.5, 3.5);
            glPopMatrix();
            break;
        }
    }
};
vector<firework>myFirework(6);
void change_color_material(int value) {
    switch (value) {
    case WHITE:
        setMaterial(0,0,0,0,0,0,1,1,1,1,1,1,0);
        break;
    case GOLD:
        setMaterial(0.628281f, 0.555802f, 0.366065f,0,0,0, 0.24725f, 0.1995f, 0.0745f, 0.75164f, 0.60648f, 0.22648f, 51.2f);
        break;
    case SILVER:
        setMaterial(0.508273f, 0.508273f, 0.508273f,0,0,0, 0.19225f, 0.19225f, 0.19225f, 0.50754f, 0.50754f, 0.50754f,51.2f);
        break;
    case PERL:
        setMaterial(0.296648f, 0.296648f, 0.296648f, 0.922f,0,0,0,0, 0.25f, 0.20725f, 0.20725f, 0.922f, 1.0f, 0.829f, 0.829f, 0.922f,11.264f);
        break;
    case CYAN_RUBBER:
        setMaterial(0.04f, 0.7f, 0.7f,0,0,0, 0.0f, 0.05f, 0.05f, 0.4f, 0.5f, 0.5f,10.0f);
        break;
    case RED_PLASTIC:
        setMaterial(0.7f, 0.6f, 0.6f,0,0,0, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f,32);
        break;
    case COPPER:
        setMaterial(0.256777f, 0.137622f, 0.086014f,0,0,0, 0.19125f, 0.0735f, 0.0225f, 0.7038f, 0.27048f, 0.0828f,12.8f);
        break;
    case BLACK_PLACTIC:
        setMaterial(0.50f, 0.50f, 0.50f,0,0,0, 0.0f, 0.0f, 0.0f, 0.01f, 0.01f, 0.01f,32.0f);
        break;
    }
}
void change_color(int value) {  //�]�w�C��
    switch (value) {
    case ICE_COLOR:
        glColor3f(235 / 255.0, 1.0, 1.0);
        break;
    case WAND_WOOD_COLOR:
        glColor3f(158 / 255.0, 79 / 255.0, 0);
        break;
    case ROBOT_BLUE_MAIN:
        glColor3f(219 / 255.0, 1, 1);
        break;
    case ROBOT_BLUE_SUB:
        glColor3f(173 / 255.0, 214 / 255.0, 1);
        break;
    case ROBOT_PINK_MAIN:
        glColor3f(1, 1, 1);
        break;
    case ROBOT_PINK_SUB:
        glColor3f(255 / 255.0, 219 / 255.0, 237 / 255.0);
        break;
    case HOME_COLOR:
        glColor3f(105 / 255.0, 66 / 255.0, 27 / 255.0);
        break;
    }
}
void init_camera() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            u[i][j] = 0;
        }
        u[i][i] = 1;
    }
    eye[0] = Eye[0];
    eye[1] = Eye[1];
    eye[2] = Eye[2];

    eyeAngx = 0;
    eyeAngy = 0;
    eyeAngz = 0;
    //�� ���� w/h �����i�}������(40~70)
    //�w�qlrbtnf
    double zNear = 0, zFar = 0, aspect = 0, fovy = 0;
    zNear = 20;                      
    zFar = 1000;
    aspect = width / (double)height;
    fovy = 45;

    double z1, x1, y1, z2, x2, y2;
    z1 = zNear / cos((fovy / 2.0) * PI / 180.0); //����
    y1 = z1 * sin((fovy / 2.0) * PI / 180.0);    //�e
    x1 = y1 * aspect;

    z2 = zFar / cos((fovy / 2.0) * PI / 180.0); //����
    y2 = z2 * sin((fovy / 2.0) * PI / 180.0);    //�e
    x2 = y2 * aspect;

    clippingWindowPerspective[_l] = -x1;    //l
    clippingWindowPerspective[_r] = x1;     //r
    clippingWindowPerspective[_b] = -y1;    //b
    clippingWindowPerspective[_t] = y1;     //t
    clippingWindowPerspective[_n] = zNear;  //n
    clippingWindowPerspective[_f] = zFar;   //f

    //-40.0, 40.0, -40.0, 40.0, -100.0, 200
    clippingWindowOrtho[_l] = -40;    //l
    clippingWindowOrtho[_r] = 40;     //r
    clippingWindowOrtho[_b] = -40;    //b
    clippingWindowOrtho[_t] = 40;     //t
    clippingWindowOrtho[_n] = -100;   //n
    clippingWindowOrtho[_f] = 1000;    //f
}
void myinit()
{
    
    glClearColor(0.0, 0.0, 0.0, 1.0);      /*set the background color BLACK */
    /*Clear the Depth & Color Buffers */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);   /*Enable mornalization  */
    glEnable(GL_DEPTH_TEST);  /*Enable depth buffer for shading computing */

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);    /*Enable lighting effects */
    glEnable(GL_LIGHT0);        /*Turn on light0 */

    /*-----Define some global lighting status -----*/
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); /* local viewer �ݪ��H���O�b�L����*/
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient); /*global ambient ���쪺���ҥ�*/

    /*-----Enable face culling ���������n���p�� -----*/
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    //fog
    glFogf(GL_FOG_START, 0.0);
    glFogf(GL_FOG_END, 200.0);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_DENSITY, fogOpacity);

    glFlush();/*Enforce window system display the results*/
    
    /*---- Compute cos(5.0) and sin(5.0) ----*/
    cv = cos(5.0 * PI / 180.0);
    sv = sin(5.0 * PI / 180.0);

    //�Ӭ۾���m�]�m�A��v���׳]�m(��l��)
    init_camera();

    //�w�q�����H�C��
    myRobot.setColor(ROBOT_BLUE_MAIN, ROBOT_BLUE_SUB);
    pupu.setColor(ROBOT_BLUE_MAIN, ROBOT_BLUE_SUB);
    jakao.setColor(ROBOT_PINK_MAIN, ROBOT_PINK_SUB);
    pupu.isMagician = 1;   //���۪������H
    pupu.carry_mw = 0;
    //��ê���y�Ц���
    for (pp p : river) {
        for (int i = p.a1; i < p.a2; i++) {
            limit.push_back({ p.x + p.r * cos(i * 0.01745) ,p.z + p.r * sin(i * 0.01745) });
        }
    }

    
    create_texture();
}
void draw_SnowMan(int colorMaterial,int pattern,bool isPattern) {
    
    //GL_TEXTURE_SPHERE
//GL_OBJECT_PLANE
//GL_EYE_PLANE
    if (isPattern) {
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, textName[pattern]);
        glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
        if(pattern == HEART_PINK) glScalef(0.3,0.3,0.3);
        glMatrixMode(GL_MODELVIEW);
    }


    glPushMatrix();
    change_color_material(colorMaterial);
    glTranslatef(0, 2, 0);                   //����{�l
    glutSolidSphere(2, 30, 30);              //�e�{�l ���|4
         
    if (isPattern) {
        glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);
    }

    //���l
    change_color_material(PERL);
    glPushMatrix();
    glTranslatef(0,0,1.8);
    glutSolidSphere(0.3, 10, 10);

    glPushMatrix();
    glTranslatef(0, 0.75, -0.15);
    glutSolidSphere(0.3, 10, 10);
    glPopMatrix();

    glTranslatef(0, -0.75, -0.15);
    glutSolidSphere(0.3, 10, 10);

    glPopMatrix();

    //�k��
    change_color_material(COPPER);
    glPushMatrix();
    glTranslatef(1,0,0);
    glRotatef(-33,0,0,1);

    glPushMatrix();
    glRotatef(270,1,0,0);
    draw_cylinder(0.15,0.15,3);
    glPopMatrix();

    glTranslatef(0, 1.5, 0);

    glPushMatrix();
    glRotatef(20, 0, 0, 1);
    glRotatef(270, 1, 0, 0);
    draw_cylinder(0.15, 0.15, 1.5);
    glPopMatrix();

    glPopMatrix();

    //����
    glPushMatrix();
    glTranslatef(-1, 0, 0);
    glRotatef(33, 0, 0, 1);

    glPushMatrix();
    glRotatef(270, 1, 0, 0);
    draw_cylinder(0.15, 0.15, 3);
    glPopMatrix();

    glTranslatef(0, 1.5, 0);

    glPushMatrix();
    glRotatef(-20, 0, 0, 1);
    glRotatef(270, 1, 0, 0);
    draw_cylinder(0.15, 0.15, 1.5);
    glPopMatrix();

    glPopMatrix();

    if (isPattern) {
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, textName[pattern]);
        glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    }

    //�Y
    change_color_material(colorMaterial);
    glTranslatef(0, 3, 0);               //�b�����Y  �M���魫�|0.5
    glPushMatrix();
    glutSolidSphere(1.5, 30, 30);        //���|3

    if (isPattern){
        glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);
    }

    //����
    change_color_material(BLACK_PLACTIC);
    glColor3f(0,0,0);
    glPushMatrix();                          //����  push3
    glTranslatef(0.6, 0, 1.3);
    glScalef(0.4, 0.8, 0.4);
    glutSolidSphere(0.5, 10, 10);
    change_color_material(PERL);                      //����
    glTranslatef(0, 0.15, 0.15);
    glutSolidSphere(0.33, 10, 10);
    glPopMatrix();                           //pop3

    change_color_material(BLACK_PLACTIC);
    glPushMatrix();                          //push3
    glTranslatef(-0.6, 0, 1.3);
    glScalef(0.4, 0.8, 0.4);
    glutSolidSphere(0.5, 10, 10);
    change_color_material(PERL);                        //����
    glTranslatef(0, 0.15, 0.15);
    glutSolidSphere(0.33, 10, 10);
    glPopMatrix();                           //pop3

    glPopMatrix();                          //pop3
    glPopMatrix();                       //���}�Y pop2
    glPopMatrix();                       //���}�{�l���Шt pop1


}
void draw_cube() {
    //���U��0,0,0
    glPushMatrix();
    glTranslatef(0.5,0.5,0.5);
    glutSolidCube(1);
    glPopMatrix();
    //int i;
    //glPolygonMode(GL_FRONT, GL_FILL);
    //for (i = 0; i < 6; i++) {     /* draw the six faces one by one */
    //    glNormal3fv(normal[i]);
    //    glBegin(GL_POLYGON);  /* Draw the face */
    //    glTexCoord2f(0.0, 0.0); glVertex3fv(points[face[i][0]]);
    //    glTexCoord2f(1.0, 0); glVertex3fv(points[face[i][1]]);
    //    glTexCoord2f(1, 1); glVertex3fv(points[face[i][2]]);
    //    glTexCoord2f(0, 1); glVertex3fv(points[face[i][3]]);
    //    glEnd();
    //}
}
void draw_cube(int x) {
    //���U��0,0,0

    glPushMatrix();
    glTranslatef(0.5, 0.5, 0.5);
    glutWireCube(1);
    glPopMatrix();

    //int i;
    //glPolygonMode(GL_FRONT, x);
    //for (i = 0; i < 6; i++) {     /* draw the six faces one by one */
    //    glNormal3fv(normal[i]);
    //    glBegin(GL_POLYGON);  /* Draw the face */
    //    glVertex3fv(points[face[i][0]]);
    //    glVertex3fv(points[face[i][1]]);
    //    glVertex3fv(points[face[i][2]]);
    //    glVertex3fv(points[face[i][3]]);
    //    glEnd();
    //}
}
void draw_disk(int r) {
    if (mycircle == NULL) {
        mycircle = gluNewQuadric();
        gluQuadricDrawStyle(mycircle, GLU_FILL);
    }
    gluQuadricTexture(mycircle, GL_TRUE);
    glPushMatrix();
    glRotatef(270, 1, 0, 0);
    gluDisk(mycircle,
        0,              // inner radius 
        r,              // outer radius 
        360,            // 16-side polygon 
        3);
    glPopMatrix();
}
void draw_circle(double size, int wid) {    //�j�p �u�e��
    glLineWidth(wid); 
    glNormal3f(0, 1, 0);
    glBegin(GL_POLYGON);
    for (int i = 360; i >= 0; --i) {
        glVertex3f(size * cos(i * 0.01745), 0, size * sin(i * 0.01745));
    }
    glEnd();
}
void draw_square(int hei, int wid,int sz) {     //���b�a�W������� �w�q: x�b�V���e�Az�V����
    glLineWidth(sz);
    glPushMatrix();
    glScaled(wid, 0, hei);
    glNormal3f(0, 1, 0);
    glBegin(GL_POLYGON);
    glVertex3f(0, 0.0, 1);
    glVertex3f(1, 0.0, 1);
    glVertex3f(1, 0.0, 0);
    glVertex3f(0, 0.0, 0);
    glEnd();
    glPopMatrix();
}
void draw_magic_wand() {
    myMagic_wand.draw();
}
void draw_home() {               //�������I
    float offsetx = 35;
    float offsetz = 30;
    setMaterial(0, 0, 0, 0, 0, 0, 0);
    change_color(HOME_COLOR);
    //�W�l 70 * 60
    glPushMatrix();
    glTranslatef(-offsetx, 0, -offsetz);
    glScalef(2.5, 40, 2.5);
    draw_cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(offsetx, 0, -offsetz);
    glScalef(2.5, 40, 2.5);
    draw_cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-offsetx, 0, offsetz);
    glScalef(2.5, 40, 2.5);
    draw_cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(offsetx, 0, offsetz);
    glScalef(2.5, 40, 2.5);
    draw_cube();
    glPopMatrix();

    glPushMatrix();                               //��γ� 
    glTranslatef(-offsetx, 40, -offsetz);
    glScalef(offsetx * 2, 1, 1);
    draw_cube();
    glPopMatrix();

    glPushMatrix();                               //��γ�
    glTranslatef(-offsetx, 40, offsetz);
    glScalef(offsetx * 2, 1, 1);
    draw_cube();
    glPopMatrix();

    for (int i = -offsetx; i <= offsetx; i += 5) {   //���γ�
        glPushMatrix();
        glTranslatef(i, 40, -offsetz);
        glScalef(2, 1, 2 * offsetz);
        draw_cube();
        glPopMatrix();
    }
}
void draw_debug(){
    setMaterial(0, 0, 0, 1, 122 / 255.0, 189, 0);
    glPushMatrix();
    glColor3f(1, 122 / 255.0, 189/255.0); //��

    glPushMatrix();
    for (int i = 0; i < debugModeCmd; i++) {
        glPushMatrix();
        glScalef(5, 5, 5);
        draw_cube(GL_LINE);
        glPopMatrix();
        glTranslatef(5, 0, 0);
    }
    glPopMatrix();

    setMaterial(0, 0, 0, 128 / 255.0, 128 / 255.0, 1, 0);
    glColor3f(128 / 255.0, 128 / 255.0, 1); //��
    glPushMatrix();
    for (int i = 0; i < debugModeCmd; i++) {
        glPushMatrix();
        glScalef(5, 5, 5);
        draw_cube(GL_LINE);
        glPopMatrix();
        glTranslatef(0, 5, 0);
    }
    glPopMatrix();

    setMaterial(0, 0, 0, 1, 1, 168 / 255.0, 0);
    glColor3f(1, 1, 168 / 255.0);  //��
    glPushMatrix();
    for (int i = 0; i < debugModeCmd; i++) {
        glPushMatrix();
        glScalef(5, 5, 5);
        draw_cube(GL_LINE);
        glPopMatrix();
        glTranslatef(0, 0, 5);
    }
    glPopMatrix();

    glPopMatrix();
}
void draw_magic_field() {
    //�]�k�} 60*60
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRotatef(90, 0, 1, 0);                          //����90��(��y�b) -> ���ܤ�G�}�f
    draw_circle(30, 1);
    draw_circle(29, 1);
    draw_circle(27.8, 1); //1.2
    draw_circle(27.3, 1); //0.5

    glLineWidth(1);                        //���u
    for (int i = 360; i >= 0; i -= 15) {
        glNormal3f(0, 1, 0);
        glBegin(GL_LINES);
        glVertex3f(27.3 * cos(i * 0.01745), 0, 27.3 * sin(i * 0.01745));
        glVertex3f(25.3 * cos(i * 0.01745), 0, 25.3 * sin(i * 0.01745));
        glEnd();
    }

    draw_circle(25.3, 1);   //2
    draw_circle(24.8, 1);   //0.5

    glLineWidth(2);
    for (int t = 0; t < 3; t++) {            //�P�P�}
        glNormal3f(0, 1, 0);
        glBegin(GL_POLYGON);
        for (int i = 360 + (t * 30); i > (t * 30); i -= 90) {
            glVertex3f(24.8 * cos(i * 0.01745), 0, 24.8 * sin(i * 0.01745));
        }
        glEnd();
    }

    draw_circle(17, 1);       //6

    draw_circle(14, 4);
    for (float t = 0; t < 12; t += 0.5) {     //��G�}
        glLineWidth(4);
        glNormal3f(0, 1, 0);
        glBegin(GL_POLYGON);
        for (int i = 360; i >= 0; --i)
            glVertex3f((14 - t * 0.2) * cos(i * 0.01745), 0, (t * 0.23) + (14 - t * 0.2) * sin(i * 0.01745));
        glEnd();
    }

    for (int i = 360, j = 0; i >= 0; i -= 15, j++) { //�Ӷ��}
        if (i % 30 == 0) {
            glLineWidth(1);
            glNormal3f(0, 1, 0);
            glBegin(GL_LINES);
            glVertex3f(15.8 * cos((i + 5) * 0.01745), 0, 15.8 * sin((i + 5) * 0.01745));
            glVertex3f(8.2 * cos((i + 5) * 0.01745), 0, 8.2 * sin((i + 5) * 0.01745));
            glVertex3f(15.8 * cos((i - 5) * 0.01745), 0, 15.8 * sin((i - 5) * 0.01745));
            glVertex3f(8.2 * cos((i - 5) * 0.01745), 0, 8.2 * sin((i - 5) * 0.01745));
            glEnd();
        }
        if (i % 60 == 0) continue;
        if (j % 4 == 2) glLineWidth(1);
        else glLineWidth(2);
        glNormal3f(0, 1, 0);
        glBegin(GL_LINES);
        glVertex3f(17 * cos(i * 0.01745), 0, 17 * sin(i * 0.01745));
        glVertex3f(8.2 * cos(i * 0.01745), 0, 8.2 * sin(i * 0.01745));
        glEnd();
    }
    draw_circle(8.2, 2);         //9.5
    draw_circle(7.7, 2);         //0.5
}
void draw_scene(int mode) {
    if (mode == MAGICFIELD) {        //�]�k�} ��m(30,30) ��ɭ���(60,60)
        setMaterial(0, 0, 0, 0, 0, 0, 0);
        if (!myRobot.isMagician) change_color(ICE_COLOR);
        else glColor3f(235 / 255.0, 244 / 255.0, 255 / 255.0);
        glPushMatrix();
        glTranslatef(30, 0, 30);       //�k�}�� lcs
        draw_magic_field();
        glPopMatrix();
        if (debugMode) {
            myFloor.draw();
            draw_debug();
        }

        glPushMatrix();
        glTranslatef(30, 7, 30);      //�k�M�� lcs  �Ʀb�Ť�
        myMagic_wand.setPos(30, 7, 30);
        if (myMagic_wand.show) draw_magic_wand();
        glPopMatrix();
        glDisable(GL_FOG);
    }
    else if (mode == GRASSLAND) {

        setMaterial(0, 0, 0, 0, 0, 0, 0);
        glColor4f(204 / 255.0, 1, 204 / 255.0, 1);
        glColor3f(1 / 255.0, 152 / 255.0, 89 / 255.0); //��a
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        glPushMatrix();
        glTranslatef(0, -10.5, 0);
        glTranslatef(0,0, -100);
        glScalef(300, 10, 300);
        draw_cube();
        glPopMatrix();

        glColor4f(204 / 255.0, 1, 204 / 255.0,1);     //���   
        glPushMatrix();
        glTranslatef(0, -0.3, -100);
        draw_square(300, 300,1);
        glPopMatrix();

        glColor3f(1.0, 1, 1);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.5);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, textName[FLOWER]);
        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
        glScalef(5,5,5);
        glMatrixMode(GL_MODELVIEW);
        glLineWidth(1);
        //glColor3f(1,1,1);
        glPushMatrix();
        glTranslatef(0, 0, -100);
        glScaled(300, 0, 300);
        glNormal3f(0, 1, 0);
        glBegin(GL_POLYGON);
        glTexCoord2f(0, 1); glVertex3f(0, 0.1, 1);
        glTexCoord2f(1, 1); glVertex3f(1, 0.1, 1);
        glTexCoord2f(1, 0); glVertex3f(1, 0.1, 0);
        glTexCoord2f(0, 0); glVertex3f(0, 0.1, 0);
        glEnd();
        glPopMatrix();
        glDisable(GL_ALPHA_TEST);
        glDisable(GL_TEXTURE_2D);
       

        glPushMatrix();                             //�ಾ�k�}(17,12) 20*20
        //glColor3f(188 / 255.0, 217 / 255.0, 246 / 255.0);
        glColor4f(1, 1, 1,0.7);
        glTranslatef(17, 0.3, 12);
        glScalef(1 / 3.0, 1 / 3.0, 1 / 3.0);
        draw_magic_field();
        glPopMatrix();


        if (!isLitposOpen && !isLitdirOpen && !isLitspotOpen && isLighting) {
            glPushMatrix();                             //�ಾ�k�}(100,5) 10*10
            setMaterial(0, 0, 0, 0.7, 0.7, 0.7,0);
            glColor3f(1, 1, 1);
            glTranslatef(100, 0.3, 6);
            glScalef(1 / 6.0, 1 / 6.0, 1 / 6.0);
            draw_magic_field();
            glPopMatrix();
        }
        setMaterial(0, 0, 0, 0, 0, 0, 0);
        //glColor3f(0, 0, 0);         //��l�u(�}�o��)
        //glPushMatrix();
        //for (int i = 0; i < 200; i += 10) {
        //    if(i%100 == 0) glLineWidth(3);
        //    else if (i%50  == 0) glLineWidth(2);
        //    else glLineWidth(1);
        //    glBegin(GL_LINES);
        //    glVertex3f(i, 0.1, 0);
        //    glVertex3f(i, 0.1, 200);
        //    glVertex3f(0, 0.1, i);
        //    glVertex3f(200, 0.1, i);
        //    glEnd();
        //}
        //glPopMatrix();

        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        //pool + river
        //{ {-10,120 ,100,0,30},{190,120 ,100,180,220}, {24,9 ,100,-3,30}, {100,70 ,100,80,120}, {130,267 ,100,260,290},{200,80 ,100,93,113} };
        
        //glColor3f(188 / 255.0, 217 / 255.0, 246 / 255.0);
        glColor3f(1, 1, 1);

        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, textName[POOL]);
        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
        //glTranslatef(30,0,30);
        glTranslatef(0.5, 0.5, 0);
        glRotatef(poolAng,0,0,1);
        glTranslatef(-0.5, -0.5, 0);
        //glScalef(2, 2, 2);
        glMatrixMode(GL_MODELVIEW);

        glPushMatrix();
        glTranslatef(30, 0.51, 160);
        draw_disk(30);
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);

        //203, 233, 231
        glColor3f(203 / 255.0, 233 / 255.0, 231 / 255.0);
        for (int z = 0; z > -97; z -= 3){ //���᩵��
            glPushMatrix();
            glTranslatef(123.5, 0.5, z);
            draw_disk(5);
            glPopMatrix();
        }
        for (int x = 200; x <= 300; x += 3) {  //���k����
            glPushMatrix();
            glTranslatef(x, 0.5, 180 );
            draw_disk(5);
            glPopMatrix();
        }
        for (const pp p : river) {
            for (int i = p.a1; i < p.a2; i++) {
                glPushMatrix();
                glTranslatef(p.x + p.r * cos(i * 0.01745), 0.5, p.z + p.r * sin(i * 0.01745));
                draw_disk(5);
                glPopMatrix();
            }
        }

        //pool��rock
        for (int i = 10; i < 356; i += 2) {
            glPushMatrix();
            glTranslatef(30 + 30 * cos(i * 0.01745), 0.5, 160 + 30 * sin(i * 0.01745));
            if (i % 7 == 0) {
                glColor3f(92 / 255.0, 92 / 255.0, 92 / 255.0);
                glutSolidSphere(1.75, 10, 10);
            }
            else if (i % 5 == 0) {
                glColor3f(191 / 255.0, 191 / 255.0, 191 / 255.0);
                glutSolidSphere(1.5, 10, 10);
            }
            else if (i % 3 == 0) {
                glColor3f(122 / 255.0, 122 / 255.0, 122 / 255.0);
                glutSolidSphere(2, 10, 10);
            }
            else if (i % 2 == 0) {
                glColor3f(61 / 255.0, 61 / 255.0, 61 / 255.0);
                glutSolidSphere(1.25, 10, 10);
            }
            else {
                glColor3f(38 / 255.0, 38 / 255.0, 38 / 255.0);
                glutSolidSphere(0.5, 10, 10);
            }
            glPopMatrix();
        }
        //rock �~��
        for (int j = 0; j < rock.size(); j++) {
            pp p = rock[j];
            int l = p.a1, r = p.a2;
            float offset = 0;
            if (j == 2) l -= 3;
            if (j == 3) r += 5;
            if (j >= 3)offset += 0.4;
            for (int i = l; i < r; i++) {
                glPushMatrix();
                if (i % 7 == 0) {
                    glTranslatef(p.x + 3.75 + offset + p.r * cos(i * 0.01745), 0.5, p.z + 3.75 + offset + p.r * sin(i * 0.01745));
                    glColor3f(92 / 255.0, 92 / 255.0, 92 / 255.0);
                    glutSolidSphere(1.75, 10, 10);
                }
                else if (i % 5 == 0) {
                    glTranslatef(p.x + 4 + offset + p.r * cos(i * 0.01745), 0.5, p.z + 4 + offset + p.r * sin(i * 0.01745));
                    glColor3f(191 / 255.0, 191 / 255.0, 191 / 255.0);
                    glutSolidSphere(1.5, 10, 10);
                }
                else if (i % 3 == 0) {
                    glTranslatef(p.x + 3.5 + offset + p.r * cos(i * 0.01745), 0.5, p.z + 3.5 + offset + p.r * sin(i * 0.01745));
                    glColor3f(122 / 255.0, 122 / 255.0, 122 / 255.0);
                    glutSolidSphere(2, 10, 10);
                }
                else if (i % 2 == 0) {
                    glTranslatef(p.x + 3.75 + offset + p.r * cos(i * 0.01745), 0.5, p.z + 3.75 + offset + p.r * sin(i * 0.01745));
                    glColor3f(61 / 255.0, 61 / 255.0, 61 / 255.0);
                    glutSolidSphere(1.25, 10, 10);
                }
                else {
                    glTranslatef(p.x + 4 + offset + p.r * cos(i * 0.01745), 0.5, p.z + 4 + offset + p.r * sin(i * 0.01745));
                    glColor3f(38 / 255.0, 38 / 255.0, 38 / 255.0);
                    glutSolidSphere(0.5, 10, 10);
                }
                glPopMatrix();
            }
        }
        //rock ����       
        for (int j = 0; j < rock.size(); j++) {
            pp p = rock[j];
            int l = p.a1, r = p.a2;
            float offset = 0;
            if (j == 4) {
                r -= 4;
                offset -= 1;
            }
            if (j == 0) r += 2;
            if (j >= 3) offset -= 0.5;
            if (j == 6) l -= 5;
            for (int i = l; i < r; i++) {
                glPushMatrix();
                if (i % 7 == 0) {
                    glTranslatef(p.x - 3.75 + offset + p.r * cos(i * 0.01745), 0.5, p.z - 3.75 + offset + p.r * sin(i * 0.01745));
                    glColor3f(92 / 255.0, 92 / 255.0, 92 / 255.0);
                    glutSolidSphere(1.75, 10, 10);
                }
                else if (i % 5 == 0) {
                    glTranslatef(p.x - 4 + offset + p.r * cos(i * 0.01745), 0.5, p.z - 4 + offset + p.r * sin(i * 0.01745));
                    glColor3f(191 / 255.0, 191 / 255.0, 191 / 255.0);
                    glutSolidSphere(1.5, 10, 10);
                }
                else if (i % 3 == 0) {
                    glTranslatef(p.x - 3.5 + offset + p.r * cos(i * 0.01745), 0.5, p.z - 3.5 + offset + p.r * sin(i * 0.01745));
                    glColor3f(122 / 255.0, 122 / 255.0, 122 / 255.0);
                    glutSolidSphere(2, 10, 10);
                }
                else if (i % 2 == 0) {
                    glTranslatef(p.x - 3.75 + offset + p.r * cos(i * 0.01745), 0.5, p.z - 3.75 + offset + p.r * sin(i * 0.01745));
                    glColor3f(61 / 255.0, 61 / 255.0, 61 / 255.0);
                    glutSolidSphere(1.25, 10, 10);
                }
                else {
                    glTranslatef(p.x - 4 + offset + p.r * cos(i * 0.01745), 0.5, p.z - 4 + offset + p.r * sin(i * 0.01745));
                    glColor3f(38 / 255.0, 38 / 255.0, 38 / 255.0);
                    glutSolidSphere(0.5, 10, 10);
                }
                glPopMatrix();
            }
        }
        //���᩵��rock
        for (int z = 0; z > -97; z-=4) { 
            //glTranslatef(123.5, 0.5, z);
            glPushMatrix();
            if (abs(z) % 7 == 0) {
                glTranslatef(123.5 - 3.75 , 0.5, z - 3.75);
                glColor3f(92 / 255.0, 92 / 255.0, 92 / 255.0);
                glutSolidSphere(1.75, 10, 10);
            }
            else if (abs(z) % 5 == 0) {
                glTranslatef(123.5 - 4 , 0.5, z - 4);
                glColor3f(191 / 255.0, 191 / 255.0, 191 / 255.0);
                glutSolidSphere(1.5, 10, 10);
            }
            else if (abs(z) % 3 == 0) {
                glTranslatef(123.5 - 3.5 , 0.5, z - 3.5);
                glColor3f(122 / 255.0, 122 / 255.0, 122 / 255.0);
                glutSolidSphere(2, 10, 10);
            }
            else if (abs(z) % 2 == 0) {
                glTranslatef(123.5 - 3.75, 0.5, z - 3.75 );
                glColor3f(61 / 255.0, 61 / 255.0, 61 / 255.0);
                glutSolidSphere(1.25, 10, 10);
            }
            else {
                glTranslatef(123.5 - 4, 0.5,z - 4 );
                glColor3f(38 / 255.0, 38 / 255.0, 38 / 255.0);
                glutSolidSphere(0.5, 10, 10);
            }
            glPopMatrix();
            //�k
            glPushMatrix();
            if (abs(z) % 7 == 0) {
                glTranslatef(123.5 + 3.75, 0.5, z + 3.75);
                glColor3f(92 / 255.0, 92 / 255.0, 92 / 255.0);
                glutSolidSphere(1.75, 10, 10);
            }
            else if (abs(z) % 5 == 0) {
                glTranslatef(123.5 + 4, 0.5, z + 4);
                glColor3f(191 / 255.0, 191 / 255.0, 191 / 255.0);
                glutSolidSphere(1.5, 10, 10);
            }
            else if (abs(z) % 3 == 0) {
                glTranslatef(123.5 + 3.5, 0.5, z + 3.5);
                glColor3f(122 / 255.0, 122 / 255.0, 122 / 255.0);
                glutSolidSphere(2, 10, 10);
            }
            else if (abs(z) % 2 == 0) {
                glTranslatef(123.5 + 3.75, 0.5, z + 3.75);
                glColor3f(61 / 255.0, 61 / 255.0, 61 / 255.0);
                glutSolidSphere(1.25, 10, 10);
            }
            else {
                glTranslatef(123.5 + 4, 0.5, z + 4);
                glColor3f(38 / 255.0, 38 / 255.0, 38 / 255.0);
                glutSolidSphere(0.5, 10, 10);
            }
            glPopMatrix();
        }
        //���k����rock
        for (int x = 200; x <= 300; x += 2) {
            //glTranslatef(x, 0.5, 180);
            glPushMatrix();
            if (abs(x) % 7 == 0) {
                glTranslatef(x - 3.75, 0.5, 180 - 3.75);
                glColor3f(92 / 255.0, 92 / 255.0, 92 / 255.0);
                glutSolidSphere(1.75, 10, 10);
            }
            else if (abs(x) % 5 == 0) {
                glTranslatef(x - 4, 0.5, 180 - 4);
                glColor3f(191 / 255.0, 191 / 255.0, 191 / 255.0);
                glutSolidSphere(1.5, 10, 10);
            }
            else if (abs(x) % 3 == 0) {
                glTranslatef(x - 3.5, 0.5, 180 - 3.5);
                glColor3f(122 / 255.0, 122 / 255.0, 122 / 255.0);
                glutSolidSphere(2, 10, 10);
            }
            else if (abs(x) % 2 == 0) {
                glTranslatef(x - 3.75, 0.5, 180 - 3.75);
                glColor3f(61 / 255.0, 61 / 255.0, 61 / 255.0);
                glutSolidSphere(1.25, 10, 10);
            }
            else {
                glTranslatef(x - 4, 0.5, 180 - 4);
                glColor3f(38 / 255.0, 38 / 255.0, 38 / 255.0);
                glutSolidSphere(0.5, 10, 10);
            }
            glPopMatrix();
            //�k
            glPushMatrix();
            if (abs(x) % 7 == 0) {
                glTranslatef(x + 3.75, 0.5, 180 + 3.75);
                glColor3f(92 / 255.0, 92 / 255.0, 92 / 255.0);
                glutSolidSphere(1.75, 10, 10);
            }
            else if (abs(x) % 5 == 0) {
                glTranslatef(x + 4, 0.5, 180 + 4);
                glColor3f(191 / 255.0, 191 / 255.0, 191 / 255.0);
                glutSolidSphere(1.5, 10, 10);
            }
            else if (abs(x) % 3 == 0) {
                glTranslatef(x + 3.5, 0.5, 180 + 3.5);
                glColor3f(122 / 255.0, 122 / 255.0, 122 / 255.0);
                glutSolidSphere(2, 10, 10);
            }
            else if (abs(x) % 2 == 0) {
                glTranslatef(x + 3.75, 0.5, 180 + 3.75);
                glColor3f(61 / 255.0, 61 / 255.0, 61 / 255.0);
                glutSolidSphere(1.25, 10, 10);
            }
            else {
                glTranslatef(x + 4, 0.5, 180 + 4);
                glColor3f(38 / 255.0, 38 / 255.0, 38 / 255.0);
                glutSolidSphere(0.5, 10, 10);
            }
            glPopMatrix();
        }
        //road
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LI);

        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, textName[WOOD_FLOOR]);
        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
        glScalef(0.125,0.125,0.125);
        glMatrixMode(GL_MODELVIEW);

        for (int i = 115; i < 168; i += 6) {              //180�q�Y
            glNormal3f(0,1,0);
            glBegin(GL_POLYGON);
            //glColor3f(133 / 255.0, 66 / 255.0, 0 / 255.0);
            glColor3f(1, 1, 1);
            glTexCoord2f(0, 0); glVertex3f(150 + 120 * cos(i * 0.01745), 0.4, 0 + 80 * sin(i * 0.01745));
            glTexCoord2f(0, 1); glVertex3f(150 + 120 * cos((i + 3) * 0.01745), 0.4, 0 + 80 * sin((i + 3) * 0.01745));
            glTexCoord2f(1, 1); glVertex3f(150 + 150 * cos((i + 3) * 0.01745), 0.4, 0 + 100 * sin((i + 3) * 0.01745));
            glTexCoord2f(1, 0); glVertex3f(150 + 150 * cos(i * 0.01745), 0.4, 0 + 100 * sin(i * 0.01745));
            glEnd();
        }
        for (int i = -46; i < 30; i += 6) {
            glNormal3f(0, 1, 0);
            glBegin(GL_POLYGON);
            //glColor3f(133 / 255.0, 66 / 255.0, 0 / 255.0);
            glColor3f(1, 1, 1);
            glTexCoord2f(1, 1); glVertex3f(4 + 120 * cos(i * 0.01745), 0.4, 150 + 80 * sin(i * 0.01745));
            glTexCoord2f(1, 0); glVertex3f(4 + 120 * cos((i + 3) * 0.01745), 0.4, 150 + 80 * sin((i + 3) * 0.01745));
            glTexCoord2f(0, 0); glVertex3f(4 + 150 * cos((i + 3) * 0.01745), 0.4, 150 + 100 * sin((i + 3) * 0.01745));
            glTexCoord2f(0, 1); glVertex3f(4 + 150 * cos(i * 0.01745), 0.4, 150 + 100 * sin(i * 0.01745));
            glEnd();
        }
        glDisable(GL_TEXTURE_2D);
        //home   
        glPushMatrix();
        glTranslatef(145, 0, 130);               //70*50
        draw_home();
        glPopMatrix();

        //big chair
        myBig_chair.setPos(160, 0, 110);
        glPushMatrix();                        //40*15
        glTranslatef(160, 25, 110);            //����
        glRotatef(45, 0, 1, 0);
        glRotatef(180, 0, 1, 0);
        myBig_chair.draw();
        glPopMatrix();
        //�˹��~
        {
            //����
            glPushMatrix();
            glTranslatef(30, 0.5, 125);
            glScalef(3, 3, 3);
            draw_SnowMan(GOLD, NONE, 0);
            glPopMatrix();

            //�ȹ�
            glPushMatrix();
            glTranslatef(17, 0.5, 130);
            glRotatef(15, 0, 1, 0);
            glScalef(2.5, 2.5, 2.5);
            draw_SnowMan(SILVER, NONE, 0);
            glPopMatrix();

            //�콦��
            glPushMatrix();
            glTranslatef(42, 0.5, 130);
            glRotatef(-15, 0, 1, 0);
            glScalef(2, 2, 2);
            draw_SnowMan(RED_PLASTIC, NONE, 0);
            glPopMatrix();

            //�ž󽦹�
            glPushMatrix();
            glTranslatef(90, 0.5, 68);
            glRotatef(-35, 0, 1, 0);
            glScalef(3.5, 3.5, 3.5);
            draw_SnowMan(CYAN_RUBBER, NONE, 0);
            glPopMatrix();

            //�����Ź�
            glPushMatrix();
            glTranslatef(71.5, 0.5, 158);
            glRotatef(-17, 0, 1, 0);
            glScalef(2, 2, 2);
            draw_SnowMan(WHITE, CHECKBOARD_BLUE, 1);
            glPopMatrix();

            //���߹�
            glPushMatrix();
            glTranslatef(65.5, 0.5, 158);
            glRotatef(17, 0, 1, 0);
            glScalef(2, 2, 2);
            draw_SnowMan(WHITE, HEART_PINK, 1);
            glPopMatrix();

            //�n�ȮǪ��� �k��
            glPushMatrix();
            glTranslatef(177, 0.5, 140);
            draw_tree(10, 30);
            glPopMatrix();

            //�n�ȮǪ��� ����
            glPushMatrix();
            glTranslatef(137, 0.5, 90);
            draw_tree(10, 30);
            glPopMatrix();
        }
        //�O�L
        {
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0.5);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            glBindTexture(GL_TEXTURE_2D, textName[FOREST]);

            glMatrixMode(GL_TEXTURE);
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);
            //��
            setMaterial(0, 0, 0, 0, 0, 0, 0);
            glColor3f(1, 1, 1);
            glPushMatrix();
            glTranslatef(-15, 0, -5);
            glScalef(126, 63, 1);
            draw_board();
            glPopMatrix();

            glPushMatrix();
            glTranslatef(-10, 0, -60);
            glScalef(150, 75, 1);
            draw_board();
            glPopMatrix();

            glPushMatrix();
            glTranslatef(140, 0, -15);
            glScalef(136, 88, 1);
            draw_board();
            glPopMatrix();

            glPushMatrix();
            glTranslatef(110, 0, -40);
            glScalef(136, 88, 1);
            draw_board();
            glPopMatrix();

            glPushMatrix();
            glTranslatef(170, 0, -90);
            glScalef(150, 75, 1);
            draw_board();
            glPopMatrix();

            glDisable(GL_ALPHA_TEST);
            glDisable(GL_TEXTURE_2D);
        }

        //GL_TEXTURE_SPHERE
        //GL_OBJECT_LINEAR
        //GL_EYE_LINEAR
        
        //�ѪŲ��l
        if (!(viewStyle & 1)) {
            cout << "sky";
            glDisable(GL_CULL_FACE);
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_ALPHA_TEST);
            glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
            glEnable(GL_TEXTURE_GEN_T);
            glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
            glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

            glAlphaFunc(GL_GREATER, 0.5);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            glBindTexture(GL_TEXTURE_2D, textName[STARSKY]);

            glMatrixMode(GL_TEXTURE);
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);

            glColor3f(1, 1, 1);
            glPushMatrix();
            glTranslatef(150, 0, 50);
            glutSolidSphere(300, 100, 100);
            glPopMatrix();

            glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
            glDisable(GL_TEXTURE_GEN_T);
            glDisable(GL_ALPHA_TEST);
            glDisable(GL_TEXTURE_2D);
            glCullFace(GL_BACK);
            glEnable(GL_CULL_FACE);
        }
        //�]��
        {
            change_color(HOME_COLOR);
            //glColor3f(162 / 255.0, 92 / 255.0, 21 / 255.0);
            glPushMatrix();
            glTranslatef(215,0,10);
            glRotatef(-90, 0, 1, 0);
            glPushMatrix();
            glTranslatef(0,10,0);
            glScalef(155, 3, 0.5);
            draw_cube();
            glPopMatrix();

            for (int i = 0; i < 15; i++) {
                glPushMatrix();
                glTranslatef(5+i*10, 0, 0);
                glScalef(4, 18, 2);  
                draw_cube();
                glPopMatrix();
            }

            glPopMatrix();
        }
        //EEVEE
        {
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0.5);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            glBindTexture(GL_TEXTURE_2D, textName[ELF]);
            glMatrixMode(GL_TEXTURE);
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);
            draw_billboard(115, -30, 50, 50);

            glBindTexture(GL_TEXTURE_2D, textName[EEVEE_1]);
            glMatrixMode(GL_TEXTURE);
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);
            draw_billboard(270, 50, 20, 20);

            glBindTexture(GL_TEXTURE_2D, textName[EEVEE_2]);
            glMatrixMode(GL_TEXTURE);
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);
            draw_billboard(255, 80, 20, 20);

            glBindTexture(GL_TEXTURE_2D, textName[EEVEE_3]);
            glMatrixMode(GL_TEXTURE);
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);
            draw_billboard(270, 90, 20, 20);

            glBindTexture(GL_TEXTURE_2D, textName[EEVEE_4]);
            glMatrixMode(GL_TEXTURE);
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);
            draw_billboard(240, 55, 20, 20);

            glBindTexture(GL_TEXTURE_2D, textName[EEVEE_5]);
            glMatrixMode(GL_TEXTURE);
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);
            draw_billboard(250, 120, 20, 20);

            glBindTexture(GL_TEXTURE_2D, textName[EEVEE_6]);
            glMatrixMode(GL_TEXTURE);
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);
            draw_billboard(235, 90, 20, 20);

           
            glBindTexture(GL_TEXTURE_2D, textName[eevee_ani]);
            glMatrixMode(GL_TEXTURE);
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);
            draw_billboard(30, 90, 37.5, 20);


            glDisable(GL_ALPHA_TEST);
            glDisable(GL_TEXTURE_2D);
        }
    }
}
void draw_cylinder(double up, double down, double height) {
    if (cylind == NULL) {
        cylind = gluNewQuadric();
    }
    /*--- Draw a cylinder ---*/
    gluCylinder(cylind, up, down, /* radius of top and bottom circle */
        height,                   /* height of the cylinder */
        20,                       /* use 12-side polygon approximating circle*/
        4);                       /* Divide it into 3 sections */
}
vector<pair<float, int>>tree_light;
void do_tree_rand(int height) {
    tree_light.clear();
    float hei, r;
    int ang, layer = height * 100 / 3.0;
    float diff = height / 3.0;
    //hei: 20~30 (0~10)
    for (int i = 0; i < 2 * diff; i++) {
        hei = rand() % layer / 100.0;
        ang = rand() % 360;
        tree_light.push_back({ hei,ang });
    }
    //hei: 10~20 (10~20)
    for (int i = 0; i < 8 * diff; i++) {
        hei = rand() % layer / 100.0 + diff;
        ang = rand() % 360;
        tree_light.push_back({ hei,ang });
    }
    //hei: 0~10 (20~30)�̩�
    for (int i = 0; i < 10 * diff; i++) {
        hei = rand() % layer / 100.0 + (diff * 2);
        ang = rand() % 360;
        tree_light.push_back({ hei,ang });
    }
}
void draw_tree(int button,int height) {

    float r;
    glPushMatrix();
    glRotatef(270,1,0,0);
    setMaterial(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    glColor3f(162 / 255.0, 92 / 255.0, 21 / 255.0);
    draw_cylinder(3, 3, 5);
    glTranslatef(0,0,4);
    glColor3f(0, 97 / 255.0, 48 / 255.0);
    draw_cylinder(button,0, height);
    glPopMatrix();

    if (!isLitdirOpen && !isLitposOpen && !isLitspotOpen && isLighting) {
        setMaterial(0, 0, 0, 1, 1, 1, 0);
        glColor3f(1, 235 / 255.0, 244 / 255.0);
        for (int i = 0; i < tree_light.size(); i++) {
            r = tree_light[i].first * button / (float)height;
            glPushMatrix();
            glTranslatef(r * cos(tree_light[i].second * 0.01745), 4 + height - tree_light[i].first, r * sin(tree_light[i].second * 0.01745));
            glutSolidSphere(0.5, 10, 10);
            glPopMatrix();
        }
    }
}
void draw_robot() {
    if (sitOnChair) return;  //���b�Ȥl�W���n�e
    myRobot.draw();
}
void draw_camera() {
    mtx[0] = u[0][0];
    mtx[1] = u[0][1];
    mtx[2] = u[0][2];
    mtx[3] = 0.0;
    mtx[4] = u[1][0];
    mtx[5] = u[1][1];
    mtx[6] = u[1][2];
    mtx[7] = 0.0;
    mtx[8] = u[2][0];
    mtx[9] = u[2][1];
    mtx[10] = u[2][2];
    mtx[11] = 0.0;
    mtx[12] = 0.0;
    mtx[13] = 0.0;
    mtx[14] = 0.0;
    mtx[15] = 1.0;
    //glMultMatrixd(mtx);

    glPushMatrix();
    glMultMatrixd(mtx);
    glRotatef(180, 0, 1, 0);
    glScalef(3,3,3);
    camera.draw();
    glPopMatrix();
}
void draw_view() {
    draw_scene(scene);  

    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]);      //�����H
    draw_robot();
    glPopMatrix();

    if (camera_show) {
        glPushMatrix();
        glTranslatef(eye[0], eye[1], eye[2]);
        draw_camera();
        glPopMatrix();
        if(view_volume_show)draw_view_volume();
    }
}
void draw_view_volume() {
    //------------------------------------���T���@----------------------------------------------
    setMaterial(0, 0, 0, 0, 0, 0, 0);
    glColor4f(1, 1, 1, 0.5);
    double wwn = fabs(clippingWindowPerspective[_r] - clippingWindowPerspective[_l]) / 2, hhn = fabs(clippingWindowPerspective[_t] - clippingWindowPerspective[_b]) / 2, ddn = -clippingWindowPerspective[_n], ddf = -clippingWindowPerspective[_f];
    double wwf = fabs(wwn * ddf / ddn), hhf = fabs(hhn * ddf / ddn);
    glBegin(GL_TRIANGLES);
    glVertex3f(eye[0], eye[1], eye[2]);
    //ltn
    glVertex3f(eye[0] - wwn * u[0][0] + hhn * u[1][0] + ddn * u[2][0], eye[1] - wwn * u[0][1] + hhn * u[1][1] + ddn * u[2][1], eye[2] - wwn * u[0][2] + hhn * u[1][2] + ddn * u[2][2]); //ltn
    //rtn
    glVertex3f(eye[0] + wwn * u[0][0] + hhn * u[1][0] + ddn * u[2][0], eye[1] + wwn * u[0][1] + hhn * u[1][1] + ddn * u[2][1], eye[2] + wwn * u[0][2] + hhn * u[1][2] + ddn * u[2][2]); //rtn

    glVertex3f(eye[0], eye[1], eye[2]);
    //lbn
    glVertex3f(eye[0] - wwn * u[0][0] - hhn * u[1][0] + ddn * u[2][0], eye[1] - wwn * u[0][1] - hhn * u[1][1] + ddn * u[2][1], eye[2] - wwn * u[0][2] - hhn * u[1][2] + ddn * u[2][2]);
    //ltn
    glVertex3f(eye[0] - wwn * u[0][0] + hhn * u[1][0] + ddn * u[2][0], eye[1] - wwn * u[0][1] + hhn * u[1][1] + ddn * u[2][1], eye[2] - wwn * u[0][2] + hhn * u[1][2] + ddn * u[2][2]);

    glVertex3f(eye[0], eye[1], eye[2]);
    //rtn
    glVertex3f(eye[0] + wwn * u[0][0] + hhn * u[1][0] + ddn * u[2][0], eye[1] + wwn * u[0][1] + hhn * u[1][1] + ddn * u[2][1], eye[2] + wwn * u[0][2] + hhn * u[1][2] + ddn * u[2][2]);
    //rbn
    glVertex3f(eye[0] + wwn * u[0][0] - hhn * u[1][0] + ddn * u[2][0], eye[1] + wwn * u[0][1] - hhn * u[1][1] + ddn * u[2][1], eye[2] + wwn * u[0][2] - hhn * u[1][2] + ddn * u[2][2]);

    glVertex3f(eye[0], eye[1], eye[2]);
    //rbn
    glVertex3f(eye[0] + wwn * u[0][0] - hhn * u[1][0] + ddn * u[2][0], eye[1] + wwn * u[0][1] - hhn * u[1][1] + ddn * u[2][1], eye[2] + wwn * u[0][2] - hhn * u[1][2] + ddn * u[2][2]);
    //lbn
    glVertex3f(eye[0] - wwn * u[0][0] - hhn * u[1][0] + ddn * u[2][0], eye[1] - wwn * u[0][1] - hhn * u[1][1] + ddn * u[2][1], eye[2] - wwn * u[0][2] - hhn * u[1][2] + ddn * u[2][2]);
    glEnd();

    //------------------------------------�~�T���@----------------------------------------------
    glColor4f(235 / 255.0, 1, 1, 0.5);
    glBegin(GL_QUADS);
    //ltn
    glVertex3f(eye[0] - wwn * u[0][0] + hhn * u[1][0] + ddn * u[2][0], eye[1] - wwn * u[0][1] + hhn * u[1][1] + ddn * u[2][1], eye[2] - wwn * u[0][2] + hhn * u[1][2] + ddn * u[2][2]); //ltn
    //rtn
    glVertex3f(eye[0] + wwn * u[0][0] + hhn * u[1][0] + ddn * u[2][0], eye[1] + wwn * u[0][1] + hhn * u[1][1] + ddn * u[2][1], eye[2] + wwn * u[0][2] + hhn * u[1][2] + ddn * u[2][2]); //rtn
    //rtf
    glVertex3f(eye[0] + wwf * u[0][0] + hhf * u[1][0] + ddf * u[2][0], eye[1] + wwf * u[0][1] + hhf * u[1][1] + ddf * u[2][1], eye[2] + wwf * u[0][2] + hhf * u[1][2] + ddf * u[2][2]); //rtf
    //ltf
    glVertex3f(eye[0] - wwf * u[0][0] + hhf * u[1][0] + ddf * u[2][0], eye[1] - wwf * u[0][1] + hhf * u[1][1] + ddf * u[2][1], eye[2] - wwf * u[0][2] + hhf * u[1][2] + ddf * u[2][2]); //ltn

    //lbn
    glVertex3f(eye[0] - wwn * u[0][0] - hhn * u[1][0] + ddn * u[2][0], eye[1] - wwn * u[0][1] - hhn * u[1][1] + ddn * u[2][1], eye[2] - wwn * u[0][2] - hhn * u[1][2] + ddn * u[2][2]);
    //ltn
    glVertex3f(eye[0] - wwn * u[0][0] + hhn * u[1][0] + ddn * u[2][0], eye[1] - wwn * u[0][1] + hhn * u[1][1] + ddn * u[2][1], eye[2] - wwn * u[0][2] + hhn * u[1][2] + ddn * u[2][2]);
    //ltf
    glVertex3f(eye[0] - wwf * u[0][0] + hhf * u[1][0] + ddf * u[2][0], eye[1] - wwf * u[0][1] + hhf * u[1][1] + ddf * u[2][1], eye[2] - wwf * u[0][2] + hhf * u[1][2] + ddf * u[2][2]);
    //lbf
    glVertex3f(eye[0] - wwf * u[0][0] - hhf * u[1][0] + ddf * u[2][0], eye[1] - wwf * u[0][1] - hhf * u[1][1] + ddf * u[2][1], eye[2] - wwf * u[0][2] - hhf * u[1][2] + ddf * u[2][2]);

    //rtn
    glVertex3f(eye[0] + wwn * u[0][0] + hhn * u[1][0] + ddn * u[2][0], eye[1] + wwn * u[0][1] + hhn * u[1][1] + ddn * u[2][1], eye[2] + wwn * u[0][2] + hhn * u[1][2] + ddn * u[2][2]);
    //rbn
    glVertex3f(eye[0] + wwn * u[0][0] - hhn * u[1][0] + ddn * u[2][0], eye[1] + wwn * u[0][1] - hhn * u[1][1] + ddn * u[2][1], eye[2] + wwn * u[0][2] - hhn * u[1][2] + ddn * u[2][2]);
    //rbf
    glVertex3f(eye[0] + wwf * u[0][0] - hhf * u[1][0] + ddf * u[2][0], eye[1] + wwf * u[0][1] - hhf * u[1][1] + ddf * u[2][1], eye[2] + wwf * u[0][2] - hhf * u[1][2] + ddf * u[2][2]);
    //rtf
    glVertex3f(eye[0] + wwf * u[0][0] + hhf * u[1][0] + ddf * u[2][0], eye[1] + wwf * u[0][1] + hhf * u[1][1] + ddf * u[2][1], eye[2] + wwf * u[0][2] + hhf * u[1][2] + ddf * u[2][2]);

    //rbn
    glVertex3f(eye[0] + wwn * u[0][0] - hhn * u[1][0] + ddn * u[2][0], eye[1] + wwn * u[0][1] - hhn * u[1][1] + ddn * u[2][1], eye[2] + wwn * u[0][2] - hhn * u[1][2] + ddn * u[2][2]);
    //lbn
    glVertex3f(eye[0] - wwn * u[0][0] - hhn * u[1][0] + ddn * u[2][0], eye[1] - wwn * u[0][1] - hhn * u[1][1] + ddn * u[2][1], eye[2] - wwn * u[0][2] - hhn * u[1][2] + ddn * u[2][2]);
    //lbf
    glVertex3f(eye[0] - wwf * u[0][0] - hhf * u[1][0] + ddf * u[2][0], eye[1] - wwf * u[0][1] - hhf * u[1][1] + ddf * u[2][1], eye[2] - wwf * u[0][2] - hhf * u[1][2] + ddf * u[2][2]);
    //rbf
    glVertex3f(eye[0] + wwf * u[0][0] - hhf * u[1][0] + ddf * u[2][0], eye[1] + wwf * u[0][1] - hhf * u[1][1] + ddf * u[2][1], eye[2] + wwf * u[0][2] - hhf * u[1][2] + ddf * u[2][2]);
    glEnd();

    //------------------------------------�~�u�u----------------------------------------------
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    //rbf
    glVertex3f(eye[0] + wwf * u[0][0] - hhf * u[1][0] + ddf * u[2][0], eye[1] + wwf * u[0][1] - hhf * u[1][1] + ddf * u[2][1], eye[2] + wwf * u[0][2] - hhf * u[1][2] + ddf * u[2][2]);

    glVertex3f(eye[0], eye[1], eye[2]);
    //lbf
    glVertex3f(eye[0] - wwf * u[0][0] - hhf * u[1][0] + ddf * u[2][0], eye[1] - wwf * u[0][1] - hhf * u[1][1] + ddf * u[2][1], eye[2] - wwf * u[0][2] - hhf * u[1][2] + ddf * u[2][2]);

    glVertex3f(eye[0], eye[1], eye[2]);
    //rbf
    glVertex3f(eye[0] + wwf * u[0][0] - hhf * u[1][0] + ddf * u[2][0], eye[1] + wwf * u[0][1] - hhf * u[1][1] + ddf * u[2][1], eye[2] + wwf * u[0][2] - hhf * u[1][2] + ddf * u[2][2]);

    glVertex3f(eye[0], eye[1], eye[2]);
    //rtf
    glVertex3f(eye[0] + wwf * u[0][0] + hhf * u[1][0] + ddf * u[2][0], eye[1] + wwf * u[0][1] + hhf * u[1][1] + ddf * u[2][1], eye[2] + wwf * u[0][2] + hhf * u[1][2] + ddf * u[2][2]);

    glVertex3f(eye[0], eye[1], eye[2]);
    //ltf
    glVertex3f(eye[0] - wwf * u[0][0] + hhf * u[1][0] + ddf * u[2][0], eye[1] - wwf * u[0][1] + hhf * u[1][1] + ddf * u[2][1], eye[2] - wwf * u[0][2] + hhf * u[1][2] + ddf * u[2][2]);

    glVertex3f(eye[0], eye[1], eye[2]);
    //lbf
    glVertex3f(eye[0] - wwf * u[0][0] - hhf * u[1][0] + ddf * u[2][0], eye[1] - wwf * u[0][1] - hhf * u[1][1] + ddf * u[2][1], eye[2] - wwf * u[0][2] - hhf * u[1][2] + ddf * u[2][2]);

    glVertex3f(eye[0], eye[1], eye[2]);
    //rtf
    glVertex3f(eye[0] + wwf * u[0][0] + hhf * u[1][0] + ddf * u[2][0], eye[1] + wwf * u[0][1] + hhf * u[1][1] + ddf * u[2][1], eye[2] + wwf * u[0][2] + hhf * u[1][2] + ddf * u[2][2]); //rtf

    glVertex3f(eye[0], eye[1], eye[2]);
    //ltf
    glVertex3f(eye[0] - wwf * u[0][0] + hhf * u[1][0] + ddf * u[2][0], eye[1] - wwf * u[0][1] + hhf * u[1][1] + ddf * u[2][1], eye[2] - wwf * u[0][2] + hhf * u[1][2] + ddf * u[2][2]); //ltn
    glEnd();

    //------------------------------------���|�諬----------------------------------------------
    glLineWidth(3);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_POLYGON);
    //rbn
    glVertex3f(eye[0] + wwn * u[0][0] - hhn * u[1][0] + ddn * u[2][0], eye[1] + wwn * u[0][1] - hhn * u[1][1] + ddn * u[2][1], eye[2] + wwn * u[0][2] - hhn * u[1][2] + ddn * u[2][2]);
    //lbn
    glVertex3f(eye[0] - wwn * u[0][0] - hhn * u[1][0] + ddn * u[2][0], eye[1] - wwn * u[0][1] - hhn * u[1][1] + ddn * u[2][1], eye[2] - wwn * u[0][2] - hhn * u[1][2] + ddn * u[2][2]);
    //ltn
    glVertex3f(eye[0] - wwn * u[0][0] + hhn * u[1][0] + ddn * u[2][0], eye[1] - wwn * u[0][1] + hhn * u[1][1] + ddn * u[2][1], eye[2] - wwn * u[0][2] + hhn * u[1][2] + ddn * u[2][2]); //ltn
    //rtn
    glVertex3f(eye[0] + wwn * u[0][0] + hhn * u[1][0] + ddn * u[2][0], eye[1] + wwn * u[0][1] + hhn * u[1][1] + ddn * u[2][1], eye[2] + wwn * u[0][2] + hhn * u[1][2] + ddn * u[2][2]); //rtn
    glEnd();

    /*
    glColor4f(1, 0, 1, 0.5);
    glBegin(GL_POLYGON);
    glVertex3f(x2 , y2 , zFar);
    glVertex3f(x2 , -y2, zFar);
    glVertex3f(-x2, -y2, zFar);
    glVertex3f(-x2, y2 , zFar);
    glEnd();
    */
}
void make_projection(int x)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (x == 3) {
        //gluPerspective(fovy, aspect, zNear, zFar); 
        glFrustum(clippingWindowPerspective[_l], clippingWindowPerspective[_r], clippingWindowPerspective[_b], clippingWindowPerspective[_t], clippingWindowPerspective[_n], clippingWindowPerspective[_f]);
    }
    else {
        glOrtho(clippingWindowOrtho[_l], clippingWindowOrtho[_r], clippingWindowOrtho[_b], clippingWindowOrtho[_t], clippingWindowOrtho[_n], clippingWindowOrtho[_f]);
    }
    glMatrixMode(GL_MODELVIEW);
}
void make_view(int x)
{
    //�۾���m    �۾���Ǫ���m   �۾��V�W������
    //gluLookAt(pos[0]-5, 30, pos[2] + 30, pos[0], 15, pos[2], 0.0, 1.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    switch (x) {
    case 0:       /* X direction parallel viewing */
        gluLookAt(0.0, 0.0, 30.0, 30.0, 0.0, 30.0, 0.0, 1.0, 0.0);
        break;
    case 1:       /* Y direction parallel viewing */
        gluLookAt(30.0, 30.0, 30.0, 30.0, 0.0, 30.0, 0.0, 0.0, -1.0);
        break;
    case 2:       /* Z direction parallel viewing */
        gluLookAt(30.0, 0.0, 60.0, 30.0, 0.0, 30.0, 0.0, 1.0, 0.0);
        break;
    case 3:       /* Perspective */
        /* In this sample program, eye position and Xe, Ye, Ze are computed
           by ourselves. Therefore, use them directly; no trabsform is
           applied upon eye coordinate system
           */
        //gluLookAt(30.0, 30.0, 80.0, 30.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        //u�Oeye�ಾ�x�}(transformation matx)
        gluLookAt(eye[0], eye[1], eye[2], eye[0] - u[2][0], eye[1] - u[2][1], eye[2] - u[2][2], u[1][0], u[1][1], u[1][2]);
        glGetFloatv(GL_MODELVIEW_MATRIX, eyeMtx);
        compute_ab_axes();
        //gluLookAt(eye[0], eye[1], eye[2], 0,0, 0, u[1][0], u[1][1], u[1][2]);
        break;
    case 5:
        gluLookAt(pos[0] - 5, 30, pos[2] + 30, pos[0], 15, pos[2], 0.0, 1.0, 0.0);
        glGetFloatv(GL_MODELVIEW_MATRIX, eyeMtx);
        compute_ab_axes();
        break;
    }
}
void setDirectionLight() {
    if (!isLitdirOpen) return;
    glDisable(GL_LIGHTING);

    glColor4f(1, 1, 185 / 255.0,0.9);
    glPushMatrix();
    glTranslatef(litdir_position[0], litdir_position[1], litdir_position[2]);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glutSolidSphere(3, 10, 10);

    glColor4f(1, 0.78, 0.58, 0.5);
    glutSolidSphere(3.8, 10, 10);
    glPopMatrix();
    if (!isLitdirColorOpen) {
        litdir_diffuse[0] = litdirIntensity;
        litdir_diffuse[1] = litdirIntensity;
        litdir_diffuse[2] = litdirIntensity;

        litdir_specular[0] = litdirIntensity;
        litdir_specular[1] = litdirIntensity;
        litdir_specular[2] = litdirIntensity;
    }
    else {
        litdir_diffuse[0] = litdirIntensity * litdirColor[0];
        litdir_diffuse[1] = litdirIntensity * litdirColor[1];
        litdir_diffuse[2] = litdirIntensity * litdirColor[2];

        litdir_specular[0] = litdirIntensity * litdirColor[0];
        litdir_specular[1] = litdirIntensity * litdirColor[1];
        litdir_specular[2] = litdirIntensity * litdirColor[2];
    }
    glLightfv(GL_LIGHT0, GL_POSITION, litdir_direction);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, litdir_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, litdir_specular);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
}
void setPositionLight() {
    if (!isLitposOpen) return;
    //glDisable(GL_LIGHTING);
    //glDisable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
    setMaterial(0, 0, 0, 116 / 255.0, 116 / 255.0, 185 / 255.0, 0);
    glColor3f(0.98,0.98,0.98);
    glPushMatrix();
    glTranslatef(litpos_position[0], litpos_position[1], litpos_position[2]);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glutSolidSphere(3, 10, 10);
    glPopMatrix();

    if (isLitposColorOpen) {
        litpos_diffuse[0] = litposColor[0] * litposIntensity;
        litpos_diffuse[1] = litposColor[1] * litposIntensity;
        litpos_diffuse[2] = litposColor[2] * litposIntensity;
        litpos_specular[0] = litposColor[0] * litposIntensity;
        litpos_specular[1] = litposColor[1] * litposIntensity;
        litpos_specular[2] = litposColor[2] * litposIntensity;
    }
    else {
        litpos_diffuse[0] = litpos_diffuse[1] = litpos_diffuse[2] = litposIntensity;
        litpos_specular[0] = litpos_specular[1] = litpos_specular[2] = litposIntensity;
    }
    glLightfv(GL_LIGHT1, GL_POSITION, litpos_position);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, litpos_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, litpos_specular);

    //glLightfv();
    glEnable(GL_LIGHT1);
    //glEnable(GL_LIGHTING);
}
void setSpotLight() {

    if (!isLitspotOpen) return;

    glPushMatrix();
    glRotatef(myRobot.angle_y, 0, 1, 0);
    glRotatef(myRobot.angle_x, 1, 0, 0);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, litspot_direction);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]);      //�����H��m
    glPushMatrix();
    glScalef(2.5, 2.5, 2.5);
    glTranslatef(myRobot.x, myRobot.y, myRobot.z);                      //�㰦���y��
    glRotatef(myRobot.angle_y, 0, 1, 0);
    glRotatef(myRobot.angle_x, 1, 0, 0);

    glTranslatef(0, 4.75, 0);                   //����{�l
    node tp = ball_cor(2, 90, 30);
    glPushMatrix();
    glTranslatef(tp.x, tp.y, tp.z);             //����k�ӻH
    glRotatef(myRobot.right_h->shoulderAng_z, 0, 0, 1);             //-35��b������� ��z�౱��k
    glRotatef(myRobot.right_h->shoulderAng_x, 1, 0, 0);
    //forarms
    glTranslatef(0, 0.75, 0);               //���� �ӻH�W��0.25 + �ꤤ��0.75(�e1.5�����u) - 0.25���|
    glTranslatef(0, 0.75, 0);               //���u�e�ݤ���
    glRotatef(myRobot.right_h->elbowAng_x, 1, 0, 0);         //�����y
    glRotatef(myRobot.right_h->elbowAng_y, 0, 1, 0);
    glRotatef(myRobot.right_h->elbowAng_z, 0, 0, 1);
    //��e�u
    glTranslatef(0, 0.75, 0);                //���� ��b�e��0.25 + �ꤤ��0.75(�e1.5����e�u) - 0.25���|
    glTranslatef(0, 0.75, 0);               //��e�u�e��   
    //�������V���Ӧb�o��
    glRotatef(myRobot.right_h->fingerAng_y, 0, 1, 0);
    glPushMatrix();
    glTranslatef(0, 0.3, 0);
    glRotatef(myRobot.magic_wand_carry->angle_x, 1, 0, 0);
    glRotatef(myRobot.magic_wand_carry->angle_y, 0, 1, 0);
    glRotatef(myRobot.magic_wand_carry->angle_z, 0, 0, 1);
    glScalef(myRobot.magic_wand_carry->scale, myRobot.magic_wand_carry->scale, myRobot.magic_wand_carry->scale);
    glTranslatef(0, 0, -7);
    //�����n�Ϯg �Ϯg�E�J ����o��
    setMaterial(0.316228, 0.316228, 0.316228, 1, 1, 168 / 255.0, 64);
    //glColor3f(1, 0, 0);              //����(��)
    glPushMatrix();
    glTranslatef(0, 0, 14);
    //����

    glutSolidSphere(2, 10, 10);                 //9.5
    glLightfv(GL_LIGHT2, GL_POSITION, litspot_position);           //0,0,0

    glPopMatrix();
    glPopMatrix();                              //pop3
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    
    litspot_diffuse[0] = litspot_diffuse[1] = litspot_diffuse[2] = litspotIntensity;
    litspot_specular[0] = litspot_specular[1] = litspot_specular[2] = litspotIntensity;

    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, litspotCutoffAng);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, litspot_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, litspot_specular);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 0);
    //glLightf(GL_LIGHT2,GL_);

   glEnable(GL_LIGHT2);
   glEnable(GL_LIGHTING);
}
void setFireworkLight() {
    if (isLitfireOpen == 0) return;

    if (myFirework[0].cnt != 0) {
        glPushMatrix();
        glTranslatef(0,0,0);
        myFirework[0].draw(1);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(110, -10, 0);
        myFirework[0].draw(1);
        glPopMatrix();
    }
    if (myFirework[1].cnt != 0) {
        glPushMatrix();
        glTranslatef(15, -7, 0);
        myFirework[1].draw(2);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(115, 15, 0);
        myFirework[1].draw(2);
        glPopMatrix();
    }
    if (myFirework[2].cnt != 0) {
        glPushMatrix();
        glTranslatef(30, 5, 0);
        myFirework[2].draw(3);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(130, -1, 0);
        setMaterial(0, 0, 0, 1, 0, 0, 0);
        myFirework[2].draw(3);
        glPopMatrix();
    }

    if (myFirework[3].cnt != 0) {
        glPushMatrix();
        glTranslatef(50, 3, 0);
        myFirework[3].draw(1);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(150,10, 0);
        myFirework[3].draw(1);
        glPopMatrix();
    }
    if (myFirework[4].cnt != 0) {
        glPushMatrix();
        glTranslatef(70, 12, 0);
        myFirework[4].draw(2);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(170, -3, 0);
        myFirework[4].draw(2);
        glPopMatrix();
    }
    if (myFirework[5].cnt != 0) {
        glPushMatrix();
        glTranslatef(88, 0, 0);
        myFirework[5].draw(3);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(200, 10, 0);
        myFirework[5].draw(3);
        glPopMatrix();
    }
    litfire_diffuse[0] = litfire_diffuse[1] = litfire_diffuse[2] = litfireIntensity;
    litfire_specular[0] = litfire_specular[1] = litfire_specular[2] = litfireIntensity;

    glLightfv(GL_LIGHT2, GL_POSITION, litfire_position);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, litfire_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, litfire_specular);

    //glLightfv();
    //glEnable(GL_LIGHT2);
    //glEnable(GL_LIGHTING);
}
void display()
{
    //printf("display\n");
    /*Clear previous frame and the depth buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*----Define the current eye position and the eye-coordinate system---*/
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    int vs = 0;
    if (scene == GRASSLAND) {   //���u��2�ص���
        if (viewStyle & 1) vs = 5;
        else vs = 3;
    }
    else {
        vs = viewStyle;
    }
    switch (vs) {
    case 0:
        glViewport(0, 0, width, height);
        make_projection(0);
        make_view(0);
        setDirectionLight();
        setPositionLight();
        setSpotLight();
        draw_view();
        break;
    case 1:
        glViewport(0, 0, width, height);
        make_projection(0);
        make_view(1);
        setDirectionLight();
        setPositionLight();
        setSpotLight();
        draw_view();
        break;
    case 2:
        glViewport(0, 0, width, height);
        make_projection(0);
        make_view(2);
        setDirectionLight();
        setPositionLight();
        setSpotLight();
        draw_view();
        break;
    case 3:                                      //�z����v
        glViewport(0, 0, width, height);
        make_projection(3);
        make_view(3);
        setDirectionLight();
        setPositionLight();
        setSpotLight();
        setSpotLight();
        setFireworkLight();
        draw_view();
        break;
    case 4:
        make_projection(0);

        glViewport(0, height / 2, width / 2, height / 2);  //���U �e��
        make_view(0);
        setDirectionLight();
        setPositionLight();     
        setSpotLight();
        draw_view();
        
        glViewport(width / 2, height / 2, width / 2, height / 2);
        make_view(1);
        setDirectionLight();
        setPositionLight(); 
        setSpotLight();
        draw_view();

        glViewport(0, 0, width / 2, height / 2);
        make_view(2);
        setDirectionLight();
        setPositionLight();
        setSpotLight();
        draw_view();

        glViewport(width / 2, 0, width / 2, height / 2);
        make_projection(3);
        make_view(3);
        setDirectionLight();
        setPositionLight();
        setSpotLight();
        draw_view();

        break;
    case 5:
        glViewport(0, 0, width, height);
        make_projection(0);
        make_view(5); //lookat
        setDirectionLight();
        setPositionLight();   
        setSpotLight();
        setFireworkLight();
        draw_view();
        break;
    }

    //�ʧ@
    //�۾���m    �۾���Ǫ���m   �۾��V�W������

    glutSwapBuffers();
    return;
}
void my_reshape(int w, int h)
{
    printf("res\n");
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //glOrtho(-40.0, 50.0, -40.0, 40.0, 0.0, 120); 

   //if(see)
    glOrtho(-40.0, 40.0, -40.0, 40.0, -100.0, 200);
    //else 
    //   glOrtho(-100.0, 100.0, -100.0, 100.0, -100.0, 200);
    width = w; height = h;
}
void timerFunc(int nTimerID) {
    int tp = 0,tp1 = 0,tp2 = 0,tp3,tp4,tp5;
    switch (nTimerID) {
    case RUNTIMER:                //�����]
        preKey = -1;
        break;
    case JUMPTIMER:               //���D
        if (!myRobot.jump()) {
            isLock = LOCK;        //���ɤ��i�H��
            glutTimerFunc(100, timerFunc, JUMPTIMER);
        }
        else {
            if(isLitspotOpen)  myRobot.carryLight();
            else myRobot.stand();
            isLock = UNLOCK;
        }
        glutPostRedisplay();
        break;
    case JUMPONWANDTIMER:           //���W�k��
        if (!myRobot.jumpOnWand()) {
            isLock = LOCK;
            glutTimerFunc(100, timerFunc, JUMPONWANDTIMER);
        }
        else {
            isLock = UNLOCK;
        }
        glutPostRedisplay();
        break;
    case JUMPTOFLOORTIMER:          //���^�a��
        if (!myRobot.jumpToFloor()) {
            isLock = LOCK;
            glutTimerFunc(100, timerFunc, JUMPTOFLOORTIMER);
        }
        else {
            isLock = UNLOCK;
            if (isLitspotOpen)  myRobot.carryLight();
            else myRobot.stand();
        }
        glutPostRedisplay();
        break;
    case GRASSLAND_ANIMATION:                //�n���\�\�\
        myBig_chair.move();
        poolAng++;
        eevee_ani++;
        if (eevee_ani >= 70) eevee_ani = 14;
        if (scene == GRASSLAND) {
            glutTimerFunc(100, timerFunc, GRASSLAND_ANIMATION);
        }
        glutPostRedisplay();
        break;
    case DEBUG_MODE:
        if (debugModeCmd == 12) return;
        else glutTimerFunc(100 - debugModeCmd*4, timerFunc, DEBUG_MODE);
        debugModeCmd++;
        glutPostRedisplay();
        break;
    case OUT_LINE_LEFT:
        if (myFloor.light[2] == 12) {
            myFloor.light[2] = 0;
            return;
        }
        myFloor.light[2]++;
        glutTimerFunc(100, timerFunc, OUT_LINE_LEFT);
        glutPostRedisplay();
        break;
    case OUT_LINE_RIGHT:
        if (myFloor.light[3] == 12) {
            myFloor.light[3] = 0;
            return;
        }
        myFloor.light[3]++;
        glutTimerFunc(100, timerFunc, OUT_LINE_RIGHT);
        glutPostRedisplay();
        break;
    case OUT_LINE_FRONT:
        if (myFloor.light[4] == 12) {
            myFloor.light[4] = 0;
            return;
        }
        myFloor.light[4]++;
        glutTimerFunc(100, timerFunc, OUT_LINE_FRONT);
        glutPostRedisplay();
        break;
    case OUT_LINE_BACK:
        if (myFloor.light[5] == 12) {
            myFloor.light[5] = 0;
            return;
        }
        myFloor.light[5]++;
        glutTimerFunc(100, timerFunc, OUT_LINE_BACK);
        glutPostRedisplay();
        break;
    case TREE_LIGHT:
        do_tree_rand(30);
        if (scene == GRASSLAND) {
            glutTimerFunc(500, timerFunc, TREE_LIGHT);
        }
        glutPostRedisplay();
        break;
    case FIREWORK0:
        tp = myFirework[0].changeStatus();
        if (tp <= 60) {
           glutTimerFunc(33 + tp*0.85, timerFunc, FIREWORK0);
        }
        else if (tp <= 80) {
            glutTimerFunc( 40, timerFunc, FIREWORK0);
        }
        else if (tp <= 90) {
            glutTimerFunc(60, timerFunc, FIREWORK0);
        }else myFirework[0].resetStatus();
        glutPostRedisplay();
        break;
    case FIREWORK1:
        tp1 = myFirework[1].changeStatus();
        if (tp1 <= 50) {
           glutTimerFunc(38 + tp*0.85, timerFunc, FIREWORK1);
        }
        else if (tp1 <= 70) {
            glutTimerFunc( 40, timerFunc, FIREWORK1);
        }
        else if (tp1 <= 85) {
            glutTimerFunc(40, timerFunc, FIREWORK1);
        }
        else if (tp1 <= 110) {
            glutTimerFunc(35, timerFunc, FIREWORK1);
        }
        else if (tp1 <= 120) {
            glutTimerFunc(55, timerFunc, FIREWORK1);
        }
        else myFirework[1].resetStatus();
        glutPostRedisplay();
        break;
    case FIREWORK2:
        tp2 = myFirework[2].changeStatus();
        if (tp2 <= 40) {
            glutTimerFunc(38 + tp * 0.85, timerFunc, FIREWORK2);
        }
        else if (tp2 <= 55) {
            glutTimerFunc(50, timerFunc, FIREWORK2);
        }
        else if (tp2 <= 65) {
            glutTimerFunc(44, timerFunc, FIREWORK2);
        }
        else if (tp2 <= 80) {
            glutTimerFunc(45, timerFunc, FIREWORK2);
        }
        else if (tp2 <= 90) {
            glutTimerFunc(40, timerFunc, FIREWORK2);
        }
        else myFirework[2].resetStatus();
        //litfire_position[1] = max(50, tp2);
        glutPostRedisplay();
        break;
    case FIREWORK3:
        tp3 = myFirework[3].changeStatus();
        if (tp3 <= 60) {
            glutTimerFunc(30 + tp * 0.75, timerFunc, FIREWORK3);
        }
        else if (tp3 <= 80) {
            glutTimerFunc(37, timerFunc, FIREWORK3);
        }
        else if (tp3 <= 90) {
            glutTimerFunc(50, timerFunc, FIREWORK3);
        }
        else myFirework[3].resetStatus();
        glutPostRedisplay();
        break;
    case FIREWORK4:
        tp4 = myFirework[4].changeStatus();
        if (tp4 <= 50) {
            glutPostRedisplay();
            glutTimerFunc(40 + tp * 0.55, timerFunc, FIREWORK4);
        }
        else if (tp4 <= 70) {
            glutPostRedisplay();
            glutTimerFunc(50, timerFunc, FIREWORK4);
        }
        else if (tp4 <= 85) {
            glutPostRedisplay();
            glutTimerFunc(30, timerFunc, FIREWORK4);
        }
        else if (tp4 <= 110) {
            glutPostRedisplay();
            glutTimerFunc(25, timerFunc, FIREWORK4);
        }
        else if (tp4 <= 120) {
            glutPostRedisplay();
            glutTimerFunc(40, timerFunc, FIREWORK4);
        }
        else myFirework[4].resetStatus();
        glutPostRedisplay();
        break;
    case FIREWORK5:
        tp5 = myFirework[5].changeStatus();
        if (tp5 <= 40) {
            glutPostRedisplay();
            glutTimerFunc(47 + tp * 0.35, timerFunc, FIREWORK5);
        }
        else if (tp5 <= 55) {
            glutPostRedisplay();
            glutTimerFunc(40, timerFunc, FIREWORK5);
        }
        else if (tp5 <= 65) {
            glutPostRedisplay();
            glutTimerFunc(54, timerFunc, FIREWORK5);
        }
        else if (tp5 <= 80) {
            glutPostRedisplay();
            glutTimerFunc(65, timerFunc, FIREWORK5);
        }
        else if (tp5 <= 90) {
            glutPostRedisplay();
            glutTimerFunc(30, timerFunc, FIREWORK5);
        }
        else myFirework[5].resetStatus();
        glutPostRedisplay();
        break;
    case FIREWORKLIGHT:
        if (litfireCnt <= 60) {
            litfireCnt++;
            litfire_position[1]++;
            litfireIntensity += 1/60.0;
            glutPostRedisplay();
            glutTimerFunc(40, timerFunc, FIREWORKLIGHT);
        }
        else if (litfireCnt == 61) {
            litfireCnt++;
            //cout << "61\n";
            litfireIntensity = 1;
            glutPostRedisplay();
            glutTimerFunc(9750, timerFunc, FIREWORKLIGHT);
        }
        else if(litfireCnt <= 101) {
            //cout << litfireCnt << " ";
            litfireCnt++;
            litfireIntensity -= 1/40.0;
            //cout << litfireIntensity << "\n";
            glutPostRedisplay();
            glutTimerFunc(50, timerFunc, FIREWORKLIGHT);
        }
        else {
            litfire_position[1] = 0;
            litfireIntensity = 0;
            isLitfireOpen = 0;
            litfireCnt = 0;
            glDisable(GL_LIGHT2);
            glutPostRedisplay();
        }
        
    }
}
bool detectCollision(int x, int y, int z) { //�����I��
    //�P�_�I���ê��
    for (int i = -7; myMagic_wand.show && i < 7; i++) {
        if (getDis(x, z, myMagic_wand.x, myMagic_wand.z + i) < 3.5) return 1;  //magic wand
    }
    if (scene == GRASSLAND) {
        if (myRobot.moveMode != FLY) {
            for (auto i : limit) {
                if (getDis(x, z, i.first, i.second) < 5) return 1;     //river
            }
            if (getDis(x, z, 30, 160) < 30) return 1;                  //pool
        }
        for (auto i : pillar) {
            if (getDis(x, z, i.first, i.second) < 1.25 + 2 * 2.5) return 1;     //pillar �W�l    
        }
        if (getDis(x, z, myBig_chair.x, myBig_chair.z) < 18 + 2 * 2.5) return 1;  //chair
    }

    //�P�_���
    if (scene == MAGICFIELD) {
        if (debugMode) {
            if (x < 0) { //��
                if(!myFloor.light[2]) glutTimerFunc(100, timerFunc, OUT_LINE_LEFT);
            }
            else if (x > 60) { //�k
                if (!myFloor.light[3])glutTimerFunc(100, timerFunc, OUT_LINE_RIGHT);
            }
            else if (z < 0) { //��
                if (!myFloor.light[5])glutTimerFunc(100, timerFunc, OUT_LINE_BACK);
            }
            else if (z > 60) { //�e
                if(!myFloor.light[4]) glutTimerFunc(100, timerFunc, OUT_LINE_FRONT);
            }
        }
        if (x < 0 || x > 60 || z < 0 || z > 60) return 1;
    }
    else if (scene == GRASSLAND) {
        if (x < 0 || x > 200 || z < 0 || z > 200) return 1;
    }
    return 0;
}
void my_move_order(unsigned char key) {        //�򲾰ʬ������P�_
    if (sitOnChair) return; //�p�G���b�Ȥl�W�N�����
    float tpPos[3] = { pos[0], pos[1], pos[2] };
    float offset = 0;
    if (myRobot.moveMode == WALK && preKey == key && (key == 'W' || key == 'w' || key == 'A' || key == 'a' || key == 'S' || key == 's' || key == 'D' || key == 'd'))
        myRobot.moveMode = RUN;                       //0.3���s��� �N�ܦ��]�]
    if (myRobot.moveMode == RUN) offset = 2;          //�]�B�@����2
    else if (myRobot.moveMode == WALK) offset = 0.5;  //�����@��0.5
    else if (myRobot.moveMode == FLY) offset = 3;     //����@��5

    if (key == 'S' || key == 's') {
        if (myRobot.isOnWand) {        //����Ҧ�
            myRobot.angle_y = 90;
        }
        else {                         //�@�먫���Ҧ�
            myRobot.angle_y = 0;
            myRobot.move();
        }
        tpPos[2] += offset;
    }
    else if (key == 'W' || key == 'w') {
        if (myRobot.isOnWand) {
            myRobot.angle_y = 90;
        }
        else {
            myRobot.angle_y = 180;
            myRobot.move();
        }
        tpPos[2] -= offset;
    }
    else if (key == 'A' || key == 'a') {
        if (myRobot.isOnWand) {
            myRobot.angle_y = 0;
        }
        else {
            myRobot.angle_y = 270;
            myRobot.move();
        }
        tpPos[0] -= offset;
    }
    else if (key == 'D' || key == 'd') {
        if (myRobot.isOnWand) {
            myRobot.angle_y = 0;
        }
        else {
            myRobot.angle_y = 90;
            myRobot.move();
        }
        tpPos[0] += offset;
    }
    else if (key == 'r' || key == 'R') {            //����
        myRobot.angle_y += 5;
        angley++;
        if (!myRobot.isOnWand) {
            myRobot.moveMode = TURN;
            myRobot.move();    //�b�a�O�~�n�ʸ}
        }
    }
    if (detectCollision(tpPos[0], tpPos[1], tpPos[2])) return;
    for (int i = 0; i < 3; i++) pos[i] = tpPos[i];
    display();
}
bool change_view_order(unsigned char key) {
    //cout << key << "\n";
    if (key == 'y' || key == 'Y') {
        viewStyle++;
        viewStyle %= 6;
        display();
        return 1;
    }
    if (key == 'x' || key == 'X') {
        if (scene != MAGICFIELD) return 1;   //�u���b�]�k�}���o�ӼҦ�
        if (debugModeCmd != 0 && debugModeCmd != 12) return 1;  //�e���ʵe�S�]�� ���n�@�Ȩ�s
        debugMode ^= 1;
        debugModeCmd = 0;
        if (debugMode) {
            glutTimerFunc(100, timerFunc, DEBUG_MODE);
        }
        return 1;
    }
    float  x[3], y[3], z[3];
    int i;
    if (key == 19) {       //�U ctrl + w
        for (int i = 0; i < 3; i++) eye[i] -= 1 * u[1][i];  
    }
    else if (key == 23) {   //�W ctrl + s    
        for (int i = 0; i < 3; i++) eye[i] += 1 * u[1][i];
    }
    else if (key == 4) {   //�k ctrl + d     
        for (int i = 0; i < 3; i++) eye[i] += 1 * u[0][i];
    }
    else if (key == 1) {   //�� ctrl + a   
        for (int i = 0; i < 3; i++) eye[i] -= 1 * u[0][i];
    }
    if (key == 17) {  //���e ctrl + q
        for (i = 0; i < 3; i++) eye[i] -= 1 * u[2][i];
    }
    else if (key == 5) { //���� ctrl + e
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
    else if (key == 127) { //ctrl + backspace
        init_camera();
    }
    else if (key == 3) { //zoom in ctrl + c
        if (fabs(clippingWindowPerspective[_l] - clippingWindowPerspective[_r]) > 10 || fabs(clippingWindowPerspective[_l] - clippingWindowPerspective[_r]) > 10 *width / height) {
            clippingWindowPerspective[_l] += 2;
            clippingWindowPerspective[_r] -= 2;
            clippingWindowPerspective[_b] += 2 * width / height;
            clippingWindowPerspective[_t] -= 2 * width / height;
        }   
        if (fabs(clippingWindowOrtho[_l] - clippingWindowOrtho[_r]) > 10 || fabs(clippingWindowOrtho[_l] - clippingWindowOrtho[_r]) > 10 * width / height) {
            clippingWindowOrtho[_l] += 2;
            clippingWindowOrtho[_r] -= 2;
            clippingWindowOrtho[_b] += 2 * width / height;
            clippingWindowOrtho[_t] -= 2 * width / height;
        }
    }
    else if (key == 22) { //zoom out ctrl + v
        if (fabs(clippingWindowPerspective[_l] - clippingWindowPerspective[_r]) < 45 || fabs(clippingWindowPerspective[_l] - clippingWindowPerspective[_r]) < 45 * width / height) {
            clippingWindowPerspective[_l] -= 2;
            clippingWindowPerspective[_r] += 2;
            clippingWindowPerspective[_b] -= 2 * width / height;
            clippingWindowPerspective[_t] += 2 * width / height;
        }
        if (fabs(clippingWindowOrtho[_l] - clippingWindowOrtho[_r]) < 150 || fabs(clippingWindowOrtho[_l] - clippingWindowOrtho[_r]) < 150 * width / height) {
            clippingWindowOrtho[_l] -= 2;
            clippingWindowOrtho[_r] += 2;
            clippingWindowOrtho[_b] -= 2 * width / height;
            clippingWindowOrtho[_t] += 2 * width / height;
        }
    }
    else if (key == 2) { //�����h ctrl + b
        if(clippingWindowPerspective[_f] < 1000) clippingWindowPerspective[_f] += 2;
    }
    else if (key == 14) { //������ ctrl + n
        if (clippingWindowPerspective[_f] > 40) clippingWindowPerspective[_f] -= 2;
    }
    else if (key == 16) {  //ctrl + p
        camera_show ^= 1;
        if (camera_show) view_volume_show = 1;
    }
    else if (key == 15) { //ctrl + o
        if(camera_show) view_volume_show ^= 1;
    }
     else return 0;
    display();
}
void special_func(int key, int x, int y) {
    cout <<"special: " << key << "\n";
    if (change_view_order(key)) return;
}
void keyboardUp_func(unsigned char key, int x, int y) {
    if (isLock == LOCK) return;
    glutTimerFunc(200, timerFunc, RUNTIMER);
    if (myRobot.moveMode != FLY && preKey != key)  myRobot.moveMode = WALK;
    preKey = key;
    if (myRobot.moveMode != FLY && (key == 'W' || key == 'w' || key == 'A' || key == 'a' || key == 'S' || key == 's' || key == 'D' || key == 'd' || key == 'r' || key == 'R')) {
        if (isLitspotOpen)  myRobot.carryLight();
        else myRobot.stand();
    }
    if (key == ' ') {                   //��
        if (myRobot.isMagician) {
            if (myRobot.isOnWand) {
                glutTimerFunc(100, timerFunc, JUMPTOFLOORTIMER);
                myRobot.moveMode = WALK;
            }
            else {
                myRobot.jump_ready();
                glutTimerFunc(100, timerFunc, JUMPONWANDTIMER);
                myRobot.moveMode = FLY;
                glDisable(GL_LIGHT2);
                isLitspotOpen = 0;
            }
        }
        else {
            myRobot.jump_ready();
            glutTimerFunc(100, timerFunc, JUMPTIMER);
        }
    }
    display();
}
void keybaord_fun(unsigned char key, int x, int y) {
    if(change_view_order(key)) return;
    if (isLock == LOCK) return;
    my_move_order(key);
    if ((key == 'p' || key == 'P')) {                     //���ʪF��
        if (myMagic_wand.show) {
            for (int i = -7; i < 7; i++) {
                if (getDis(pos[0], pos[2], myMagic_wand.x, myMagic_wand.z + i) < 3.5 + 5) {           //���k��
                    myMagic_wand.show = 0;
                    myRobot.carry_mw = 1;
                    myRobot.isMagician = 1;
                }
            }
        }
        if (scene == GRASSLAND && getDis(pos[0], pos[2], myBig_chair.x, myBig_chair.z) < 18 + 2 * 2.5 + 2) {   //���W�Ȥl
            sitOnChair = !sitOnChair;
        }
        if (scene == GRASSLAND && getDis(pos[0], pos[2], 100, 6) < 10 && !isLitposOpen && !isLitdirOpen && !isLitspotOpen && isLighting) {
            isLitfireOpen = 1;
            glEnable(GL_LIGHT2);
            glutTimerFunc(660, timerFunc, FIREWORK0);
            glutTimerFunc(500, timerFunc, FIREWORK1);
            glutTimerFunc(550, timerFunc, FIREWORK2);
            glutTimerFunc(770, timerFunc, FIREWORK3);
            glutTimerFunc(650, timerFunc, FIREWORK4);
            glutTimerFunc(700, timerFunc, FIREWORK5);

            glutTimerFunc(3650, timerFunc, FIREWORK0);
            glutTimerFunc(3650, timerFunc, FIREWORK1);
            glutTimerFunc(4000, timerFunc, FIREWORK2);
            glutTimerFunc(3850, timerFunc, FIREWORK3);
            glutTimerFunc(4050, timerFunc, FIREWORK4);
            glutTimerFunc(3800, timerFunc, FIREWORK5);

            glutTimerFunc(8200, timerFunc, FIREWORK0);
            glutTimerFunc(8100, timerFunc, FIREWORK1);
            glutTimerFunc(8350, timerFunc, FIREWORK2);
            glutTimerFunc(8750, timerFunc, FIREWORK3);
            glutTimerFunc(8550, timerFunc, FIREWORK4);
            glutTimerFunc(8250, timerFunc, FIREWORK5);

            glutTimerFunc(500, timerFunc, FIREWORKLIGHT);
        }
    }
    else if ((int)key == 13) {          //enter �i�J��a(�_�l�I17 12)
        if (scene == MAGICFIELD && myRobot.isMagician && getDis(pos[0], pos[2], 30, 30) < 30) {
            pos[0] = 17;
            pos[2] = 12;
            scene = GRASSLAND;
            glutTimerFunc(100, timerFunc, GRASSLAND_ANIMATION);
            glutTimerFunc(1000, timerFunc, TREE_LIGHT);
            //�����
            litdir_init[0] = 100;
            litdir_init[1] = 0;
            litdir_init[2] = 100;
            litdirR = 120;
            //�I��
            litpos_position[0] = 100;
            litpos_position[1] = 70;
            litpos_position[2] = 100;
        }                               //enter �i�J�]�k�}(�_�l�I30 30)
        else if (scene == GRASSLAND && getDis(pos[0], pos[2], 17, 12) <= 10) {
            pos[0] = pos[2] = 30;
            scene = MAGICFIELD;
            //�����
            litdir_init[0] = 30;
            litdir_init[1] = 0;
            litdir_init[2] = 30;
            litdirR = 50;
            //�I��
            litpos_position[0] = 30;
            litpos_position[1] = 70;
            litpos_position[2] = 30;
        }
        litdir_position[0] = litdirR * cos(litdirAng * 0.01745) + litdir_init[0];
        litdir_position[1] = litdirR * sin(litdirAng * 0.01745) + litdir_init[1];
        litdir_position[2] = litdir_init[2];

        litdir_direction[0] = litdir_position[0] - litdir_init[0];
        litdir_direction[1] = litdir_position[1] - litdir_init[1];
        litdir_direction[2] = litdir_position[2] - litdir_init[2];
    }
    if (isLighting && key == '1') {   //�n���O�O�Ҧ��~�i�H���}�O
        isLitdirOpen ^= 1;
        if (isLitdirOpen) {
            glEnable(GL_LIGHT0);
        }
        else {
            glDisable(GL_LIGHT0);
        }
    }
    else if (isLighting && key == '2') {   //�C��model
        isLitdirColorOpen ^= 1;
    }
    else if (isLitdirOpen && key == '3') {
        litdirAng += 1;
        if (litdirAng > 180) litdirAng = 0;

        litdir_position[0] = litdirR * cos(litdirAng * 0.01745) + litdir_init[0];
        litdir_position[1] = litdirR * sin(litdirAng * 0.01745) + litdir_init[1];
        litdir_position[2] = litdir_init[2];

        litdir_direction[0] = litdir_position[0] - litdir_init[0];
        litdir_direction[1] = litdir_position[1] - litdir_init[1];
        litdir_direction[2] = litdir_position[2] - litdir_init[2];

        if (litdirAng  <= 20) {  //0 ~ 20  rgb�]255,69,0�^ ��  +130,+143
            litdirColor[0] = 1;
            litdirColor[1] = (69 + (litdirAng * 130.0 / 20.0)) / 255.0;
            litdirColor[2] = (0 + (litdirAng * 143.0 / 20.0)) / 255.0;
        }
        else if (litdirAng <= 40) { //20 ~40 rgb(255, 199, 143) �� +56 +61
            litdirColor[0] = 1;
            litdirColor[1] = (199 + ((litdirAng-20.0) * 56.0 / 20.0)) / 255.0;
            litdirColor[2] = (143 + ((litdirAng-20.0) * 61.0 / 20.0)) / 255.0;
        }
        else if (litdirAng <= 70) { //40 ~ 70 rgb(255, 255, 204) +51
            litdirColor[0] = 1;
            litdirColor[1] = 1;
            litdirColor[2] = (204 + ((litdirAng-40.0) * 51.0 / 30.0)) / 255.0;
        }
        else if(litdirAng <= 110){ //70 ~ 110 �� 
            litdirColor[0] = 1;
            litdirColor[1] = 1;
            litdirColor[2] = 1;
        }
        else if (litdirAng <= 140) {  //110 ~ 140 �� -> �� rgb(255, 255, 204) -51
            litdirColor[0] = 1;
            litdirColor[1] = 1;
            litdirColor[2] = (255 - ((litdirAng - 110.0) * 51.0 / 30.0)) / 255.0;
            //cout << litdirColor[2] << " \n";
        }
        else if (litdirAng <= 160) {  //140 ~ 160 �� -> �� rgb(255, 199, 143) -56 -61
            litdirColor[0] = 1;
            litdirColor[1] = (255 - ((litdirAng - 140.0) * 56.0 / 20.0)) / 255.0;
            litdirColor[2] = (204 - ((litdirAng - 140.0) * 61.0 / 20.0)) / 255.0;
        }
        else if (litdirAng < 180) {  //160 ~ 180 ��->��  rgb�]255,69,0�^ -130,-143
            litdirColor[0] = 1;
            litdirColor[1] = (199 - ((litdirAng - 160.0) * 130.0 / 20.0)) / 255.0;
            litdirColor[2] = (143 - ((litdirAng - 160.0) * 143.0 / 20.0)) / 255.0;
        }

    }
    else if(isLitdirOpen && key == '4') {
        litdirIntensity -= 0.1;
        litdirIntensity = fmax(litdirIntensity, 0);
    }
    else if (isLitdirOpen && key == '5') {
        litdirIntensity += 0.1;
        litdirIntensity = fmin(litdirIntensity, 1);
    }
    else if (isLighting && key == '6') {
        isLitposOpen ^= 1;
        if (isLitposOpen) {
            glEnable(GL_LIGHT1);
        }
        else {
            glDisable(GL_LIGHT1);
        }
    }
    else if (isLitposOpen && key == '7') {
        isLitposColorOpen ^= 1;
    }
    else if (isLitposOpen && key == '8') {
        litposIntensity -= 0.1;
        litposIntensity = fmax(litposIntensity, 0);
    }
    else if (isLitposOpen && key == '9') {
        litposIntensity += 0.1;
        litposIntensity = fmin(litposIntensity, 1);
    }
    else if (key == '0') {
        isLighting ^= 1;
        if (isLighting) {
            glEnable(GL_LIGHTING);
        }
        else {
            glDisable(GL_LIGHTING);
            glDisable(GL_LIGHT1);
            glDisable(GL_LIGHT0);
            glDisable(GL_LIGHT2);
            isLitdirOpen = 0;
            isLitposOpen = 0;
            isLitspotOpen = 0;
        } 
    }
    else if (key == 'g' || key == 'G') {  //�}�O
    if (myRobot.moveMode == FLY) return;
        isLitspotOpen ^= 1;
        if (isLitspotOpen) {
            glEnable(GL_LIGHT2);
            myRobot.carryLight();
        }
        else {
            glDisable(GL_LIGHT2);
            myRobot.stand();
        }
    }
    else if (key == 'h' || key == 'H') {  //���z
        litspotIntensity = fmax(0, litspotIntensity - 0.1);
    }
    else if (key == 'j' || key == 'J') {  //���j
        litspotIntensity = fmin(1, litspotIntensity + 0.1);
    }
    else if (key == 'k' || key == 'K') {  //cutoff �p
        litspotCutoffAng = fmax(15.0, litspotCutoffAng - 5);
        //cout << litspotCutoffAng << "\n";
    }
    else if (key == 'l' || key == 'L') {  //cutoff �j
        litspotCutoffAng = fmin(90.0, litspotCutoffAng + 5);
        //cout << litspotCutoffAng << "\n";
    }
    else if (key == 'c' || key == 'C') {
        isFogOpen ^= 1;
        if (isFogOpen) glEnable(GL_FOG);
        else glDisable(GL_FOG);
    }
    else if (key == 'v' || key == 'V') {
        fogMode++;
        fogMode %= 3;
        int mode = 0;
        if (fogMode == 0) mode = GL_LINEAR;
        else if (fogMode == 1) mode = GL_EXP;
        else mode = GL_EXP2;
        glFogi(GL_FOG_MODE, mode);
    }
    else if (key == 'b' || key == 'B') {
        fogColorMode++;
        fogColorMode %= 2;
        if (fogColorMode == 0) {
            fogColor[0] = fogColor[1] = fogColor[2] = 1;
        }
        //rgb(92,108,108)�I����
        else if (fogColorMode == 1) {
            fogColor[0] = 92 / 255.0;
            fogColor[1] =  fogColor[2] = 108 / 255.0;
        }
        fogColor[3] = 0.1;
        glFogfv(GL_FOG_COLOR, fogColor);/*set the fog color */
    }
    else if (key == 'n' || key == 'N') { //�H
        fogOpacity = max((fogOpacity - 0.01), 0.01);
        glFogf(GL_FOG_DENSITY, fogOpacity);
    }
    else if (key == 'm' || key == 'M') {
        fogOpacity = min((fogOpacity + 0.01), 0.1);
        glFogf(GL_FOG_DENSITY, fogOpacity);
    }
    display();
}
int main(int argc, char** argv)
{
    srand(time(NULL));
    /*-----Initialize the GLUT environment-------*/
    glutInit(&argc, argv);

    /*-----Depth buffer is used, be careful !!!----*/
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(600,600);
    glutCreateWindow("RobotTexture");

    myinit();      /*---Initialize other state varibales----*/

    /*----Associate callback func's whith events------*/
    glutDisplayFunc(display);

    glutReshapeFunc(my_reshape);
    glutKeyboardFunc(keybaord_fun);

    glutSpecialFunc(special_func);
    glutKeyboardUpFunc(keyboardUp_func);


    //gladLoadGL();
    glutMainLoop();
}