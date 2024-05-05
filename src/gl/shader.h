//
// Created by valdemar on 14.10.17.
//

#pragma once

#include <glad/glad.h>

#include <filesystem>
#include <glm/glm.hpp>
#include <string>

namespace rewind_viewer::gl {

/**
 * Class representing ShaderProgram
 */
class Shader {
 public:
  Shader(const std::filesystem::path &vertex, const std::filesystem::path &fragment,
         const std::filesystem::path &geom = std::filesystem::path());
  ~Shader();

  void use() const;
  [[nodiscard]] GLuint id() const;
  [[nodiscard]] GLint uniform(const std::string &name) const;

  void set_mat4(const std::string &name, const glm::mat4 &v) const;
  void set_mat4(const std::string &name, float *pv) const;
  void set_vec2(const std::string &name, const glm::vec2 &v) const;
  void set_vec3(const std::string &name, const glm::vec3 &v) const;
  void set_vec4(const std::string &name, const glm::vec4 &v) const;
  void set_float(const std::string &name, float val) const;
  void set_int(const std::string &name, GLint val) const;
  void set_uint(const std::string &name, GLuint val) const;

  void bind_uniform_block(const std::string &name, GLuint binding_point) const;

 private:
  GLuint program_ = 0;
};

}  // namespace rewind_viewer::render
