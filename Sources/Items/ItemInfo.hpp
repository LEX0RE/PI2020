#ifndef ITEM_INFO_HPP
#define ITEM_INFO_HPP

#include "../VisualComponents/Image.hpp"
#include "../SceneManager.hpp"
#include <string>
#include <map>


///
/// \brief Classe contenant l'information de base d'un item. Agit similairement à une Resource.
///
struct ItemInfo {
	typedef void*(*ItemConstructor)(ushort amount); // Typedef de constructeur d'item

	bool equipable;				///< Indique si l'item peut être équipé.
	bool usable;				///< Indique si l'item peut être utilisé.
	Texture* texture;			///< Pointeur de texture pour les rejets d'items au sol. (cube avec cette texture)
	std::string id;				///< Identifiant dans le gestionnaire.
	std::string name;			///< Nom de l'item qui sera affiché
	std::string description;	///< Description de l'item qui sera affiché
	ItemConstructor ctor;		///< Pointeur de fonction appelant le constructeur de l'item spécifique.
	
	/// \param equipable Indique si l'item peut être équipé.
	/// \param usable Indique si l'item peut être utilisé.
	/// \param icon Texture utilisé pour afficher l'item dans l'inventaire.
	/// \param position Position, en pixels, de l'emplacement de l'icône. 
	/// \param id Chaîne de caractère de l'identifiant
	/// \param name Nom affiché de l'item lors d'un survolement de la souris sur celui-ci.
	/// \param description Description affichée de l'item lors d'un survolement de la souris sur celui-ci.
	ItemInfo(bool equipable, bool usable, Texture* icon, std::string id, std::string name, std::string description, ItemConstructor ctor = nullptr) {
		this->equipable = equipable;
		this->usable = usable;
		this->texture = icon;
		this->id = id;
		this->name = name;
		this->description = description;
		this->ctor = ctor;
	}
};

#endif
