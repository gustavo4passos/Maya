#ifndef LAYER_H
#define LAYER_H

#include <string>

#include "Mesh.h"
#include "Tileset.h"
#include "Renderer.h"

class Layer {
public:
	Layer(const std::string& name, int width, int height, Tileset* tileset, float zDistance, bool foreground = false);
	~Layer();

	void Draw(Renderer* renderer);

	inline const std::string& name() const { return _name; }
	inline int width() const { return _width; }
	inline int height() const { return _height; } 
	inline float zDistance() const { return _zDistance; }
	inline bool IsForeground() const { return _foreground; }
	
private:
	std::string _name;
	int _width, _height;
	float _zDistance;
	Tileset* _tileset;
	bool _foreground;
};

#endif
