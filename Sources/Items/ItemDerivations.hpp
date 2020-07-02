#ifndef ITEM_DERIVATION_HPP
#define ITEM_DERIVATION_HPP

class Player;

#include "ItemManager.hpp"
#include "../Entities/Player.hpp"

///
/// \brief Classe d'exemple à suivre pour l'ensemble des items.
//
struct Potion50 : public Item { 

	/// \brief Fonction appelant le constructeur.
	/// \param amount 
	static inline void* Constructor(ushort amount) {
		return (void*) new Potion50(amount);
	}

	Potion50(ushort count = 1) : Item(nullptr, count){
		this->count = count;
	}

	void Use() {
		ushort hp = PLAYER_PTR->GetHP() + 50;
		PLAYER_PTR->SetLife(hp); //Gaël: Il faudrait changer le nom de cette méthode... SetHP serait mieux.
		this->used = true;
	}

};

struct SwordItem : public Item {

    /// \brief Fonction appelant le constructeur.
    /// \param amount
    static inline void* Constructor(ushort amount) {
        return (void*) new SwordItem(amount);
    }

    SwordItem(ushort count = 1) : Item(nullptr, count){
        this->count = count;
    }

    void Use() {

    }

};



#endif
