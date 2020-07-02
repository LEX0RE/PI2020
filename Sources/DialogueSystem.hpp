#ifndef VISUALCOMPONENTS_DIALOGUESYSTEM_HPP_
#define VISUALCOMPONENTS_DIALOGUESYSTEM_HPP_

#include "Typedef.hpp"
#include "VisualComponents/HintBox.hpp"
#include "Chrono.hpp"

#include <iostream>
#include <fstream>

#define delay 0.05

using namespace std;

/// \brief Classe pour la gestion des dialogues
///
/// Classe allant chercher le bon dialogue dans tous les dialogues du jeu et l'affichant
///
class DialogueSystem {
	map<string, string> dialogues; ///< Liste des dialogues répertoriés selon leur identifiant
	string dialogueID; ///< Identifiant du dialogue à afficher.
	string nextDialogueID; ///< identifiant du prochain dialogue à afficher.
	uint character; ///< Caractère où le dialogue est rendu
	string title; ///< Titre du dialogue.
	HintBox* box; ///< Boîte de dialogue.
	uint boxCount; ///< Compteur de caractères avant la boite de dialogue.
	Chrono chrono; ///< Chrono qui mesure le temps depuis la dernière mise à jour du texte.
	SDL_Color color; ///< Couleur du texte
	bool skip; ///< Passe à la prochaine boîte quand vrai.

public:
	DialogueSystem() {
		character = 0;
		skip = false;

		string title;
		string str;
		ifstream fs("Dialogues.txt");
		color = {255, 255, 255, 255};

		box = new HintBox();
		SetPosition();

		str.clear();
		fs >> str;

		while(!fs.eof()) {
			if(str[0] == '#'){
				title = str.substr(1, str.length() - 1);

				str.clear();
				fs >> str;
				while(str != "#end"){
					dialogues[title] += str + " ";

 					str.clear();
					fs >> str;
				}
			}
			str.clear();
			fs >> str;
		}
		fs.close();
		SetDialogue("Exemple");
    }

	/// \brief Sélectionne la couleur du dialogue choisie
	/// \param Couleur choisie
	void SetColor(SDL_Color color = {255, 255, 255, 255}){
			this->color = color;
		}

    /// \brief Sélectionne le dialogue à utiliser et confirme qu'il a bien été sélectionné.
    /// \param dialogue Dialogue choisi.
    /// \return Si le dialogue existe ( true ) ou pas ( false ).
	bool SetDialogue(string dialogue){
		if(dialogues[dialogue].size())
			dialogueID = dialogue;

		boxCount = 1;
		character = 0;

		return dialogues[dialogue].size();
	}

	/// \brief Retourne un iterator qui pointe sur le prochain dialogue après celui affiché.
	/// \return Un iterator qui pointe sur le prochain dialogue après celui affiché.
	map<string, string>::iterator ItNextDialogue(){
		return next(dialogues.find(this->dialogueID), 1);
	}

	/// \brief Sélectionne le prochain dialogue à utiliser et confirme qu'il a bien été sélectionné.
	/// \param dialogue Dialogue choisi.
	/// \return Si le dialogue existe ( true ) ou pas ( false ).
	bool SetNextDialogue(string dialogue){
		if(dialogues[dialogue].size())
			nextDialogueID = dialogue;

		return dialogues[dialogue].size();
	}

	/// \brief Sélectionne le titre.
	/// \param title Titre choisi.
	void SetTitle(string title){
		this->title = title;
	}

	/// \brief Affichage du dialogue.
	void Draw(){

		if((chrono.InDelay(delay) && ((boxCount + character - 1) != dialogues[dialogueID].size()) && (dialogues[dialogueID][boxCount + character + 1] != '*')) || skip)
		    Update();

		box->visible = true;
		box->Draw();
		box->visible = false;
	}

	/// \brief Mets à jour le dialogue pour l'affichage en ajoutant un caractère.
	void Update(){
        box->ResetText();

        if(skip && (dialogues[dialogueID][boxCount + character + 1] == '*')){
            boxCount += character + 2;
            character = 0;
            skip = false;
        }

        else if(skip && (dialogues[dialogueID].size() > (boxCount + character))){
			while((dialogues[dialogueID].size() != (boxCount + character)) && (dialogues[dialogueID][boxCount + character + 2] != '*'))
				character++;
			skip = false;
		}

		if(title != "")
			box->AddText(title.c_str(), color);

		character++;

		if((dialogues[dialogueID].size() <= 100) || (character <= 100))
			box->AddText(dialogues[dialogueID].substr(boxCount, character).c_str(), color);

		else{
			box->AddText(dialogues[dialogueID].substr(boxCount, boxCount + 99).c_str(), color);
			if((dialogues[dialogueID].size() <= 200) || (character <= 200))
				box->AddText(dialogues[dialogueID].substr(boxCount + 100, boxCount + character - 100).c_str(), color);

			else{
				box->AddText(dialogues[dialogueID].substr(boxCount + 100, boxCount + 199).c_str(), color);
				box->AddText(dialogues[dialogueID].substr(boxCount + 200, boxCount + character - 200).c_str(), color);
			}
		}

		chrono.Reset();
	}
	
	/// \brief Change la position de la boite de dialogue.
	/// \param x Positon en X
	/// \param y Position en Y
	void SetPosition(uint x = (1024.0 / 4.0) - 50, uint y = ((768.0 / 4.0) * 3.0)){
		box->SetPosition(x, y);
	}

	/// \brief Permet de passer le dialogue plus rapidement ou de changer de boîte de dialogue si rendu à la fin du texte.
	void SkipText(){
		if((dialogues[dialogueID].size() == character + boxCount) && (nextDialogueID != "")){
			dialogueID = nextDialogueID;
			nextDialogueID = "";
			boxCount = 1;
			skip = false;
		}

		else if((boxCount + character - 1) != dialogues[dialogueID].size())
			skip = true;
	}

	/// \brief Obtiens le ID du dialogue actuellement affiché.
	/// \return ID du dialogue.
	string GetDialogueID(){
		return dialogueID;
	}

};

#endif /* VISUALCOMPONENTS_DIALOGUESYSTEM_HPP_ */
