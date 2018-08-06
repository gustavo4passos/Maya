#ifndef LAYER_H
#define LAYER_H

#include <string>
#include "Renderer.h"

class Layer {
public:
	Layer(const std::string& name, int width, int height, const std::string& tilesetName, float zDistance = 1.0, bool foreground = false);
	~Layer();

	virtual void Draw(Renderer* renderer);

	inline const std::string& name() const { return _name; }
	inline int width() const { return _width; }
	inline int height() const { return _height; } 
	inline float zDistance() const { return _zDistance; }
	inline bool IsForeground() const { return _foreground; }
	
protected:
	std::string _name;
	int _width, _height;
	float _zDistance;
	std::string _tilesetName;
	bool _foreground;
};

#endif
