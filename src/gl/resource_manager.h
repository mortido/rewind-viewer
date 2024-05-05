//
// Created by valdemar on 22.10.17.
//

#pragma once

#include <glad/glad.h>

#include <filesystem>
#include <vector>

namespace rewind_viewer::gl {

/**
 * Class for handling any OpenGl resources - vertex arrays, buffers, textures
 */
class ResourceManager {
 public:
  explicit ResourceManager();
  ~ResourceManager();

  GLuint gen_vertex_array();
  GLuint remove_vertex_array();
  GLuint gen_buffer();
  GLuint load_texture(const std::filesystem::path &path, bool gen_mipmap = true,
                      GLint wrap_s = GL_CLAMP_TO_EDGE, GLint wrap_t = GL_CLAMP_TO_EDGE,
                      GLint flt_min = GL_LINEAR, GLint flt_mag = GL_LINEAR);
 private:
  std::vector<GLuint> vaos_;
  std::vector<GLuint> buffers_;
  std::vector<GLuint> textures_;
};

}  // namespace rewind_viewer::render
