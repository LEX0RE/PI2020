/// \file FirstPersonCam.hpp
#ifndef FIRSTPERSONCAM_HPP
#define FIRSTPERSONCAM_HPP
#include "../Camera.hpp"
#include "../Event.hpp"
#include "../Vector3.hpp"
#include "../Matrix44.hpp"
#include "../GLContext.hpp"
#include "../Chrono.hpp"

/// \brief Classe de camera à la première personne.
///
/// Classe permettant de créer et d'utiliser une caméra à la première personne sans collisions.
///
struct FirstPersonCam : public Camera{
    bool origin; ///< Si true, la souris reste au point d'origine. Sinon, elle est libre.
    bool w,a,s,d; ///< Indique si une touche est enfoncée.
    double movingSpeed; ///< Indique la vitesse de déplacement.
    double sens; ///< Indique la vitesse de rotation.

    /// \param position Position de la caméra.
    /// \param target Le point que la caméra fixe ( Exemple: position du joueur ).
    /// \param up Vecteur représentant le haut de la caméra.
    /// \param movingSpeed Vitesse de déplacement de la caméra.
    /// \param sens Sensitivité de la rotation de la caméra.
    FirstPersonCam(Vector3 position, Vector3 target, Vector3 up, double movingSpeed, double sens) : Camera(position, target, up) {
        origin = true;
        w = false; a = false; s = false; d = false;
        this->movingSpeed = movingSpeed;
        this->sens = sens;
    }

    /// \brief Obtien le vecteur up.
    /// \return Vecteur up.
    inline Vector3 GetUp(){
        return up;
    }

    /// \brief Update la position de la caméra.
    /// \param mouvement Mouvement de la caméra.
    void Move(Vector3 mouvement){
        SetPosition(position + mouvement);
        SetTarget(target + mouvement);
    }

    /// \brief Update la position de la caméra.
    void Update(){
        double deltaTime = DeltaTime::time;

        if(origin)
            SDL_SetRelativeMouseMode(SDL_TRUE);
        else
            SDL_SetRelativeMouseMode(SDL_FALSE);

        Vector3 front = target - position;
        front /= front.GetNorm();
        Vector3 right = front%up;
        right /= right.GetNorm();

        if(w){
            SetPosition({position.x + (front.x*movingSpeed*deltaTime), position.y, position.z + (front.z*movingSpeed*deltaTime)});//a changer pour delta temps.
            SetTarget({target.x + (front.x*movingSpeed*deltaTime), target.y, target.z + (front.z*movingSpeed*deltaTime)});
        }
        if(s){
            SetPosition({position.x - (front.x*movingSpeed*deltaTime), position.y, position.z - (front.z*movingSpeed*deltaTime)});//a changer pour delta temps.
            SetTarget({target.x - (front.x*movingSpeed*deltaTime), target.y, target.z - (front.z*movingSpeed*deltaTime)});
        }
        if(a){
            SetPosition({position.x - (right.x*movingSpeed*deltaTime), position.y, position.z - (right.z*movingSpeed*deltaTime)});//a changer pour delta temps.
            SetTarget({target.x - (right.x*movingSpeed*deltaTime), target.y, target.z - (right.z*movingSpeed*deltaTime)});
       }
       if(d){
            SetPosition({position.x + (right.x*movingSpeed*deltaTime), position.y, position.z + (right.z*movingSpeed*deltaTime)});//a changer pour delta temps.
            SetTarget({target.x + (right.x*movingSpeed*deltaTime), target.y, target.z + (right.z*movingSpeed*deltaTime)});
       }
    }

    /// \brief Gère les évenements.
    void Notification(){
        double deltaTime = DeltaTime::time;

        if(Event::GetType() == SDL_KEYDOWN){
            switch(Event::GetKey()){
            case 'w':
                w = true;
                break;
            case 's':
                s = true;
                break;
            case 'a':
                a = true;
                break;
            case 'd':
                d = true;
                break;
            case 'o':
                origin = !origin;
                break;
            }
        }
        if(Event::GetType() == SDL_KEYUP){
            switch(Event::GetKey()){
            case 'w':
                w = false;
                break;
            case 's':
                s = false;
                break;
            case 'a':
                a = false;
                break;
            case 'd':
                d = false;
                break;
            }
        }
        if(Event::GetType() == SDL_MOUSEMOTION){
            SDL_Point relativePosition = Event::GetMouseRelativePosition();
            if(relativePosition.x != 0){
                Matrix44<double> rotationX;
                rotationX.LoadRotation((double)relativePosition.x*sens*-deltaTime, up);//a changer pour delta temps.
                ApplyCamRotation(rotationX);
            }
            if(relativePosition.y != 0){
                Vector3 front = target - position;
                front /= front.GetNorm();

                //Empêche de regarder trop haut ou trop bas et faire un 180
                if(front.z < 0.1 && front.z > 0 && up.y < 0.1){
                    front.z = 0.1;
                    target = front + position;
                }
                else if(front.z > -0.1 && front.z < 0 && up.y < 0.1){
                    front.z = -0.1;
                    target = front + position;
                }
                else{
                    front /= front.GetNorm();
                    Vector3 right = front%up;
                    right /= right.GetNorm();
                    Matrix44<double> rotationY;
                    rotationY.LoadRotation((double)relativePosition.y*sens*-deltaTime, right);//a changer pour delta temps.
                    ApplyCamRotation(rotationY);
                }
            }
        }
    }
};

#endif // FIRSTPERSONCAM_HPP
