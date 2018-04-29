#ifndef WINDOW_H
#define WINDOW_H

#include "SDL.h"

class Window {
public:
  Window(const char* title, int width, int height, int openGLMajorVersion, int openGLMinorVersion, bool vsync = false, bool fullscreen = false);
  ~Window();

  bool Init();
  void Close();
  void Swap();

  int width() const { return _width; }
  int height() const { return _height; }

  void SetResolution(int width, int height);
  void SetFullscreen(bool fullscreen);
  void SetVsync(bool vsync);

private:
  const char* _title;
  int _width;
  int _height;
  int _openGLMajorVersion;
  int _openGLMinorVersion;
  bool _fullscreen;
  bool _vsync;

  SDL_Window* _windowPtr;
  SDL_GLContext _openGLContext;

  bool InitSDLVideoSubsystem();
};

#endif
