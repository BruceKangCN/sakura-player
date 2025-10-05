#include <string_view>

#include "exception.hpp"

#include "window.hpp"

using sakura::ex::GLException;
using sakura::ex::GLExceptionKind;

namespace sakura::gl {

Window::Window(std::u8string title, util::Size2D size)
{
    m_window = glfwCreateWindow(
        size.width,
        size.height,
        reinterpret_cast<const char*>(title.c_str()),
        nullptr,
        nullptr
    );

    if (!m_window) {
        constexpr const char* msg = "Failed to create GLFW window";
        throw GLException(GLExceptionKind::GLFWWindowCreationFailed, msg);
    }

    // TODO: should this window be made current?
    glfwMakeContextCurrent(m_window);
}

Window::~Window()
{
    // `glfwDestroyWindow` is a no-op on `nullptr`
    glfwDestroyWindow(m_window);
}

std::int64_t Window::get_native_handle() const noexcept
{
    return reinterpret_cast<std::intptr_t>(m_window);

    // TODO
}

} // namespace sakura::gl
