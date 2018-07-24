#ifndef REGION_H
#define REGION_H

#include <map>
#include <string>
#include <vector>

class Level;
class Renderer;

class Region {
public:
    Region();
    ~Region();
    void HandleInput();
    void Update();
    void Render(Renderer* renderer, float  deltaTime);

    bool AddLevel(Level* level, const std::string& levelID);
    bool ChangeCurrentLevel(const std::string& levelID);

    const std::string& currentSubRegion() const { return _currentLevel; }
    std::vector<std::string> SubRegionList();

private:
    bool HasLevelBenSet();
    std::string _currentLevel;
    std::map<std::string, Level*> _levels;
};

#endif