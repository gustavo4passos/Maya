#include "../include/PlayState.h"
#include "../include/InputModule.h"
#include "../include/GameStateMachine.h"
#include "../include/Game.h"

void PlayState::Update(){
    if(InputModule::WasKeyReleased(ESC)){
        GameStateMachine::PopState();
    }

    //---------incomplete---------

}

void PlayState::Render(){
    //-----------waiting for vinicius------------
}

void PlayState::OnEnter(){
    //----------waiting for gustavo to teach us how to use LuaScript---------
}

void PlayState::OnExit(){
    //----------clear the textures (waiting for gustavinho-sama teach how to clear this fuck fuckness-----)
}