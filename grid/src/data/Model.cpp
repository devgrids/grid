#include "Model.h"

#include "spdlog/spdlog.h"
#include "zar/util/assimp_helper.h"


grid::Model::Model(const char*& path)
{
    _name = path;

    Assimp::Importer loader;
    constexpr uint32_t flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace |
        aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_ValidateDataStructure |
        aiProcess_ImproveCacheLocality | aiProcess_FixInfacingNormals |
        aiProcess_GenUVCoords | aiProcess_FlipUVs;

    const aiScene* ai_scene = loader.ReadFile(path, flags);
    if (!ai_scene || ai_scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !ai_scene->mRootNode)
    {
        spdlog::error("Failed to load model -> {}", loader.GetErrorString());
        return;
    }

    // load meshes
    this->parse_node_data(ai_scene, ai_scene->mRootNode);

    // load materials
    this->parse_materials(ai_scene, path);
}

grid::Model::~Model()
= default;

void grid::Model::render(zar::GLShader* shader)
{
    if (_materials.size() > 1)
    {
        for (auto& mesh : _meshes)
        {
            // shader->setMaterial(_materials.at(mesh.material_index()), 3);
            mesh.draw_elements();
        }
        return;
    }
    if (_materials.size() == 1)
    {
        // shader->setMaterial(_materials.front(), 3);
        for (auto& mesh : _meshes) { mesh.draw_elements(); }
        return;
    }
}

bool grid::Model::has_materials() const
{
    return !_materials.empty();
}

void grid::Model::parse_node_data(const aiScene* ai_scene, aiNode* ai_node)
{
    for (uint32_t i = 0; i < ai_node->mNumMeshes; i++)
    {
        parse_mesh_data(ai_scene->mMeshes[ai_node->mMeshes[i]]);
    }

    for (uint32_t i = 0; i < ai_node->mNumChildren; i++)
    {
        parse_node_data(ai_scene, ai_node->mChildren[i]);
    }
}

void grid::Model::parse_mesh_data(aiMesh* ai_mesh)
{
    zar::Vertices vertices;
    zar::Indices indices;

    // vertices
    for (uint32_t i = 0; i < ai_mesh->mNumVertices; i++) {
        zar::Vertex vertex;
        // positions
        
        vertex.position = zar::util::assimp_helper::to_vec3(ai_mesh->mVertices[i]);

        // normals
        // vertex.normal = AssimpHelpers::toVec3(ai_mesh->mNormals[i]);

        // texcoords
        vertex.text_coords.x = ai_mesh->mTextureCoords[0][i].x;
        vertex.text_coords.y = ai_mesh->mTextureCoords[0][i].y;

        // tangent, bitangent
        // vertex.bitangent = AssimpHelpers::toVec3(ai_mesh->mBitangents[i]);
        // vertex.tangent = AssimpHelpers::toVec3(ai_mesh->mTangents[i]);

        // push to array
        vertices.push_back(vertex);
    }

    // indices	
    for (uint32_t i = 0; i < ai_mesh->mNumFaces; i++) {
        for (uint32_t k = 0; k < ai_mesh->mFaces[i].mNumIndices; k++) {
            indices.push_back(ai_mesh->mFaces[i].mIndices[k]);
        }
    }

    zar::GLMesh mesh(vertices, indices);
    mesh.set_name(ai_mesh->mName.C_Str());
    mesh.set_material_index(ai_mesh->mMaterialIndex);

    // push to list
    this->_meshes.push_back(mesh);
}

const grid::Texture& grid::Model::parse_texture(aiMaterial* ai_material, aiTextureType type, const char* directory)
{
    return {};
}

void grid::Model::parse_materials(const aiScene* ai_scene, const char* directory)
{
}
