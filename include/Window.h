#ifndef WINDOW_H
#define WINDOW_H

#include "SDL2/SDL.h"

class Window {
public:
  Window(const char* title, int width, int height, int openGLMajorVersion = 3, int openGLMinorVersion = 3, 
         bool vsync = false, bool fullscreen = false);
  ~Window();

  bool Init();
  void Close();
  void Swap();

  // Current window size
  inline int width() const { return _width; }
  inline int height() const { return _height; }

  void SetResolution(int width, int height);
  void SetVsync(bool vsync);
  
  // Fullscreen mode uses a borderless window using the same size as the current display resolution
  // Therefore activating fullscreen will change the size of the window
  // The current size of the window can be retrieved by width() and height()
  void SetFullscreen(bool fullscreen);
  void ToggleFullscreen();

private:
  const char* _title;
  int _width;
  int _height;
  int _windowedWidth;
  int _windowedHeight;
  int _openGLMajorVersion;
  int _openGLMinorVersion;
  bool _vsync;
  bool _fullscreen;

  SDL_Window* _windowPtr;
  SDL_GLContext _openGLContext;

  bool InitSDLVideoSubsystem();
};

#endif
