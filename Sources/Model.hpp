/// \file Model.hpp
#ifndef MODEL_HPP
#define MODEL_HPP

#include "Observer.hpp"
#include <map>

/// \brief Classe représentant un modèle.
class Model : public Observer {
protected: 
	std::map<uint, void(*)(Model*)> eventRouter; ///< Associe un numéro d'événement à une fonction.

public: 
	Model() {
		eventRouter = std::map<uint, void(*)(Model*)>();
	}
	
	/// \brief Ajoute à la map de routage une fonction associée à un numéro d'événement.<
	/// \param event Numéro d'événement.
	/// \param function Pointeur de fonction void avec un Model* en paramètre.
	inline void Add(uint event, void(* function)(Model*)) {
		eventRouter[event] = function;
	}
	
	/// \brief Gère les événements 
	virtual void Notification() = 0;
};

#endif
