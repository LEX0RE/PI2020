#ifndef QUESTMANAGER_HPP
#define QUESTMANAGER_HPP
#include "Quest.hpp"
#include <string>

#define UNKNOWN_QUEST 2

/// \brief Gère les quests et est propre à chaque joueurs.
class QuestManager {
	std::map<std::string, Quest*> quests; ///< std::map de quests avec un std::string comme identifiant.
	std::map<std::string, Quest*> completed; ///< std::map de quests complétées avec un std::string comme identifiant.
	public:
	typedef std::map<std::string, Quest*>::iterator QuestIterator;

	QuestManager() {
		quests = std::map<std::string, Quest*>();
		completed = std::map<std::string, Quest*>();
	}

	~QuestManager() {
		for(QuestIterator it = quests.begin(), end = quests.end(); it != end; it++)
			delete it->second;
		for(QuestIterator it = completed.begin(), end = completed.end(); it != end; it++)
			delete it->second;
	}

	/// \brief Ajoute une Quest.
	/// \param quest Pointeur de Quest. 
	inline void operator+=(Quest* quest) {
		quests[quest->GetQuestName()] = quest;
	}

	/// \brief Supprime une Quest.
	/// \param name Nom de la quête à supprimer.
	void operator-=(std::string name) {
		delete quests[name];
		quests.erase(name);
	}

	/// \brief Vérifie si la Quest spécifiée est complétée.
	/// \param name Nom de la Quest.
	/// \return retourne un booléen où true signifie que la quête est complétée.
	char VerifyCompletition(std::string name) {
		if(quests[name])
			return false;
		else if(completed[name])
			return true;
		return UNKNOWN_QUEST;
	}

	/// \brief Obtient un pointeur de quête.
	/// \param name Nom de la Quest à obtenir.
	/// \return Retourne un pointeur de Quête. Ne pas libérer de la mémoire. Le gestionnaire s'en charge!
	inline Quest* operator[](std::string name) {
		return quests[name];
	}
	
	/// \brief Obtient un itérateur de la std::map de quêtes complétées.
	/// \return Retourne un QuestIterator.
	inline QuestIterator GetFirstCompleted() {
		return completed.begin();
	}

	/// \brief Obtient le dernier noeud de la map de quêtes complétées. 
	/// \return Retourne un QuestIterator
	inline QuestIterator GetLastCompleted() {
		return completed.end();
	}
	
	/// \brief Obtient un itérateur de la std::map de quête.
	/// \return Retourne un QuestIterator.
	inline QuestIterator GetFirst() {
		return quests.begin();
	}

	/// \brief Obtient le dernier noeud de la map. 
	/// \return Retourne un QuestIterator
	inline QuestIterator GetLast() {
		return quests.end();
	}

	/// \brief Retourne le nombre de quête.
	/// \return Retourne un entier non signé de 32 bits.
	inline uint GetQuestCount() const {
		return quests.size();
	}
	
	/// \brief Dessine le contenu des quêtes
    void Draw() {
        QuestIterator* toDelete = nullptr;
        for(QuestIterator it = GetFirst(), end = GetLast(); it != end; it++) {
            if(toDelete){
                quests.erase(*toDelete);
                delete toDelete;
                toDelete = nullptr;
            }

            if(it->second->VerifyCompletition()) {
                std::string itName = it->first;
                Quest* itQuest = it->second;
                toDelete = new QuestIterator(it);
                completed[itName] = itQuest;
            }
            else
                it->second->Draw();
        }

        if(toDelete){
            quests.erase(*toDelete);
            delete toDelete;
            toDelete = nullptr;
        }
	}
}; 

#endif 
