#pragma once

#include "Material.h"

namespace grid
{
    class Shader : public zar::GLShader
    {
    public:
        Shader(const std::string& name);

        void set_material(const Material& material, int first_texture_unit) const;
    };
}
