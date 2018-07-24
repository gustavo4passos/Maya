#ifndef TILESET_H
#define TILESET_H

#include <string>

class Tileset {
public:
	Tileset(const std::string& source, const std::string& name, int width, int height, 
	        int tileWidth, int tileHeight, int margin, int spacing, int ncolumns, int nrows) :
			_source(source.c_str()),
			_name(name.c_str()),
			_width(width),
			_height(height),
			_tileWidth(tileWidth),
			_tileHeight(tileHeight),
			_nColumns(ncolumns),
			_nRows(nrows), 
			_margin(margin),
			_spacing(spacing)
			{}

	// Tileset data
	inline const std::string& source() { return _source; }
	inline const std::string& name() { return _name; }
	inline int width() const { return _width; }
	inline int height() const { return _height; }
	inline int tileWidth() const { return _tileWidth; }
	inline int tileHeight() const { return _tileHeight; }
	inline int nRows() const { return _nRows; }
	inline int nColumns() const { return _nColumns; } 
	inline int margin() const { return _margin; }
	inline int spacing() const { return _spacing; }

private:
	std::string _source;
	std::string _name;

	int _width, _height;
	int _tileWidth, _tileHeight;	
	int _nColumns, _nRows;
	int _margin, _spacing;
};

#endif
