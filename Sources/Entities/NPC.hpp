#ifndef PI2020_NPC_HPP
#define PI2020_NPC_HPP

#include "../Meshes/CollisionableMesh.hpp"
#include "../Resources/Texture.hpp"

/// \brief Classe représentant un NPC.
///
/// Classe permettant de gerer des NPC.
///
struct NPC : public CTexturedMesh3{
    const char* name; ///< Nom du NPC.
    bool interacted; ///< Si le joueur est en intéraction avec le NPC.
    bool gaveQuest; ///< Si le NPC a deja donné une quête au joueur.

    /// \param name Le nom du NPC.
    /// \param position Position du NPC.
    NPC(const char* name, Vector3 position) : CTexturedMesh3(){
        this->name = name;
        interacted = false;
        gaveQuest = false;
        Texture* tex = GetResource("AliceColor", Texture);
        this->CopyMesh(GetResource("AliceDefault", TexturedMesh3));
        this->LoadMeshInfo(GetResource("AliceDefault", TexturedMesh3)->info);
        this->SetTextureID(tex->GetId());
        this->MoveBy(position);
        this->hitbox->MoveBy(position);
    }

    /// \brief Si le NPC est près d'un autre mesh.
    /// \param m L'autre mesh.
    bool IfClose(CTexturedMesh3* m){
        return(CheckCollisionSphere(m));
    }


};
#endif //PI2020_NPC_HPP
