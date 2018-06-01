#include "../include/PlayState.h"
#include "../include/InputModule.h"
#include "../include/GameStateMachine.h"
#include "../include/Game.h"

const std::string PlayState::_playID = "PLAY";

void PlayState::Update(){
    if(InputModule::WasKeyReleased(InputModule::ESC)){
        GameStateMachine::PopState();
    }

    //---------incomplete---------

}

void PlayState::Render(){
    //-----------waiting for vinicius------------
}

bool PlayState::OnEnter(){
    return true;
    //----------waiting for gustavo to teach us how to use LuaScript---------
}

bool PlayState::OnExit(){
    return true;
    //----------clear the textures------------
}
