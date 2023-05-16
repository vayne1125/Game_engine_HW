#include "Billboard.h"
extern mytex* myTex;
void Billboard::draw(float x, float z, float w, float h, texture *tex, float* eyeMtx,unsigned int programID)
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
    
    mesh* meshForBillboard = new mesh(programID, { v0[0],v0[1],v0[2],0,1,0,0,1,v1[0],v1[1],v1[2],1,1,0,0,1,v2[0],v2[1],v2[2],1,0,0,0,1,
        v2[0],v2[1],v2[2],1,0,0,0,1,v3[0],v3[1],v3[2],0,0,0,0,1,v0[0],v0[1],v0[2],0,1,0,0,1});

    glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
    glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);

    
    tex->use(programID);
    meshForBillboard->draw(programID);
    
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND); 

    delete meshForBillboard;
    meshForBillboard = NULL;
}