#pragma once

namespace sakura::util {

struct Size2D {
    int width = 0;
    int height = 0;

    constexpr Size2D() = default;
    constexpr Size2D(const Size2D&) = default;
    constexpr Size2D& operator=(const Size2D&) = default;
    ~Size2D() = default;

    constexpr Size2D(int width = 0, int height = 0) : width(width), height(height) {}
};

constexpr Size2D operator+(Size2D lhs, Size2D rhs)
{
    return {lhs.width + rhs.width, lhs.height + rhs.height};
}

constexpr Size2D operator-(Size2D lhs, Size2D rhs)
{
    return {lhs.width - rhs.width, lhs.height - rhs.height};
}

enum class GraphicsAPI {
    OpenGL,
    Vulkan,
};

} // namespace sakura::util
