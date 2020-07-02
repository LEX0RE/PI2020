#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "../Items/ItemDerivationFoward.hpp"
#include "PlayerInfo.hpp"

/// \brief Classe représentant le joueur.
///
/// Classe permettant de gerer les meshs du joueur, ses mouvements, les caméras, les stats, etc.
///
class Player : public CMTexturedMesh3{

    PlayerGui* gui;             ///< Contient tout ce qui est de l'interface graphique.
    PlayerState* state;         ///< Contient tout ce qui est de l'état du joueur.
    PlayerStatistics* stats;    ///< Contient tout ce qui est des statistiques.
    PlayerEquipement* equip;    ///< Contient l'équipement du joueur.
    FirstPersonCam* firstcam;   ///< Caméra 1e personne.
    ThirdPersonCam* thirdcam;   ///< Caméra 3e personne.
    CTexturedMesh3* ground;     ///< Représente la map de collision.

    /*
    bool state->w,state->a,state->s,state->d,state->e,state->q,state->f,state->space; ///< Indique si une touche est enfoncée.
    bool state->first; ///< Indique si la caméra est en mode 1e personne.
    double stats->movingSpeed; ///< Indique la vitesse de déplacement.
    double stats->rotationSpeed; ///> indique la vitesse de rotation.
    
    VisualProgress* gui->lifeBar; ///< Barre de vie du joueur.
    VisualProgress* gui->expBar; ///< Barre d'ex^périence du joueur.
    QuestManager* stats->questManager; ///< Gestinnaire de quête.
    uint state->animIndex; ///< Indice de l'animation qui sera en cours
    map<uint, TexturedMesh3*> state->sprites; ///< Dictionnaire contenant tout les sprite d'animations
    bool state->animStart; ///< Indique si une animation est lancé

    Item** playerInventory; ///< Inventaire qui va contenir tous les éléments du joueur

    Chrono* chrono; ///< chrono de player.
    
    Arrow* equip->arrow; ///< Pointeur vers la dernière flèche tiré par le joueur.
    HoldingItem equip->type; ///< Ce que le joueur tien dans sa main.
    Sword* equip->sword; ///< Épée du joueur.
    Bow* equip->bow; ///< Épée du joueur.
    uint stats->level; ///< Niveau du joueur.
    Label* gui->nameText,* gui->lifeText,* gui->levelText; ///< Informations du personnage : Nom, Point de vie, Niveau
    bool state->slope; ///< Indique si le joueur est en train de monter une pente.
    bool state->onGround; ///< Indique si le joueur touche au sol.
    uint stats->damage; ///< Dommage que le joueur peut faire
    */
public:

    /// \param front Vecteur de face du joueur
    /// \param up Vecteur vers le haut du joueur
    /// \param stats->movingSpeed Vitesse de déplacement du joueur.
    /// \param stats->rotationSpeed Vitesse de déplacement du joueur.
    Player(Vector3 front, Vector3 up, double movingSpeed, double rotationSpeed, double cameraRotationSpeed) : CMTexturedMesh3(front, up){
        
	    state = new PlayerState();
        stats = new PlayerStatistics(1, 1, movingSpeed, rotationSpeed);
	
	// items de départs: 
	    stats->AddItem(ItemManager::Get("potion50"), 1);
        stats->AddItem(ItemManager::Get("sword"), 2);

        // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
	
        //a changer pour le modèle du joueur
        Texture* tex = GetResource("AliceColor", Texture);
        MeshInfo* info;
        info = LoadMesh(tex->GetId(), "resources/AliceMarche_000010.obj");
        LoadMeshInfo(info);
        
		state->sprites[1] = new CTexturedMesh3(GetResource("AliceMarche", TexturedMesh3)->GetMaxFrame());
		((CTexturedMesh3*)state->sprites[1])->CopyMesh(GetResource("AliceMarche", TexturedMesh3));
        ((CTexturedMesh3*)state->sprites[1])->SetTextureID(tex->GetId());
		((CTexturedMesh3*)state->sprites[1])->SwitchAnimated();

        firstcam = new FirstPersonCam({0.0,0.0,0.0}, {0.0,0.0,5.0}, {0.0,1.0,0.0}, stats->movingSpeed, cameraRotationSpeed);
        thirdcam = new ThirdPersonCam({0.0,0.0,-5.0}, {0.0,0.0,0.0}, {0.0,1.0,0.0}, stats->movingSpeed, cameraRotationSpeed*10.0);

        tex = GetResource("grass", Texture);
        ground = new CTexturedMesh3();
        info = ground->LoadMesh(tex->GetId(), "resources/GroundMapV1.obj");
        ground->LoadMeshInfo(info);

        equip = new PlayerEquipement(new Arrow(front,up,{0.0,0.0},ground), new Sword(position), new Bow(position));
        equip->sword->MoveBy({0.18,0.01,0.25});
		Matrix44d rotation;
		rotation.LoadXRotation(-1);
		equip->sword->ApplyLocalRotation(rotation);
        equip->bow->MoveBy({0.5,0.0,1.0});
        rotation.LoadXRotation(4.7124);
        equip->bow->ApplyLocalRotation(rotation);
		
        gui = new PlayerGui(
            new VisualProgress({0.0,10.0}, {314.3, 18.0}, GetResource("lifeBarPlayer", Texture), 100, 100, (0.2945736434 * 279), {314.3,41.2}),
            new VisualProgress({0.0,749.0}, {1024.0, 20.0}, GetResource("xpBar", Texture), 1000, 0),
            new Label({35.0, 5.0}, GetResource("comic18", Font), "Alice", {255,255,255}, true),
            nullptr,
            nullptr
        );
        gui->lifeText = new Label({210.0, 28.0}, GetResource("comic10", Font), (to_string(gui->lifeBar->GetCount()) + "/" + to_string(gui->lifeBar->GetTotal())).c_str(), {255,255,255});
        gui->lifeBar->SetRefPoint((0.2945736434 * 314.3), ((0.2945736434 * 314.3) * 0.90));
        gui->levelText = new Label({275.0, 28.0}, GetResource("comic10", Font), ("LV: " + to_string(stats->level)).c_str(), {255,255,255});
		gui->lifeBar->SetPositionTexture(0.0, 3.0);
    }
    
    ~Player(){
    	for(uint i = 0, size = state->sprites.size(); i < size; i++)
    		delete state->sprites[i];
    }

    /// \brief Obtien le vecteur front.
    /// \return Retourne le vecteur front.
    inline Vector3 GetFront(){
        return front;
    }
    
    /// \brief Accède à la caméra de première personne.
    /// \return Retourne un pointeur de FirstPersonCam.
    inline FirstPersonCam* GetFirstCam() {
        return firstcam;
    }
    /// \brief Accède à la caméra de troisième personne.
    /// \return Retourne un pointeur de ThirdPersonCam.
    inline ThirdPersonCam* GetThirdCam() {
        return thirdcam;
    }

    /// \return Retourne le QuestManager
    inline QuestManager* GetQuest(){
        return stats->questManager;
    }

    /// \brief Affichage du personnage en 3e personne ou de l'arc/épée/etc en 1e personne
    void DrawPlayer(){
        if(state->first){
            firstcam->ApplyView();
        }
        else{
            thirdcam->ApplyView();
            if(!state->animIndex)
				Draw();
			else {
				((CTexturedMesh3 *) state->sprites[state->animIndex])->Draw();
				if(stats->chrono->InDelay(0.01)) {
					((CTexturedMesh3 *) state->sprites[state->animIndex])->LoopFrame();
					stats->chrono->Reset();
				}
			}
            if(DebugPanel::options[CHARACTER_BOUNDING_BOX])
                hitbox->Draw();
        }
        if(DebugPanel::options[GROUND_GRID])
            ground->Draw();

        equip->arrow->DrawArrow();
        if(equip->type == HoldingItem::SWORD)
            equip->sword->Draw();
        if(equip->type == HoldingItem::BOW)
            equip->bow->Draw();
    }

    /// \brief Ajoute un Item (s'il y a assez de place) dans l'inventaire.
	/// \param info Pointeur constant de ItemInfo. Celui-ci doit ABSOLUMENT provenir du ItemManager.
	/// \param count Nombre du même item ajouté à l'inventaire.
	/// \return Retourne un booléen où true signifie que l'item a bien été inséré. False signifie qu'il n'y a plus de place.
    inline void AddItem(const ItemInfo* info, ushort count) {
        stats->AddItem(info, count);
    }

