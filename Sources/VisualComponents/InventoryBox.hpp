/// \file InventoryBox.hpp
#ifndef PI2020_INVENTORYBOX_HPP
#define PI2020_INVENTORYBOX_HPP

#include "../Typedef.hpp"
#include "../VisualComp.hpp"
#include "../Items/ItemManager.hpp"
#include "../Meshes/TexturedMesh.hpp"
#include "../Meshes/ColoredMesh.hpp"
#include "../Entities/Player.hpp"
#include "HintBox.hpp"
#include "Image.hpp"
#include "LoadingBar.hpp"

#define ITEM_SQUARE_SIZE Vector2(80, 80)
#define ITEM_SQUARE_POS(pos) Vector2(pos.x + 5, pos.y + 5)
#define ITEM_SQUARE_BORDER_SIZE Vector2(90, 90)
#define ITEM_SQUARE_COLOR {120, 120, 120, 180}
#define ITEM_SQUARE_SPACE 110.0
#define INVENTORY_SCENE_BACKGROUND GetResource("loading", Texture)
#define INVENTORY_SCENE_BACKGROUND_SIZE Vector2(1024, 768)
#define INVENTORY_BACKGROUND GetResource("InventoryBox1", Texture)
#define INVENTORY_BACKGROUND_POS Vector2(20.0, 100.0)
#define INVENTORY_BACKGROUND_SIZE Vector2(630.0, 630.0)
#define INVENTORY_CHARACTER_IMG GetResource("AliceBlack", Texture)
#define INVENTORY_CHARACTER_POS Vector2(720.0, 180.0)
#define INVENTORY_CHARACTER_SIZE Vector2(132, 293)
#define INVENTORY_QUITLABEL_FONT GetResource("endoralt50", Font)
#define INVENTORY_MAX_ITEM 18
#define INVENTORY_STORABLE_ITEM 16
#define INVENTORY_STORABLE_ITEM_X 4
#define INVENTORY_STORABLE_ITEM_Y 4
#define INVENTORY_STARTPOS_SQUARES Vector2(120.0, 200.0)
#define INVENTORY_STARTPOS_EQUIP Vector2(850.0, 180.0)
#define INVENTORY_STATBOX_BACKGROUND GetResource("InventoryBox2", Texture)
#define INVENTORY_STATBOX_POS Vector2(650.0, 100.0)
#define INVENTORY_STATBOX_SIZE Vector2(356.0, 630.0)

void InventoryControl(VisualComp* v);

typedef struct ItemSquare {
    Item* item;                         ///< Pointeur de l'item. 
    ColoredMesh2* box;                  ///< Couleur et carré de fond
    ColoredMesh2* selectionBorder;      ///< Couleur de sélection
    Vector2 position;                   ///< Position sur l'écran
    Image* icon;                        ///< îcone
    HintBox* hint;                      ///< Boîte d'information
    bool selected;                      ///< Indique si la case est sélectionnée

    /// \param position Position du carré à l'écran
    /// \param item Item assigné à la case.
    ItemSquare(Vector2 position, Item* item) {
        SDL_Color color = {255, 255, 255, 255};
        Vector2 vectorPosition = {position.x, position.y};
        this->box = new ColoredMesh2();
        this->selectionBorder = new ColoredMesh2();
        this->item = item;
        this->selected = false;
        this->selectionBorder->LoadRect(ITEM_SQUARE_COLOR, position, ITEM_SQUARE_BORDER_SIZE);
        this->box->LoadRect({180, 180, 180, 100}, ITEM_SQUARE_POS(position), ITEM_SQUARE_SIZE);
        this->hint = nullptr;
        this->icon = nullptr;
        this->selected = false;
        this->position = position;
        
        if(item) {
            this->icon = new Image(ITEM_SQUARE_POS(position), ITEM_SQUARE_SIZE, item->info->texture->GetId());
            this->hint = new HintBox();
            this->hint->AddText(item->info->name.c_str(), color, true);
            this->hint->AddText(item->info->description.c_str(), color, true);
        }
    }

    ~ItemSquare() {
        delete box;
        delete selectionBorder;
        delete hint;
    }
} * ItemSquare_p; //renommage de ItemSquare* par ItemSquare_p. 

