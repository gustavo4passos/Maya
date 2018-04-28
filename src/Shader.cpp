#include "../include/Shader.h"

#include <fstream>
#include <sstream>

#include "../include/ErrorHandler.h"

Shader::Shader(const std::string& vsFilepath, const std::string& fsFilepath)
  : _vertexShaderFilepath(vsFilepath), _fragmentShaderFilepath(fsFilepath)
{
  _shaderSource = ParseShader(vsFilepath, fsFilepath);
  _programID = CreateShader(_shaderSource);
}

Shader::~Shader() {
  glDeleteProgram(_programID);
}

void Shader::Bind() {
  glUseProgram(this->_programID);
}

void Shader::Unbind() {
  glUseProgram(0);
}

ShaderSource Shader::ParseShader(const std::string& vsFilepath, const std::string& fsFilepath) {
  std::ifstream vsFile(vsFilepath.c_str());
  std::ifstream fsFile(fsFilepath.c_str());

  std::stringstream vsSource;
  std::stringstream fsSource;

  std::string line;

  // Checks if vsFile was opened successfully
  if(vsFile.fail()) {
    LOG_ERROR("Unable to open file " + vsFilepath);
    DEBUG_BREAK();
  }

  // Checks if fsFile was opened succesfully
  if(fsFile.fail()) {
    LOG_ERROR("Unable to open file " + fsFilepath);
    DEBUG_BREAK();
  }

  // Reads vertex shader file to string vsSource
  while(getline(vsFile, line)) {
    vsSource << line << '\n';
  }

  // Reads fragment shader file to string fsSource
  while(getline(fsFile, line)) {
    fsSource << line << '\n';
  }

  ShaderSource shaderSource;
  shaderSource.vertexShaderSource = vsSource.str();
  shaderSource.fragmentShaderSource = fsSource.str();

  return shaderSource;
}

GLuint Shader::CreateShader(const ShaderSource& shaderSource) {
  GLuint program = glCreateProgram();

  GLuint vs = CompileShader(GL_VERTEX_SHADER, shaderSource.vertexShaderSource);
  GLuint fs = CompileShader(GL_FRAGMENT_SHADER, shaderSource.fragmentShaderSource);

  glAttachShader(_programID, vs);
  glAttachShader(_programID, fs);
  glLinkProgram(_programID);
}

GLuint Shader::CompileShader(GLenum shaderType, const std::string& source) {
  GLuint shader = glCreateShader(shaderType);
  const char* src = source.c_str();
  glShaderSource(shader, 1, &src, NULL);
  glCompileShader(shader);

  GLint compileStatus;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
  if(compileStatus != GL_TRUE) {
    GLchar log[512];
    glGetShaderInfoLog(shader, 512, NULL, log);
    LOG_ERROR("Unable to compile shader: file: " +
      (shaderType == GL_VERTEX_SHADER ? _vertexShaderFilepath : _fragmentShaderFilepath) +
      std::string(" \n") + std::string(log));
  }

  return shader;
}

GLint Shader::GetUniformLocation(const std::string& name) {
  GLint location = glGetUniformLocation(this->_programID, name.c_str());

  // If uniform isn't found in program
  if(location == -1) LOG_ERROR("Unable to find uniform: " + name);

  return location;
}
