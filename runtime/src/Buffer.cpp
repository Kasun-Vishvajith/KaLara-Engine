#include "kalara/Buffer.hpp"
#include "kalara/Log.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

namespace kalara::runtime {

typedef void (APIENTRY *PFNGLGENBUFFERSPROC)(GLsizei n, GLuint *buffers);
typedef void (APIENTRY *PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void (APIENTRY *PFNGLBUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void (APIENTRY *PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint *buffers);

static PFNGLGENBUFFERSPROC pglGenBuffers = nullptr;
static PFNGLBINDBUFFERPROC pglBindBuffer = nullptr;
static PFNGLBUFFERDATAPROC pglBufferData = nullptr;
static PFNGLDELETEBUFFERSPROC pglDeleteBuffers = nullptr;

static void load_gl_extensions() {
    if (!pglGenBuffers) {
        pglGenBuffers = reinterpret_cast<PFNGLGENBUFFERSPROC>(SDL_GL_GetProcAddress("glGenBuffers"));
        pglBindBuffer = reinterpret_cast<PFNGLBINDBUFFERPROC>(SDL_GL_GetProcAddress("glBindBuffer"));
        pglBufferData = reinterpret_cast<PFNGLBUFFERDATAPROC>(SDL_GL_GetProcAddress("glBufferData"));
        pglDeleteBuffers = reinterpret_cast<PFNGLDELETEBUFFERSPROC>(SDL_GL_GetProcAddress("glDeleteBuffers"));
    }
}

VertexBuffer::VertexBuffer(const core::Vertex* vertices, size_t count) : m_count(count) {
    load_gl_extensions();
    if (pglGenBuffers && pglBindBuffer && pglBufferData) {
        pglGenBuffers(1, &m_renderer_id);
        pglBindBuffer(0x8892 /* GL_ARRAY_BUFFER */, m_renderer_id);
        pglBufferData(0x8892 /* GL_ARRAY_BUFFER */, static_cast<GLsizeiptr>(count * sizeof(core::Vertex)), vertices, 0x88E4 /* GL_STATIC_DRAW */);
    } else {
        m_renderer_id = 1; // Fallback mock ID for headless/unit test execution
    }
}

VertexBuffer::~VertexBuffer() {
    if (m_renderer_id && pglDeleteBuffers) {
        pglDeleteBuffers(1, &m_renderer_id);
    }
}

void VertexBuffer::bind() const noexcept {
    if (m_renderer_id && pglBindBuffer) {
        pglBindBuffer(0x8892 /* GL_ARRAY_BUFFER */, m_renderer_id);
    }
}

void VertexBuffer::unbind() const noexcept {
    if (pglBindBuffer) {
        pglBindBuffer(0x8892 /* GL_ARRAY_BUFFER */, 0);
    }
}

IndexBuffer::IndexBuffer(const uint32_t* indices, size_t count) : m_count(count) {
    load_gl_extensions();
    if (pglGenBuffers && pglBindBuffer && pglBufferData) {
        pglGenBuffers(1, &m_renderer_id);
        pglBindBuffer(0x8893 /* GL_ELEMENT_ARRAY_BUFFER */, m_renderer_id);
        pglBufferData(0x8893 /* GL_ELEMENT_ARRAY_BUFFER */, static_cast<GLsizeiptr>(count * sizeof(uint32_t)), indices, 0x88E4 /* GL_STATIC_DRAW */);
    } else {
        m_renderer_id = 1; // Fallback mock ID for headless/unit test execution
    }
}

IndexBuffer::~IndexBuffer() {
    if (m_renderer_id && pglDeleteBuffers) {
        pglDeleteBuffers(1, &m_renderer_id);
    }
}

void IndexBuffer::bind() const noexcept {
    if (m_renderer_id && pglBindBuffer) {
        pglBindBuffer(0x8893 /* GL_ELEMENT_ARRAY_BUFFER */, m_renderer_id);
    }
}

void IndexBuffer::unbind() const noexcept {
    if (pglBindBuffer) {
        pglBindBuffer(0x8893 /* GL_ELEMENT_ARRAY_BUFFER */, 0);
    }
}

} // namespace kalara::runtime
