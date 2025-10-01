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
    GLException(GLExceptionKind kind, const std::string& msg)
        : std::runtime_error(msg)
        , m_kind(kind)
    {}

    [[nodiscard]] GLExceptionKind kind() const noexcept { return m_kind; }
};

} // namespace sakura::ex
