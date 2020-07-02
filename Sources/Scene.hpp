/// \file Scene.hpp
#ifndef SCENE_HPP
#define SCENE_HPP

#define TRANSIT ((Player*)transit)

#include <map>
#include <list>
#include <queue>
#include "Event.hpp"
#include "Chrono.hpp"
#include "GLContext.hpp"
#include "VisualComp.hpp"
#include "Meshes/CollisionableMesh.hpp"

#define INVENTORY_MAXIMUM_COUNT 18

struct ItemInfo; 
struct ItemMesh3;


/// \brief Options pour la méthode "NotifySceneManager" de Scene, utilisé par le SceneManager.
enum NOTIFY_SCENEMANAGER_OPT {
	DRAW = 0,
	FREE = 1,
	PUSH = 2,
	POP = 3
};

/// \brief Classe représentant une scène.
///
/// Classe représentant une scène ainsi que tout ces composantes orthogonaux et de projections.
///
class Scene {
public:
	typedef uint EventNb;
	
protected:
	std::map<uint, VisualComp*> ortho; ///< Collection de VisualComponent. (Rendu en 2D).
	std::map<EventNb, Observable*> events; ///< Collection d'observable. Sert à diriger les événements.
	
public:
	NOTIFY_SCENEMANAGER_OPT options; ///< Options utilisé par le SceneManager pour savoir quoi faire avec la scène.
	std::string nextScene; ///< Indique la prochaine scène à charqer après la suppresion de cette scène. Écrire "POP" si la scène est empliée.
	void* transit;

	/// \param transit Transit doit être un pointeur de Player. 
	Scene(void* transit = nullptr) {
		this->ortho = std::map<uint, VisualComp*>();
		this->events = std::map<EventNb, Observable*>(); 
		this->nextScene = "null";
		this->options = NOTIFY_SCENEMANAGER_OPT::DRAW;
		this->transit = transit;
	}
	
	~Scene() {
		for(auto it : ortho)
			delete it.second;

		for(auto it : events) 
			delete it.second;
	}

	/// \brief Règle les membres locals afin d'indiquer quoi faire avec la scène au SceneManager.
	/// \param options Options de notification.
	/// \param nextSceneName Nom de la prochaine scène. Pour dépiler la présente scène, utilisez "POP".
	inline void NotifySceneManager(NOTIFY_SCENEMANAGER_OPT options, std::string nextSceneName) {
		this->nextScene = string(nextSceneName);
		this->options = options;
	}

	/// \brief Ajoute des composants visuels.
	/// \param argc nombre de composants visuels passés en paramètres.
	/// \param ... Composantes visuelles.
	void AddOrtho(uint argc, ...) {
		va_list argv;
		va_start(argv, argc);
		for(uint i = ortho.size(), m = i + argc; i < m; i++)
			ortho[i] = va_arg(argv, VisualComp*);
		va_end(argv);
	}

	/// \brief Liste les évenements auxquels la scène sera notifiée.
	/// \param argc Compte des évenements.
	void AddObservable(uint argc, ...) {
		va_list argv;
		va_start(argv, argc);
		for(uint i = events.size(), m = i + argc; i < m; i++) 
			events[va_arg(argv, uint)] = new Observable();
		va_end(argv);
	}

	/// \brief Obtient un composant visuel.
	/// \param i Indice où se trouve le VisualComp.
	/// \return La méthode retourne un VisualComp.
	inline VisualComp* GetOrtho(uint i) {
		if(i < ortho.size())
			return ortho[i];
		return nullptr;
	}

	/// \brief Ajoute des composants visuels et les inscrits aux événements spécifiés.
	/// \param argc nombre de paires passée en paramètres.
	/// \param ... un nombre d'Observer est passé en paramètre.
	void AddSubscription(EventNb enb, uint argc, ...) {
		if(!events[enb])
			events[enb] = new Observable();
		
		Observer** arr = new Observer*[argc];
		va_list argv;
		va_start(argv, argc);
		for(uint i = 0; i < argc; i++) 
			arr[i] = va_arg(argv, Observer*);
		va_end(argv);

		events[enb]->Subscribe(argc, arr); //le tableau "arr" est supprimé à l'intérieur de la méthode.
	}
	
