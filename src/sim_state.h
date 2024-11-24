
#pragma once
#include "base.h"
#include <glm/vec3.hpp>

struct SimState 
{
    bool quit = false;
    bool showImGuiWindow = true;
    
    glm::vec3 position = glm::vec3{0.0f, 0.0f, 0.0f};
    f32 color[4] = {1.0f, 0.5f, 0.0f, 1.0f};
};
