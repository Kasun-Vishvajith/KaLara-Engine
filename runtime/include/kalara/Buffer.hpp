#pragma once

#include <cstdint>
#include <cstddef>
#include "kalara/Math.hpp"

namespace kalara::runtime {

class VertexBuffer {
public:
    VertexBuffer(const core::Vertex* vertices, size_t count);
    ~VertexBuffer();

    void bind() const noexcept;
    void unbind() const noexcept;

    [[nodiscard]] uint32_t renderer_id() const noexcept { return m_renderer_id; }
    [[nodiscard]] size_t count() const noexcept { return m_count; }

private:
    uint32_t m_renderer_id{0};
    size_t m_count{0};
};

class IndexBuffer {
public:
    IndexBuffer(const uint32_t* indices, size_t count);
    ~IndexBuffer();

    void bind() const noexcept;
    void unbind() const noexcept;

    [[nodiscard]] uint32_t renderer_id() const noexcept { return m_renderer_id; }
    [[nodiscard]] size_t count() const noexcept { return m_count; }

private:
    uint32_t m_renderer_id{0};
    size_t m_count{0};
};

} // namespace kalara::runtime
