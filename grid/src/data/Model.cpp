#include "Model.h"

grid::Model::Model(string const& path, const bool gamma): zar::Model(gamma)
{
    load_model(path);
}

void grid::Model::load_model(string const& path)
{
    Assimp::Importer importer;
    scene = importer.ReadFile(
        path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    process_node(scene->mRootNode, scene);
}

void grid::Model::process_node(const aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(process_mesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        process_node(node->mChildren[i], scene);
    }
}

zar::Mesh grid::Model::process_mesh(aiMesh* mesh, const aiScene* scene)
{
    auto _mesh = zar::Model::process_mesh(mesh, scene);
    _mesh.textures = process_materials(mesh, scene);
    return _mesh;
}

vector<zar::Texture> grid::Model::process_materials(aiMesh* mesh, const aiScene* scene)
{
    auto textures = zar::Model::process_materials(mesh, scene);
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    vector<zar::Texture> diffuse_maps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
    vector<zar::Texture> specular_maps = load_material_textures(material, aiTextureType_SPECULAR,
                                                                "texture_specular");
    textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
    std::vector<zar::Texture> normalMaps = load_material_textures(material, aiTextureType_HEIGHT,
                                                                  "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    std::vector<zar::Texture> heightMaps = load_material_textures(material, aiTextureType_AMBIENT,
                                                                  "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    return textures;
}

unsigned int grid::Model::texture_from_file(const char* path, const string& directory, bool gamma)
{
    auto filename = string(path);
    filename = directory + '/' + filename;
    return Texture(filename.c_str()).get_id();
}

vector<zar::Texture> grid::Model::load_material_textures(aiMaterial* mat, aiTextureType type, string type_name)
{
    vector<zar::Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            zar::Texture texture;
            texture.id = texture_from_file(str.C_Str(), this->directory);
            texture.type = type_name;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}
