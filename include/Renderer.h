#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GL/gl.h>

#include "Camera.h"
#include "Mesh.h"
#include "Rect.h"
#include "Shader.h"
#include "Texture.h"

struct Color {
	float r;
	float g;
	float b;
	float a;
};

class Window;

// Trying to initialize or render without a valid OpenGL context is going to 
// cause an infinite loop due to how the error log system works (GLCall()) when in
// debug mode (Activated by defining M_DEBUG as a preprocessor directive. Check GLCall.h)

class Renderer {
public:
	Renderer();
	~Renderer();
	void Clean();

	// Initializes the renderer data
	bool Init();
	void Clear();
	void SetClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a);

	void Draw(Texture* tex, Rect* srcRect, Rect* dstRect);
	void Draw(Texture tex, Rect srcRect, Rect dstRect, float scale, float angle = 0);

	void DrawTexturedMesh(Mesh* mesh, Texture* texture);

	void DrawRect(Rect* rect, Color* color);
	void DrawFillRect(Rect* rect, Color* color);

	void UseOffscreenFramebuffer();
	void UseDefaultFramebuffer();
	void RenderOffscreenFramebuffer();	

	void SetViewportSize(int w, int h);
	
private:
	Window* _windowPtr;

	VertexArray* _spriteVAO;
	VertexArray* _primitivesVAO;
	VertexArray* _offscreenVAO;
	VertexBuffer* _spriteVBO;
	VertexBuffer* _primitivesVBO;
	VertexBuffer* _offscreenVBO;

	GLuint osframebuffer;
	Texture* fbtexture;

	Shader* _spriteShader;
	Shader* _primitivesShader;
	Shader* _meshShader;
	Shader* _offscreenRenderShader;

	int _viewportW, _viewportH;  

	void PreparePrimitiveForDrawing(Rect* rect, Color* color);
	void BindShader(Shader* shader); //Only binds if not already bound
	void BindTexture(Texture* texture); //Only binds if not already bound

	//Print info about the current graphics device and glsl version
	void PrintInfo();

	// Internal resolution data
	static const int INTERNAL_RESOLUTION_W = 480;
	static const int INTERNAL_RESOLUTION_H = 270;
	static const float ASPECT_RATIO; 

	float _xScaleFactor, _yScaleFactor;
	
	//The currenly bound shader and texture id is stored to avoid binding 
	//an object that is already bound to the current state. This is done in
	//order to try to decrease the latency caused by  changing the OpenGL 
	//state unnecesserily
	unsigned int _currentlyBoundShader, _currentlyBoundTexture;
};

#endif
