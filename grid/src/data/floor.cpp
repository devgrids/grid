#include "floor.h"

#include "../utility/dev.h"
#include "Texture.h"

grid::Floor::Floor(std::string path, const glm::vec3 volumen, const float repeat)
{
    this->shader = dev::get_shader("terrain");
    this->volumen = volumen;
    this->repeat = repeat;

    path = "assets\\textures\\terrain\\" + path;
    floor_texture = Texture(path.c_str()).get_id();

    config_terrain();
    config_shader();
}

void grid::Floor::render(zar::GLCamera& camera, glm::vec3 ambient)
{
    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.get_view_matrix();
    glm::mat4 projection = camera.get_projection_matrix(dev::get_aspect_viewport());
    shader.set_mat4("view", view);
    shader.set_mat4("projection", projection);
    shader.set_vec3("ambient", ambient);

    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, floor_texture);
    shader.set_mat4("model", glm::mat4(1.0f));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void grid::Floor::config_terrain()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
		
    plane_vertices[0] = volumen.x;
    plane_vertices[1] = volumen.y;
    plane_vertices[2] = volumen.z;
    plane_vertices[3] = repeat;
    plane_vertices[4] = 0.0f;
		
    plane_vertices[5] = -volumen.x;
    plane_vertices[6] = volumen.y;
    plane_vertices[7] = volumen.z;
    plane_vertices[8] = 0.0f;
    plane_vertices[9] = 0.0f;
		
    plane_vertices[10] = -volumen.x;
    plane_vertices[11] = volumen.y;
    plane_vertices[12] = -volumen.z;
    plane_vertices[13] = 0.0f;
    plane_vertices[14] = repeat;
		
    plane_vertices[15] = volumen.x;
    plane_vertices[16] = volumen.y;
    plane_vertices[17] = volumen.z;
    plane_vertices[18] = repeat;
    plane_vertices[19] = 0.0f;
		
    plane_vertices[20] = -volumen.x;
    plane_vertices[21] = volumen.y;
    plane_vertices[22] = -volumen.z;
    plane_vertices[23] = 0.0f;
    plane_vertices[24] = repeat;
		
    plane_vertices[25] = volumen.x;
    plane_vertices[26] = volumen.y;
    plane_vertices[27] = -volumen.z;
    plane_vertices[28] = repeat;
    plane_vertices[29] = repeat;
		
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices), &plane_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
}

void grid::Floor::config_shader()
{
	shader.use();
	shader.set_int("textureT", 0);
}
