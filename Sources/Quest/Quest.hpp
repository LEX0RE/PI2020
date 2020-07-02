#ifndef QUEST_HPP
#define QUEST_HPP

#include "../Typedef.hpp"
#include "../Observer.hpp"
#include "../VisualComp.hpp"
#include "../Entities/NPC.hpp"
#include "../Entities/Player.hpp"
#include "../DialogueSystem.hpp"
#include "../SceneManager.hpp"
#include <string>
#include <map>

#define playerPtr ((Player*)SceneManager::GetCurrentScene<Scene>()->transit)

/// \brief Classe gérant les quêtes.
class Quest : public Observer {
protected:
	bool showDialog; ///< indique si l'affichage des dialogues sont activés.
	string name; ///< nom de la quête.
	NPC* npc; ///< NPC lié à la quête.
	DialogueSystem dialogs; ///< Système de dialogues pour indiquer les différents objectifs à réaliser
	map<uint, Observable*> events; ///< Collection d'observable. Sert à diriger les événements.
	map<string, bool> objectives; ///< une std::map avec le nom de l'objectif comme identifiant et un bool pour son état.

public:
	typedef std::map<string, bool>::iterator ObjectiveIterator;
	
	/// \param name Nom de la quête
	/// \param npc Pointeur de npc
	Quest(string& name, NPC* npc) {
		this->name = name;
		this->objectives = map<string, bool>();
		this->events = map<uint, Observable*>();
		this->npc = npc;
		this->dialogs = DialogueSystem();
		this->showDialog = true;
	}

	/// \param name Nom de la quête
	/// \param npc Pointeur de npc
	Quest(const char * name, NPC* npc) {
		this->name = string(name);
		this->objectives = map<string, bool>();
		this->events = map<uint, Observable*>();
		this->npc = npc;
		this->dialogs = DialogueSystem();
		this->showDialog = true;
	}

	virtual ~Quest() {
		for(map<uint, Observable*>::iterator it = events.begin(), end = events.end(); it != end; it++) 
			delete it->second;
	}
	
	/// \brief Ferme la quête et fait que la quête ne puisse répondre à aucun évenement.
	void Close() {
		for(map<uint, Observable*>::iterator it = events.begin(), end = events.end(); it != end; it++) 
			delete it->second;
	}

	/// \brief Vérifie si les objectifs de la quêtes sont complétés. 
	/// \return Retourne un booléen où true signifie que la quête est complétée.
	bool VerifyCompletition() {
		for(ObjectiveIterator it = objectives.begin(), end = objectives.end(); it != end; it++) {
			if(!it->second)
				return false;
		}
		return true;
	}

	/// \brief Obtient l'état d'un objectif.
	/// \return Retourne un booléen où "true" signifie que l'objectif est complété.
	inline bool GetObjectiveState(string name) {
		return objectives[name];
	}

	/// \brief Obtient l'état d'un objectif.
	/// \return Retourne un booléen où "true" signifie que l'objectif est complété.
	inline bool operator[](string name) {
		return objectives[name];
	}

	/// \brief obtient le nom de la quête. 
	/// \return retourne un std::string.
	inline string GetQuestName() {
		return name;
	}

	/// \brief obtient le nombre d'objectif
	/// \brief retourne un entier non-signé de 32 bits.
	inline uint GetCount() {
		return objectives.size();
	}

	/// \brief Obtient le premier noeud de la map. 
	/// \return Retourne un ObjectiveIterator
	inline ObjectiveIterator GetFirst() {
		return objectives.begin();
	}

	/// \brief Obtient le dernier noeud de la map. 
	/// \return Retourne un ObjectiveIterator
	inline ObjectiveIterator GetLast() {
		return objectives.end();
	}
	
	/// \brief dessine le contenu de la quête.
	virtual void Draw() = 0;

	/// \brief notifie la quête d'un évenement.
	virtual void Notification() = 0;
};
#endif 
