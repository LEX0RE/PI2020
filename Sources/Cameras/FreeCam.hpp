/// \file FreeCam.hpp
#ifndef FREECAM_HPP
#define FREECAM_HPP
#include "../Camera.hpp"
#include "../Event.hpp"
#include "../Vector3.hpp"

/// \brief Classe de camera libre.
///
/// Classe permettant de créer et d'utiliser une caméra libre sans collisions.
///
struct FreeCam : public Camera{
    bool origin; ///< Si true, la souris reste au point d'origine. Sinon, elle est libre.
    bool w,a,s,d; ///< Indique si une touche est enfoncée.
    double speed; ///< Indique la vitesse de déplacement.
    double sens; ///< Indique la vitesse de rotation.

    /// \param position Position de la caméra.
    /// \param target Le point que la caméra fixe ( Exemple: position du joueur ).
    /// \param up Vecteur représentant le haut de la caméra.
    /// \param speed Vitesse de déplacement de la caméra.
    /// \param sens Sensitivité de la rotation de la caméra.
    FreeCam(Vector3 position, Vector3 target, Vector3 up, double speed, double sens) : Camera(position, target, up) {
        origin = true;
        w = false; a = false; s = false; d = false;
        this->speed = speed;
        this->sens = sens;
    }

    /// \brief Update la position de la caméra.
    void Update(){
    	double deltaTime = SceneManager::mathChrono->GetTimeComplete();

        Vector3 front = target - position;
        front /= front.GetNorm();
        Vector3 right = front%up;
        right /= right.GetNorm();

        if(w){
            SetPosition(position + (front*speed*deltaTime));
            SetTarget(target + (front*speed*deltaTime));
        }
        if(s){
            SetPosition(position - (front*speed*deltaTime));
            SetTarget(target - (front*speed*deltaTime));
        }
        if(a){
            SetPosition(position - (right*speed*deltaTime));
            SetTarget(target - (right*speed*deltaTime));
        }
        if(d){
            SetPosition(position + (right*speed*deltaTime));
            SetTarget(target + (right*speed*deltaTime));
        }
    }

    /// \brief Gère les évenements.
    void Notification(){
    	double deltaTime = SceneManager::mathChrono->GetTimeComplete();
    	        	SceneManager::mathChrono->Reset();

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
            if(origin)
                SDL_SetRelativeMouseMode(SDL_TRUE);
            else
                SDL_SetRelativeMouseMode(SDL_FALSE);

            if(relativePosition.x != 0){
                Matrix44<double> rotationX;
                rotationX.LoadRotation((double)relativePosition.x*sens*-deltaTime, up);//a changer pour delta temps.
                ApplyCamRotation(rotationX);
            }
            if(relativePosition.y != 0){
                Vector3 front = target - position;
                front /= front.GetNorm();
                Vector3 right = front%up;
                right /= right.GetNorm();
                Matrix44<double> rotationY;
                rotationY.LoadRotation((double)relativePosition.y*sens*-deltaTime, right);//a changer pour delta temps.
                ApplyCamRotation(rotationY);
            }
        }
    }
};

#endif // FREECAM_HPP
