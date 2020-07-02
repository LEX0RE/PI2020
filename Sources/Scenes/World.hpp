/// \file World.hpp
#ifndef WORLD_HPP
#define WORLD_HPP
#include "../VisualComponents/DebugPanel.hpp"
#include "../Items/Item.hpp"
#include "../Event.hpp"
#include "../GLContext.hpp"
#include "../Entities/Platform.hpp"
#include "../VisualComponents/Minimap.hpp"
#include "../Models/Chest.hpp"
#include "../Models/Tree.hpp"
#include "../Models/SpringBoard.hpp"
#include "../Quests/GUIandActions.hpp"
#include "../Quests/InventoryAndEquipment.hpp"
#include "../ProceduralGeneration.hpp"
#include "../Entities/Witch.hpp"
#include "../Entities/Goomba.hpp"
#include "../Entities/Awkward.hpp"
#include "../Entities/Zombie.hpp"
#include "../Atmosphere.hpp"

#define showWorldHitbox true
#define maxEnemyInRange 10

void OnKeyUp(VisualComp* v);
void InventoryControl(VisualComp* v);
void Pause(VisualComp* v);
void PickUpItemLabel_OnKeyUp(VisualComp* label);

void onTarget1(CTexturedMesh3* platform){
    ((Platform*)platform)->SetSpeed({0.0, 0.0, 2.0});
    ((Platform*)platform)->repeat = 400;
    ((Platform*)platform)->mouvements = 0;
}

void onLastTarget(CTexturedMesh3* null){
    Vector3 mouvement = {-200.0, 150.0, 200.0};
    mouvement -= ((Player*)SceneManager::GetCurrentScene<Scene>()->transit)->GetPosition();
    ((Player*)SceneManager::GetCurrentScene<Scene>()->transit)->MoveNoCollision(mouvement);
    //commence le donjon ? quand on tombe dans le volcan ?
}

/// \brief Classe pour la gestion du monde.
///
/// Classe contenant les éléments du monde.
///
class World : public Scene3{
    Atmosphere atmosphere;  ///< Pointeur vers la classe de l'atmosphère.
    ProceduralGeneration* proGen; ///< Pointeur vers la génération procédurale.
    DebugPanel debugPanel;  ///< Panneau de debuggage.
    Label* inventory;       ///< Pointeur vers la classe de l'inventaire.
    Label* pause;           ///< Permet d'activer le mode pause.
    Label* pickupItem;      ///< Permet de signaler au joueur qu'il peut ramasser des items.

    list<CTexturedMesh3*> CreateParcours(){
        list<CTexturedMesh3*> parcours;
        Platform* platform = new Platform({4.0, 6.0, 5.0});
        platform->Stretch({10.0, 0.2, 1.0});
        parcours.push_back(platform);

        platform = new Platform({-10.0, 7.0, 5.0});
        platform->Stretch({5.0, 0.2, 5.0});
        parcours.push_back(platform);

        Vector3 itemPos = {-10.0, 7.5, 5.0};
        AddPickableItem(new ItemMesh3(ItemManager::Get("potion50"), 1), ItemManager::Get("potion50"), itemPos);

        Target* target = new Target({-10.0, 10.0, 20.0}, onTarget1, platform);
        parcours.push_back(target);

        SpringBoard* sb = new SpringBoard({-17.0, 8.0, 17.0});
        parcours.push_back(sb);

        platform = new Platform({-22.0, 9.0, 17.0});
        platform->Stretch({3.0, 0.2, 3.0});
        parcours.push_back(platform);

        itemPos = {-22.0, 9.5, 17.0};
        AddPickableItem(new ItemMesh3(ItemManager::Get("potion50"), 1), ItemManager::Get("potion50"), itemPos);

        sb = new SpringBoard({-27.0, 9.0, 17.0});
        parcours.push_back(sb);

        platform = new Platform({-32.0, 10.0, 17.0});
        platform->Stretch({3.0, 0.2, 3.0});
        parcours.push_back(platform);

        target = new Target({-32.0, 13.0, 25.0}, onLastTarget, nullptr);
        parcours.push_back(target);
        return parcours;
    }
    
/// \brief Fait apparaitre les items drop par les ennemis à la position de leur mort
/// \param dropped Dictionnaire contenant la liste des items et le nombre de ceux ci qui seront dropper
/// \param position Position de l'ennemi
	void SpawnEnemyDrops(map<const char*, uint> dropped, Vector3 position){
    	if(dropped.size()) {
			for(auto it : dropped) {
				if(it.first != "XP")
					AddPickableItem(new ItemMesh3(ItemManager::Get(it.first), it.second), ItemManager::Get(it.first), position);
				else
					((Player*)transit)->IncreaseXP(it.second);
			}
		}
	}
public:
    /// \brief Execute le constructeur de la scène lorsqu'elle est chargée.
    /// \param param Paramètre de transit.
    /// \return Un pointeur vers la scène du monde.
    static inline Scene* LoadWorld(void* param) {
        return new World();
    }

