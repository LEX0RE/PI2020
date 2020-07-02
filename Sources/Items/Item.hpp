#ifndef ITEM_HPP
#define ITEM_HPP

#include "ItemInfo.hpp"
#include "ItemMesh.hpp"

///
/// \brief Classe contenant les informations d'item ainsi que le nombre que celle-ci contient dans l'inventaire.
///
struct Item {
	ItemInfo* info; ///< information de l'item.
	ushort count; 	///< le compte du nombre du même item il y a dans une case.
	bool used;		///< informe si l'item a été utilisé ou non.

	/// \param info Information de l'item.
	/// \param count Quantité d'item
	Item(ItemInfo* info, ushort count) {
		this->info = info;
		this->count = 1;
		this->used = false;
	}

	/// \brief Méthode propre à chaque item spécifiant ce qu'il faut faire lors de l'utilisation du ledit (<- en un seul mot) item.
	virtual void Use() = 0;

	/// \brief Ajoute un bloc-item au monde actif.
	/// \param amount La quantité qui sera jeté.
	void Drop(Vector3 position, ushort amount = 1) {
		if(this->count < amount) //pour éviter de se ramasser avec un compte de 60000+.
			amount = this->count;
		Scene* scene = SceneManager::GetStackedScene("world");
		if(scene) { 
			((Scene3*)scene)->AddPickableItem(new ItemMesh3(this->info, this->count), this->info, position);
			this->count -= amount;
		}
	}
}; 

#endif
