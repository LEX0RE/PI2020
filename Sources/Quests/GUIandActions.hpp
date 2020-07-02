/// \file GUIandActions.hpp
#ifndef GUIANDACTIONS_HPP
#define GUIANDACTIONS_HPP

#include "../Models/Target.hpp"
#include "../Quest/Quest.hpp"
#include "../Entities/NPC.hpp"
#include "../Entities/Player.hpp"
#include "../Meshes/CollisionableMesh.hpp"
#include "../Scenes/World.hpp"

/// \brief tutoriel pour la quête «GUI et Actions»
///
/// Informe du fonctionnement du GUI et de l'utilisation des armes
///
class GUIandActions : public Quest {
    Target* target;
public:
    /// \param name Nom de la quête.
    /// \param npc Pointeur de npc.
    /// \param player Pointeur vers le joueur.
    GUIandActions(const char* name, NPC* npc) : Quest(name, npc) {
        target = new Target({5.0,8.0,5.0});

        objectives["GUI"] = false;
        objectives["ATTACK"] = false;
        objectives["SHOOT"] = false;
        objectives["MESSAGE"] = false;

        dialogs.SetDialogue("LifeBar");
        dialogs.SetPosition(5, 30);
    }

    ~GUIandActions() {
        delete target;
    }

    /// \brief Affiche tout les éléments de la scène dans la fenêtre.
    void Draw(){
        //GLContext::ApplyPerspective();

        //GLContext::Clear();

        //Affiche les éléments de LA QUÊTE. Les éléments de la quête actuelle seront affichés par dessus world.
        target->Draw();

        GLContext::ApplyOrthogonal();

        dialogs.Draw();

        if((dialogs.GetDialogueID() == "LifeBar")&& (rand()%100) > 80 && playerPtr->GetHP() > 0){
            playerPtr->IncreaseLife(-1);
        }
        else if((dialogs.GetDialogueID() == "Level") && (rand()%100) > 70 && playerPtr->GetXP() < 1000){
        	if(playerPtr->GetXP() < 999)
            	playerPtr->IncreaseXP(1);
        }
        else if((dialogs.GetDialogueID() == "AttaqueArc")) {
            if (target->CheckCollisionSphere(playerPtr->GetArrow())) {
                objectives["SHOOT"] = true;
                dialogs.SetDialogue("GUIACTIONEND");
            }
        }

        //GLContext::Refresh();
    }

    /// \brief Gestion des événements
    void Notification(){
        if(Event::GetType() == SDL_KEYDOWN){
            switch(Event::GetKey()){
            //enter
            case 13:
                if((dialogs.GetDialogueID() == "LifeBar")){
                    dialogs.SetDialogue("Level");
                    dialogs.SetPosition(300, 700);
                }
                else if(dialogs.GetDialogueID() == "Level"){
                    dialogs.SetDialogue("XPBar");
                    dialogs.SetPosition(10, 500);
                }
                else if(dialogs.GetDialogueID() == "XPBar"){
                    dialogs.SetDialogue("MiniMap");
                    playerPtr->SetXP(0);
                }
                else if(dialogs.GetDialogueID() == "MiniMap"){
                    dialogs.SetDialogue("Quest");
                }
                else if(dialogs.GetDialogueID() == "Quest"){
                    dialogs.SetDialogue("Sword");
                    objectives["GUI"] = true;
                }
                else if(dialogs.GetDialogueID() == "GUIACTIONEND"){
                    objectives["MESSAGE"] = true;
                    playerPtr->SetXP(500);
                    playerPtr->SetLife(100);
                }
                break;
            case 't':
                if(dialogs.GetDialogueID() == "Sword"){
                    dialogs.SetDialogue("Attaque1");
                }
                if(dialogs.GetDialogueID() == "SwapWeapon"){
                    dialogs.SetDialogue("AttaqueArc");
                }
                break;
            }
        }
        if(Event::GetType() == SDL_MOUSEBUTTONDOWN){
            if(Event::GetMouseButton() == SDL_BUTTON_LEFT){
                Vector3 playerToTarget = {target->GetX() - playerPtr->GetPosition().x, 0.0, target->GetZ() - playerPtr->GetPosition().z};
                playerToTarget /= playerToTarget.GetNorm();
                double angle = playerToTarget.GetAngle(playerPtr->GetFront(), AngleUnit::DEGREE); //angle entre le vecteur du joueur vers la cible et du devant du joueur
                if(dialogs.GetDialogueID() == "Attaque1"){ // si ce dialogue est affiché, on le fait changer pour le suivant
                    if(target->CheckCollisionSphere(playerPtr)) // vérification s'il y a collision de sphère entre le joueur et la cible
                        if(angle <= 20 && angle >= -20){ // si le joueur est orienté vers la cible avec une certaine tolérance
                            objectives["ATTACK"] = true;
                            dialogs.SetDialogue("SwapWeapon");
                        }
                }
            }
        }
    }

    //sert a rien mais je dois l'avoir sinon classe abstraite ?
    /// \brief Gestion des événements
    void Notify(){}
};
#endif // GUIANDACTIONS_HPP
