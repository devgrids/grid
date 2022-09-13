#pragma once

namespace grid
{
    class Shader : public zar::GLShader
    {
    public:
        Shader();
        Shader(const std::string& name);

        // void set_material(const Material& material, int first_texture_unit) const;
    };
}
