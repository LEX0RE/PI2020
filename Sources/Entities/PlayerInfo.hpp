#ifndef PLAYER_INFO_HPP
#define PLAYER_INFO_HPP

#include "../VisualComponents/DebugPanel.hpp"
#include "../VisualComponents/VisualProgress.hpp"
#include "../VisualComponents/Label.hpp"

#include "../Cameras/FirstPersonCam.hpp"
#include "../Cameras/ThirdPersonCam.hpp"

#include "../GLContext.hpp"
#include "../Resources/Texture.hpp"

#include "../Models/Sword.hpp"
#include "../Models/Bow.hpp"

#include "../SceneManager.hpp"
#include "../Quest/QuestManager.hpp"
#include "../Items/Item.hpp"
#include "../Items/ItemManager.hpp"

#include "../Chrono.hpp"
#include "EntityDefines.hpp"
#include "Arrow.hpp"
#include "Enemy.hpp"

#define INVENTORY_COUNT 18

///
/// \brief Énumération des choix possible du tenu en main du joueur.
///
enum HoldingItem {
    EMPTY = 0,
    SWORD = 1,
    BOW = 2
};

///
/// \brief Structure contenant l'information sur l'interface graphique du joueur.
///
struct PlayerGui {
    VisualProgress* lifeBar;    ///< Barre de vie du joueur.
    VisualProgress* expBar;     ///< Barre d'expérience du joueur.
    Label* nameText;            ///< (?)
    Label* lifeText;            ///< Points de vie.
    Label* levelText;           ///< Nom du niveau.

	/// \param lifebar Barre de vie du joueur.
	/// \param expBar Barre d'expérience du joueur.
	/// \param nameText (?)
	/// \param lifeText Points de vie.
	/// \param levelText Nom du niveau.
	PlayerGui(VisualProgress* lifeBar, VisualProgress* expBar, Label* nameText, Label* lifeText, Label* levelText) {
		this->lifeBar = lifeBar;
		this->expBar = expBar;
		this->nameText = nameText;
		this->lifeText = lifeText;
		this->levelText = levelText;
	}

	~PlayerGui() {
		delete lifeBar;
		delete expBar;
		delete nameText;
		delete lifeText;
		delete levelText;

		lifeBar = nullptr; 
		expBar = nullptr;
		nameText = nullptr;
		lifeText = nullptr;
		levelText = nullptr;
	}

    /// \brief Affiche les composants visuels.
    void Draw() {
        lifeBar->Draw();
        expBar->Draw();
        nameText->Draw();
        lifeText->Draw();
        levelText->Draw();
    }
};

///
/// \brief Structure contenant l'information sur l'état du joueur.
///
struct PlayerState {
    bool w, a, s, d;            ///< Indique si une touche est enfoncée.
    bool e, q, f, space;        ///< Indique si une touche est enfoncée.
    bool first;                 ///< Indique si la caméra est en mode 1e personne.
    bool slope;                 ///< Indique si le joueur est en train de monter une pente.
    bool onGround;              ///< Indique si le joueur touche au sol.
    bool animStart;             ///< Indique si une animation est lancé
	bool attackSword;           ///< Indique si une attaque à l'épée à été lancée
    uint animIndex;             ///< Indice de l'animation qui sera en cours
    map<uint, TexturedMesh3*> sprites; ///< Dictionnaire contenant tout les sprite d'animations

	PlayerState() {
		w = a = s = d = e = q = f = space = false;
		attackSword = first = slope = animStart = false;
		animIndex = 0;
		sprites = map<uint, TexturedMesh3*>();
	}
};

///
/// \brief Structure contenant l'information statistique du joueur.
///
struct PlayerStatistics {
    ushort damage; 					///< Dommage que le joueur peut faire
    ushort level; 					///< Id du monde dans lequel le joueur est.
    double movingSpeed; 			///< Indique la vitesse de déplacement.
    double rotationSpeed; 			///< indique la vitesse de rotation.
    Item** inventory; 				///< Inventaire qui va contenir tous les éléments du joueur
    Chrono* chrono; 			    ///< chrono de player.
	Chrono* couldown; 			    ///< Temps de chargement de player.
    QuestManager* questManager;     ///< Gestinnaire de quête.
    map<const char*, uint> defense; ///< Défense du joueur selon le type de défense

