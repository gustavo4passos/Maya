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
			_margin(margin),
			_spacing(spacing),
			_nColumns(ncolumns),
			_nRows(nrows) 
			{}
	
	inline int width() const { return _width; }
	inline int height() const { return _height; }
	inline int tileWidth() const { return _tileWidth; }
	inline int tileHeight() const { return _tileHeight; }
	inline int nRows() const { return _nRows; }
	inline int nColumns() const { return _nColumns; } 
	inline const std::string& source() { return _source; }
	inline const std::string& name() { return _name; }

private:
	std::string _source;
	std::string _name;

	int _width, _height;
	int _tileWidth, _tileHeight;	
	int _margin, _spacing;
	int _nColumns, _nRows;
};

#endif
