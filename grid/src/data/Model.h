#pragma once
#include <map>

#include "texture.h"
#include "zar/api/opengl/gl_mesh.h"
#include "zar/api/opengl/gl_model.h"
#include "zar/data/material.h"


namespace grid
{
    class __declspec(dllexport) Model : public zar::GLModel
    {
    public:
        explicit Model(std::string const& path, bool gamma = false);

    private:
        void load_model(std::string const& path);
        void process_node(const aiNode* node);
        inline zar::GLMesh process_mesh(aiMesh* mesh) override;
        inline std::vector<zar::Material> process_materials(aiMesh* mesh) override;
        static unsigned int texture_from_file(const char* path, const std::string& directory, bool gamma = false);
        std::vector<zar::Material> load_materials(const aiMaterial* mat) const;
        void get_texture_diffuse(std::vector<zar::Material>& materials, const aiMaterial* mat) const;
        void get_texture_specular(std::vector<zar::Material>& materials, const aiMaterial* mat) const;
        void get_texture_normal(std::vector<zar::Material>& materials, const aiMaterial* mat) const;
        void get_texture_height(std::vector<zar::Material>& materials, const aiMaterial* mat) const;

        std::map<std::string, zar::Material> map_materials;
    };
}
