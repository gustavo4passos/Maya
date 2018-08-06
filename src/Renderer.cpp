#include "../include/Renderer.h" 
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "../include/GLCall.h"
#include "../include/Window.h"

Renderer::Renderer() :
	_spriteVAO(NULL),
	_spriteVBO(NULL),
	_spriteShader(NULL),
	_currentlyBoundShader(0),
	_currentlyBoundTexture(0)
{ }

Renderer::~Renderer() {
	delete _spriteVAO;
	delete _primitivesVAO;

	delete _spriteVBO;
	delete _primitivesVBO;

	delete _spriteShader;
	delete _primitivesShader;

	_spriteVAO = NULL;
	_spriteVBO = NULL;

	_primitivesVAO = NULL;
	_primitivesVBO = NULL;

	_spriteShader = NULL;
	_primitivesShader = NULL; 
}

bool Renderer::Init(Camera* camera) {
	PrintInfo();

	// Enable blend
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCall(glEnable(GL_BLEND));

	// Generic vertex data on which model transformation can be made generically
	float spriteVertexData[] = {
		0.f, 0.f, 0.f, 0.f,
		1.f, 0.f, 1.f, 0.f, 
		1.f, 1.f, 1.f, 1.f,
		0.f, 1.f, 0.f, 1.f
	};

	float primitivesVertexData[] = {
		0.f, 0.f,
		1.f, 0.f,
		1.f, 1.f,
		0.f, 1.f
	};

	float offscreenVertexData[] = {
		-1.f,  1.f, 0.f, 1.f,
		 1.f,  1.f, 1.f, 1.f,
		 1.f, -1.f, 1.f, 0.f,
		-1.f, -1.f, 0.f, 0.f
	};
		 
	_spriteVBO = new VertexBuffer(spriteVertexData, sizeof(spriteVertexData));
	_primitivesVBO = new VertexBuffer(primitivesVertexData, sizeof(primitivesVertexData));
	_offscreenVBO = new VertexBuffer(offscreenVertexData, sizeof(offscreenVertexData));

	VertexBufferLayout spriteVertexDataLayout = VertexBufferLayout();
	spriteVertexDataLayout.Push<float>(2);
	spriteVertexDataLayout.Push<float>(2);

	VertexBufferLayout primitivesVertexDataLayout = VertexBufferLayout();
	primitivesVertexDataLayout.Push<float>(2);

	VertexBufferLayout offscreenVertexDataLayout = VertexBufferLayout();
	offscreenVertexDataLayout.Push<float>(2);
	offscreenVertexDataLayout.Push<float>(2);

	_spriteVAO = new VertexArray();
	_spriteVAO->AddBuffer(_spriteVBO, &spriteVertexDataLayout);

	_primitivesVAO = new VertexArray();
	_primitivesVAO->AddBuffer(_primitivesVBO, &primitivesVertexDataLayout);

	_offscreenVAO = new VertexArray();
	_offscreenVAO->AddBuffer(_offscreenVBO, &offscreenVertexDataLayout);

	_spriteShader = new Shader("../res/shaders/sprite_shader.vert", "../res/shaders/sprite_shader.frag");
	_primitivesShader = new Shader("../res/shaders/primitives_shader.vert",	"../res/shaders/primitives_shader.frag");	
	_meshShader = new Shader("../res/shaders/mesh_shader.vert", "../res/shaders/mesh_shader.frag");
	_offscreenRenderShader = new Shader("../res/shaders/offscreen_render.vert", "../res/shaders/offscreen_render.frag");

	// Set orthographic projection to the standard normalized space
	// Setting a diferent projection requires a call to SetViewportSize()
	glm::mat4 ortho = glm::ortho(-1.f, 1.f, -1.f, 1.f);

	BindShader(_spriteShader);
	_spriteShader->SetUniformMat4("ortho", glm::value_ptr(ortho));

	BindShader(_primitivesShader);
	_primitivesShader->SetUniformMat4("ortho", glm::value_ptr(ortho));

	BindShader(_meshShader);
	_meshShader->SetUniformMat4("ortho", glm::value_ptr(ortho));

	GLCall(glGenFramebuffers(1, &osframebuffer));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, osframebuffer));

	fbtexture = new Texture(NULL, INTERNAL_RESOLUTION_W, INTERNAL_RESOLUTION_H, 3);
	BindTexture(fbtexture); 

	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbtexture->textureID(), 0));
	
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		LOG_ERROR("Framebuffer is not complete");
		return false;
	}

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	_camera = camera;

	return true;
}

