/// \file Bow.hpp
#ifndef BOW_HPP
#define BOW_HPP
#include "../Meshes/TexturedMesh.hpp"
#include "../Resources/Texture.hpp"

/// \brief Classe représentant un arc.
///
/// Classe permettant la création facile de ce modèle.
///
struct Bow : public TexturedMesh3{
    /// \param position Position du modèle
    Bow(Vector3 position){
        Texture* tex = GetResource("ShieldTexture", Texture);
        this->CopyMesh(GetResource("Bow", TexturedMesh3));
        this->SetTextureID(tex->GetId());
        this->MoveBy(position);
    }
};
#endif // BOW_HPP
