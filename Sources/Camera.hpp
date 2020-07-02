/// \file Camera.hpp
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SDL2/SDL_opengl.h>

#include "Observer.hpp"
#include "Vector3.hpp"
#include "Matrix44.hpp"

/// \brief Classe représentant une camera.
///
/// Classe représentant une caméra dans un environnement en 3 dimensions.
///
class Camera : public Observer{
protected:
    Vector3 position; ///< Position de la caméra
    Vector3 target; ///< Position de la cible
    Vector3 up; ///< Direction du haut de la caméra.

    Matrix44d view; ///< Matrice de vue.
    bool reloadView; ///< Rechargement de la matrice de vue.

    /// \brief Chargement de la matrice de vue.
    void LoadView(){
        Vector3 front = target - position;
        front /= front.GetNorm();
        Vector3 right = front % up;
        right /= right.GetNorm();
        up = right % front;
        up /= up.GetNorm();

        view.LoadView(front, right, up);
    }

public:
    /// \param position Position de la caméra
    /// \param target Position de la cible
    /// \param up Direction du haut de la caméra
    Camera(const Vector3& position, const Vector3& target, const Vector3& up){
        this->position = position;
        this->target = target;
        this->up = up;

        LoadView();
    }

    /// \brief Application de la matrice de rotation sur la caméra.
    /// \param rotation Matrice de Rotation.
    void ApplyCamRotation(Matrix44<double> rotation){
        up = {0,1,0};
        Vector3 front = target - position;
        rotation.ApplyRotation(front);
        target = front + position;
        reloadView = true;
        ApplyView();
    }

    /// \brief Application de la matrice de vue.
    void ApplyView() {
      if (reloadView)
        LoadView();

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glMultMatrixd(view.matrix);
      glTranslated(-position.x, -position.y, -position.z);
    }

    /// \brief Détermination de la position de la caméra.
    /// \param position Position de la caméra.
    void SetPosition(const Vector3& position){
        this->position = position;
        reloadView = true;
    }

    /// \brief Détermination de la position de la cible.
    /// \param target Position de la cible.
    void SetTarget(const Vector3& target){
        this->target = target;
        reloadView = true;
    }

    /// \brief Obtention de la position de la caméra.
    /// \return position de la caméra.
    inline Vector3 GetPosition(){
        return position;
    }

    /// \brief Obtention de la position de la cible.
    /// \return position de la cible.
    inline Vector3 GetTarget(){
        return target;
    }

    /// \brief Update la position de la caméra.
    virtual void Update() = 0;

    /// \brief Notification d'un évenement.
    virtual void Notification() = 0;
};

#endif
