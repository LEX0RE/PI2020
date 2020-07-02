/// \file SpringBoard.hpp
#ifndef SPRINGBOARD_HPP
#define SPRINGBOARD_HPP

#include "../Meshes/CollisionableMesh.hpp"
#include "../Resources/Texture.hpp"
#include "../Entities/Player.hpp"
#include "../SceneManager.hpp"

/// \brief Classe représentant un tremplin.
///
/// Classe permettant la création facile de ce modèle.
///
struct SpringBoard : public CTexturedMesh3{
    /// \param position Position du modèle
    SpringBoard(Vector3 position){
        Texture* tex = GetResource("crateimg", Texture);
        this->CopyMesh(GetResource("crateobj", TexturedMesh3));
        this->LoadMeshInfo(GetResource("crateobj", TexturedMesh3)->info);
        this->SetTextureID(tex->GetId());
        this->MoveBy(position);
        this->hitbox->MoveBy(position);

        Matrix44d stretch;
        stretch.LoadYStretch(0.2);
        ApplyLocalRotation(stretch);
        hitbox->ApplyLocalRotation(stretch);
        hitbox->size.y *= 0.1;
        hitbox->minsize.y *= 0.1;
    }

    /// \brief Appelé lorsque le joueur entre en collision avec le tremplin.
    void CollisionPlayerTop(){
        ((Player*)SceneManager::GetCurrentScene<Scene>()->transit)->Jump();
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

#endif // SPRINGBOARD_HPP
