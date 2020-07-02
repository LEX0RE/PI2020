#ifndef PI2020_ENEMY_HPP
#define PI2020_ENEMY_HPP

#include "../Chrono.hpp"
#include "../Resources/Texture.hpp"
#include "../Meshes/CollisionableMesh.hpp"
#include "../VisualComponents/DoubleProgress.hpp"
#include "../VisualComponents/Label.hpp"
#include "EntityDefines.hpp"
#include <stdlib.h>
#include <cstdarg>

#define showHitbox true
#define generateMaxRange 200

/// \brief fonction déterminant comment le Goomba recoit des dommages.
/// \param goomba Mesh du Goomba
/// \param player Mesh du player
/// \param mouvement Mouvement du joueur
bool JumpDamage(CTexturedMesh3* goomba, CTexturedMesh3* player, Vector3 mouvement){
	if(mouvement.y < 0)
		if(player->CheckCollisionFace(goomba, {player->GetPosition().x, player->GetPosition().y + player->hitbox->minsize.y, player->GetPosition().z}, {0.0, -1.0, 0.0}))
			if(player->CheckCollisionBox(goomba, mouvement))
				return true;
	return false;
}

/// \brief fonction déterminant comment les autres ennemis recoivent des dommages.
/// \param ennemi Mesh de l'ennemi
/// \param player Mesh du player
/// \param mouvement Mouvement du joueur
bool Damage(CTexturedMesh3* enemy, CTexturedMesh3* player, Vector3 mouvement){
	if((mouvement.y < 0.01) && (mouvement.y > -0.01)) {
		mouvement.y += 0.5;
		return player->CheckCollisionBox(enemy, mouvement);
	}
	return false;
}

/// \brief Classe représentant un ennemie.
///
/// Classe permettant de gerer les meshs et les mouvements de l'ennemie.
///
class Enemy : public CMTexturedMesh3{
protected:
    DoubleProgress* lifeBar; ///< Barre de vie de l'ennemie.
    Label* name; ///< Nom de l'ennemie.
	double movingSpeed; ///< Indique la vitesse de déplacement.
	CTexturedMesh3* ground; ///< Représente la map de collision.
	bool ifShowInfo; ///< Indique si les informations de l'ennemi seront affichés
	Vector3* playerPosition; ///< Pointeur sur la position du player.
	list<CTexturedMesh3*>* cMeshes; ///< Pointeur de la liste de mesh collisionnabel avec l'ennemi
	double attackRange; ///< Distance à laquelle l'ennemi peut attaquer le joueur.
	Chrono* chrono; ///< chrono de enemmi.
	uint attackDamage; ///< Dommage de l'ennemi sur le joueur lors d'une attaque
	bool dead; ///< Indique si l'ennemi est mort ou non.
	uint level; ///< Indique le niveau de l'ennemi
	map<const char*, uint> defense; ///< Indique la défense de l'ennemi selon le type.
	const char* damageType; ///< Indique le type de dommage de l'ennemi.
	bool (*pAttaque)(CTexturedMesh3*, CTexturedMesh3*, Vector3); ///< Pointeur de fonction sur la réussite d'une attaque envers l'ennemi
	map<const char*, uint> drops; ///< Contient les statistique de drop de l'ennemi
	bool enableDrops; ///< Indique si l'ennemi va drops ses items.
public:
    /// \param name Le nom de l'ennemie.
    /// \param position Position de l'ennemie.
    /// \param front Vecteur de face de l'ennemie.
    /// \param up Vecteur vers le haut de l'ennemie.
    Enemy(const char* name, Vector3 front, Vector3 up, Vector3* playerPosition, list<CTexturedMesh3*>* cMeshes, uint maxHP, double movingSpeed = 1.0, double scale = 1.0, double attackRange = 1.0, uint attackDamage = 1.0, const char* damageType = "physic") : CMTexturedMesh3(front, up){
		if(name == "Goomba")
			pAttaque = JumpDamage;
		else
			pAttaque = Damage;
    	dead = false;
    	level = rand() % 3 + 1;
    	enableDrops = true;
    	this->damageType = damageType;
    	this->attackDamage = attackDamage * level;
    	this->attackRange = attackRange;
    	this->cMeshes = cMeshes;
    	this->playerPosition = playerPosition;
    	this->movingSpeed = movingSpeed;
    	ifShowInfo = false;
        this->name = new Label({500, 6.0}, GetResource("comic18", Font), (string(name) + " Lv " + to_string(level)).c_str(), {255,255,255});;
		this->name->SetPosition(Vector2(((566 - this->name->GetWidth()) / 2) + 330, 6.0));
        lifeBar = new DoubleProgress({330.0,3.0}, {566.0, 31.8}, GetResource("lifeBarEnemy", Texture), maxHP * level, maxHP * level);
        lifeBar->SetRefPoint((0.9961389961 * 566.0) + 330.0, ((0.9707666851 * 566.0) + 330.0));
        string colorName = string(name) + "Color";
        Texture* tex = GetResource((colorName.c_str()), Texture);
        this->CopyMesh(GetResource(name, TexturedMesh3));
        this->LoadMeshInfo(GetResource(name, TexturedMesh3)->info);
        this->SetTextureID(tex->GetId());
		if(scale != 1.0) {
			Matrix44<double> size;
			size.LoadScale(scale);
			this->Transform(size);
			hitbox->Transform(size);
		}
		tex = GetResource("PineTexture", Texture);
		ground = new CTexturedMesh3();
		info = ground->LoadMesh(tex->GetId(), "resources/GroundMapV1.obj");
		ground->LoadMeshInfo(info);
		this->position = {0,0,0};
		hitbox->MoveTo({this->position});
		MoveTo(position);
		chrono = new Chrono();
    }