void Renderer::Draw(Texture* tex, Rect* srcRect, Rect* dstRect, bool flip) {
	if(!tex){
		LOG_ERROR("Texture is NULL.");
		DEBUG_BREAK();
		return;
	}

	_spriteVAO->Bind();
	BindTexture(tex);
	BindShader(_spriteShader);

	glm::mat4 model = glm::mat4(1.f);

	// Translate
	model = glm::translate(model, glm::vec3(dstRect->x(), dstRect->y(), 0.f));

	// Camera translation
	if(_camera != NULL){
		model = glm::translate(model, glm::vec3(-_camera->x(), -_camera->y(), 0.f));
	}

	// Model translation
	model = glm::scale(model, glm::vec3(dstRect->w(), dstRect->h(), 1.f));

	_spriteShader->SetUniformMat4("model", glm::value_ptr(model));

	if(flip) {
		_spriteShader->SetUniform4f("srcrct", srcRect->x() + srcRect->w(), srcRect->y(), -srcRect->w(), srcRect->h());
	}
	else {
		_spriteShader->SetUniform4f("srcrct", srcRect->x(), srcRect->y(), srcRect->w(), srcRect->h());		
	}
	GLCall(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));
}


void Renderer::DrawTexturedMesh(Mesh* mesh, Texture* texture, float parallax){
	if(mesh == NULL) {
		LOG_ERROR("Unable to draw textured mesh: mesh is null.");
		DEBUG_BREAK();
		return;
	}

	if(texture == NULL){
		LOG_ERROR("Unable to draw texture mesh: texture is null.");
		DEBUG_BREAK();
		return;
	}

	BindShader(_meshShader);
	BindTexture(texture);
	mesh->Bind();

	// Camera translation
	glm::mat4 translate = glm::mat4(1.f);
	if(_camera){
		translate = glm::translate(glm::mat4(1.f), glm::vec3(-_camera->x() * parallax, -_camera->y() * parallax, 0.f));
	}
	_meshShader->SetUniformMat4("model", glm::value_ptr(translate));

	GLCall(glDrawArrays(GL_TRIANGLES, 0, mesh->count()));
}

void Renderer::DrawRepeatingLayer(Texture* tex, int textureHeight, float y,  float parallax) {
	Rect src = Rect(0 , 0, INTERNAL_RESOLUTION_W, textureHeight);
	Rect dst = Rect(0, y, INTERNAL_RESOLUTION_W, textureHeight);
	
	if(_camera != nullptr) {
		src.setX(_camera->x() * parallax);
		src.setY(-y + _camera->y() * parallax);

		dst.setX(_camera->x());
		dst.setY(_camera->y());
	}

	Draw(tex, &src, &dst);
}

void Renderer::DrawRect(Rect* rect, Color* color) {
	if(!rect){
		LOG_ERROR("Rect is NULL.");
		DEBUG_BREAK();
		return;
	}
	if(!color){
		LOG_ERROR("Color is NULL.");
		DEBUG_BREAK();
		return;
	}

	PreparePrimitiveForDrawing(rect, color);
	GLCall(glDrawArrays(GL_LINE_LOOP, 0, 4));
}

void Renderer::DrawFillRect(Rect* rect, Color* color) {
	if(!rect){
		LOG_ERROR("Rect is NULL.");
		DEBUG_BREAK();
		return;
	}
	if(!color){
		LOG_ERROR("Color is NULL.");
		DEBUG_BREAK();
		return;
	}

	PreparePrimitiveForDrawing(rect, color);
	GLCall(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));
}

void Renderer::UseCamera(Camera* camera){
	_camera = camera;
}

