#pragma once
#include "Material.h"

namespace grid
{
    struct Model
    {
        Model(const char*& path);
        ~Model();
        Model() = default;

        std::vector<Material>& materials() { return _materials; }
        const std::vector<zar::GLMesh>& meshes() { return _meshes; }
        const std::string& name() { return _name; }

        void render(zar::GLShader* shader);
        bool has_materials() const;

    private:
        void parse_node_data(const aiScene* ai_scene, aiNode* ai_node);
        void parse_mesh_data(aiMesh* ai_mesh);

    protected:
        const Texture& parse_texture(aiMaterial* ai_material, aiTextureType type, const char* directory);
        void parse_materials(const aiScene* ai_scene, const char* directory);

        std::vector<Material> _materials;
        std::vector<zar::GLMesh> _meshes;
        std::string _name;
    };
}