    World() {
        GLContext::SetPerspective(90.0, 0.1, 1000.0);
        GLContext::SetOrthogonal();
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_TEXTURE_2D);

        GLfloat lightPosition[] = {40, 60, 20,1};
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

        AddObservable(6, SDL_KEYDOWN, SDL_KEYUP, SDL_MOUSEMOTION, SDL_MOUSEWHEEL, SDL_MOUSEBUTTONDOWN, SDL_MOUSEBUTTONUP);

        atmosphere.setRadius(1000);


        if (!this->transit)
            this->transit = (void*) new Player({0.0,0.0,1.0}, {0.0,1.0,0.0}, 5.0, 1.0, 0.05);
        inventory = new Label({10.0, 25.0}, GetResource("endoralt50", Font), "[tab] Inventory", {0, 0, 0, 255});
        pause = new Label({10.0, 70.0}, GetResource("endoralt50", Font), "[esc] Pause", {0, 0, 0, 255});
        pickupItem = new Label({10.0, 115}, GetResource("endoralt50", Font), "[ p ] Pick up Item", {0, 0, 0, 255});
        Label* xpos = new Label({10.0, 155}, GetResource("endoralt50", Font), std::to_string(((Player*)transit)->GetPosition().x).c_str(), {0, 0, 0, 255});
        Label* ypos = new Label({10.0, 200}, GetResource("endoralt50", Font), std::to_string(((Player*)transit)->GetPosition().y).c_str(), {0, 0, 0, 255});
        Label* zpos = new Label({10.0, 200}, GetResource("endoralt50", Font), std::to_string(((Player*)transit)->GetPosition().z).c_str(), {0, 0, 0, 255});

        pickupItem->visible = false; //on veut que ce label soit affiché seulement lorsque le joueur touche à un item.
        pickupItem->Add(SDL_KEYUP, PickUpItemLabel_OnKeyUp);

        Texture* tex = GetResource("grass", Texture);
        TexturedMesh3* map = new TexturedMesh3();
        map->CopyMesh(GetResource("MainMap", TexturedMesh3));
        map->SetTextureID(tex->GetId());
        tMeshList.push_back(map);
        map->CopyMesh(GetResource("Volcano", TexturedMesh3));
        Matrix44d scale;
        scale.LoadScale(0.5);
        map->ApplyLocalRotation(scale);
        map->MoveTo({-200.0, 5.0, 200.0});
        tMeshList.push_back(map);

        tMeshList.push_back(GetResource("WaterPlane", TexturedMesh3));

        ((Player*)transit)->MoveNoCollision({0.0,12.0,0.0});
	    
	    
	    // Gaël: Pas besoin d'itérer sur une fonction qui retourne une liste!
        // On a qu'à l'assigné et hop le tour est joué!
	    
        /*for(auto it : CreateParcours()){
            cMeshList.push_back(it);
        }*/
        cMeshList = CreateParcours();

        proGen = new ProceduralGeneration(80, 80, 80, {30.0, 30.0, -80.0}, {200.0, 30.0, 200.0});

        for (auto it : proGen->GetTreeList())
            cMeshList.push_back(it);

