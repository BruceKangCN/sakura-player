#pragma once

#include <functional>

#include "gl/window.hpp"
#include "util.hpp"

namespace sakura {

class EventLoop {
    sakura::gl::Window& m_window;
    sakura::util::GraphicsAPI m_api;

public:
    EventLoop(sakura::gl::Window& window, sakura::util::GraphicsAPI api);
    EventLoop(const EventLoop&) = delete;
    EventLoop& operator=(const EventLoop&) = delete;
    EventLoop(EventLoop&&) noexcept = delete;
    EventLoop& operator=(EventLoop&&) noexcept = delete;
    ~EventLoop() = default;

    void run(const std::function<void()>& fn);

protected:
    void stepPrepare();
    void stepFinalize();
};

} // namespace sakura
