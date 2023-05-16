#include "mesh.h"
mesh::mesh(int programID, const vector<float>& vec)
{
    vertex_count = vec.size() / 8;

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
    {
        float tot = 0;
        glm::vec3 tot2 = {0,0,0};
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
        cout << mc.x << " " << mc.y << " " << mc.z << "\n";
        
        
       // cout << tot << "\n";
        
    }
}

void mesh::draw(int programID)
{

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAOID);
    //glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawArrays(GL_TRIANGLES, 0, vertex_count);
}
