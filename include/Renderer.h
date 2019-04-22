#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GL/gl.h>

#include "Camera.h"
#include "Mesh.h"
#include "Rect.h"
#include "Shader.h"
#include "Texture.h"

// Represents a RGBA color. Attributes should range from 0.0 ~ 1.0
struct Color {
	float r;
	float g;
	float b;
	float a;
};

class Window;
class InfoMenuGL3;

// Trying to initialize or render without a valid OpenGL context is going to 
// cause an infinite loop due to how the error log system works (GLCall()) when in
// debug mode (Activated by defining M_DEBUG as a preprocessor directive. Check GLCall.h)

class Renderer {
public:
	Renderer();
	~Renderer();
	void Clean();

	// Initializes the renderer data
	bool Init(Camera* camera = nullptr);
	void Clear();
	void SetClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a);

	void Draw(Texture* tex, Rect* srcRect, Rect* dstRect, bool flip = false);
	void Draw(Texture tex, Rect srcRect, Rect dstRect, float scale, float angle = 0);

	// TODO(Gustavo): The mesh rendering should not implement parallax
	void DrawTexturedMesh(Mesh* mesh, Texture* texture, float parallax = 1.f);

	// TODO(Gustavo): Poor generalization, not robust. Need a more generic way of drawing repeating textures.
	void DrawRepeatingLayer(Texture* tex, int textureHeight, float y, float parallax = 1.f);

	void DrawRect(Rect* rect, Color* color);
	void DrawFillRect(Rect* rect, Color* color);

	// Set up a camera for use. If nullptr, render using the default view space
	void UseCamera(Camera* camera = nullptr);

	void UseOffscreenFramebuffer();
	void UseDefaultFramebuffer();
	void RenderOffscreenFramebuffer();	

	void SetViewportSize(int w, int h);

	// Convert the screen coordinates to the internal coordinates
	Vector2D ConvertViewportCoordsToInternalResCoords(int x, int y);

	friend InfoMenuGL3;
	
private:
	Camera* _camera;

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
	void BindShader(Shader* shader); //Only binds shader if not already bound
	void BindTexture(Texture* texture); //Only binds texture if not already bound

	//Prints info about the current graphics device and glsl version
	void PrintInfo();

	// Internal resolution data
	static const int INTERNAL_RESOLUTION_W = 480;
	static const int INTERNAL_RESOLUTION_H = 270;
	static constexpr float ASPECT_RATIO = (1.f * INTERNAL_RESOLUTION_W) / INTERNAL_RESOLUTION_H; 

	//The currenly bound shader and texture id is stored in order to avoid  
	//unnecessary binding. 
	unsigned int _currentlyBoundShader, _currentlyBoundTexture;
};

#endif