    ~Enemy(){
        delete lifeBar;
        delete name;
        delete ground;
        delete chrono;
    }

    /// \brief Obtient le vecteur front.
    /// \return Retourne le vecteur front.
    inline Vector3 GetFront(){         // A revoir si c'est utile
        return front;
    }
	
    /// \brief Ajoute les drops sur la liste de drops du monstres.
    /// \param argc Nombre de Drops * 2
    /// \param ... Item, suivi du poucentage en int
    void AddDrops(uint argc, ...) {
		va_list argv;
		va_start(argv, argc);
		for(uint i = 0; i < argc; i = i + 2) {
			const char* name = va_arg(argv, const char*);
			uint number = va_arg(argv, uint);
			drops[name] = number;
		}
		va_end(argv);
    }
	
	/// \brief Retroune un dictionnaire avec les drops et le pourcentage de chance.
	/// \return map contenant le nom des items drops et le nombre qui est dropper.
    map<const char*, uint> Drop() {
		map<const char *, uint> dropped;
		if(enableDrops) {
			uint chance;
			if(drops.size()) {
				for(auto it : drops) {
					uint numberDropped = 0;
					for(uint i = numberDropped; (i == numberDropped) && (i < 3); i++) {
						chance = rand() % 100;
						if(chance < it.second)
							numberDropped++;
					}
					if(numberDropped)
						dropped[it.first] = numberDropped;
				}
			}
			chance = rand() % 100;
			dropped["XP"] = chance * level;
		}
    	return dropped;
	}
    
	/// \brief Obtient la distance à laquelle l'ennemi peut attaquer.
	/// \return Retourne une distance en double.
	inline double GetAttackRange(){         // A revoir si c'est utile
		return attackRange;
	}
	
	/// \brief Obtient le type de dommage de l'ennemi.
	/// \return Retourne une distance en double.
	inline const char* GetDamageType(){
		return damageType;
	}
	
	/// \brief Envoie des dommages à l'ennemi
	/// \param damage Dommage envoyé à l'ennemi
	/// \param Type de dommage (Magique, Physique, ...)
	void ReceiveDamage(uint damage, const char* type = "physic"){
		if(!defense[type])
			defense[type] = 0;
		SetLife(GetHP() - (damage - defense[type]));
		if(!GetHP())
			dead = true;
	}
	
	/// \brief Regarde si l'ennemi peut recevoir les dommages
	/// \param damage Dommage envoyé à l'ennemi
	/// \param Type de dommage (Magique, Physique, ...)
	/// \return Si l'ennemi va recevoir des dommages ou non
	bool CheckDamage(CTexturedMesh3* player, Vector3 mouvement){
		CTexturedMesh3* tempPlayer = player;
		CTexturedMesh3* tempEnemy = this;
		return pAttaque(tempEnemy, tempPlayer, mouvement);
	}
    
    /// \brief Débute le mouvement vers le joueur
    /// L'ennemi se rapproche du joueur jusqu'à ce qu'il puisse l'attaquer
    void MoveAttack(){
        double deltaTime = DeltaTime::time;
		if(deltaTime > 0.1)
			deltaTime = 0.1;
		Vector3 right = front % up;
		right.Normalize();
		up = right % front;
		up.Normalize();
		Vector3 mouvement = *playerPosition - this->position;
		mouvement.Normalize();
		mouvement.y = 0;
		Vector3 pUP = (front % mouvement);
		pUP.Normalize();
		if((mouvement.x >= 0.05) || (mouvement.z >= 0.05)) {
			if(pUP == up) {
				if(mouvement.GetAngle(front, RADIAN) < 0.05)
					ApplyModelRotation(mouvement.GetAngle(front, RADIAN));
				else
					ApplyModelRotation(mouvement.GetAngle(front, RADIAN) * deltaTime);
			} else {
				if(mouvement.GetAngle(front, RADIAN) < 0.05)
					ApplyModelRotation(-mouvement.GetAngle(front, RADIAN));
				else
					ApplyModelRotation(-mouvement.GetAngle(front, RADIAN) * deltaTime);
			}
		}
		mouvement *= 0.5 * movingSpeed * deltaTime;
		Move(mouvement);
		chrono->Reset();
    }
	
	
	/// \brief Débute le combat avec le joueur
	/// L'ennemi commence à attaquer l'ennemi selon son attaque
	/// \return Dommage reçu par le joueur.
	virtual uint Attack() = 0;
    
