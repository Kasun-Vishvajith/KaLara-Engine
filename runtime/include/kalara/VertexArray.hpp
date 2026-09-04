#pragma once

#include "kalara/Buffer.hpp"
#include <memory>
#include <vector>

namespace kalara::runtime {

class VertexArray {
public:
    VertexArray();
    ~VertexArray();

    void bind() const noexcept;
    void unbind() const noexcept;

    void add_vertex_buffer(std::shared_ptr<VertexBuffer> vertex_buffer);
    void set_index_buffer(std::shared_ptr<IndexBuffer> index_buffer);

    [[nodiscard]] const std::vector<std::shared_ptr<VertexBuffer>>& vertex_buffers() const noexcept { return m_vertex_buffers; }
    [[nodiscard]] const std::shared_ptr<IndexBuffer>& index_buffer() const noexcept { return m_index_buffer; }
    [[nodiscard]] uint32_t renderer_id() const noexcept { return m_renderer_id; }

private:
    uint32_t m_renderer_id{0};
    std::vector<std::shared_ptr<VertexBuffer>> m_vertex_buffers;
    std::shared_ptr<IndexBuffer> m_index_buffer;
};

} // namespace kalara::runtime
