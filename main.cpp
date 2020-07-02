/// \file main.cpp
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_opengl.h>

#include "Chrono.hpp"
#include "Event.hpp"
#include "GLContext.hpp"

#include "SceneManager.hpp"
#include "Scenes/Loading.hpp"
#include "Scenes/MainMenu.hpp"
#include "Scenes/MainOptions.hpp"
#include "Scenes/World.hpp"
#include "Scenes/Pause.hpp"
#include "Scenes/Exchange.hpp"
#include "Scenes/Inventory.hpp"
#include "Scenes/Dungeon.hpp"

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
	glEnable(GL_CULL_FACE);
	srand(time(NULL));
    GLContext glContext("Legend Of S.I.M.");

    SceneManager::frameLabel = new Label({900.0, 0.0}, new Font("resources/Fonts/ENDORALT.ttf", 40), "FPS: ", {255,255,255});
    SceneManager::dTLabel = new Label({800.0, 40.0}, new Font("resources/Fonts/ENDORALT.ttf", 40), "DeltaTemps: ", {255,255,255});
    
    SceneManager::AddScene("Loading", Loading::LoadLoading);
    SceneManager::AddScene("MainMenu", MainMenu::LoadMainMenu);
    SceneManager::AddScene("MainOptions", MainOptions::LoadMainOptions);
    SceneManager::AddScene("World", World::LoadWorld);
    SceneManager::AddScene("Pause", Pause::LoadPause);
    SceneManager::AddScene("Exchange", Exchange::LoadExchange);
    SceneManager::AddScene("Inventory", Inventory::LoadInventory);
    SceneManager::AddScene("Dungeon", Dungeon::LoadDungeon);
    SceneManager::SetCurrentScene("Loading");

    SceneManager::Draw();

    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
    return 0;
}
