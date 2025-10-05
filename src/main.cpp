#include "gl/context.hpp"
#include "gl/window.hpp"
#include "imgui/context.hpp"

int main(int argc, char* argv[])
{
    using namespace sakura;

    // TODO: parse command line arguments
    static_cast<void>(argc);
    static_cast<void>(argv);

    gl::GLFW glfw {};

    gl::Window window {u8"Sakura Player", {1280, 720}};

    imgui::Context imguiCtx {window, util::GraphicsAPI::OpenGL};

    imguiCtx.createEventLoop().run([&]() {
        ImGui::Begin("Hello, world!");
        ImGui::Text("This is some useful text.");
        ImGui::End();
    });

    return 0;
}
