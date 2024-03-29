// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#pragma once

#include "ui/opengl/System.hpp"

/**
 * This class represents an OpenGL 3.0 / ES2.0 shader.
 */
class GLShader {
  const GLuint id;

public:
  explicit GLShader(GLenum type) noexcept:id(glCreateShader(type)) {}

  GLShader(const GLShader &) = delete;
  GLShader &operator=(const GLShader &) = delete;

  ~GLShader() noexcept {
    glDeleteShader(id);
  }

  GLuint GetId() const noexcept {
    return id;
  }

  void Source(const char *_source) noexcept {
    const GLchar *source = (const GLchar *)_source;
    glShaderSource(id, 1, &source, nullptr);
  }

  void Compile() noexcept {
    glCompileShader(id);
  }

  [[gnu::pure]]
  GLint GetCompileStatus() const noexcept {
    GLint status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    return status;
  }

  [[gnu::pure]]
  GLint GetInfoLogLength() const noexcept {
    GLint length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    return length;
  }

  void GetInfoLog(char *buffer, GLsizei max_size) noexcept {
    glGetShaderInfoLog(id, max_size, nullptr, (GLchar *)buffer);
  }
};

/**
 * This class represents an OpenGL 3.0 / ES2.0 program.
 */
class GLProgram {
  const GLuint id;

public:
  GLProgram() noexcept:id(glCreateProgram()) {}

  GLProgram(const GLProgram &) = delete;
  GLProgram &operator=(const GLProgram &) = delete;

  ~GLProgram() noexcept {
    glDeleteProgram(id);
  }

  GLuint GetId() const noexcept {
    return id;
  }

  void AttachShader(const GLShader &shader) noexcept {
    glAttachShader(id, shader.GetId());
  }

  void Link() noexcept {
    glLinkProgram(id);
  }

  [[gnu::pure]]
  GLint GetLinkStatus() const noexcept {
    GLint status;
    glGetProgramiv(id, GL_LINK_STATUS, &status);
    return status;
  }

  [[gnu::pure]]
  GLint GetInfoLogLength() const noexcept {
    GLint length;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
    return length;
  }

  void GetInfoLog(char *buffer, GLsizei max_size) noexcept {
    glGetProgramInfoLog(id, max_size, nullptr, (GLchar *)buffer);
  }

  void Validate() noexcept {
    glValidateProgram(id);
  }

  void Use() noexcept {
    glUseProgram(id);
  }

  [[gnu::pure]]
  GLint GetUniformLocation(const char *name) const noexcept {
    return glGetUniformLocation(id, (const GLchar *)name);
  }

  [[gnu::pure]]
  GLint GetAttribLocation(const char *name) const noexcept {
    return glGetAttribLocation(id, (const GLchar *)name);
  }

  void BindAttribLocation(GLuint index, const char *name) noexcept {
    glBindAttribLocation(id, index, (const GLchar *)name);
  }
};
