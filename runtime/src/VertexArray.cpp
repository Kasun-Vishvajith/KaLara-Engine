#include "kalara/VertexArray.hpp"
#include "kalara/Log.hpp"

#include <SDL3/SDL_opengl.h>

namespace kalara::runtime {

VertexArray::VertexArray() {
    m_renderer_id = 1;
}

VertexArray::~VertexArray() {
    m_vertex_buffers.clear();
    m_index_buffer.reset();
}

void VertexArray::bind() const noexcept {
    if (m_index_buffer) {
        m_index_buffer->bind();
    }
}

void VertexArray::unbind() const noexcept {
    if (m_index_buffer) {
        m_index_buffer->unbind();
    }
}

void VertexArray::add_vertex_buffer(std::shared_ptr<VertexBuffer> vertex_buffer) {
    if (vertex_buffer) {
        m_vertex_buffers.push_back(vertex_buffer);
    }
}

void VertexArray::set_index_buffer(std::shared_ptr<IndexBuffer> index_buffer) {
    m_index_buffer = index_buffer;
}

} // namespace kalara::runtime
