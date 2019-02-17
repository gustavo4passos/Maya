#include "GameState.h"

class LevelEditorState : public GameState {
public:
    LevelEditorState();
    virtual bool OnEnter();
    virtual bool OnExit();
    virtual void Update();
    virtual void HandleInput();
    virtual void Render(Renderer* renderer, float deltaTime);

    virtual const std::string& stateID() { return _stateID; }

private:
    static const std::string _stateID;
};