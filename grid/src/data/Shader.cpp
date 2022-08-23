#include "Shader.h"

grid::Shader::Shader(const std::string& name): GLShader(
    ("assets/shaders/" + name + "_vertex.glsl").c_str(),
    ("assets/shaders/" + name + "_fragment.glsl").c_str()
)
{
}

// void grid::Shader::set_material(const Material& material, const int first_texture_unit) const
// {
//     // colors
//     this->set_vec3("u_albedo", material.albedo);
//     this->set_vec3("u_emissive", material.emissive);
//
//     // factors
//     this->set_1f("u_opacity", material.opacity);
//     this->set_1f("u_roughness", material.roughness);
//     this->set_1f("u_metalness", material.metalness);
//     this->set_1f("u_occlusion", material.occlusion);
//     this->set_1f("u_mapNormal", material.normal_map);
//     this->set_1f("u_albedoScale", material.albedo_scale);
//     this->set_1f("u_emissiveScale", material.emissive_scale);
//
//     // maps
//     int curr_unit = first_texture_unit;
//     this->set_sampler_2d("u_albedo_map", material.albedo_map, curr_unit++);
//     this->set_sampler_2d("u_normal_map", material.normal_map, curr_unit++);
//     this->set_sampler_2d("u_emissive_map", material.emissive_map, curr_unit++);
//     this->set_sampler_2d("u_metalness_map", material.metalness_map, curr_unit++);
//     this->set_sampler_2d("u_roughness_map", material.roughness_map, curr_unit++);
//     this->set_sampler_2d("u_occlusion_map", material.occlusion_map, curr_unit++);
// }
