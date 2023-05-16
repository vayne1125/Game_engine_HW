#include "mesh.h"
mesh::mesh(){};
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
}

void mesh::draw(int programID)
{

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAOID);
    //glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawArrays(GL_TRIANGLES, 0, vertex_count);
}
