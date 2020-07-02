/// \file Tree.hpp
#ifndef PI2020_TREE_HPP
#define PI2020_TREE_HPP

#include "../Meshes/CollisionableMesh.hpp"
#include "../Resources/Texture.hpp"

/// \brief Classe représentant un arbre.
///
/// Classe permettant la création facile de ce modèle.
///
struct Tree : public CTexturedMesh3{
    /// \param position Position du modèle
    Tree(Vector3 position){
        Texture* tex = GetResource("PineTexture", Texture);
        this->CopyMesh(GetResource("PineTree", TexturedMesh3));
        this->LoadMeshInfo(GetResource("PineTree", TexturedMesh3)->info);
        this->SetTextureID(tex->GetId());
        this->MoveBy(position);
        this->hitbox->MoveBy(position);
    }
};
#endif //PI2020_TREE_HPP
