#pragma once
#include <map>

#include "Shader.h"

namespace grid
{
    class Asset
    {
    public:
        Asset();
        static std::map<std::string, Shader> shaders;
        static Shader load_shader(const std::string& name);
        static Shader get_shader(const std::string& name);
        static GLuint load_cubemap(std::vector<std::string> faces);
        static float get_aspect_viewport();
        static void clear();
        
        constexpr static unsigned int SCREEN_WIDTH = 1280;
        constexpr static unsigned int SCREEN_HEIGHT = 720;
    
    private:
        
    };
}
