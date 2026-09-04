#pragma once

#include <string>
#include <string_view>
#include <cstdint>
#include "kalara/Math.hpp"

namespace kalara::runtime {

class Shader {
public:
    Shader(std::string_view vertex_src, std::string_view fragment_src);
    ~Shader();

    void bind() const noexcept;
    void unbind() const noexcept;

    void set_color(std::string_view name, const core::Vector4& color) const noexcept;

    [[nodiscard]] uint32_t renderer_id() const noexcept { return m_renderer_id; }

private:
    uint32_t m_renderer_id{0};
};

} // namespace kalara::runtime
