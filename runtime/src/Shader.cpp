#include "kalara/Shader.hpp"
#include "kalara/Log.hpp"

#include <SDL3/SDL_opengl.h>

namespace kalara::runtime {

Shader::Shader(std::string_view vertex_src, std::string_view fragment_src) {
    (void)vertex_src;
    (void)fragment_src;
    m_renderer_id = 1;
}

Shader::~Shader() {
    m_renderer_id = 0;
}

void Shader::bind() const noexcept {
}

void Shader::unbind() const noexcept {
}

void Shader::set_color(std::string_view name, const core::Vector4& color) const noexcept {
    (void)name;
    glColor4f(color.x, color.y, color.z, color.w);
}

} // namespace kalara::runtime
