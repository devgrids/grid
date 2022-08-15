#pragma once
#include "../config.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "spdlog/spdlog.h"

#include <zar/api/opengl/GLShader.h>
#include <zar/core/interfaces/IApplication.h>


// if (GLenum err = glewInit()) return 0;