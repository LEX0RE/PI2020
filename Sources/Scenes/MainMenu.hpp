/// \file MainMenu.hpp
#ifndef MAINMENU_HPP
#define MAINMENU_HPP
#include "../Scene.hpp"
#include "../Event.hpp"
#include "../GLContext.hpp"
#include "../VisualComp.hpp"
#include "../SceneManager.hpp"
#include "../VisualComponents/Label.hpp"
#include "../VisualComponents/Image.hpp"

void OnClickNewGame(VisualComp* v);
void OnClickLoadGame(VisualComp* v);
void OnClickOptions(VisualComp* v);

/// \brief Classe pour la gestion du menu principal.
///
/// Classe contenant les éléments du menu principal.
///
class MainMenu : public Scene{
public:
    /// \brief Execute le constructeur de la scène lorsqu'elle est chargée.
    /// \param param Paramètre de transit
    /// \return Nouvelle instance de la scène MainMenu.
    static inline Scene* LoadMainMenu(void* param){
        return new MainMenu();
    }

    MainMenu() : Scene(){
		GLContext::SetOrthogonal();
        //à faire avec le ressource manager un jour ?
        Label* newgame = new Label({375, 380}, GetResource("endoralt100", Font), "- New Game -", {255, 255, 255, 255}, true);
        Label* loadgame = new Label({385, 500}, GetResource("endoralt100", Font), "- Load Save -", {255, 255, 255, 255}, true);
        Label* options = new Label({410, 620}, GetResource("endoralt100", Font), "- Options -", {255, 255, 255, 255}, true);
        //Verson a
        //AddOrtho(6, new Image("Background1.png", {0,0}, {1024,768}), new Label({310,54},"ENDORALT.ttf", 130, "Legend of S.I.M", {0,0,0,255}), new Label({300,50},"ENDORALT.ttf", 125, "Legend of S.I.M", {255,255,255,255}), newgame, loadgame, options);
        //Version b
        AddOrtho(6,
            new Image({0, 0}, {1024, 768}, GetResource("loading", Texture)->GetId()),
            new Label({250, 54}, GetResource("endoralt180", Font), "Legend of S.I.M", {0, 0, 0, 200}), 
            new Label({240,50}, GetResource("endoralt180", Font), "Legend of S.I.M", {55, 175, 212, 255}), 
            newgame, 
            loadgame, 
            options
        );
        AddSubscription(SDL_MOUSEBUTTONUP, 3, newgame, loadgame, options);
        newgame->Add(SDL_MOUSEBUTTONUP, OnClickNewGame);
        loadgame->Add(SDL_MOUSEBUTTONUP, OnClickLoadGame);
        options->Add(SDL_MOUSEBUTTONUP, OnClickOptions);
	    
	// Ajoutez ici les ItemInfos que va contenir l'ItemManager
	// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
        ItemManager::AddInfo(
		2,
        new ItemInfo(false, true, GetResource("PotionImg", Texture), "potion50", "Potion +50", "Ajoute 50 ptn de vie", Potion50::Constructor),
        new ItemInfo(true, false, GetResource("SwordImg", Texture), "sword", "Épée", "Attaque en appuyant sur ", SwordItem::Constructor)
	);
	
	// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
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

/// \brief Passe à la scène de jeu.
/// \param v Composantes visuelles.
void OnClickNewGame(VisualComp* v){
    int x,  y;
    SDL_GetMouseState(&x, &y);
    if(v->IsInside({(double)x,(double)y}))
        SceneManager::GetCurrentScene<MainMenu>()->NotifySceneManager(FREE, "World");
}

/// \brief Passe à la scène de sauvegardes.
/// \param v Composantes visuelles.
void OnClickLoadGame(VisualComp* v){
    /*
    int x,  y;
    SDL_GetMouseState(&x, &y);
    if(v->IsInside({(double)x,(double)y}))

    */
}

/// \brief Passe à la scène d'options.
void OnClickOptions(VisualComp* v){
    int x,  y;
    SDL_GetMouseState(&x, &y);
    if(v->IsInside({(double)x,(double)y}))
        SceneManager::GetCurrentScene<MainMenu>()->NotifySceneManager(PUSH, "MainOptions");
}

#endif // MAINMENU_HPP
