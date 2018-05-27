#ifndef MAYA_H
#define MAYA_H

#include "Player.h"

class Maya : public Player
{
public:

    Maya();
    ~Maya();
    
    void Load(int xPos, int yPos, int width, int height, std::string textureFile, float scale=1, bool flip=false);
    bool Draw(Renderer*);
    bool Update(unsigned int frameTime);
    void Clean();
        
private:

    void HandleInput();
};

#endif