#pragma once
#include "../config.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "spdlog/spdlog.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/pbrmaterial.h>
#include <assimp/postprocess.h>

#include <zar/api/opengl/gl_shader.h>
#include <zar/core/interfaces/application.h>


// if (GLenum err = glewInit()) return 0;