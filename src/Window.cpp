#include "../include/Window.h"

#include <string>

#include "GL/glew.h"
#define GLEW_STATIC

#include "../include/Logger.h"

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
  _windowHndl(nullptr)
  { }

Window::~Window() {
  Close();
}
bool Window::Init() {

  if(!InitSDLVideoSubsystem()) {
    return false;
  }

  if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, _openGLMajorVersion)){
	  LOG_ERROR("Unable to set OpenGL context major version. " + std::string(SDL_GetError()));
	  return false;
  }
  if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, _openGLMinorVersion)){
	  LOG_ERROR("Unable to set OpenGL context minor version. " + std::string(SDL_GetError()));
	  return false;
  }
  if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE)){
	  LOG_ERROR("Unable to set OpenGL context profile mask. " + std::string(SDL_GetError()));
	  return false;
  }

  _windowHndl = SDL_CreateWindow(_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		                _windowedWidth, _windowedHeight, SDL_WINDOW_OPENGL);

  if(_windowHndl == nullptr) {
    LOG_ERROR("Unable to create window. " + std::string(SDL_GetError()));
    return false;
  }
  else {
    _openGLContext = SDL_GL_CreateContext(_windowHndl);
    if(_openGLContext == nullptr) {
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
  if(_windowHndl != nullptr){
    SDL_DestroyWindow(_windowHndl);
    _windowHndl = nullptr;
  }

  if(SDL_WasInit(SDL_INIT_VIDEO)){
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
  }
}

void Window::Swap() {
  SDL_GL_SwapWindow(_windowHndl);
}

void Window::SetFullscreen(bool fullscreen) {
  if(fullscreen) {

    // Get current display mode
    SDL_DisplayMode current;
    
    if(SDL_GetCurrentDisplayMode(0, &current)){
      LOG_ERROR("Unable to get display mode. " + std::string(SDL_GetError()));
    }
    else{
      SDL_SetWindowSize(_windowHndl, current.w, current.h);
	
      // In case of error, restore windowed mode and resolution
       if(SDL_SetWindowFullscreen(_windowHndl, SDL_WINDOW_FULLSCREEN_DESKTOP)) {
         SDL_SetWindowFullscreen(_windowHndl, 0);
         SDL_SetWindowSize(_windowHndl, _windowedWidth, _windowedHeight);
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
      SDL_SetWindowSize(_windowHndl, _windowedWidth, _windowedHeight);
      
      if(SDL_SetWindowFullscreen(_windowHndl, 0)) {
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

  SDL_SetWindowSize(_windowHndl, width, height);

  _windowedWidth = width;
  _windowedHeight = height;
  _width = width;
  _height = height;
}

void Window::SetVsync(bool vsync) {
  if(SDL_GL_SetSwapInterval((int)vsync) < 0) {
    LOG_ERROR("Unable to use Vsync. " + std::string(SDL_GetError()));
  }
  _vsync = vsync;
}

void Window::ToggleFullscreen(){
  if(_fullscreen){
	  SetFullscreen(false);
  }
  else{
	  SetFullscreen(true);
  }
}

bool Window::ShowQuitMessageBox() {
	SDL_MessageBoxButtonData cancelButton;
	cancelButton.flags = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;
	cancelButton.buttonid = 0;
	cancelButton.text = "Cancel";

	SDL_MessageBoxButtonData okButton;
	okButton.flags = 0;
	okButton.buttonid= 1;
	okButton.text = "Ok";

	SDL_MessageBoxButtonData buttons[2];
	buttons[0] = cancelButton;
	buttons[1] = okButton;
	
	SDL_MessageBoxData messageBox;
	messageBox.flags = SDL_MESSAGEBOX_WARNING;
	messageBox.window = _windowHndl;
	messageBox.title = "Maya";
	messageBox.message = "Do you really want to quit?";
	messageBox.numbuttons = 2;
	messageBox.buttons = buttons;
	messageBox.colorScheme = nullptr; //Uses the OS color scheme
	
	int buttonPressed;
	if(SDL_ShowMessageBox(&messageBox, &buttonPressed)){
		LOG_ERROR("Unable to create quit message box. " + std::string(SDL_GetError()));
	}
	return (bool)buttonPressed;
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

std::set<std::pair<int, int> > Window::RetrieveDisplayModes()  {
    std::set<std::pair<int, int> > displayModes;

    int nDisplayModes = SDL_GetNumDisplayModes(0);

    for(int i = 0; i < nDisplayModes; i++) {
        SDL_DisplayMode displayMode;
        SDL_GetDisplayMode(0, i, &displayMode);

        // Ignore mode if it's refresh rate is < 60
        if(displayMode.refresh_rate < 60) continue;

        displayModes.insert(std::make_pair(displayMode.w, displayMode.h));
    }

    return displayModes;
}