#External deppendencies
ifeq ($(OS), Windows_NT)
SDL_INCLUDE_PATHS = -Iextern\windows\SDL2\include
GLEW_INCLUDE_PATH = -Iextern\windows\glew-2.0.0\include
TINYXML2_INCLUDE_PATH = -Iextern\windows\tinyxml2-6.2.0\include
LUA_INCLUDE_PATH = -Iextern\windows\lua-5.3.4\include
endif
ifeq ($(OS), Linux)
endif

#Additional libraries
ifeq ($(OS), Windows_NT)
SDL_LIB_PATH = -Iextern\windows\lib
GLEW_LIB_PATH = -Lextern\windows\glew-2.0.0\lib
LUA_LIB_PATH = -Lextern\windows\lua-5.3.4\lib
TINYXML2_LIB_PATH = -Lextern\windows\tinyxml2-6.2.0\lib
endif
ifeq ($(OS), LINUX)
endif

LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lOpenGL32 -lglew32 -lglu32 -llua53 -ltinyxml2
ifeq ($(OS), Windows_NT)
COMPILER_FLAGS = -Wall
endif
ifeq ($(OS), Linux)
endif

bin/Application.exe: build/Application.o build/Shader.o build/Window.o \
	build/Vector2D.o build/LuaScript.o build/TestInput.o build/Renderer.o
	g++ build/Window.o build/Vector2D.o build/Shader.o build/Application.o build/LuaScript.o build/Renderer.o \
	$(LUA_LIB_PATH) $(SDL_LIB_PATH) $(GLEW_LIB_PATH) $(TINYXML2_LIB_PATH) $(LINKER_FLAGS) $(COMPILER_FLAGS) \
	-o bin/Application

build/Application.o: src/Application.cpp
	g++ -c src/Application.cpp $(SDL_INCLUDE_PATHS) $(TINYXML2_INCLUDE_PATH) $(GLEW_INCLUDE_PATH) $(LUA_INCLUDE_PATH) $(COMPILER_FLAGS) \
	-o build/Application.o

build/Shader.o: src/Shader.cpp
	g++ -c src/Shader.cpp $(GLEW_INCLUDE_PATH) $(COMPILER_FLAGS) -o build/Shader.o

build/Window.o: src/Window.cpp
	g++ -c src/Window.cpp $(SDL_INCLUDE_PATHS) $(GLEW_INCLUDE_PATH) $(SDL_LIB_PATH) $(LINKER_FLAGS) $(COMPILER_FLAGS) -o build/Window.o

build/Vector2D.o: src/Vector2D.cpp
	g++ -c src/Vector2D.cpp $(COMPILER_FLAGS) -o build/Vector2D.o

build/LuaScript.o: src/LuaScript.cpp include/LuaScript.h
	g++ -c src/LuaScript.cpp $(LUA_INCLUDE_PATH) $(COMPILER_FLAGS) -o build/LuaScript.o

build/Renderer.o: include/Renderer.h src/Renderer.cpp
	g++ -c src/Renderer.cpp $(GLEW_INCLUDE_PATH) -o build/Renderer.o

clean:
ifeq ($(OS), Windows_NT)
	del build\*.o
endif
ifeq ($(OS), Linux)
	rm build\*.o
endif
