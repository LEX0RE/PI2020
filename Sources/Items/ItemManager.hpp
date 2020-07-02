#ifndef ITEM_MANAGER_HPP
#define ITEM_MANAGER_HPP

#include "ItemInfo.hpp"
#include "ItemManager.hpp"

#define PLAYER_PTR ((Player*)(SceneManager::GetCurrentScene<Scene>()->transit))

///
/// \brief Classe gérant et gardant en mémoire les informations des items.
///
class ItemManager {
	static std::map<std::string, ItemInfo*> itemInfo; ///< std::map contenant des ItemInfo ayant un std::string comme identifiant.

public:
	typedef std::map<std::string, ItemInfo*>::iterator ItemInfoIterator; 

	/// \brief Ajoute un certain nombre de ItemInfo à la std::map. 
	/// \param argc Compte du nombre d'élément à ajouter.
	static void AddInfo(uint argc, ...) {
		va_list argv;
		va_start(argv, argc);
		for(uint i = itemInfo.size(), m = argc + i; i < m; i++) {
			ItemInfo* info = va_arg(argv, ItemInfo*);
			itemInfo[info->id] = info;
		}
		va_end(argv);
	}

	/// \brief Obtient l'information de l'item selon un identifiant.
	/// \param id Identifiant.
	/// \return Retourne nullptr si la donnée spécifiée n'existe pas. Autrement, retourne un pointeur d'ItemInfo.
	static inline ItemInfo* Get(std::string id) {
		return itemInfo[id];
	}

	/// \brief Retourne le premier élément de la std::map.
	/// \return Retourne un ItemInfoIterator.
	static inline ItemInfoIterator GetFirst() {
		return itemInfo.begin();
	}

	/// \brief Retourne le dernier élément de la std::map.
	/// \return Retourne un ItemInfoIterator.
	static inline ItemInfoIterator GetLast() {
		return itemInfo.end();
	}
};
std::map<std::string, ItemInfo*> ItemManager::itemInfo = std::map<std::string, ItemInfo*>();



#endif

