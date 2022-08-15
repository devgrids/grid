// #pragma once
// #include "Texture.h"
//
// namespace zar
// {
//     struct Material
//     {
//         ZAR_INLINE Material() = default;
//         ZAR_INLINE ~Material() = default;
//         ZAR_INLINE Material(const Material&) = default;
//
//         std::string name;
//
//         float opacity = 1.0f;
//         float occlusion = 0.5f;
//         float roughness = 0.8f;
//         float metalness = 0.1f;
//         float albedo_scale = 1.0f;
//         float emissive_scale = 1.0f;
//
//         Texture albedo_map;
//         Texture height_map;
//         Texture normal_map;
//         Texture opacity_map;
//         Texture emissive_map;
//         Texture occlusion_map;
//         Texture roughness_map;
//         Texture metalness_map;
//
//         glm::vec3 albedo = glm::vec3(0.0f);
//         glm::vec3 emissive = glm::vec3(0.0f);
//     };
//
//     using SharedMaterial = std::shared_ptr<Material>;
// }
