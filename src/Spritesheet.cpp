#include "../include/Spritesheet.h"

#include "../include/Rect.h"
#include "../include/Renderer.h"
#include "../include/ResourceManager.h"

Spritesheet::Spritesheet(const std::string& textureName, int nRows, int nColumns)
:	_textureName(textureName),
	_nRows(nRows),
	_nColumns(nColumns)
{
	// Note(Gustavo): It is safe to assume that the we won't be trying to 
	// access a null pointer here, because the ResourceManager will 
	// stop execution if the texture is not in memory
	_spriteWidth = ResourceManager::GetTexture(_textureName)->width();
	_spriteHeight = ResourceManager::GetTexture(_textureName)->height();

	_frameWidth = _spriteWidth / _nColumns;
	_frameHeight = _spriteHeight / _nRows;
}

Spritesheet::Spritesheet(const Spritesheet& s)
:	_textureName(s._textureName),
	_nRows(s._nRows),
	_nColumns(s._nColumns),
	_spriteWidth(s._spriteWidth),
	_spriteHeight(s._spriteHeight),
	_frameWidth(s._frameWidth),
	_frameHeight(s._frameHeight)
{ }

void Spritesheet::RenderFrame(Renderer* renderer, int frame, const Vector2D& pos, bool flip) {
	Rect src = Rect(FrameXCoord(frame), FrameYCoord(frame), _frameWidth, _frameHeight);
	Rect dst = Rect(pos.x(), pos.y(), _frameWidth, _frameHeight);

	renderer->Draw(ResourceManager::GetTexture(_textureName), &src, &dst, flip);
}

int Spritesheet::FrameXCoord(int frame) {
	return FrameColumn(frame) * _frameWidth;
}

int Spritesheet::FrameYCoord(int frame) {
	return FrameRow(frame) * _frameHeight;
}

int Spritesheet::FrameRow(int frame) {
	return frame / _nColumns;
}

int Spritesheet::FrameColumn(int frame) {
	return frame % _nColumns;
}
