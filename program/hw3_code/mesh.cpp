#include "mesh.h"
mesh::mesh(){};
mesh::mesh(unsigned int programID,vector<float>& vec,int type){
    vertex_count = vec.size() / 8;
    if(type == 1)  //phy
    {
        float tot = 0;                  //體積
        glm::vec3 tot2 = {0,0,0};       //類質心
        for(int i=0;i<vertex_count/3;i++){
            glm::vec3  a = {vec[i*24+0],vec[i*24+1],vec[i*24+2]};
            glm::vec3  b = {vec[i*24+8],vec[i*24+9],vec[i*24+10]};
            glm::vec3  c = {vec[i*24+16],vec[i*24+17],vec[i*24+18]};
            glm::vec3  A = b-a;
            glm::vec3  B = c-a;
            tot += glm::cross(A,B).x * (1/6.0*B.x + 1/6.0*A.x + a.x/2);
            tot2 += glm::cross(A,B)*(A*A/12.0f + A*B/12.0f + A*a/3.0f + B*B/12.0f + B*a/3.0f + a*a/2.0f);
        }
        mc = tot2 / tot;
        glm::vec3 xyz = {0,0,0};
        float xy = 0,zy = 0,xz = 0;
        //高斯散度定理
        for(int i=0;i<vertex_count/3;i++){
            vec[i*24+0] -= mc[0];
            vec[i*24+1] -= mc[1];
            vec[i*24+2] -= mc[2];

            vec[i*24+8] -= mc[0];
            vec[i*24+9] -= mc[1];
            vec[i*24+10] -= mc[2];

            vec[i*24+16] -= mc[0];
            vec[i*24+17] -= mc[1];
            vec[i*24+18] -= mc[2];

            vertices.push_back({vec[i*24+0],vec[i*24+1],vec[i*24+2]});
            vertices.push_back({vec[i*24+8],vec[i*24+9],vec[i*24+10]});
            vertices.push_back({vec[i*24+16],vec[i*24+17],vec[i*24+18]});

            glm::vec3  a = {vec[i*24+0],vec[i*24+1],vec[i*24+2]};
            glm::vec3  b = {vec[i*24+8],vec[i*24+9],vec[i*24+10]};
            glm::vec3  c = {vec[i*24+16],vec[i*24+17],vec[i*24+18]};
            glm::vec3  A = b-a;
            glm::vec3  B = c-a;

  
            glm::vec3 cross_AB = glm::cross(A,B);
            xyz += 1/3.0f*cross_AB*((A*A*A)/20.0f + (A*A*B)/20.0f + (A*A*a)/4.0f + A*B*B/20.0F + A*B*a/4.0f + (A*a*a)/2.0f + B*B*B/20.0f + B*B*a/4.0f +  B*a*a/2.0f + a*a*a/2.0f);
            
            xy += 1/2.0f*cross_AB.x*((A.x*A.x*A.y)/20.0f    + (A.x*A.x*B.y)/60.0f + (A.x*A.x*a.y)/12.0f + (A.x*A.y*B.x)/30.0f + (A.x*A.y*a.x)/6.0f + (A.x*B.x*B.y)/30.0f + (A.x*B.x*a.y)/12.0f + 
                  (A.x*B.y*a.x)/12.0f + (A.x*a.x*a.y)/3.0f  + (A.y*B.x*B.x)/60.0f + (A.y*B.x*a.x)/12.0f + (A.y*a.x*a.x)/6.0f + (B.x*B.x*B.y)/20.0f + (B.x*B.x*a.y)/12.0f + 
                  (B.x*B.y*a.x)/6.0f  + (B.x*a.x*a.y)/3.0f  + (B.y*a.x*a.x)/6.0f  + (a.x*a.x*a.y)/2.0f);
            
            zy += 1/2.0f*cross_AB.z*((A.z*A.z*A.y)/20.0f    + (A.z*A.z*B.y)/60.0f + (A.z*A.z*a.y)/12.0f + (A.z*A.y*B.z)/30.0f + (A.z*A.y*a.z)/6.0f + (A.z*B.z*B.y)/30.0f + (A.z*B.z*a.y)/12.0f + 
                  (A.z*B.y*a.z)/12.0f + (A.z*a.z*a.y)/3.0f  + (A.y*B.z*B.z)/60.0f + (A.y*B.z*a.z)/12.0f + (A.y*a.z*a.z)/6.0f + (B.z*B.z*B.y)/20.0f + (B.z*B.z*a.y)/12.0f + 
                  (B.z*B.y*a.z)/6.0f  + (B.z*a.z*a.y)/3.0f  + (B.y*a.z*a.z)/6.0f  + (a.z*a.z*a.y)/2.0f);            
                  
            xz += 1/2.0f*cross_AB.x*((A.x*A.x*A.z)/20.0f    + (A.x*A.x*B.z)/60.0f + (A.x*A.x*a.z)/12.0f + (A.x*A.z*B.x)/30.0f + (A.x*A.z*a.x)/6.0f + (A.x*B.x*B.z)/30.0f + (A.x*B.x*a.z)/12.0f + 
                  (A.x*B.z*a.x)/12.0f + (A.x*a.x*a.z)/3.0f  + (A.z*B.x*B.x)/60.0f + (A.z*B.x*a.x)/12.0f + (A.z*a.x*a.x)/6.0f + (B.x*B.x*B.z)/20.0f + (B.x*B.x*a.z)/12.0f + 
                  (B.x*B.z*a.x)/6.0f  + (B.x*a.x*a.z)/3.0f  + (B.z*a.x*a.x)/6.0f  + (a.x*a.x*a.z)/2.0f); 
        }
        I[0][0] = xyz.y + xyz.z;
        I[1][1] = xyz.x + xyz.z;
        I[2][2] = xyz.x + xyz.z;
        
        I[0][1] = I[1][0] = -xy;
        I[0][2] = I[2][0] = -xz;
        I[1][2] = I[2][1] = -zy;

        //cout << I[0][0] << "\n";
    }else if(type == 2){   //for choose obj but not phy
        for(int i=0;i<vertex_count/3;i++){
            vertices.push_back({vec[i*24+0],vec[i*24+1],vec[i*24+2]});
            vertices.push_back({vec[i*24+8],vec[i*24+9],vec[i*24+10]});
            vertices.push_back({vec[i*24+16],vec[i*24+17],vec[i*24+18]});
        }
        cout << "slime!!\n";
    }

    { //VAO VBO
        unsigned int VBOID;
        glGenVertexArrays(1, &VAOID); // VAO
        glBindVertexArray(VAOID);     // 現在使用的VAO是誰

        glGenBuffers(1, &VBOID);      // VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBOID);     // 現在使用的VBO是誰

        glBufferData(GL_ARRAY_BUFFER, 8 * vertex_count * sizeof(float), vec.data(), GL_STATIC_DRAW);

        glBindAttribLocation(programID, 0, "vertexCor");
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
        glEnableVertexAttribArray(0);

        glBindAttribLocation(programID, 0, "textureCor");
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindAttribLocation(programID, 0, "normalCor");
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
}

const vector<glm::vec3> &mesh::getVertices()
{
    return vertices;
}

void mesh::draw(int programID)
{

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAOID);
    //glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawArrays(GL_TRIANGLES, 0, vertex_count);
}
