/// \file Target.hpp
#ifndef TARGET_HPP
#define TARGET_HPP
#include "../Meshes/CollisionableMesh.hpp"
#include "../Resources/Texture.hpp"

/// \brief Classe représentant une cible.
///
/// Classe permettant la création facile de ce modèle.
///
struct Target : public CTexturedMesh3{
    void(*Action)(CTexturedMesh3*); ///< Pointeur de la fonction a appeller lorsque la cible est touché.
    CTexturedMesh3* activation; ///< Mesh qui va être affecté par la cible lorsqu'elle est touché.

    /// \param position Position du modèle
    /// \param Action Pointeur de la fonction a appeller lorsque la cible est touché.
    /// \param activation Pointeur vers le mesh qui va être affecté par la cible lorsqu'elle est touché.
    Target(Vector3 position, void(*Action)(CTexturedMesh3*) = nullptr, CTexturedMesh3* activation = nullptr){
        Texture* tex = GetResource("CibleColor", Texture);
        this->CopyMesh(GetResource("Cible", TexturedMesh3));
        this->LoadMeshInfo(GetResource("Cible", TexturedMesh3)->info);
        this->SetTextureID(tex->GetId());
        this->MoveBy(position);
        this->hitbox->MoveBy(position);
        this->Action = Action;
        this->activation = activation;
    }

    /// \brief Appelé lorsqu'une flèche entre en collision avec la cible.
    void CollisionArrow(){
        Action(activation);
    }

    double GetX(){
        return position.x;
    }

    double GetY(){
        return position.y;
    }

    double GetZ(){
        return position.z;
    }

    Vector3 GetPosition(){
        return position;
    }

};

#endif // TARGET_HPP
