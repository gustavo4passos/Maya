#ifndef LEVELFILE_H
#define LEVELFILE_H

#include <string>
#include "tinyxml.h"

struct GameObjectNode {
    int id;
    float x, y;
    int w, h;
};

class LevelFile {
public:
    LevelFile(const std::string& filename);

private:
    std::string _filename;

};


#endif