    /// \brief Supprime un item de l'inventaire.
	/// \param index Case de l'inventaire à vider.
	/// \param count Quantité à supprimer de l'inventaire.
    inline void RemoveItem(Uint8 index, ushort count) {
        stats->RemoveItem(index, count);
    }

    /// \brief Obtient l'inventaire du joueur.
    /// \return Retourne un tableau de pointeur d'item. NE PAS SUPPRIMER.
    inline Item** GetItems() {
        return stats->inventory;
    }
	
	/// \brief Affichage les informations du personnage comme la vie et l'expérience
    void DrawInfo() {
		state->attackSword = false;
    	gui->lifeBar->Draw();
    	gui->expBar->Draw();
    	gui->nameText->Draw();
		gui->lifeText->Draw();
		gui->levelText->Draw();
    }

    /// \brief Sauvegarde l'état du joueur ainsi que de la partie.
    void SaveState() {
        /*
        P = Position du joueur
        state->w = Monde dans lequel le joueur était
        state->q = Quête
        O = Objectifs et state->s'ils sont complétés
        I = Item
        X = Expérience
        H = Points de vie.
        state->e = Fin.
        

        FILE* fptr = fopen("Save.txt", "state->w");
        //
        // Position/Monde
        fprintf(fptr, "P %state->f %state->f\n state->w %s\n", (float)position.x, (float)position.y, SceneManager::GetCurrentSceneName().c_str());

        //
        // Succès/Quêtes

        for(QuestManager::QuestIterator it = stats->questManager->GetFirst(), end = stats->questManager->GetLast(); it != end; it++) {
            fprintf(fptr, "%s %s\n", it->state->first.c_str());
            for(Quest::ObjectiveIterator it_oi = it->second->GetFirst(), end_oi = it->second->GetLast(); it_oi != end_oi; it_oi++) {
                bool completed = it_oi->second;
                fprintf(fptr, "\tO %s %i\n", it_oi->state->first.c_str(), (int)completed);
            }
        }

        //
        //Inventaire/Armure
        for(Uint8 i = 0; i < 18; i++) 
            //printf("I %i %i", this->playerInventory[i]->GetID(), this->playerInventory[i]->GetPosition());

        //
        //HP et XP
        fprintf(fptr, "X %i \nH %i", (int) gui->expBar->GetCount(), (int) gui->lifeBar->GetCount());
        fclose(fptr);
        */
    }

    /// \brief Fait sauter le joueur.
    void Jump(){
        speed.y = 1.2;
        state->onGround = false;
    }

    /// \brief Fait bouger le joueur sans collision.
    void MoveNoCollision(Vector3 mouvement){
        MoveBy(mouvement);
        thirdcam->Move(mouvement);
        firstcam->Move(mouvement);
        hitbox->MoveBy(mouvement);
        equip->sword->MoveBy(mouvement);
        equip->bow->MoveBy(mouvement);
        for(uint i = 1; i <= state->sprites.size(); i++)
            ((CTexturedMesh3*)state->sprites[i])->MoveTo(GetPosition());
    }

    /// \brief Bouge le joueur, ses caméras et sa hitbox state->s'il n'y a pas de collisions.
    /// \param CMeshes liste des meshs pouvant entrer en collision avec le joueur.
	/// \param enemyList liste des ennemis pouvant entrer en collision avec le joueur.
    /// \param mouvement Vecteur de mouvement du joueur.
    void Move(list<CTexturedMesh3*> CMeshes, list<CTexturedMesh3*> npcList, list<CMTexturedMesh3*> enemyList, Vector3 mouvement){
        if(mouvement.x != 0.0 || mouvement.y != 0.0 || mouvement.z != 0.0){
            bool collision = false;
            for(auto it : CMeshes){
                if(CheckCollisionSphere(it)){
                    if(CheckCollisionBox(it, mouvement) || it->CheckCollisionBox(this, mouvement*-1))
                        collision = true;
                }
            }
            for(auto it : npcList){
                if(CheckCollisionSphere(it)){
                    if(state->f)
                        ((NPC*)it)->interacted = true;
                }
            }
            if(!collision){
                Vector3 direction = CheckCollisionFaceH(ground, {position.x, position.y + info->minsize.y, position.z}, mouvement);
                //pente trop abrupte
                if(direction.y >= 0.1 && (abs(direction.x) < 0.4 || abs(direction.z) < 0.4)){
                    state->slope = true;
                    MoveNoCollision({0.0, 0.05, 0.0});
                }
                else{
                    state->slope = false;
                    mouvement.y += direction.y;
                    MoveNoCollision(mouvement);
                }
            }
            else{
                speed.x = 0;
                speed.z = 0;
            }
        }
    }

