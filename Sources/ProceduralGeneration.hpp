/// \file ProceduralGeneration.hpp
#ifndef PI2020_PROCEDURALGENERATION_HPP
#define PI2020_PROCEDURALGENERATION_HPP

#include "Models/Chest.hpp"
#include "Models/Tree.hpp"
#include "Models/Rock.hpp"
#include "Resources/Texture.hpp"
#include "Mesh.hpp"
#include <stdlib.h>
#include <time.h>
/// \brief Classe s'occupant de la génération procédurale.
///
/// Classe faisant la génération procédurale de végétaux, minéraux et buttins.
///
class ProceduralGeneration{
private:
    CTexturedMesh3* ground; ///< Représente la map de collision.
    list<Chest*> chestList;///< Représente une liste de coffres.
    list<Tree*> treeList;///< Représente une liste d'arbres.
    list<Rock*> rockList;///< Représente une liste d'arbres.
    Texture* tex;//< Représente un pointeur de texture.
    MeshInfo* info;//< Représente un pointeur MeshInfo.
    bool collision, collisionTree, collisionRock, collisionChest;//< Représente des variables booléennes pour les collisions d'arbres, de roches et de coffres.
    InvisibleMesh3* maskMesh;//< Représente la mesh invisible pour générer le décor.
    ushort treeCount, rockCount, chestCount;//< Représente le nombre d'arbre, de roche et de coffres.

public:
    /// \param treeCount Nombre d'arbres
    /// \param rockCount Nombre de roches
    /// \param chestCount Nombre de coffres
    /// \param position Position de la mesh invisible
    /// \param size Grandeur de la mesh invisible
ProceduralGeneration(ushort treeCount, ushort rockCount, ushort chestCount, Vector3 position, Vector3 size){
    this->rockCount = rockCount;
    this->treeCount = treeCount;
    this->chestCount = chestCount;

    srand(time(NULL));

    tex = GetResource("PineTexture", Texture);
    ground = new CTexturedMesh3();
    info = ground->LoadMesh(tex->GetId(), "resources/GroundMapV1.obj");
    ground->LoadMeshInfo(info);

    maskMesh = new InvisibleMesh3(position, {0.0, 0.0, 0.0}, size, 1);

    for (uint i = 0; i <= treeCount;i++){//génération procédurale d'arbres selon le MaskMesh
        Tree* tree = new Tree({maskMesh->GetPosition().x + rand() % (int)round(maskMesh->size.x),
                               30.0,
                               maskMesh->GetPosition().z + rand() % (int)round(maskMesh->size.z)});

            collision = false;
            while(!collision){ // tant qu'il n'y a pas de collision avec le sol
                if(tree->CheckCollisionFace(ground,tree->GetPosition(), {0.0,-1.0,0.0})) {
                    collision= true;
                }
                else{
                    tree->MoveBy({0.0,-1.0,0.0}); // on fait descendre de 1 en y jusqu'à ce que ça touche le sol
                    tree->hitbox->MoveBy({0.0,-1.0,0.0});
                }

                if(tree->GetPosition().y < -10)
                    collision = true;

            }

            collisionTree = false;
            if(treeList.size() != 0){ // si la liste d'arbres n'est pas vide
                for(auto it : treeList)
                    if(it->CheckCollisionSphere(tree)){ // si l'arbre est généré à un endroit où il y en a déjà un
                        collisionTree = true;
                        i--;
                    }
            }

            if (collisionTree == false) // s'il n'y a pas de colllisions, on push l'arbre dans la liste
                treeList.push_back(tree);

    }


    for (uint i = 0; i <= rockCount;i++){//génération procédurale de roches selon le MaskMesh
        Rock* rock = new Rock({maskMesh->GetPosition().x + rand() % (int)round(maskMesh->size.x),
                               30.0,
                               maskMesh->GetPosition().z + rand() % (int)round(maskMesh->size.z)});


        collision = false;
        while(!collision){ // tant qu'il n'y a pas de collision avec le sol
            if(rock->CheckCollisionFace(ground,rock->GetPosition(), {0.0,-1.0,0.0})) {
                collision= true;
            }
            else{
                rock->MoveBy({0.0,-1.0,0.0}); // on fait descendre de 1 en y jusqu'à ce que ça touche le sol
                rock->hitbox->MoveBy({0.0,-1.0,0.0});
            }

            if(rock->GetPosition().y < -10)
                collision = true;

        }

        collisionRock = false;
        if(rockList.size() != 0){ // si la liste de roches n'est pas vide
            for(auto it : rockList)
                if(it->CheckCollisionSphere(rock)){ // si la roche est généré à un endroit où il y en a déjà un
                    collisionRock = true;
                    i--;
                }
        }

        collisionTree = false;
        if(treeList.size() != 0){ // si la liste d'arbres n'est pas vide
            for(auto it : treeList)
                if(it->CheckCollisionSphere(rock)){ // si la roche est généré à un endroit où il y a deja un arbre
                    collisionTree = true;
                    i--;
                }
        }

        if (collisionTree == false and collisionRock == false) // s'il n'y a pas de colllisions, on push la roche dans la liste
            rockList.push_back(rock);
    }


    for (uint i = 0; i <= chestCount; i++){//génération procédurale de coffres selon le MaskMesh
        Chest* chest = new Chest({maskMesh->GetPosition().x + rand() % (int)round(maskMesh->size.x),
                               30.0,
                               maskMesh->GetPosition().z + rand() % (int)round(maskMesh->size.z)});

        collision = false;
        while(!collision){ // tant qu'il n'y a pas de collision avec le sol
            if(chest->CheckCollisionFace(ground,chest->GetPosition(), {0.0,-1.0,0.0})) {
                collision= true;
            }
            else{
                chest->MoveBy({0.0,-1.0,0.0}); // on fait descendre de 1 en y jusqu'à ce que ça touche le sol
                chest->hitbox->MoveBy({0.0,-1.0,0.0});
            }

            if(chest->GetPosition().y < -10)

                collision = true;
        }

        collisionChest = false;
        if(chestList.size() != 0){ // si la liste de coffres n'est pas vide
            for(auto it : chestList)
                if(it->CheckCollisionSphere(chest)){ // si le coffre est générée à un endroit où il y en a déjà un
                    collisionChest = true;
                    i--;
                }
        }

        collisionRock = false;
        if(rockList.size() != 0){ // si la liste de roches n'est pas vide
            for(auto it : rockList)
                if(it->CheckCollisionSphere(chest)){ // si le coffre est généré à un endroit où il y a une roche

                    collisionRock = true;
                    i--;
                }
        }

        collisionTree = false;
        if(treeList.size() != 0){ // si la liste d'arbres n'est pas vide
            for(auto it : treeList)
                if(it->CheckCollisionSphere(chest)){ // si le coffre est généré à un endroit où il y a deja un arbre
                    collisionTree = true;
                    i--;
                }
        }

        if (collisionTree == false and collisionRock == false and collisionChest == false) // s'il n'y a pas de collisions, on push le coffre dans la liste
            chestList.push_back(chest);

    }
}

    ~ProceduralGeneration(){
        delete ground, maskMesh, treeList, rockList, chestList;
    }

    list<Tree*> GetTreeList(){
        return treeList;
    }

    list<Rock*> GetRockList(){
        return rockList;
    }

    list<Chest*> GetChestList(){
        return chestList;
    }

void Draw(){ // procédure générant tout ce qu'il faut
    // Nous n'utiliserons pas cette méthode, la génération du décor se fera plutôt àpartir de la CMeshList dans World.hpp

    /* for(auto it : treeList){ // génération des arbres contenus dans la liste

        it->Draw();
    }

    for(auto it : rockList){ // génération de roches contenus dans la liste

        it->Draw();
    }

    for(auto it : chestList){ // génération de coffres contenus dans la liste

        it->Draw();
    }*/

  // maskMesh->Draw();
}

};



#endif //PI2020_PROCEDURALGENERATION_HPP







