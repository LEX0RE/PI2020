/// \file Pause.hpp
#ifndef PAUSE_HPP
#define PAUSE_HPP

#include "../Scene.hpp"
#include "../Event.hpp"
#include "../GLContext.hpp"
#include "../SceneManager.hpp"
#include "../VisualComponents/Label.hpp"
#include "../VisualComponents/Image.hpp"

/// \brief Sauvegarde l'état du jeu.
/// \param v Composante visuelle.
void OnClickSaveGame(VisualComp* v){
    int x,  y;
    SDL_GetMouseState(&x, &y);
    //if(v->IsInside({(double)x,(double)y}))
        //save
}

/// \brief Sauvegarde l'état du jeu.
/// \param v Composante visuelle.
void OnClickQuitGame(VisualComp* v){
    int x,  y;
    SDL_GetMouseState(&x, &y);
    if(v->IsInside({(double)x,(double)y}))
        SceneManager::isOpen = false;
}

/// \brief Retourne au jeu.
/// \param v Composante visuelle.
void OnClickBackGame(VisualComp* v);

/// \brief Classe pour la gestion du menu de pause.
///
/// Classe contenant les éléments du menu de pause.
///
class Pause : public Scene{
public:
    /// \brief Execute le constructeur de la scène lorsqu'elle est chargée.
    /// \param param Paramètre de transit
    /// \return Nouvelle instance de la scène.
    static inline Scene* LoadPause(void* param){
        return new Pause();
    }

    Pause(){
        SDL_SetRelativeMouseMode(SDL_FALSE);
        GLContext::SetOrthogonal();
        Label* savegame = new Label({375, 380}, GetResource("endoralt100", Font), "- Save Game -", {255, 255, 255, 255});
        Label* backgame = new Label({355, 500}, GetResource("endoralt100", Font), "- Back to Game -", {255, 255, 255, 255});
        Label* quit = new Label({440, 620}, GetResource("endoralt100", Font), "- Quit -", {255, 255, 255, 255});
        AddOrtho(6,
            new Image({0, 0}, {1024, 768}, GetResource("loading", Texture)->GetId()),
            new Label({250, 54}, GetResource("endoralt180", Font), "Legend of S.I.M", {0, 0, 0, 200}),
            new Label({240,50}, GetResource("endoralt180", Font), "Legend of S.I.M", {55, 175, 212, 255}),
            savegame,
            backgame,
            quit
        );
        AddSubscription(SDL_MOUSEBUTTONUP, 3, savegame, backgame, quit);
        savegame->Add(SDL_MOUSEBUTTONUP, OnClickSaveGame);
        backgame->Add(SDL_MOUSEBUTTONUP, OnClickBackGame);
        quit->Add(SDL_MOUSEBUTTONUP, OnClickQuitGame);
    }

    /// \brief Affiche tout les VisualComp de la scène dans la fenêtre.
    void Draw(){
        GLContext::ApplyOrthogonal();
        glDisable(GL_DEPTH_TEST);
        for(auto it : ortho){
            it.second->Draw();
        }
        glEnable(GL_DEPTH_TEST);
    }
};

/// \brief Retourne au jeu.
/// \param v Composante visuelle.
void OnClickBackGame(VisualComp* v){
    int x,  y;
    SDL_GetMouseState(&x, &y);
    if(v->IsInside({(double)x,(double)y}))
        SceneManager::GetCurrentScene<Scene>()->NotifySceneManager(POP, "World");
}

#endif // PAUSE_HPP
