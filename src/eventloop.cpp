#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include <cassert>
#include <cstdio>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "eventloop.hpp"

using sakura::gl::Window;
using sakura::util::GraphicsAPI;

namespace sakura {

EventLoop::EventLoop(Window& window, GraphicsAPI api)
    : m_window(window)
    , m_api(api)
{
    assert(static_cast<GLFWwindow*>(window));
}

void EventLoop::run(const std::function<void()>& fn)
{
    while (!m_window.should_close()) {
        stepPrepare();
        fn();
        stepFinalize();
    }
}

void EventLoop::stepPrepare()
{
    switch (m_api) {
    case GraphicsAPI::OpenGL:
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        // nothing to do
        break;
    case GraphicsAPI::Vulkan:
        // TODO
        break;
    default:
        break;
    }

    ImGui::NewFrame();

    glfwPollEvents();
}

void EventLoop::stepFinalize()
{
    ImGui::Render();

    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();

    switch (m_api) {
    case GraphicsAPI::OpenGL:
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        break;
    case GraphicsAPI::Vulkan:
        // TODO
        break;
    default:
        break;
    }

    glfwSwapBuffers(m_window);
}

} // namespace sakura

