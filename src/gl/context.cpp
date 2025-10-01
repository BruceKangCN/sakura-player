#include <GLFW/glfw3.h>

#include "exception.hpp"

#include "context.hpp"

namespace sakura::gl {

GLFW::GLFW()
{
    if (!glfwInit()) {
        throw ex::GLException(
            ex::GLExceptionKind::GLFWInitFailed,
            "Failed to initialize GLFW"
        );
    }
}

GLFW::~GLFW()
{
    glfwTerminate();
}

} // namespace sakura::gl
