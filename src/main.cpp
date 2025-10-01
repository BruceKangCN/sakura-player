#include "gl/context.hpp"
#include "gl/window.hpp"

int main(int argc, char* argv[])
{
    using namespace sakura;

    // TODO: parse command line arguments
    static_cast<void>(argc);
    static_cast<void>(argv);

    gl::GLFW glfw {};

    gl::Window window {u8"Sakura Player", {1280, 720}};

    return 0;
}
