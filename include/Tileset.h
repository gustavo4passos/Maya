#ifndef TILESET_H
#define TILESET_H

#include <string>

class Tileset {
public:
	Tileset(const std::string& source, const std::string& name, int width, int height, 
	        int tileWidth, int tileHeight, int ncolumns, int nrows) :
			_source(source.c_str()),
			_name(name.c_str()),
			_width(width),
			_height(height),
			_tileWidth(tileWidth),
			_tileHeight(tileHeight),
			_nColumns(ncolumns),
			_nRows(nrows) 
			{}
	
	inline int width() { return _width; }
	inline int height() { return _height; }
	inline int tileWidth() { return _tileWidth; }
	inline int tileHeight() { return _tileHeight; }
	inline const std::string& source() { return _source; }
	inline const std::string& name() { return _name; }

private:
	std::string _source;
	std::string _name;

	int _width, _height;
	int _tileWidth, _tileHeight;	
	int _nColumns, _nRows;
};

#endif
