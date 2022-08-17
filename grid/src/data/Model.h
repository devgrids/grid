#pragma once
#include "texture.h"
#include "zar/data/Model.h"

namespace grid
{
    class __declspec(dllexport) Model final : public zar::Model
    {
    public:
        Model(string const& path, bool gamma = false);

    private:
        void load_model(string const& path);
        void process_node(const aiNode* node, const aiScene* scene);
        inline zar::Mesh process_mesh(aiMesh* mesh, const aiScene* scene) override;
        inline vector<zar::Texture> process_materials(aiMesh* mesh, const aiScene* scene) override;
        unsigned int texture_from_file(const char* path, const string& directory, bool gamma = false);
        vector<zar::Texture> load_material_textures(aiMaterial* mat, aiTextureType type, string type_name);
    };
}
