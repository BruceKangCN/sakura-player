#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "eventloop.hpp"
#include "gl/window.hpp"
#include "util.hpp"

namespace sakura::imgui {

class [[maybe_unused]] Context {
    ImGuiContext* m_context = nullptr;
    sakura::gl::Window& m_window;
    sakura::util::GraphicsAPI m_api;

public:
    Context(sakura::gl::Window& window, sakura::util::GraphicsAPI api);
    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;
    Context(Context&&) noexcept = delete;
    Context& operator=(Context&&) noexcept = delete;
    ~Context();

    sakura::EventLoop createEventLoop();
};

} // namespace sakura::imgui
