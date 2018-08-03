#ifndef REGION_H
#define REGION_H

#include <map>
#include <string>
#include <vector>

#include "EventListener.h"
#include "ResourceManager.h"

class Level;
class Renderer;

class Region : public EventListener {
public:
    Region(const std::string& filename);
    ~Region();
    void HandleInput();
    void Update();
    void RenderBackground(Renderer* renderer, float  deltaTime);
    void RenderForeground(Renderer* renderer, float deltaTime);
    bool AddResource(ResourceType resourceType, const std::string& resourceID);
    
    bool AddLevel(Level* level, const std::string& levelID);
    bool ChangeCurrentLevel(const std::string& levelID);

    const std::string& currentSubRegion() const { return _currentLevel; }
    std::vector<std::string> SubRegionList();

    const std::string& filename() const { return _filename; }
    virtual bool OnNotify(Event* event);

private:    
    bool HasLevelBenSet();

    std::string _filename;

    std::string _currentLevel;
    std::map<std::string, Level*> _levels;

    std::map<ResourceType, std::vector< std::string > > _resources;
};

#endif