	/// \brief affiche la scène.
	virtual void Draw() = 0;
	
	/// \brief Dirige les événements aux observeurs concernés.
	/// \param enb Numéro d'événement (uint).
	inline void Notify(EventNb enb) {
		if(events[enb]) 
			events[enb]->Notify();
	}
	
};

///
/// \brief Classe de généralisation de Scene contenant du contenu à afficher en 3D.
///
class Scene3 : public Scene { 
protected:

	// Gaël: je pourrais renommer Scene en Scene2 ... 
	//       mais je suis trop paresseux...  Too Bad!

	map<ushort, CTexturedMesh3*> item;	///< Map de tous les items au sol. (cube avec l'image de l'item en question)
	map<ushort, ItemInfo*> itemInfo;	///< Map des ItemInfos liés au item-blocs.	
	list<CMTexturedMesh3*> enemyList; 	///< Liste de toutes les ennemies.
	list<CTexturedMesh3*> cMeshList; 	///< Liste de tous les mesh avec lesquels il y a collision.
	list<CTexturedMesh3*> npcList; 		///< Liste de tous les NPCs du monde.
	list<TexturedMesh3*> tMeshList; 	///< Liste de tous les mesh texturés avec aucune collision.

	/// \brief Méthode retournant un élément d'une liste spécifique.
	/// \param index indice auquel l'élément se trouve.
	/// \param list Liste dans laquel il faut itérer.
	/// \return Retourn un pointeur de type 'T*'.
	template <class T> T* GetListedElement(uint index, std::list<T*> list) {
		uint max = list.size(), i = 0;
		if(index > max)
			return nullptr;
		
		ListIterator<T*> it = list.begin();
		while(i < index) {
			i++; 
			it++;
		}
		
		return *it;
	}

	/// \brief Supprime le contenu de la map d'items.
	void DeleteItemMap() {
		for(ushort i = 0, m = item.size(); i < m; i++)
			delete item[i];
	}

	/// \brief supprime le contenu d'une liste.
	/// \param list Pointeur de liste.
	/// \param T Type de la liste.
	template <class T> inline void DeleteList(std::list<T> *list) {
		for(ListIterator<T*> it = list->begin(), end = list->end(); it != end; it++) 
			delete *it;
	}

public:
	// ↓ "Typedef" pour rendre la déclaration d'itérateur moins longue.
	template <class T> using ListIterator = typename std::list<T>::iterator; 

	Scene3() : Scene() {
		item = map<ushort, CTexturedMesh3*>();
		itemInfo = map<ushort, ItemInfo*>();
		enemyList = list<CMTexturedMesh3*>();
		cMeshList = list<CTexturedMesh3*>();
		npcList = list<CTexturedMesh3*>();
		tMeshList = list<TexturedMesh3*>();
	}

	virtual ~Scene3() {
		//DeleteList<CTexturedMesh3>(&this->npcList);
		//DeleteList<CTexturedMesh3>(&this->cMeshList);
		//DeleteList<TexturedMesh3>(&this->tMeshList);
		//DeleteList<CMTexturedMesh3>(&this->enemyList);
		DeleteItemMap();
	}

	/// \brief Ajoute un pointeur de TexturedMesh3 à la liste tMeshList.
	/// \param tm3 une TexturedMesh3.
	inline void AddTextured(TexturedMesh3* tm3) {
		this->tMeshList.push_back(tm3);
	}

	/// \brief Ajoute un pointeur de CMTexturedMesh3 à la liste cMeshList.
	/// \param ctm3 une CMTexturedMesh3.
	inline void AddCollisionable(CTexturedMesh3* ctm3) {
		this->cMeshList.push_back(ctm3);
	}

