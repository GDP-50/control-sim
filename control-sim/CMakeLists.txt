add_executable(sim main.cpp)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

target_link_libraries(sim gfx)
target_link_libraries(sim GLEW glfw OpenGL::GL)