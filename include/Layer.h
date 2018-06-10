#ifndef LAYER_H
#define LAYER_H

#include <string>

#include "Mesh.h"
#include "Tileset.h"
#include "Renderer.h"

class Layer {
public:
	Layer(const std::string& name, int width, int height, Tileset* tileset);
	~Layer();

	void Draw(Renderer* renderer);

	int width() const { return _width; }
	int height() const { return _height; } 
	const std::string& name() const { return _name; }
	
private:
	std::string _name;
	int _width, _height;
	Tileset* _tileset;
};

#endif
