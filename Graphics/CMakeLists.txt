add_library(
    gfx STATIC
    src/MainGFX/Graphics.hpp
    src/MainGFX/Graphics.cpp
    src/Shaders/shader.hpp
    src/Shaders/shader.cpp
    src/Shaders/SimpleVertexShader.vertexshader
    src/Shaders/SimpleFragmentShader.fragmentshader
    src/MainGFX/controls.hpp
    src/MainGFX/controls.cpp
)

target_include_directories(gfx PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")