        for (auto it : proGen->GetRockList())
            cMeshList.push_back(it);

        for (auto it : proGen->GetChestList())
            cMeshList.push_back(it);

        NPC* guiAndActionsNPC = new NPC("Bob", {5.0,6.0,10.0});
        npcList.push_back(guiAndActionsNPC);

        NPC* inventoryAndEquipmentNPC = new NPC("Gerald", {15.0,9.0,10.0});
        npcList.push_back(inventoryAndEquipmentNPC);

        for(uint i = 0; i < maxEnemyInRange; i++) {
			Awkward* awkwards = new Awkward(((Player*)transit)->GetPointPosition(), &cMeshList);
			awkwards->GenPosition(GetPlayerPosition());
			enemyList.push_back(awkwards);
		}

        


        AddOrtho(6,
            inventory,
            pause,
            new Minimap({20, 570}, {160, 160}, ((Player*)transit), GetResource("worldMap", Texture)),
            xpos,
            ypos,
            zpos
        );

        //Créer les NPC
        /*
        //Créer les ennemies
		for(uint i = 0; i < maxEnemyInRange; i++) {
			Goomba* goomba = new Goomba(((Player*)transit)->GetPointPosition(), &cMeshList);
			goomba->GenPosition(GetPlayerPosition());
			enemyList.push_back(goomba);
		}
		for(uint i = 0; i < maxEnemyInRange; i++) {
			Witch* witch = new Witch(((Player*)transit)->GetPointPosition(), &cMeshList);
			witch->GenPosition(GetPlayerPosition());
			enemyList.push_back(witch);
		}
		for(uint i = 0; i < maxEnemyInRange; i++) {
			Zombie* zombie = new Zombie(((Player*)transit)->GetPointPosition(), &cMeshList);
			zombie->GenPosition(GetPlayerPosition());
			enemyList.push_back(zombie);
		}*/
		
		
        AddSubscription(SDL_KEYUP, 1, inventory);