    /// \brief Bouge le joueur, si il n'y a rien sous lui.
	/// \param CMeshes liste des meshs pouvant entrer en collision avec le joueur.
	/// \param enemyList liste des ennemis pouvant entrer en collision avec le joueur.
	/// \param mouvement Vecteur de mouvement du joueur.
    void ApplyGravity(list<CTexturedMesh3*> CMeshes, list<CMTexturedMesh3*> enemyList, Vector3 mouvement){
        bool collision = false;
        //collision avec la map
        if(speed.y <= 0 && CheckCollisionFace(ground, {position.x, position.y + info->minsize.y, position.z}, {0.0,-1.0,0.0}))//mouvement?
            collision = true;
        //collision avec des meshs
        for(auto it : CMeshes){
            if(CheckCollisionSphere(it)){
                if(CheckCollisionBox(it, mouvement) || it->CheckCollisionBox(this, mouvement*-1)){
                    collision = true;
                    it->CollisionPlayerTop();
                }
            }
        }
		for(auto it : enemyList) {
			if((mouvement.y > 0.1) || (mouvement.y < -0.1)) {
				if(((Enemy*)it)->CheckDamage(this, mouvement)) {
					((Enemy*)it)->ReceiveDamage(stats->damage);
					speed.y = 1.2;
				}
			}
		}
        if(collision && speed.y < 0.0){
            speed.y = 0.0;
            state->onGround = true;
        }
        else{
            if(speed.y > maxFallingSpeed)
                speed.y -= gravityForce;
            if(speed.y != 0.0){
                MoveNoCollision(mouvement);
                state->onGround = false;
            }
        }
    }

