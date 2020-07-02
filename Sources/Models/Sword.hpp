/// \file Sword.hpp
#ifndef SWORD_HPP
#define SWORD_HPP
#include "../Meshes/TexturedMesh.hpp"
#include "../Resources/Texture.hpp"

/// \brief Classe représentant un épée.
///
/// Classe permettant la création facile de ce modèle.
///
struct Sword : public TexturedMesh3{
    /// \param position Position du modèle
    Sword(Vector3 position){
        Texture* tex = GetResource("SwordTexture", Texture);
        this->CopyMesh(GetResource("Sword", TexturedMesh3));
		Matrix44<double> size;
		size.LoadScale(0.5);
		this->Transform(size);
        this->SetTextureID(tex->GetId());
        this->MoveBy(position);
    }
};
#endif // SWORD_HPP
