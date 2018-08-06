#include "../include/Layer.h"

#include "../include/ResourceManager.h"

Layer::Layer(const std::string& name, int width, int height, const std::string& tilesetName, float zDistance, bool foreground) 
:	_name(name.c_str()),
	_width(width),
	_height(height),
	_zDistance(zDistance),
	_tilesetName(tilesetName),
	_foreground(foreground)
{  }

Layer::~Layer() {
	ResourceManager::DeleteMesh(_name);
}	

void Layer::Draw(Renderer* renderer) {
	renderer->DrawTexturedMesh(ResourceManager::GetMesh(_name), ResourceManager::GetTexture(_tilesetName), _zDistance);
}

