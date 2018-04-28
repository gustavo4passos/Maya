#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <GL/glew.h>
#include <GL/gl.h>

struct ShaderSource {
  std::string vertexShaderSource;
  std::string fragmentShaderSource;
};

class Shader {
public:
  Shader(const std::string& vsFilepath, const std::string& fsFilepath);
  ~Shader();

  void Bind();
  void Unbind();

  // Set uniforms
  void SetUniform4f(const char* name, float v0, float v1, float v2, float v3);

private:
  GLuint _programID;
  ShaderSource _shaderSource;

  std::string _vertexShaderFilepath;
  std::string _fragmentShaderFilepath;

  ShaderSource ParseShader(const std::string& vsFilepath, const std::string& fsFilepath);
  GLuint CreateShader(const ShaderSource& shaderSource);
  GLuint CompileShader(GLenum shaderType, const std::string& source);
  GLint GetUniformLocation(const std::string& name);

};

#endif
