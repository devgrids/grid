#include "model.h"
#include <vector>

grid::Model::Model(std::string const& path, const bool gamma): zar::GLModel(gamma)
{
    spdlog::error("model: {}", path);
    load_model(path);
}

void grid::Model::load_model(std::string const& path)
{
    Assimp::Importer importer;
    scene = importer.ReadFile(
        path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        spdlog::error("ERROR::ASSIMP:: {}", importer.GetErrorString());
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    process_node(scene->mRootNode);
}

void grid::Model::process_node(const aiNode* node)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(process_mesh(mesh));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        process_node(node->mChildren[i]);
    }
}

zar::GLMesh grid::Model::process_mesh(aiMesh* mesh)
{
    auto _mesh = zar::GLModel::process_mesh(mesh);
    _mesh.materials = process_materials(mesh);
    return _mesh;
}

std::vector<zar::Material> grid::Model::process_materials(aiMesh* mesh)
{
    zar::GLModel::process_materials(mesh);
    const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    return load_materials(material);
}

unsigned int grid::Model::texture_from_file(const char* path, const std::string& directory, bool gamma)
{
    auto filename = std::string(path);
    filename = directory + '/' + filename;
    return Texture(filename.c_str()).get_id();
}

std::vector<zar::Material> grid::Model::load_materials(const aiMaterial* mat) const
{
    std::vector<zar::Material> materials;
    get_texture_diffuse(materials, mat);
    get_texture_specular(materials, mat);
    get_texture_normal(materials, mat);
    get_texture_height(materials, mat);
    return materials;
}

void grid::Model::get_texture_diffuse(std::vector<zar::Material>& materials, const aiMaterial* mat) const
{
    // spdlog::info("get_texture_diffuse({})", mat->GetTextureCount(aiTextureType_DIFFUSE));
    for (unsigned int i = 0; i < mat->GetTextureCount(aiTextureType_DIFFUSE); i++)
    {
        if (i >= materials.size()) { materials.push_back({}); }

        aiString str;
        mat->GetTexture(aiTextureType_DIFFUSE, i, &str);
        materials[i].diffuse_map.id = texture_from_file(str.C_Str(), this->directory);
        materials[i].diffuse_map.path = str.C_Str();
    }
}

void grid::Model::get_texture_specular(std::vector<zar::Material>& materials, const aiMaterial* mat) const
{
    // spdlog::info("get_texture_specular({})", mat->GetTextureCount(aiTextureType_SPECULAR));
    for (unsigned int i = 0; i < mat->GetTextureCount(aiTextureType_SPECULAR); i++)
    {
        if (i >= materials.size()) { materials.push_back({}); }

        aiString str;
        mat->GetTexture(aiTextureType_SPECULAR, i, &str);
        materials[i].specular_map.id = texture_from_file(str.C_Str(), this->directory);
        materials[i].specular_map.path = str.C_Str();
    }
}

void grid::Model::get_texture_normal(std::vector<zar::Material>& materials, const aiMaterial* mat) const
{
    // spdlog::info("get_texture_normal({})", mat->GetTextureCount(aiTextureType_HEIGHT));
    for (unsigned int i = 0; i < mat->GetTextureCount(aiTextureType_HEIGHT); i++)
    {
        if (i >= materials.size()) { materials.push_back({}); }

        aiString str;
        mat->GetTexture(aiTextureType_HEIGHT, i, &str);
        materials[i].normal_map.id = texture_from_file(str.C_Str(), this->directory);
        materials[i].normal_map.path = str.C_Str();
    }
}

void grid::Model::get_texture_height(std::vector<zar::Material>& materials, const aiMaterial* mat) const
{
    // spdlog::info("get_texture_height({})", mat->GetTextureCount(aiTextureType_AMBIENT));
    for (unsigned int i = 0; i < mat->GetTextureCount(aiTextureType_AMBIENT); i++)
    {
        if (i >= materials.size()) { materials.push_back({}); }

        aiString str;
        mat->GetTexture(aiTextureType_AMBIENT, i, &str);
        materials[i].height_map.id = texture_from_file(str.C_Str(), this->directory);
        materials[i].height_map.path = str.C_Str();
    }
}
