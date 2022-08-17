#pragma once
#include <map>

#include "texture.h"
#include "zar/data/Model.h"

namespace grid
{
    class __declspec(dllexport) Model : public zar::Model
    {
    public:
        explicit Model(string const& path, bool gamma = false);

    private:
        void load_model(string const& path);
        void process_node(const aiNode* node);
        inline zar::Mesh process_mesh(aiMesh* mesh) override;
        inline vector<zar::Material> process_materials(aiMesh* mesh) override;
        static unsigned int texture_from_file(const char* path, const string& directory, bool gamma = false);
        vector<zar::Material> load_materials(const aiMaterial* mat) const;
        void get_texture_diffuse(vector<zar::Material>& materials, const aiMaterial* mat) const;
        void get_texture_specular(vector<zar::Material>& materials, const aiMaterial* mat) const;
        void get_texture_normal(vector<zar::Material>& materials, const aiMaterial* mat) const;
        void get_texture_height(vector<zar::Material>& materials, const aiMaterial* mat) const;

        map<std::string, zar::Material> map_materials;
    };
}
