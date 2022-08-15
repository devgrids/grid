// #pragma once
// #include "Material.h"
//
// namespace zar
// {
//     class GLMesh;
//
//     struct Model
//     {
//         ZAR_INLINE Model(const char*& path);
//         // ZAR_INLINE virtual ~Model();
//         ZAR_INLINE Model() = default;
//
//         ZAR_INLINE std::vector<Material>& materials() { return _materials; }
//         ZAR_INLINE const std::vector<GLMesh>& meshes() { return _meshes; }
//         ZAR_INLINE const std::string& name() { return _name; }
//
//         ZAR_INLINE void render(GLShader* shader);
//         ZAR_INLINE bool has_materials() const;
//
//     private:
//         ZAR_INLINE void parse_node_data(const aiScene* ai_scene, aiNode* ai_node);
//         ZAR_INLINE void parse_mesh_data(aiMesh* ai_mesh);
//
//     protected:
//         ZAR_INLINE const Texture& parse_texture(aiMaterial* ai_material, aiTextureType type, const char* directory);
//         ZAR_INLINE void parse_materials(const aiScene* ai_scene, const char* directory);
//
//         std::vector<Material> _materials;
//         std::vector<GLMesh> _meshes;
//         std::string _name;
//     };
// }
