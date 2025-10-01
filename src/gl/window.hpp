#pragma once

#include <cstdint>
#include <string>

#include <GLFW/glfw3.h>

#include "util.hpp"

namespace sakura::gl {

class Window {
    GLFWwindow* m_window = nullptr;

public:
    // TODO: maybe add full screen support for window creation?
    Window(std::u8string title = u8"untitled", util::Size2D = {1280, 720});
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) noexcept = default;
    Window& operator=(Window&&) noexcept = default;
    virtual ~Window();

    explicit Window(GLFWwindow* window) : m_window(window) {}
    operator GLFWwindow*() noexcept { return m_window; }
    operator const GLFWwindow*() const noexcept { return m_window; }

    /// get the native window handle in `int64_t` representation.
    ///
    /// mainly used to set `wid` for MPV instance.
    std::int64_t get_native_handle() const noexcept;
};

} // namespace sakura::gl
