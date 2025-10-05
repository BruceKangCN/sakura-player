#include <string_view>

#include <GLFW/glfw3.h>

#include "exception.hpp"

#include "context.hpp"

using sakura::ex::GLException;
using sakura::ex::GLExceptionKind;

namespace sakura::gl {

GLFW::GLFW()
{
    if (!glfwInit()) {
        constexpr const char* msg = "Failed to initialize GLFW";
        throw GLException(GLExceptionKind::GLFWInitFailed, msg);
    }
}

GLFW::~GLFW()
{
    glfwTerminate();
}

} // namespace sakura::gl