    /// \brief Update la position du joueur et de sa caméra.
    /// \param CMeshes liste des meshs pouvant entrer en collision avec le joueur.
	/// \param enemyList liste des ennemis pouvant entrer en collision avec le joueur.
    void Update(list<CTexturedMesh3*> CMeshes, list<CTexturedMesh3*> npcList, list<CMTexturedMesh3*> enemyList){
        double deltaTime = DeltaTime::time;
        //bouge le mesh du joueur et des caméras
        if(state->space && state->onGround)
            Jump();
        Vector3 right = front%up;
        right /= right.GetNorm();
        up = right%front;
        up /= up.GetNorm();
        if(state->w && !state->slope){
            if(speed.z < 1)
                speed.z += 0.01;
        }
        if(state->s && !state->slope){
            if(speed.z > -1)
                speed.z -= 0.01;
        }
        if(state->a && !state->slope){
            if(speed.x > -1)
                speed.x -= 0.01;
        }
        if(state->d && !state->slope){
            if(speed.x < 1)
                speed.x += 0.01;
        }
        Vector3 mouvement;
        if(state->first){
            Vector3 front = firstcam->GetTarget() - firstcam->GetPosition();
            Vector3 right = front%firstcam->GetUp();
            mouvement = front * speed.z * stats->movingSpeed * deltaTime;
            Move(CMeshes, npcList, enemyList, mouvement);
            mouvement = right * speed.x * stats->movingSpeed * deltaTime;
            Move(CMeshes, npcList, enemyList, mouvement);
        }
        else{
            mouvement = front * speed.z * stats->movingSpeed * deltaTime;
            Move(CMeshes, npcList, enemyList, mouvement);
            mouvement = right * speed.x * stats->movingSpeed * deltaTime;
            Move(CMeshes, npcList, enemyList, mouvement);
        }
        if((!state->a && !state->d && !(speed.y < 0 && (speed.z > 0.1 || speed.x > 0.1))) || state->slope)
            speed.x *= slowingSpeed;
        if((!state->w && !state->s && !(speed.y < 0 && (speed.z > 0.1 || speed.x > 0.1))) || state->slope)
            speed.z *= slowingSpeed;
        if(state->q || state->e) {
            Matrix44d rotationY;
            if(state->q) {
                ApplyModelRotation(deltaTime * stats->rotationSpeed);
                rotationY.LoadYRotation(-deltaTime * stats->rotationSpeed);
                for(uint i = 1; i <= state->sprites.size(); i++)
                    ((CTexturedMesh3*) state->sprites[i])->ApplyLocalRotation(rotationY);
                Vector3 prevPos = equip->sword->GetPosition();
                equip->sword->MoveTo({0.5,0.0,1.0});
                equip->bow->MoveTo({0.5,0.0,1.0});
                equip->sword->Transform(rotationY);
                equip->bow->Transform(rotationY);
                equip->sword->MoveTo(prevPos);
                equip->bow->MoveTo(prevPos);
                if(turnCamWithPlayer) {
                    Vector3 distance = front * (-thirdcam->zoom + 5.0);
                    thirdcam->SetPosition(GetPosition() - distance);
                    thirdcam->SetUp(up);
                    thirdcam->SetTarget(GetPosition());
                }
            }
            else{
                ApplyModelRotation(-deltaTime * stats->rotationSpeed);
                rotationY.LoadYRotation(deltaTime * stats->rotationSpeed);
                for(uint i = 1; i <= state->sprites.size(); i++)
                    ((CTexturedMesh3 *) state->sprites[i])->ApplyLocalRotation(rotationY);
                Vector3 prevPos = equip->sword->GetPosition();
                equip->sword->MoveTo({0.5,0.0,1.0});
                equip->bow->MoveTo({0.5,0.0,1.0});
                equip->sword->Transform(rotationY);
                equip->bow->Transform(rotationY);
                equip->sword->MoveTo(prevPos);
                equip->bow->MoveTo(prevPos);
                if(turnCamWithPlayer) {
                    Vector3 distance = front * (-thirdcam->zoom + 5.0);
                    thirdcam->SetPosition(GetPosition() - distance);
                    thirdcam->SetUp(up);
                    thirdcam->SetTarget(GetPosition());
                }
            }
        }
        if(state->first){
            SDL_SetRelativeMouseMode(SDL_TRUE);
            Vector3 frontCam = firstcam->GetTarget() - firstcam->GetPosition();
            frontCam /= frontCam.GetNorm();
            Vector3 upCam = {0.0, 1.0, 0.0};
            Vector3 rightCam = frontCam%upCam;
            rightCam /= rightCam.GetNorm();
            Vector3 pos = {position.x + frontCam.x - rightCam.x/2, position.y + frontCam.y, position.z + frontCam.z};
            equip->sword->MoveTo(pos);
            equip->bow->MoveTo(pos);
        }
        else
            thirdcam->Update();
        mouvement = up * speed.y * stats->movingSpeed * deltaTime;
        ApplyGravity(CMeshes, enemyList, mouvement);

        equip->arrow->Update(CMeshes);
    }

    /// \brief Lance une flèche.
    void Shoot(){
		equip->arrow->damage = 1;
        Vector3 direction;
        if(state->first)
            direction = firstcam->GetTarget() - firstcam->GetPosition();
        else
            direction = front;
        direction /= direction.GetNorm();
        equip->arrow->Reset(direction);
        equip->arrow->MoveNoCollision(GetPosition()-equip->arrow->GetPosition());
        equip->arrow->SetSpeed({10.0, direction.y});
    }

    /// \brief Change entre premiere et troisieme personne.
    void SwitchCam(){
        if(state->first){
            SDL_SetRelativeMouseMode(SDL_FALSE);
            state->first = false;
            MoveTo(firstcam->GetPosition());
            hitbox->MoveTo(firstcam->GetPosition());
            thirdcam->SetTarget(firstcam->GetPosition());
            Vector3 distance = front*(-thirdcam->zoom+5.0);
            thirdcam->SetPosition(firstcam->GetPosition() - distance);
            front = {0.0,0.0,1.0};
            thirdcam->zoom = 0;
            thirdcam->SetUp({0.0,1.0,0.0});
        }
        else{
            state->first = true;
        }
    }
    
