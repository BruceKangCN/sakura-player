#pragma once

#include <stdexcept>

namespace sakura::ex {

enum class GLExceptionKind {
    Unknown,
    GLFWInitFailed,
    GLFWWindowCreationFailed,
    GLADLoadFailed,
};

class GLException : public std::runtime_error {
    GLExceptionKind m_kind = GLExceptionKind::Unknown;

public:
    GLException(GLExceptionKind kind, const char* msg)
        : std::runtime_error(msg)
        , m_kind(kind)
    {}

    [[nodiscard]] GLExceptionKind kind() const noexcept { return m_kind; }
};

enum class ImGuiExceptionKind {
    Unknown,
    VersionMismatch,
    GLFWInitFailed,
    OpenGLInitFailed,
    VulkanInitFailed,
};

class ImGuiException : public std::runtime_error {
    ImGuiExceptionKind m_kind = ImGuiExceptionKind::Unknown;

public:
    ImGuiException(ImGuiExceptionKind kind, const char* msg)
        : std::runtime_error(msg)
        , m_kind(kind)
    {}

    [[nodiscard]] ImGuiExceptionKind kind() const noexcept { return m_kind; }
};

} // namespace sakura::ex
