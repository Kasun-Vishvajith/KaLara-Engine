#include "kalara/Texture.hpp"
#include "kalara/Log.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

namespace kalara::runtime {

typedef void (APIENTRY *PFNGLACTIVETEXTUREPROC)(GLenum texture);
static PFNGLACTIVETEXTUREPROC pglActiveTexture = nullptr;

Texture2D::Texture2D(uint32_t width, uint32_t height, const uint8_t* pixels)
    : m_width(width), m_height(height) {
    m_metadata.id = core::UUID::generate();
    m_metadata.type = AssetType::Texture;
    m_metadata.is_valid = true;

    glGenTextures(1, &m_renderer_id);
    glBindTexture(GL_TEXTURE_2D, m_renderer_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(width), static_cast<GLsizei>(height),
                 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

Texture2D::Texture2D(const Image& image)
    : Texture2D(image.width(), image.height(), image.pixels().data()) {
    m_metadata.path = image.metadata().path;
    m_metadata.name = image.metadata().name;
}

Texture2D::~Texture2D() {
    if (m_renderer_id) {
        glDeleteTextures(1, &m_renderer_id);
    }
}

std::shared_ptr<Texture2D> Texture2D::create(const Image& image) {
    return std::make_shared<Texture2D>(image);
}

void Texture2D::bind(uint32_t slot) const noexcept {
    if (!pglActiveTexture) {
        pglActiveTexture = reinterpret_cast<PFNGLACTIVETEXTUREPROC>(SDL_GL_GetProcAddress("glActiveTexture"));
    }
    if (pglActiveTexture) {
        pglActiveTexture(0x84C0 /* GL_TEXTURE0 */ + slot);
    }
    glBindTexture(GL_TEXTURE_2D, m_renderer_id);
}

void Texture2D::unbind() const noexcept {
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace kalara::runtime
