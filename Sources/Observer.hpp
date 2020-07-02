/// \file Observer.hpp
#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <list>
#include <cstdarg>
#include "Typedef.hpp"

/// \brief Classe devant être dérivée pour être souscrit à un Observable.
struct Observer {
	/// \brief Méthode servant à la notification de l'Observer.
	virtual void Notification() = 0;
};

/// \brief Classe notifiant les Observers lorsqu'un évenement se déclanche.
class Observable {
	std::list<Observer*> observers; ///< Liste contenant les Observers. Sert à notifier les Observers lorsqu'un évenement est déclanché.
	
public:
	
	Observable() {
		observers = std::list<Observer*>();
	}
	
	/// \brief Souscrit un certain nombre d'Observer.
	/// \param argc Paramètre définissant le nombre d'éléments à souscrire.
	/// \param ... Élément qui observe.
	void Subscribe(uint argc, ...) {
		va_list argv;
		va_start(argv, argc);
		for(uint i = 0; i < argc; i++)
			observers.push_back(va_arg(argv, Observer*));
		va_end(argv);
	}

	/// \brief Souscrit un certain nombre d'Observer.
	/// \param argc Paramètre définissant le nombre d'éléments à souscrire.
	/// \param argv Tableau contenant les éléments à souscrire.
	void Subscribe(uint argc, Observer** argv) {
		for(uint i = 0; i < argc; i++)
			observers.push_back(argv[i]);
		delete[] argv;
		argv = nullptr;
	}
	
	/// \brief Méthode notifiant les éléments souscrits.
	void Notify() {
		for(auto it : observers)
			it->Notification();
	}
};

#endif