    /// \brief Obtient les points de vie du joueur
    /// \return Nombre de point de vie du joueur
    inline ushort GetHP(bool inPercentage = false){
        if (inPercentage)
            return gui->lifeBar->GetPercentage();
        else
            return gui->lifeBar->GetCount();
    }
	
	/// \brief Obtient les points d'expérience du joueur
	/// \return Nombre de point d'expérience du joueur
	inline ushort GetXP(bool inPercentage = false){
        if (inPercentage)
            return gui->expBar->GetPercentage();
        else
            return gui->expBar->GetCount();
	}
	
	/// \brief Obtient le dommage du joueur
	/// \return Nombre de point de dommage du joueur
	inline uint GetDamage(){
		return stats->damage;
	}
	
    /// \brief Gère les évènements.
    void Notification(){
        if(Event::GetType() == SDL_MOUSEBUTTONUP){
            if(Event::GetMouseButton() == SDL_BUTTON_LEFT){
            		switch(equip->type){
            			case HoldingItem::BOW:
							Shoot();
							break;
            			case HoldingItem::SWORD:
            				if(stats->couldown->InDelay(1.0)) {
								state->attackSword = true;
								stats->couldown->Reset();
							}
            				break;
            		}
			}
        }
        if(Event::GetType() == SDL_KEYDOWN){
            switch(Event::GetKey()){
            case 'w':
				state->animIndex = 1;
				if(!state->animStart) {
					state->animStart = true;
					stats->chrono->Reset();
				}
                state->w = true;
                break;
            case 's':
				state->animIndex = 1;
				if(!state->animStart) {
					state->animStart = true;
					stats->chrono->Reset();
				}
                state->s = true;
                break;
            case 'a':
				state->animIndex = 1;
				if(!state->animStart) {
					state->animStart = true;
					stats->chrono->Reset();
				}
                state->a = true;
                break;
            case 'd':
				state->animIndex = 1;
				if(!state->animStart) {
					state->animStart = true;
					stats->chrono->Reset();
				}
                state->d = true;
                break;
            case 'e':
                state->e = true;
                break;
            case 'q':
                state->q = true;
                break;
            case 'f':
                state->f = true;
                break;
            case 'c': //pour test, quand on reste pris, tombe sous la map, etc...
                MoveNoCollision({0.0, 10.0, 0.0});
                break;
            //state->space
            case 32:
                state->space = true;
                break;
            case 'g':
                SwitchCam();
                break;
            case 't':
                if(equip->type == HoldingItem::EMPTY)
                    equip->type = HoldingItem::SWORD;
                else if(equip->type == HoldingItem::SWORD){
                    equip->type = HoldingItem::BOW;
                    SwitchCam();
                }
                else{
                    equip->type = HoldingItem::EMPTY;
                    SwitchCam();
                }
                break;
            }
        }
        if(Event::GetType() == SDL_KEYUP){
            switch(Event::GetKey()){
            case 'w':
                state->w = false;
                break;
            case 's':
                state->s = false;
                break;
            case 'a':
                state->a = false;
                break;
            case 'd':
                state->d = false;
                break;
            case 'e':
                state->e = false;
                break;
            case 'q':
                state->q = false;
                break;
            case 'f':
                state->f = false;
                break;
            //state->space
            case 32:
                state->space = false;
                break;
            }
			if((!state->w) && (!state->s) && (!state->a) && (!state->d)) {
				for(uint i = 1; i <= state->sprites.size(); i++)
					((CTexturedMesh3*)state->sprites[i])->SetFrame(0);
				state->animIndex = state->animStart = 0;
			}
        }
    }

    /// \brief Retourne la flèche.
    Arrow* GetArrow(){
        return equip->arrow;
    }

