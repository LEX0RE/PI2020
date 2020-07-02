/// \file Chest.hpp
#ifndef CHEST_HPP
#define CHEST_HPP
#include "../Meshes/CollisionableMesh.hpp"
#include "../Resources/Texture.hpp"

/// \brief Classe représentant un coffre.
///
/// Classe permettant la création facile de ce modèle.
///
struct Chest : public CTexturedMesh3{
    /// \param position Position du modèle
    Chest(Vector3 position){
        Texture* tex = GetResource("ShieldTexture", Texture);
        this->CopyMesh(GetResource("ChestClosed", TexturedMesh3));
        this->LoadMeshInfo(GetResource("ChestClosed", TexturedMesh3)->info);
        this->SetTextureID(tex->GetId());
        this->MoveBy(position);
        this->hitbox->MoveBy(position);
    }
};

#endif // CHEST_HPP
