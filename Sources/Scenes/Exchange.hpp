/// \file Inventory.hpp
#ifndef PI2020_EXCHANGE_HPP
#define PI2020_EXCHANGE_HPP

#define BUYING true
#define SELLING false

#include "../Scene.hpp"
#include "../Event.hpp"
#include "../VisualComponents/Label.hpp"
#include "../VisualComponents/InventoryBox.hpp"
#include "../VisualComponents/Image.hpp"
#include "../GLContext.hpp"
#include "../SceneManager.hpp"

void OnClickExit(VisualComp* v);
void OnClickBuying(VisualComp* v);
void OnClickSelling(VisualComp* v);

/// \brief Classe pour la gestion de l'échange.
///
/// Classe contenant les éléments lors d'un échange.
///

class Exchange : public Scene {
    InventoryGui* inventoryBox;
    Image* backGround;
    Label* exit;
    Label* buying;
    Label* selling;

public:
    /// \brief Execute le constructeur de la scène lorsqu'elle est chargée.
    /// \param param Paramètre de transit.
    /// \return Nouvelle instance de la scène Exchange.
    static inline Scene* LoadExchange(void* param){
        return new Exchange();
    }

    Exchange() : Scene(){
        GLContext::SetOrthogonal();

        backGround = new Image({0,0}, {1024, 768}, GetResource("loading", Texture)->GetId());
        exit = new Label({560, 110}, GetResource("endoralt50", Font), "X", {255, 255, 255, 0});
        buying = new Label({160, 105}, GetResource("endoralt50", Font), "Acheter", {255, 255, 255, 0});
        selling = new Label({280, 105}, GetResource("endoralt50", Font), "Vendre", {255, 255, 255, 0});
        Image* alice = new Image(INVENTORY_CHARACTER_POS, INVENTORY_CHARACTER_SIZE, INVENTORY_CHARACTER_IMG->GetId());
        Image* statsBox = new Image(INVENTORY_STATBOX_POS, INVENTORY_STATBOX_SIZE, INVENTORY_STATBOX_BACKGROUND->GetId());
        inventoryBox = new InventoryGui((Player*)transit);


        AddOrtho(7, backGround, statsBox, inventoryBox, exit, buying, selling, alice);

        AddSubscription(SDL_MOUSEBUTTONUP, 3, exit, buying, selling);

        exit->Add(SDL_MOUSEBUTTONUP, OnClickExit);
        buying->Add(SDL_MOUSEBUTTONUP, OnClickBuying);
        selling->Add(SDL_MOUSEBUTTONUP, OnClickSelling);
    }

    ~Exchange() {
        delete inventoryBox, buying, selling;
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

    void setBuying() {
        AddOrtho(6, backGround, inventoryBox, exit, buying, selling);
    }

    void setSelling() {
        // TODO Fix sellerbox?
        AddOrtho(6, backGround, inventoryBox, exit, buying, selling);
    }
};

/// \brief Si le X est appuyé, que se passe-t-il?.
void OnClickExit(VisualComp* v){
    int x,  y;
    SDL_GetMouseState(&x, &y);
    // Ici j'assume que le menu va être ouvert durant la partie (world) et non dans le menu principal.
    if(v->IsInside({(double)x,(double)y}))
        SceneManager::GetCurrentScene<Exchange>()->NotifySceneManager(POP, "World");
}

/// \brief Si le boutton acheter appuyé, que se passe-t-il?.
void OnClickBuying(VisualComp* v){
    int x,  y;
    SDL_GetMouseState(&x, &y); // Finir gestion inventaire.
    if(v->IsInside({(double)x,(double)y}))
        SceneManager::GetCurrentScene<Exchange>()->setBuying();
}

/// \brief Si le boutton vendre est appuyé, que se passe-t-il?.
void OnClickSelling(VisualComp* v){
    int x,  y;
    SDL_GetMouseState(&x, &y);
    if(v->IsInside({(double)x,(double)y})) // Finir gestion inventaire.
        SceneManager::GetCurrentScene<Exchange>()->setSelling();
}

#endif //PI2020_EXCHANGE_HPP