///
/// \brief Classe gérant l'inventaire.
///
typedef struct InventoryGui : public VisualComp{
    ItemSquare_p * squares; ///< Tableau de Pointeurs d'ItemSquare.
    Player* player; ///< Pointeur du player.
    short* selected; ///< Tableau pour sauvegarder les indices de cases.

    /// \brief Réinitialise le tableau d'indice et désactive les contours de sélections.
    /// \param squareIndex Indice de la case
    void ResetSelection() {
        squares[selected[0]]->selected = false;
        squares[selected[1]]->selected = false;
        selected[0] = -1;
        selected[1] = -1;
    }

    /// \brief Vérifie si la sélection est vide.
    inline bool IsSelectionEmpty() {
        return ((selected[0] == -1) && (selected[1] == -1));
    }

    /// \brief Vérifie si la sélection a une case.
    inline bool IsSecondSelection() {
        return ((selected[0] != -1) && (selected[1] == -1));
    }

    /// \brief Réinisitalise une case de l'inventaire.
    /// \param squareIndex Indice de la case
    void ResetSquare(ushort squareIndex) {

        if(squares[squareIndex]->hint)
            delete squares[squareIndex]->hint;
        if(squares[squareIndex]->icon)
            delete squares[squareIndex]->icon;

        squares[squareIndex]->hint = nullptr;
        squares[squareIndex]->icon = nullptr;
        squares[squareIndex]->item = nullptr;
    }

    void Switch() {
        Item* i1 = squares[selected[0]]->item;
        Item* i2 = squares[selected[1]]->item;
        Vector2 v1 = squares[selected[0]]->position;
        Vector2 v2 = squares[selected[1]]->position;
        delete squares[selected[0]];
        delete squares[selected[1]]; 
        squares[selected[0]] = new ItemSquare(v1, i2);
        squares[selected[1]] = new ItemSquare(v2, i1);
        ResetSelection();
    }

     /// \brief Spécifie quoi faire lorsqu'une touche a été appuyée.
    void OnKeyUp() {
        uint key = Event::GetKey();
        switch(key) {

            case SDLK_TAB: {
                InventoryControl(nullptr);
                RefreshPlayerInventory();
                break;
            }


            case SDLK_r: {
                //TODO: Drop l'item sélectionné dans scene.
                if(selected[0] != -1) {
                	Scene3* scene = (Scene3*)SceneManager::GetStackedScene("World");
                    Vector3 position = ((Player*)scene->transit)->GetPosition();
                    scene->AddPickableItem(new ItemMesh3(squares[selected[0]]->item->info, 1), squares[selected[0]]->item->info, position);
                    squares[selected[0]]->item->count--;
                    if(squares[selected[0]]->item->count == 0) {
                        Vector2 position = squares[selected[0]]->position;
                        delete squares[selected[0]];
                        squares[selected[0]] = new ItemSquare(position, nullptr);
                    }
				}
                break;
            }
				
			case SDLK_u: {
               if(selected[0] != -1) {
                   squares[selected[0]]->item->Use();

                   if(squares[selected[0]]->item->used) { //si l'item a été utilisé
                       squares[selected[0]]->item->count--;
                       if(squares[selected[0]]->item->count == 0) { //s'il ne reste plus d'item, alors on vide la case.
                           Vector2 position = squares[selected[0]]->position;
                           delete squares[selected[0]];
                           squares[selected[0]] = new ItemSquare(position, nullptr);
                       }   
                   }
               }
               break;
            }
			
        }
    }

    /// \brief Spécifie quoi faire lorsqu'une touche de la souris a été appuyée.
    void OnClick() {
        SDL_Point pos = Event::GetMousePosition();
        
        for(Uint8 i = 0; i < INVENTORY_MAX_ITEM; i++) {
            if(squares[i]->box->IsInside({(double)pos.x, (double)pos.y})) {
                if(IsSelectionEmpty() && squares[i]->item) { //si rien n'est sélectionné
                    selected[0] = i;
                    squares[selected[0]]->selected = true;
                }
                else if(IsSecondSelection() && squares[selected[0]]->item) { //si une case a été sélectionné (donc on en sélectionne une 2e)
                    selected[1] = i;

                    if(selected[0] == selected[1]) //si la même case est sélectionné, alors on la désélectionne
                        ResetSelection();
                    
                    //si le déplacement de l'item par de l'inventaire et va dans l'équipement, il faut s'assurer que 
                    //l'item est équipable.
                    else if((selected[0] < INVENTORY_STORABLE_ITEM) && (selected[1] >= INVENTORY_STORABLE_ITEM)) {
                        if(squares[selected[0]]->item->info->equipable) 
                            Switch();
                        else 
                            ResetSelection();
                    }

                    //si le déplacement de l'item par de l'équipement et va dans l'inventaire, il faut s'assurer que 
                    //l'item est équipable.
                    else if((selected[0] >= INVENTORY_STORABLE_ITEM) && (selected[1] < INVENTORY_STORABLE_ITEM)) {
                        if(squares[selected[0]]->item) {
                            if(squares[selected[0]]->item->info->equipable) 
                                Switch();
                        }
                        else 
                            Switch();
                    }
                    else
                        Switch();
                }
                
                i = INVENTORY_MAX_ITEM;
            }
        }
    }

    /// \brief Spécifie quoi faire lorsque la souris se déplace.
    void OnMouseMotion() {
        SDL_Point pos = Event::GetMousePosition();
        Vector2 p = {(double)pos.x, (double)pos.y};
        for(ushort i = 0; i < INVENTORY_STORABLE_ITEM; i++) {
            if(squares[i]->item) {
                if(squares[i]->icon->IsInside(p)) {
                    squares[i]->hint->visible = true; // active l'infobulle de l'item
                    p.x += 20;
                    squares[i]->hint->SetPosition(p);
                }
                else 
                    squares[i]->hint->visible = false;
            }
        }
    }


public:
    /// \param player Pointeur du player.
    /// \param position 
    InventoryGui(Player* player, Vector2 position = INVENTORY_BACKGROUND_POS) : VisualComp(position, INVENTORY_BACKGROUND->GetSize()) {
        this->player = player;
        this->mesh = new TexturedMesh2();
        this->squares = new ItemSquare_p[INVENTORY_MAX_ITEM];
        this->selected = new short[2] {-1, -1};

        Vector2 startPos = INVENTORY_STARTPOS_SQUARES; //Assigne la position de référence des cases de l'inventaire.
        double startX = startPos.x; //Garde en mémoire la position initiale en X.
        ((TexturedMesh2*)this->mesh)->LoadRect(INVENTORY_BACKGROUND->GetId(), position, INVENTORY_BACKGROUND_SIZE);

        for(Uint8 y = 0, index = 0; y < INVENTORY_STORABLE_ITEM_Y; y++) {
            for(Uint8 x = 0; x < INVENTORY_STORABLE_ITEM_X; x++) {
                index = y * INVENTORY_STORABLE_ITEM_X + x;
                this->squares[index] = new ItemSquare(startPos, player->GetItems()[index]);
                startPos.x += ITEM_SQUARE_SPACE;
            }
            startPos.x = startX; //← ajuste le x à la valeur sauvegardée plus haut.
            startPos.y += ITEM_SQUARE_SPACE; 
        }

        //↓ place les cases de l'équipement. 
        startPos =  INVENTORY_STARTPOS_EQUIP;  
        for(Uint8 i = INVENTORY_STORABLE_ITEM; i < INVENTORY_MAX_ITEM; i++) {
            this->squares[i] = new ItemSquare(startPos, player->GetItems()[i]);
            startPos.y += 90.0;
        }

    }

    ~InventoryGui() {
        for(Uint8 i = 0; i < INVENTORY_MAX_ITEM; i++)
            delete squares[i];
        delete[] squares;
    }

    inline void Draw() {
        mesh->Draw();
        for(Uint8 i = INVENTORY_MAX_ITEM; 0 < i; --i) {
            i--;
            glDisable(GL_TEXTURE_2D);
            glEnable(GL_COLOR);
            squares[i]->box->Draw();
            if(squares[i]->selected)
                squares[i]->selectionBorder->Draw();
            glDisable(GL_COLOR);
            glEnable(GL_TEXTURE_2D);

            if(squares[i]->icon) 
                squares[i]->icon->Draw();

            if(squares[i]->hint) 
                squares[i]->hint->Draw();
            i++;
        }
        
    } 

    /// \brief Rafraichi l'inventaire du joueur.
    void RefreshPlayerInventory() {
        Item** items = player->GetItems();
        for(ushort i = 0; i < INVENTORY_MAX_ITEM; i++) 
            items[i] = squares[i]->item;
    }

    /// \brief Gère les évenements.
    void Notification() {
        uint event = Event::GetType();
        switch(event) {
            
            // lorsqu'une touche est appuyée.
            case SDL_KEYUP: {
                this->OnKeyUp();
                break;
            }

            case SDL_MOUSEBUTTONUP: {
                this->OnClick();
                break;
            }

            case SDL_MOUSEMOTION: {
                this->OnMouseMotion();
                break;
            }
        }
    }
} * InventoryGui_p;

#endif
