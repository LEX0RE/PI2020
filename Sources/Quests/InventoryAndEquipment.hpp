#ifndef QUESTS_INVENTORYANDEQUIPMENT_HPP_
#define QUESTS_INVENTORYANDEQUIPMENT_HPP_

#include "../Quest/Quest.hpp"
#include "../Entities/NPC.hpp"
#include "../Entities/Player.hpp"
#include "../Scenes/World.hpp"

/// \brief tutoriel pour la quête «Inventaire et Équipements»
///
/// Informe du fonctionnement de l'inventaire et de l'équipement
///
class InventoryAndEquipment : public Quest {
    Player* playerQuest;
public:
    /// \param name Nom de la quête.
    /// \param npc Pointeur de npc.
    /// \param player Pointeur vers le joueur.
    InventoryAndEquipment(const char* name, NPC* npc, Player* player) : Quest(name, npc) {
    	playerQuest = player;
        dialogs.SetDialogue("OpenInventory");

    	objectives["OPENINVENTORY"] = false;
    	objectives["EQUIP"] = false;
    	objectives["CLOSEINVENTORY"] = false;
    	objectives["EXCHANGE"] = false;

    	dialogs.SetTitle("Gerald");
    }

    void Draw(){
        GLContext::ApplyOrthogonal();
        dialogs.Draw();

        if((dialogs.GetDialogueID() == "UseEquip") && (playerQuest->GetItems()[17] || playerQuest->GetItems()[16])){
            if(playerQuest->GetItems()[17]){
                if(playerQuest->GetItems()[17]->info->id == "Sword") {
                    dialogs.SetDialogue("CloseInventory");
                    playerPtr->SetXP(500);
                }
            }

            else{
                dialogs.SetDialogue("CloseInventory");
                playerPtr->SetXP(500);
            }
        }
    }

    /// \brief Gestion des événements
    void Notification(){
        	if(Event::GetType() == SDL_KEYDOWN){
    		switch(Event::GetKey()) {
                //enter
                case 13:
                    if (dialogs.GetDialogueID() == "UseEquip") {
                        dialogs.SkipText();
                    }
                break;
                case SDLK_TAB:
                    if ((dialogs.GetDialogueID() == "OpenInventory") && (SceneManager::GetCurrentSceneName() == "World")) {
                        dialogs.SetDialogue("UseEquip");
                    }
                    else if ((dialogs.GetDialogueID() == "CloseInventory") && (SceneManager::GetCurrentSceneName() == "Inventory")) {
                        dialogs.SetDialogue("INVENTORYEQUIPMENTEND");
                    }
                break;
            }
    	}
    }
};

#endif /* QUESTS_INVENTORYANDEQUIPMENT_HPP_ */
