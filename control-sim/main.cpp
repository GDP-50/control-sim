#include <MainGFX/Graphics.hpp>
#include <stdio.h>

GLFWwindow * window;

int main() {
    bool windowOpened;
    if (!gfx::InitialiseGLFW()) return -1;
    window = gfx::OpenWindow("Control Sim", windowOpened);
    if (!windowOpened) return -1;
    gfx::Main(window);
    return 0;
}