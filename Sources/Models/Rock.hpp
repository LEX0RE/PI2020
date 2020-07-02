/// \file Rock.hpp
#ifndef PI2020_ROCK_HPP
#define PI2020_ROCK_HPP

#include "../Meshes/CollisionableMesh.hpp"
#include "../Resources/Texture.hpp"

/// \brief Classe représentant une roche.
///
/// Classe permettant la création facile de ce modèle.
///
struct Rock : public CTexturedMesh3{
    /// \param position Position du modèle
    Rock(Vector3 position){
        Texture* tex = GetResource("PineTexture", Texture);
        this->CopyMesh(GetResource("Rock1", TexturedMesh3));
        this->LoadMeshInfo(GetResource("Rock1", TexturedMesh3)->info);
        this->SetTextureID(tex->GetId());
        this->MoveBy(position);
        this->hitbox->MoveBy(position);
    }
};

#endif //PI2020_ROCK_HPP
