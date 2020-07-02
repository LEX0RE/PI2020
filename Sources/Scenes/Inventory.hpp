/// \file Inventory.hpp
#ifndef PI2020_INVENTORY_HPP
#define PI2020_INVENTORY_HPP

#include "../Event.hpp"
#include "../GLContext.hpp"
#include "../SceneManager.hpp"
#include "../VisualComponents/InventoryBox.hpp"
#include "../VisualComponents/HintBox.hpp"
#include "../Entities/Player.hpp"

/// \brief Classe pour la gestion de l'inventaire.
///
/// Classe contenant l'inventaire.
///
void InventoryControl (VisualComp* v){
    if (SceneManager::GetCurrentSceneName() == "World") {
        if (Event::GetKey() == SDLK_TAB) {
            glDisable(GL_LIGHTING);
            glDisable(GL_LIGHT0);
            
            SceneManager::GetCurrentScene<Scene>()->NotifySceneManager(PUSH, "Inventory");
        }
    }

    if (SceneManager::GetCurrentSceneName() == "Inventory") {
        if (Event::GetKey() == SDLK_TAB) {
            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);

            SceneManager::GetCurrentScene<Scene>()->NotifySceneManager(POP, "Inventory");
        }
    }
}

struct Inventory : public Scene {

    /// \brief Execute le constructeur de la scène lorsqu'elle est chargée.
    /// \param param Paramètre de transit.
    /// \return Nouvelle instance de la scène Inventory.
    static inline Scene* LoadInventory(void* player){
        return new Inventory((Player*) player);
    }

    /// \param player Pointeur du player.
    Inventory(Player* player){
        GLContext::SetOrthogonal();
        GLContext::ApplyOrthogonal();
        this->transit = (void*) player;

        InventoryGui* inventory = new InventoryGui(player);
        Label* quit = new Label(Vector2(0.0, 0.0), INVENTORY_QUITLABEL_FONT, "Appuyez sur [tab] pour quitter.", {255, 255, 255, 255});
        Image* back = new Image(Vector2(0.0, 0.0), INVENTORY_SCENE_BACKGROUND_SIZE, INVENTORY_SCENE_BACKGROUND->GetId());
        Image* alice = new Image(INVENTORY_CHARACTER_POS, INVENTORY_CHARACTER_SIZE, INVENTORY_CHARACTER_IMG->GetId());
        Image* statsBox = new Image(INVENTORY_STATBOX_POS, INVENTORY_STATBOX_SIZE, INVENTORY_STATBOX_BACKGROUND->GetId());
        //LoadingBar* hpBar = new LoadingBar()

        this->AddSubscription(SDL_MOUSEMOTION, 1, inventory);
        this->AddSubscription(SDL_MOUSEBUTTONUP, 1, inventory);
        this->AddSubscription(SDL_KEYUP, 1, inventory);
        for(QuestManager::QuestIterator it = TRANSIT->GetQuest()->GetFirst(), end = TRANSIT->GetQuest()->GetLast(); it != end; it++){
            if(it->first == "InventoryAndEquipment")
                this->AddSubscription(SDL_KEYUP, 1, it->second);
        }

        this->AddOrtho(5, back, statsBox, quit, alice, inventory);
    }

    ~Inventory() {
        // Gaël: la façon optimale serait d'utiliser les std::map<T,T>::iterator. Too bad!
        for(uint i = 0, count = ortho.size(); i < count; i++) {
            delete ortho[i];
            ortho[i] = nullptr;
        }
    }

    /// \brief Affiche tout les VisualComp de la scène dans la fenêtre.
    void Draw(){
        GLContext::ApplyOrthogonal();
        glDisable(GL_DEPTH_TEST);

        // Gaël: la façon optimale serait d'utiliser les std::map<T,T>::iterator. Too bad!
        for(uint i = 0, count = ortho.size(); i < count; i++) 
            ortho[i]->Draw();
        
        TRANSIT->GetQuest()->Draw();
        glEnable(GL_DEPTH_TEST);
    }
};


#endif //PI2020_INVENTORY_HPP
