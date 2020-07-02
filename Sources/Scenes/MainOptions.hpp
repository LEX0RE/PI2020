/// \file MainMenu.hpp
#ifndef MAINOPTIONS_HPP
#define MAINOPTIONS_HPP

#include "../Scene.hpp"
#include "../Event.hpp"
#include "../GLContext.hpp"
#include "../SceneManager.hpp"
#include "../VisualComponents/Label.hpp"
#include "../VisualComponents/Image.hpp"

void OnClickBack(VisualComp* v);

/// \brief Classe pour la gestion des options.
///
/// Classe contenant les éléments des options.
///
class MainOptions : public Scene{
public:
    /// \brief Execute le constructeur de la scène lorsqu'elle est chargée.
    /// \param param Paramètre de transit.
    /// \return Nouvelle instance de la scène MainOptions.
    static inline Scene* LoadMainOptions(void* param){
        return new MainOptions();
    }

    MainOptions() : Scene(){
	GLContext::SetOrthogonal();
        //à faire avec le ressource manager un jour ?
        Label* back = new Label({50, 700}, GetResource("endoralt50", Font), "- Back -", {255, 255, 255, 0});
        AddOrtho(4, 
            new Image({0, 0}, {1024, 768}, GetResource("loading", Texture)->GetId()),
            new Label({200, 200}, GetResource("endoralt50", Font), "Volume", {255, 255, 255, 0}), 
            new Label({350, 50}, GetResource("endoralt50", Font), "Options", {255, 255, 255, 0}), 
            back
        );
        AddSubscription(SDL_MOUSEBUTTONUP, 1, back);
        back->Add(SDL_MOUSEBUTTONUP, OnClickBack);
    }

    /// \brief Affiche tout les VisualComp de la scène dans la fenêtre.
    void Draw(){
    	
	GLContext::ApplyOrthogonal();
        glDisable(GL_DEPTH_TEST);
        for(auto it : ortho)
            it.second->Draw();
        glEnable(GL_DEPTH_TEST);

    }
};

/// \brief Passe à la scène du menu.
/// \param v Composante visuelle.
void OnClickBack(VisualComp* v){
    int x,  y;
    SDL_GetMouseState(&x, &y);
    if(v->IsInside({(double)x,(double)y}))
        SceneManager::GetCurrentScene<MainOptions>()->NotifySceneManager(POP, "MainMenu");
}

#endif // MAINOPTIONS_HPP
