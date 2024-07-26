#pragma once

#include <glad/glad.h>

#include "gl/resource_manager.h"
#include "gl/utils.h"

namespace rewind_viewer::gl {

struct RenderContext {
  GLuint color_vertex_vao;
  GLuint color_circle_vao;
  GLuint color_vertex_vbo;
  GLuint color_circle_vbo;
  GLuint vertex_vao;
  GLuint circle_vao;
  GLuint vertex_vbo;
  GLuint circle_vbo;
  GLuint common_ebo;

  RenderContext(ResourceManager &res) {
    common_ebo = res.gen_buffer();
    {
      vertex_vao = res.gen_vertex_array();
      vertex_vbo = res.gen_buffer();
      glBindVertexArray(vertex_vao);
      glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, common_ebo);
      const size_t stride = 2 * sizeof(float);
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, nullptr);
      glEnableVertexAttribArray(0);
      glBindVertexArray(0);
    }

    {
      color_vertex_vao = res.gen_vertex_array();
      color_vertex_vbo = res.gen_buffer();
      glBindVertexArray(color_vertex_vao);
      glBindBuffer(GL_ARRAY_BUFFER, color_vertex_vbo);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, common_ebo);
      // Point layout of RenderContext: vec4 color, vec2 pos
      const size_t stride = sizeof(uint32_t) + 2 * sizeof(float);
      glVertexAttribPointer(0, 1, GL_UNSIGNED_INT, GL_FALSE, stride, nullptr);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, cg::offset<uint32_t>(1));
      glEnableVertexAttribArray(0);
      glEnableVertexAttribArray(1);
      glBindVertexArray(0);
    }

    {
      circle_vao = res.gen_vertex_array();
      circle_vbo = res.gen_buffer();
      glBindVertexArray(circle_vao);
      glBindBuffer(GL_ARRAY_BUFFER, circle_vbo);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, common_ebo);
      const size_t stride = 5 * sizeof(float);
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, nullptr);
      glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, stride, cg::offset<float>(2));
      glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, stride, cg::offset<float>(3));
      glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, stride, cg::offset<float>(4));
      glEnableVertexAttribArray(0);
      glEnableVertexAttribArray(1);
      glEnableVertexAttribArray(2);
      glEnableVertexAttribArray(3);
      glBindVertexArray(0);
    }

    {
      color_circle_vao = res.gen_vertex_array();
      color_circle_vbo = res.gen_buffer();
      glBindVertexArray(color_circle_vao);
      glBindBuffer(GL_ARRAY_BUFFER, color_circle_vbo);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, common_ebo);
      const size_t stride = 5 * sizeof(float) + sizeof(uint32_t);

      // Offset for each attribute
//      const void* color_offset = (const void*)(0 * sizeof(uint32_t));
      const void* pos_offset = (const void*)(1 * sizeof(uint32_t));
      const void* radius_offset = (const void*)((1 * sizeof(uint32_t)) + (2 * sizeof(float)));
      const void* start_angle_offset = (const void*)((1 * sizeof(uint32_t)) + (3 * sizeof(float)));
      const void* end_angle_offset = (const void*)((1 * sizeof(uint32_t)) + (4 * sizeof(float)));

      // Specify the layout of the vertex data
      glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, stride, nullptr);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, pos_offset);  // For vec2 position
      glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, stride, radius_offset);  // For float radius
      glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, stride, start_angle_offset);  // For float start angle
      glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, stride, end_angle_offset);  // For float end angle

      // Enable the vertex attributes
      glEnableVertexAttribArray(0);
      glEnableVertexAttribArray(1);
      glEnableVertexAttribArray(2);
      glEnableVertexAttribArray(3);
      glEnableVertexAttribArray(4);

      glBindVertexArray(0);
    }
  }
};

}  // namespace rewind_viewer::gl
