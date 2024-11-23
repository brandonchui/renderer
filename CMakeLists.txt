cmake_minimum_required(VERSION 3.16)
project(renderer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
# ----------------------------
# Fetch SDL3
# ----------------------------
FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG preview-3.1.6
)

FetchContent_MakeAvailable(SDL3)


# ----------------------------
# Fetch ImGui
# ----------------------------
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.91.5
)

FetchContent_MakeAvailable(imgui)

# ----------------------------
# Fetch GLM
# ----------------------------
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 1.0.1
)

FetchContent_MakeAvailable(glm)

# ----------------------------
# Add Executable
# ----------------------------
add_executable(renderer 
    src/main.cpp 
    thirdparty/glad/src/glad.c

)

# ----------------------------
# Add ImGui Sources
# ----------------------------
set(IMGUI_SRC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_sources(renderer PRIVATE ${IMGUI_SRC})

# ----------------------------
# Include Directories
# ----------------------------
target_include_directories(renderer PRIVATE 
    ${SDL3_SOURCE_DIR}/include 
    thirdparty/glad/include
    ${imgui_SOURCE_DIR} 
    ${imgui_SOURCE_DIR}/backends 
    ${glm_SOURCE_DIR}/
    ${CMAKE_SOURCE_DIR}/src
)

# ----------------------------
# Find and Link OpenGL
# ----------------------------
find_package(OpenGL REQUIRED)

# ----------------------------
# Link Libraries
# ----------------------------
target_link_libraries(renderer PRIVATE 
    SDL3::SDL3 
    OpenGL::GL
)

# ----------------------------
# Platform-Specific Configurations
# ----------------------------
if(WIN32)
    add_custom_command(TARGET renderer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3::SDL3>
            $<TARGET_FILE_DIR:renderer>
    )
elseif(APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    target_link_libraries(renderer PRIVATE ${COCOA_LIBRARY})
elseif(UNIX)
    target_link_libraries(renderer PRIVATE pthread)
endif()

# ----------------------------
# Copy Shaders After Build
# ----------------------------
add_custom_command(TARGET renderer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:renderer>/shaders
)

# ----------------------------
# Set Output Directory
# ----------------------------
set_target_properties(renderer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
