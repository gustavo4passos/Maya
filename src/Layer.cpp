#include "../include/Layer.h"

#include "../include/ResourceManager.h"

Layer::Layer(const std::string& name, int width, int height, Tileset* tileset) :
	_name(name.c_str()),
	_width(width),
	_height(height),
	_tileset(tileset)
{  }

Layer::~Layer() {
	ResourceManager::DeleteMesh(_name);
}	

void Layer::Draw(Renderer* renderer) {
	renderer->DrawTexturedMesh(ResourceManager::GetMesh(_name), ResourceManager::GetTexture(_tileset->name()));
}