        pause->Add(SDL_KEYDOWN, Pause);
        inventory->Add(SDL_KEYUP, InventoryControl); // La fonction OnKeyUp doit être à la fin de cette scène.
        //((Player*)transit)->AddItem(4);
        //((Player*)transit)->AddItem(1); //En attendant...
        events[SDL_KEYDOWN]->Subscribe(3, pause, ((Player*)transit), ((Player*)transit)->GetFirstCam());
        events[SDL_KEYUP]->Subscribe(5, ((Player*)transit), ((Player*)transit)->GetFirstCam(), inventory, &debugPanel, pickupItem);
        events[SDL_MOUSEMOTION]->Subscribe(2, ((Player*)transit)->GetFirstCam(), ((Player*)transit)->GetThirdCam());
        events[SDL_MOUSEWHEEL]->Subscribe(1, ((Player*)transit)->GetThirdCam());
        events[SDL_MOUSEBUTTONDOWN]->Subscribe(1, ((Player*)transit)->GetThirdCam());
        events[SDL_MOUSEBUTTONUP]->Subscribe(3, ((Player*)transit), ((Player*)transit)->GetThirdCam(), &debugPanel);
    }

    ~World(){
        delete proGen;
        delete inventory;
        delete pickupItem;
    }

    /// \brief Affiche tout les éléments de la scène dans la fenêtre.
    void Draw(){
	    Enemy* enemy = nullptr;
	    for(list<CMTexturedMesh3*>::iterator it = enemyList.begin(), end = enemyList.end(); it != end; it++) {
			enemy = (Enemy *) *it;
			if(enemy->GetDead()) {
				SpawnEnemyDrops(enemy->Drop(), enemy->GetPosition());
				list<CMTexturedMesh3 *>::iterator toDelete = it;
				delete (*toDelete);
				it = enemyList.erase(toDelete);
			} else
				enemy->Update();
		}
	    ((Player*)transit)->CheckEnemyPositionForInfo(enemyList);
        GLContext::ApplyPerspective();
        ((Player*)transit)->Update(cMeshList, npcList, enemyList);
        ((Player*)transit)->DrawPlayer();
        
        atmosphere.Draw();
        // proGen->Draw();

        for(auto it : tMeshList){
            it->Draw();
        }

        for(auto it : cMeshList){
            it->Draw();
            it->Update();
        }
        
        NPC* npc = nullptr;
        for(auto it : npcList){
            npc = (NPC*) it;
            npc->Draw();
            if(DebugPanel::options[NPCS_BOUNDING_BOX])
                npc->hitbox->Draw();
        }

        for(auto it : enemyList){
            enemy = (Enemy*) it;
            enemy->Draw();
            if(enemy->GetAttackRange() >= 10.0)
                ((Witch*)enemy)->DrawFireball();
            if(DebugPanel::options[ENEMIES_BOUNDING_BOX])
                enemy->hitbox->Draw();
        }


        for(std::map<ushort, CTexturedMesh3*>::iterator it = this->item.begin(), end = item.end(); it != end; it++) {
            if(it->second)
                it->second->Draw();
        }
        
        //commence une quête, peut probablement être mieux fait. Plus pour test en ce moment.
        for(auto it : npcList){
            npc = (NPC*) it;
            if(npc->interacted && !npc->gaveQuest){
                if(strcmp(npc->name, "Bob") == 0){
                    GUIandActions* guiandActions = new GUIandActions("GUIandActions", npc);
                    events[SDL_KEYDOWN]->Subscribe(1, guiandActions);
                    events[SDL_MOUSEBUTTONDOWN]->Subscribe(1, guiandActions);
                    *(((Player*)transit)->GetQuest())+=guiandActions;
                    npc->interacted = false;
                    npc->gaveQuest = true;
                }

                if(strcmp(npc->name, "Gerald") == 0){
                	InventoryAndEquipment* inventoryAndEquipment = new InventoryAndEquipment("InventoryAndEquipment", npc, (Player*)transit);
                    *(((Player*)transit)->GetQuest()) += inventoryAndEquipment;
                    AddSubscription(SDL_KEYDOWN, 1, inventoryAndEquipment);
                	npc->interacted = false;
                	npc->gaveQuest = true;
                }
            }
        }
        ((Player*)transit)->GetQuest()->Draw();

        GLContext::ApplyOrthogonal();
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);

        pickupItem->visible = this->IsTouchingItem((Player*)transit);
        if(pickupItem->visible) 
            pickupItem->Draw();

        for(auto it : ortho)
            it.second->Draw();

        ((Player*)transit)->DrawInfo();

        for(auto it : enemyList)
		((Enemy*)it)->DrawEnemyInfo();

        debugPanel.Draw();

        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);

    }
    /// \brief Obtiens la position du joueur.
    /// \return Vecteur 3 dimensions.
    inline Vector3 GetPlayerPosition() {
        return ((Player*)transit)->GetPosition();
    }

    /// \brief Indique si le label indiquant qu'un item peut être ramassé est visible ou non.
    /// \return true = oui, false = non.
    inline bool IsPUILabelVisible() {
        return pickupItem->visible;
    }
};

/// \brief Ajoute l'item ramassé au player.
/// \param label Composant visuel.
void PickUpItemLabel_OnKeyUp(VisualComp* label) {
    World* scene = SceneManager::GetCurrentScene<World>();
    Player* player = (Player*) scene->transit;

    if(scene->IsPUILabelVisible() && (Event::GetKey() == SDLK_p)) {
        queue<ushort> q = scene->TouchedItems((CMTexturedMesh3*)scene->transit);
        ushort count = q.size();
        for(ushort i = 0; i < count; i++) {
            ushort index = q.front();
            ushort amount = scene->GetItemMesh(index)->count;
            player->AddItem(scene->PickItem(index), amount);
        } 
    }
}

void Pause(VisualComp* v){
    if(Event::GetType() == SDL_KEYDOWN && Event::GetKey() == 27)
        SceneManager::GetCurrentScene<World>()->NotifySceneManager(PUSH, "Pause");
}

#endif // WORLD_HPP