    /// \brief Augmente le montant d'xp.
    /// \param count Le montant d'xp a ajouter.
    void IncreaseXP(unsigned short count){
		while((count + gui->expBar->GetCount()) >= gui->expBar->GetTotal()) {
			stats->level++;
			count = (count + gui->expBar->GetCount()) % gui->expBar->GetTotal();
			gui->expBar->SetCount(0);
		}
		gui->expBar->SetCount(gui->expBar->GetCount() + count);
		gui->levelText->SetText(("LV: " + to_string(stats->level)).c_str());
    }
    
    /// \brief Définie le montant d'xp.
    /// \param count Le montant d'xp.
    void SetXP(short count){
		while(count >= gui->expBar->GetTotal()) {
			stats->level++;
			count -= gui->expBar->GetTotal();
		}
		while(count < 0){
			if(-count >= gui->expBar->GetCount()){
				if(stats->level == 1)
					count = 0;
				else {
					stats->level--;
					count += gui->expBar->GetCount();
				}
			}
		}
        gui->expBar->SetCount(count);
		gui->levelText->SetText(("LV: " + to_string(stats->level)).c_str());
    }

    /// \brief Augmente le niveau de vie.
    /// \param count Le montant de vie a ajouter.
    void IncreaseLife(unsigned short count){
        gui->lifeBar->SetCount(gui->lifeBar->GetCount() + count);
		gui->lifeText->SetText((to_string(gui->lifeBar->GetCount()) + "/" + to_string(gui->lifeBar->GetTotal())).c_str());
    }

    /// \brief Définie le niveau de vie.
    /// \param count Le montant de vie.
    void SetLife(unsigned short count){
        gui->lifeBar->SetCount(count);
		gui->lifeText->SetText((to_string(gui->lifeBar->GetCount()) + "/" + to_string(gui->lifeBar->GetTotal())).c_str());
    }
    
    /// \brief Vérifie la distance avec tout les ennemis
    /// \param
    void CheckEnemyPositionForInfo(list<CMTexturedMesh3*> enemyList){
    	Enemy* closestEnemy = nullptr;
    	Vector3 distance;
    	uint closestDistance = monsterInfoShowDistance + 1;
    	for (auto it : enemyList){
			((Enemy*)it)->SwitchShowInfo(false);
			distance = (it)->GetPosition() - this->position;
			if(distance.x < 0)
				distance *= -1;
			
			// Si l'ennemi de l'itérateur est assez proche et est le plus proche
			if((distance.GetNorm() < monsterInfoShowDistance) && (distance.GetNorm() < closestDistance)){
				closestEnemy = (Enemy*)it;
				closestDistance = distance.GetNorm();
			}
			
			// Si l'ennemi est assez proche pour attaquer
			if((distance.GetNorm() < monsterAttackDistance) && (distance.GetNorm() > ((Enemy*)it)->GetAttackRange()))
				((Enemy*)it)->MoveAttack();
			
			// Si l'ennemi attaque selon sa distance d'attaque
			if((distance.GetNorm() < monsterAttackDistance) && (distance.GetNorm() <= ((Enemy*)it)->GetAttackRange())) {
				if(!stats->defense[((Enemy*)it)->GetDamageType()])
					stats->defense[((Enemy*)it)->GetDamageType()] = 0;
				SetLife(GetHP() - (((Enemy*)it)->Attack() - stats->defense[((Enemy*)it)->GetDamageType()]));
			}
			
			// Si le joueur attaque l'ennemi selon la distance d'attaque et tient une épée en main
			if((state->attackSword) && (distance.GetNorm() <= playerAttackRange))
				if(((Enemy*)it)->CheckDamage(this, front * playerAttackRange)) {
					((Enemy*)it)->ReceiveDamage(stats->damage);
				}
			
			// Si l'ennemi est attaquer par une flèche
			if(((Enemy*)it)->CheckDamage(equip->arrow, front)) {
				((Enemy*)it)->ReceiveDamage(equip->arrow->damage);
				equip->arrow->MoveNoCollision(GetPosition()-equip->arrow->GetPosition());
				equip->arrow->damage = 0;
			}
    	}
    	
    	// Si il y a un ennemi assez proche pour afficher ses stats
		if(closestDistance != (monsterInfoShowDistance + 1))
			if(closestEnemy)
				closestEnemy->SwitchShowInfo(true);
    }
};

#endif // PLAYER_HPP
