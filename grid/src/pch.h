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

#include <zar/api/opengl/GLShader.h>
#include <zar/api/opengl/GLMesh.h>
#include <zar/core/interfaces/IApplication.h>


// if (GLenum err = glewInit()) return 0;