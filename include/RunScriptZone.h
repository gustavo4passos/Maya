#ifndef RUNSCRIPTZONE_H
#define RUNSCRIPTZONE_H

#include "Zone.h"

class RunScriptZone : public Zone {
public:
    RunScriptZone(const Rect& bounds, const std::string& scriptFilename);

    virtual void Activate() { };
    virtual void Update() { }

private:
    std::string _scriptFilename;
};

#endif