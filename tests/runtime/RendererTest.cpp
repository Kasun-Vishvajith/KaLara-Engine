#include "kalara/Buffer.hpp"
#include "kalara/VertexArray.hpp"
#include "kalara/Shader.hpp"
#include "kalara/Viewport.hpp"
#include <cassert>
#include <memory>

int main() {
    // Viewport Aspect Ratio calculation test
    kalara::runtime::Viewport vp{0, 0, 1920, 1080};
    assert(vp.aspect_ratio() > 1.77f && vp.aspect_ratio() < 1.78f);

    // Vertex data setup
    kalara::core::Vertex vertices[3] = {
        { { 0.0f,  0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.5f, 1.0f} },
        { {-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },
        { { 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 0.0f} }
    };

    uint32_t indices[3] = { 0, 1, 2 };

    // Test VertexArray and Buffer Ownership abstractions
    auto vao = std::make_unique<kalara::runtime::VertexArray>();
    auto vbo = std::make_shared<kalara::runtime::VertexBuffer>(vertices, 3);
    auto ibo = std::make_shared<kalara::runtime::IndexBuffer>(indices, 3);

    vao->add_vertex_buffer(vbo);
    vao->set_index_buffer(ibo);

    assert(vao->vertex_buffers().size() == 1);
    assert(vao->index_buffer()->count() == 3);

    // Test Shader abstraction
    kalara::runtime::Shader shader("#version 110\n", "#version 110\n");
    assert(shader.renderer_id() > 0);

    return 0;
}