void Renderer::UseOffscreenFramebuffer(){
	GLCall(glViewport(0, 0, INTERNAL_RESOLUTION_W, INTERNAL_RESOLUTION_H));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, osframebuffer));
}

void Renderer::UseDefaultFramebuffer(){
	GLCall(glViewport(0, 0, _viewportW, _viewportH));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Renderer::RenderOffscreenFramebuffer() {
	_offscreenVAO->Bind();
	BindTexture(fbtexture);
	BindShader(_offscreenRenderShader);

	GLCall(glDrawArrays(GL_TRIANGLE_FAN, 0, 4)); 
/*
	Rect src = Rect(0.f, 0.f, INTERNAL_RESOLUTION_W, INTERNAL_RESOLUTION_H);
	Rect dst = Rect(0.f, 0.f, _viewportW, _viewportH);
	Draw(fbtexture, &src, &dst); */
}

void Renderer::Clear() {
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::SetClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a) {

	GLCall(glClearColor(r, g, b, a));
}

void Renderer::SetViewportSize(int w, int h) {
	_viewportW = w;
	_viewportH = w / ASPECT_RATIO + 0.5f; // 0.5f is added to compensate for int truncation

	if(_viewportH > h){
		_viewportW = h * ASPECT_RATIO + 0.5f; // 0.5f is added to compensate for int truncation
		_viewportH = h;
	}

	// Centers the viewport on the screen
	int orthoX = (w - _viewportW) / 2;
	int orthoY = (h - _viewportH) / 2;

	_xScaleFactor = ((float)w) / INTERNAL_RESOLUTION_W;
	_yScaleFactor = ((float)h) / INTERNAL_RESOLUTION_H;

	// TODO(Gustavo): Is setting a fixed ortographic projection better than scalling every vertex
	// before rendering?
	// What about rendering the scene to an offscreen framebuffer texture, then render it to a 
	// quad filling the screen?

	glm::mat4 ortho = glm::ortho(0.f, (float)480, (float)270, 0.f);

	BindShader(_spriteShader);
	_spriteShader->SetUniformMat4("ortho", glm::value_ptr(ortho));

	BindShader(_primitivesShader);
	_primitivesShader->SetUniformMat4("ortho", glm::value_ptr(ortho));

	BindShader(_meshShader);
	_meshShader->SetUniformMat4("ortho", glm::value_ptr(ortho));

	GLCall(glViewport(orthoX, orthoY, _viewportW, _viewportH));
}

void Renderer::PreparePrimitiveForDrawing(Rect* rect, Color* color) {
	BindShader(_primitivesShader);
	_primitivesVAO->Bind();

	// Transformation matrix
	glm::mat4 model = glm::mat4(1.f);	
	// Translation
	model = glm::translate(model, glm::vec3(rect->x(), rect->y(), 0.f));
	// Camera translation
	if(_camera) {
		model = glm::translate(model, glm::vec3(-_camera->x(), -_camera->y(), 0.f));
	}
	// Model translation
	model = glm::scale(model,  glm::vec3(rect->w(), rect->h(), 1.f));

	_primitivesShader->SetUniformMat4("model", glm::value_ptr(model));
	_primitivesShader->SetUniform4f("fragcolor", color->r, color->g, color->b, color->a);
}

void Renderer::BindShader(Shader* shader) {
	if(_currentlyBoundShader != shader->programID()){
		shader->Bind();
		_currentlyBoundShader = shader->programID();
	}
}

void Renderer::BindTexture(Texture* tex){
	if(_currentlyBoundTexture != tex->textureID()){
		tex->Bind();
		_currentlyBoundTexture = tex->textureID();
	}
}

void Renderer::PrintInfo() {
	// Prints current graphics device and OpenGL version
	const unsigned char* graphicsDeviceInfo = glGetString(GL_RENDERER);
	const unsigned char* openGLVersion = glGetString(GL_VERSION);
	const unsigned char* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

	std::cout << graphicsDeviceInfo << std::endl;
	std::cout << "OpenGL Version: "<< openGLVersion << std::endl;
	std::cout << "GLSL Version: " << glslVersion << std::endl;
}
