#pragma once
#include "../config.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <zar/API/GLShader.h>
#include <zar/Core/Interfaces/IApplication.h>
#include <zar/API/GLCube.h>

// if (GLenum err = glewInit()) return 0;