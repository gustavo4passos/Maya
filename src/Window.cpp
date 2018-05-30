#include "../include/Window.h"

#include <string>

#include "GL/glew.h"
#define GLEW_STATIC

#include "../include/ErrorHandler.h"

Window::Window(const char* title, int width, int height, int openGLMajorVersion,
  int openGLMinorVersion, bool vsync, bool fullscreen) :
  _title(title),
  _width(width),
  _height(height),
  _windowedWidth(width),
  _windowedHeight(height),
  _openGLMajorVersion(openGLMajorVersion),
  _openGLMinorVersion(openGLMinorVersion),
  _vsync(vsync), _fullscreen(fullscreen),
  _windowPtr(NULL)
  { }

Window::~Window() {
  Close();
}
bool Window::Init() {

  if(!InitSDLVideoSubsystem()) {
    return false;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, _openGLMajorVersion);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, _openGLMinorVersion);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  _windowPtr = SDL_CreateWindow(_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		                _windowedWidth, _windowedHeight, SDL_WINDOW_OPENGL);

  if(_windowPtr == NULL) {
    LOG_ERROR("Unable to create window. " + std::string(SDL_GetError()));
    return false;
  }
  else {
    _openGLContext = SDL_GL_CreateContext(_windowPtr);
    if(_openGLContext == NULL) {
      LOG_ERROR("Unable to create OpenGL context. " + std::string(SDL_GetError()));
      return false;
    }
    else {
      glewExperimental = true;
      GLenum glewInitStatus = glewInit();
      if(glewInitStatus != GLEW_OK) {
        LOG_ERROR("Unable to initialize Glew. " +
          std::string((char*)glewGetErrorString(glewInitStatus)));
        return false;
      }
      else {

        if(_vsync) {
          SetVsync(true);
        }
        if(_fullscreen) {
          SetFullscreen(true);
        }
      }
    }
  }

  return true;
}

void Window::Close() {
  if(_windowPtr != NULL){
    SDL_DestroyWindow(_windowPtr);
    _windowPtr = NULL;
  }

  if(SDL_WasInit(SDL_INIT_VIDEO)){
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
  }
}

void Window::Swap() {
  SDL_GL_SwapWindow(_windowPtr);
}

void Window::SetFullscreen(bool fullscreen) {
  if(fullscreen) {
    // Get current display mode
    SDL_DisplayMode current;
    
    if(SDL_GetCurrentDisplayMode(0, &current)){
      LOG_ERROR("Unable to get display mode. " + std::string(SDL_GetError()));
    }
    else{
      SDL_SetWindowSize(_windowPtr, current.w, current.h);
	
      // In case of error, restore windowed mode and resolution
      if(SDL_SetWindowFullscreen(_windowPtr, SDL_WINDOW_FULLSCREEN_DESKTOP)) {
	SDL_SetWindowFullscreen(_windowPtr, 0);
	SDL_SetWindowSize(_windowPtr, _windowedWidth, _windowedHeight);	
	_fullscreen = false;

        LOG_ERROR("Unable to set fullscreen mode. " + std::string(SDL_GetError()));
      }
      else {
	      _fullscreen = true;
	      _width = current.w;
	      _height = current.h;
      }
    }
  }
  else{
      SDL_SetWindowSize(_windowPtr, _windowedWidth, _windowedHeight);
      
      if(SDL_SetWindowFullscreen(_windowPtr, 0)) {
        LOG_ERROR("Unable to set windowed mode. " + std::string(SDL_GetError()));
      }
      else{
	      _fullscreen = false;
	      _width = _windowedWidth;
	      _height = _windowedHeight;
      }
  }
}

void Window::SetResolution(int width, int height){
  if(width <= 0 && height <= 0) {
    LOG_ERROR("Invalid resolution.");
  }

  SDL_SetWindowSize(_windowPtr, width, height);
}

void Window::SetVsync(bool vsync) {
  if(SDL_GL_SetSwapInterval((int)vsync) < 0) {
    LOG_ERROR("Unable to use Vsync. " + std::string(SDL_GetError()));
  }
}

void Window::ToggleFullscreen(){
  if(_fullscreen){
	  SetFullscreen(false);
  }
  else{
	  SetFullscreen(true);
  }
}

bool Window::InitSDLVideoSubsystem() {
  if(SDL_WasInit(SDL_INIT_VIDEO) == 0) {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
      LOG_ERROR("Unable to initialize SDL_Video. " + std::string(SDL_GetError()));
      return false;
    }
  }

  return true;
}
