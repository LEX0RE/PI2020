/// \file MovingMesh.hpp
#ifndef MOVINGMESH_HPP
#define MOVINGMESH_HPP
#include "TexturedMesh.hpp"
#include "../Observer.hpp"
#include "../Event.hpp"

/// \brief Classe de mesh qui bouge.
///
/// Classe permettant de bouger un mesh avec des touches.
///
typedef class MovingTexturedMesh : public Observer, public TexturedMesh3{
protected:
    Vector3 front; ///< Vecteur pointant vers l'avant du mesh.
    Vector3 up; ///< Vecteur pointant vers le haut du mesh.
public:
    /// \param front Vecteur pointant vers l'avant du mesh.
    /// \param up Vecteur pointant vers le haut du mesh.
	/// \param maxFrame Nombre de frame total de l'animation.
    MovingTexturedMesh(Vector3 front, Vector3 up, uint maxFrame = 1) : TexturedMesh3(maxFrame){
        this->front = front;
        this->up = up;
    }

    virtual ~MovingTexturedMesh(){}
    
    /// \brief Application de la matrice de rotation sur le modèle.
    /// \param rotation Matrice de Rotation.
    void ApplyModelRotation(Matrix44d rotation){
        up = {0,1,0};
        rotation.ApplyRotation(front);
    }

    /// \brief Gère les évenements.
    virtual void Notification() = 0;
} MTexturedMesh3;

#endif // MOVINGMESH_HPP