    /// \brief Génère une positin aléatoire et déplace l'ennemi à cette position
    /// \param spot Endroit où faire apparaitre les ennemis.
    /// \param forceNewSet Force la méthode à trouver une nouvelle position
    void GenPosition(Vector3 spot, bool forceNewSet = true){
    	Vector3 movement;
    	while((movement.GetNorm() > generateMaxRange) || (forceNewSet)) {
			movement.x = rand() % ((2* generateMaxRange) + 1) - generateMaxRange;
			movement.z = rand() % ((2* generateMaxRange) + 1) - generateMaxRange;
			movement.y = 0;
			forceNewSet = false;
		}
		MoveTo(spot + movement);
		hitbox->MoveTo(spot + movement);
		double angle = (((double)(rand() % 201)) / 100) * 3.14159265358979323846;
		ApplyModelRotation(angle);
    }
	
	/// \brief Affichage des statistique de l'ennemi
    void DrawEnemyInfo(){
    	if(ifShowInfo) {
			lifeBar->Draw();
			name->Draw();
		}
    }

    /// \brief Fait bouger l'ennemie sans collision.
    /// \param mouvement Le mouvement que l'ennemi va faire
    void MoveNoCollision(Vector3 mouvement){
        MoveBy(mouvement);
        hitbox->MoveBy(mouvement);
    }
    /// \brief Bouge l'ennemi s'il n'y a pas de collisions.
    /// \param CMeshes liste des meshs pouvant entrer en collision avec l'ennemi.
    /// \param mouvement Vecteur de mouvement de l'ennemi.
    void Move(Vector3 mouvement){
        bool collision = false;
        for(auto it : *(this->cMeshes)){
            if(CheckCollisionSphere(it)){
                if(CheckCollisionBox(it, mouvement) || it->CheckCollisionBox(this, mouvement*-1))
                    collision = true;
            }
        }
        if(!collision){
            MoveNoCollision(mouvement);
            if(CheckCollisionFace(ground, {position.x, position.y + hitbox->minsize.y, position.z}, {0.0,1.0,0.0})){
                mouvement = {0.0,0.1,0.0};
                MoveNoCollision(mouvement);
            }
        }
    }

    /// \brief Bouge l'ennemi, s'il n'y a rien sous lui.
    void ApplyGravity(Vector3 mouvement){
        bool collision = false;
        if(speed.y <= 0 && CheckCollisionFace(ground, {position.x, position.y + hitbox->minsize.y, position.z}, {0.0,-1.0,0.0}))
            collision = true;
        if(collision)
            speed.y = 0.0;
        else{
            if(speed.y > maxFallingSpeed)
                speed.y -= gravityForce;
            MoveNoCollision(mouvement);
        }
    }
    
    /// \brief Échange la valeur du ifShowInfo
    /// Échange la valeur du booléen à savoir si les informations du monstres seront afficher.
    /// \param show Valeur du booléen pour l'affichage.
    inline void SwitchShowInfo(bool show){
    	ifShowInfo = show;
    }
    
    /// \brief Update la position de l'ennemie
    /// \param CMeshes liste des meshs pouvant entrer en collision avec l'ennemi.
    void Update(){
        double deltaTime = DeltaTime::time;
        Vector3 right = front%up;
        right /= right.GetNorm();
        up = right%front;
        up /= up.GetNorm();
        Vector3 mouvement = front * speed.z * movingSpeed * deltaTime;
        Move(mouvement);
        mouvement = right * speed.x * movingSpeed * deltaTime;
        Move(mouvement);
        mouvement = up * speed.y * movingSpeed * deltaTime;
        ApplyGravity(mouvement); // À régler
    }
    
    /// \brief Obtient les points de vie de l'ennemie
    /// \return Nombre de point de vie de l'ennemie
    inline ushort GetHP() const {
        return lifeBar->GetCount();
    }
	
	/// \brief Indique si l'ennemi est mort ou non
	/// \return La vie de l'ennemi (manquante ou non)
	inline bool GetDead(){
		return dead;
	}
    
    /// \brief Définie le niveau de vie.
    /// \param count Le montant de vie.
    inline void SetLife(short count){
        lifeBar->SetCount(count);
    }

    /// \brief Gère les évènements.
    void Notification(){}

};

#endif //PI2020_ENEMY_HPP