	/// \param damage Dommage qu'inflige le joueur aux ennemis.
	/// \param level Niveau du joueur.
	/// \param movingSpeed Vitesse de déplacement
	/// \param rotationSpeed Vitesse de rotation
	PlayerStatistics(ushort damage, ushort level, double movingSpeed, double rotationSpeed) {
		this->damage = damage;
		this->level = level;
		this->movingSpeed = movingSpeed;
		this->rotationSpeed = rotationSpeed;
		
		this->inventory = new Item*[INVENTORY_COUNT];
		for(Uint8 i = 0; i < INVENTORY_COUNT; i++) 
			this->inventory[i] = nullptr;
		this->chrono = new Chrono();
		this->couldown = new Chrono();
		this->questManager = new QuestManager();
	}

	~PlayerStatistics() {
		for(Uint8 i = 0; i < INVENTORY_COUNT; i++) 
			delete inventory[i];
		delete[] inventory;
		delete chrono;
		delete couldown;
		delete questManager;

		inventory = nullptr;
		chrono = nullptr;
		questManager = nullptr;
	}

	/// \brief Ajoute un Item (s'il y a assez de place) dans l'inventaire.
	/// \param info Pointeur constant de ItemInfo. Celui-ci doit ABSOLUMENT provenir du ItemManager.
	/// \param count Nombre du même item ajouté à l'inventaire.
	/// \return Retourne un booléen où true signifie que l'item a bien été inséré. False signifie qu'il n'y a plus de place.
	bool AddItem(const ItemInfo* info, ushort count) {
		for(Uint8 i = 0, m = INVENTORY_COUNT - 2; i < m; i++) {
			if(inventory[i] == nullptr) { // si on trouve le même item dans l'inventaire, alors on additionne leurs comptes.
				inventory[i] = (Item*)info->ctor(count);
				inventory[i]->info = (ItemInfo*)info;
				inventory[i]->count = count;
				return true;
			}
			else if(inventory[i]->info == info) { // si on trouve une case libre, alors on met l'item dedans.
				inventory[i]->count += count;
				return true;
			}
		}
		return false; //si le programme se rend ici, cela veut dire qu'il n'y a plus de place dans l'inventaire.
	}
	/// \brief Supprime un item de l'inventaire.
	/// \param index Case de l'inventaire à vider.
	/// \param count Quantité à supprimer de l'inventaire.
	void RemoveItem(Uint8 index, ushort count) {
		if(index < INVENTORY_COUNT) {
			if(Item* item = inventory[index]) {
				if(item->count > count) // s'il y a plus d'item que ce qu'on veut enlever, alors on réduit le compte.
					item->count -= count;
				else { // si la quantité à enlever est plus grande ou égale que le compte, alors on vide la case.
					delete item;
					inventory[index] = nullptr;
				}
			}
		}
	}
};

///
/// \brief Contient les objets que porte le joueur.
///
struct PlayerEquipement {
    Arrow* arrow; ///< Pointeur vers la dernière flèche tiré par le joueur.
    Sword* sword; ///< Épée du joueur.
    Bow* bow; ///< Épée du joueur.
    HoldingItem type; ///< Type d'item tenu par le joueur.

	/// \param arrow Pointeur d'Arrow.
	/// \param sword Pointeur de Sword.
	/// \param bow Poiteur de Bow.
	PlayerEquipement(Arrow* arrow, Sword* sword, Bow* bow) {
		this->arrow = arrow;
		this->sword = sword;
		this->bow = bow;
		this->type = HoldingItem::EMPTY;
	}

	~PlayerEquipement() {
		if(arrow)
			delete arrow;
		if(sword)
			delete sword;
		if(sword) 
			delete bow;

		arrow = nullptr;
		sword = nullptr;
		bow = nullptr;
	}
};


#endif
