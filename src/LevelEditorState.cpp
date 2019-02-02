#include "../include/LevelEditorState.h"

#include <imgui.h>
#include <imgui_impl_sdl_gl3.h>

#include "../include/GameStateMachine.h"
#include "../include/Renderer.h"
#include "../include/ResourceManager.h"
#include "../include/ServiceLocator.h"
#include "../include/Window.h"
#include <windows.h>

const std::string LevelEditorState::_stateID = "EDIT";

LevelEditorState::LevelEditorState() {

}

bool LevelEditorState::OnEnter() {
    return true;
}

bool LevelEditorState::OnExit() {

    return true;
}

void LevelEditorState::Update() {

}

void LevelEditorState::HandleInput() {

}

void LevelEditorState::Render(Renderer* renderer, float deltaTime) {
    bool quit = false;
    ImGui_ImplSdlGL3_NewFrame(ServiceLocator::GetWindow()->_windowHndl);

    if(ImGui::BeginMainMenuBar()) {
        if(ImGui::Button("Open Level")) {
            char filename[255];
            OPENFILENAME op = { };    
            op.lStructSize = sizeof(OPENFILENAME);
            op.hwndOwner = NULL;
            op.hInstance = NULL;
            op.lpstrFilter = ".TMX files\0*.tmx\0All files\0*.*\0\0";
            op.lpstrCustomFilter = NULL;
            op.lpstrFile = filename;
            op.lpstrFile[0] = '\0';
            op.nMaxFile = sizeof(filename);
            op.lpstrTitle = "Select Level File";
            op.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

            // bool userSelectedFile = GetOpenFileName(&op);

        }
        else if(ImGui::Button("Return to Game")) {
            quit = true;
        }
        ImGui::EndMainMenuBar();
    }
    ImGui::Render();
    ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());

    Rect rct = Rect(0.f, 0.f, 100, 200);
    Rect dst = Rect(0.f, 0.f, 300, 300);
    renderer->Draw(ResourceManager::GetTexture("TempleEntrance1"), &rct, &dst);
    if(quit) GameStateMachine::PopState();
}