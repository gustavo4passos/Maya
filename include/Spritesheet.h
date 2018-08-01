#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include <string>

#include "Vector2D.h"

class Renderer;

class Spritesheet {
public:
	explicit Spritesheet(const std::string& textureName, int nRows, int nColumns = 4);
	Spritesheet(const Spritesheet& s);
	~Spritesheet() { };

	void RenderFrame(Renderer* renderer, int frame, const Vector2D& pos, bool flip = false);

	inline int nRows() const { return _nRows; }
	inline int nColumns() const { return _nColumns; }

	inline std::string const& textureName() const { return _textureName; }
	
private:
	std::string _textureName;
	int _nRows, _nColumns;
	int _spriteWidth, _spriteHeight;
	int _frameWidth, _frameHeight;

	int FrameXCoord(int frame);
	int FrameYCoord(int frame);

	int FrameColumn(int frame);
	int FrameRow(int frame);
};

#endif
