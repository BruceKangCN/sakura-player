#include "exception.hpp"

#include "context.hpp"

using sakura::ex::ImGuiException;
using sakura::ex::ImGuiExceptionKind;
using sakura::util::GraphicsAPI;

namespace sakura::imgui {

Context::Context(sakura::gl::Window& window, GraphicsAPI api)
    : m_window(window)
    , m_api(api)
{
    if (!IMGUI_CHECKVERSION()) {
        throw ImGuiException(ImGuiExceptionKind::VersionMismatch, "ImGui version mismatch!");
    }

    m_context = ImGui::CreateContext();
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    switch (m_api) {
    case GraphicsAPI::OpenGL:
        if (!ImGui_ImplGlfw_InitForOpenGL(m_window, true)) {
            throw ImGuiException(ImGuiExceptionKind::GLFWInitFailed, "Failed to initialize ImGui GLFW backend");
        }

        // FIXME: Failed to initialize OpenGL loader!
        if (!ImGui_ImplOpenGL3_Init()) {
            throw ImGuiException(ImGuiExceptionKind::OpenGLInitFailed, "Failed to initialize ImGui OpenGL3 backend");
        }

        break;
    case GraphicsAPI::Vulkan:
        // TODO
        break;
    default:
        break;
    }
}

Context::~Context()
{
    auto& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    switch (m_api) {
    case GraphicsAPI::OpenGL:
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        break;
    case GraphicsAPI::Vulkan:
        // TODO
        break;
    default:
        break;
    }

    ImGui::DestroyContext(m_context);
}

sakura::EventLoop Context::createEventLoop()
{
    return sakura::EventLoop(m_window, m_api);
}

} // namespace sakura::imgui
