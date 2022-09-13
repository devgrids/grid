#include "Shader.h"
#include "zar/api/opengl/gl_camera.h"

namespace grid
{
	class Floor
	{
	public:
		Floor(std::string path, glm::vec3 volumen, float repeat);
		void render(zar::GLCamera& camera, glm::vec3 ambient);
		void config_terrain();
		void config_shader();
	private:
		Shader shader;
		unsigned int floor_texture{};
		unsigned int vao{}, vbo{};
		glm::vec3 volumen{};
		float repeat;
		float plane_vertices[30]{};
	};
}