	/// \brief Ajoute un pointeur de NPC à la liste cMeshList.
	/// \param npc un NPC.
	inline void AddNPC(CTexturedMesh3* npc) {
		this->npcList.push_back(npc);
	}

	/// \brief Ajoute un pointeur de CMTexturedMesh3 à la liste cMeshList.
	/// \param enemy une CMTexturedMesh3.
	inline void AddEnemy(CMTexturedMesh3* enemy) {
		this->enemyList.push_back(enemy);
	}

	/// \brief Retourne un pointeur de TexturedMesh3 de la liste tMeshList.
	/// \param index index de l'Enemy.
	inline CMTexturedMesh3* GetEnemy(uint index) {
		return GetListedElement<CMTexturedMesh3>(index, enemyList);
	}

	/// \brief Retourne un pointeur de TexturedMesh3 de la liste tMeshList.
	/// \param index index du NPC.
	inline CTexturedMesh3* GetNPC(uint index) {
		return GetListedElement<CTexturedMesh3>(index, npcList);
	}

	/// \brief Retourne un pointeur de TexturedMesh3 de la liste tMeshList.
	/// \param index index du CTexturedMesh3.
	inline CTexturedMesh3* GetCollisionable(uint index) {
		return GetListedElement<CTexturedMesh3>(index, cMeshList);
	}

	/// \brief Retourne un pointeur de TexturedMesh3 de la liste tMeshList.
	/// \param index index du TexturedMesh3.
	inline TexturedMesh3* GetTextured(uint index) {
		return GetListedElement<TexturedMesh3>(index, tMeshList);
	}

	/// \brief Ajoute un bloc-items.
	/// \param item Une mesh ItemMesh3.
	void AddPickableItem(CTexturedMesh3* item, ItemInfo* info, Vector3& position) {
		uint i = this->item.size();
		item->MoveTo(position);
		this->item[i] = item;
		this->itemInfo[i] = info;
	}

	/// \brief Retourne une mesh d'item spécifié.
	/// \param id Identifiant.
	/// \return Retourne un pointeur d'ItemMesh3.
	inline ItemMesh3* GetItemMesh(ushort id) {
		return (ItemMesh3*)item[id];
	}

	/// \brief Retourne l'item spécifié et l'enlève de la liste d'item-blocs.
	/// \param id Identifiant de l'item-bloc
	/// \return Retourne un ItemInfo*. Si nullptr, alors l'identifiant était invalide.
	ItemInfo* PickItem(ushort id) {
		//TODO: vérifier si l'id correspond à un item. 
		//      Si oui, supprimer la mesh et retourner 
		//      l'itemInfo.

		if(CTexturedMesh3* i = item[id]) { //si l'item-bloc existe à l'identifiant spécifié.
			item.erase(id); 
			delete i;
			ItemInfo* info = itemInfo[id];
			itemInfo.erase(id);
			return info;
		}

		return nullptr;

	}

	/// \brief Vérifie la collision des items avec le joueur
	/// \param player pointeur de player.
	inline bool IsTouchingItem(CMTexturedMesh3* player) {
		for(std::map<ushort, CTexturedMesh3*>::iterator it = item.begin(), end = item.end(); it != end; it++) {
			if(it->second->CheckCollisionSphere(player))
				return true;
		}
		return false;
	}

	/// \brief Vérifie la collision des items avec le joueur et retourne une file d'identifiants.
	/// \param player Pointeur du player.
	/// \return Retourne une std::queue<ushort>. Si la taille est 0, alors il n'y a aucune collision.
	inline queue<ushort> TouchedItems(CMTexturedMesh3* player) {
		//TODO: vérifier si le joueur est en collision avec les items.
		queue<ushort> queueItem;
		for(uint i = 0, max = item.size(); i < max; i++) {
			if(player->CheckCollisionSphere(item[i]))
				queueItem.push(i);
		}
		return queueItem;
	}
};




#endif
