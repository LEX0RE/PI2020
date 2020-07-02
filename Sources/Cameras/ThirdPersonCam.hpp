/// \file ThirdPersonCam.hpp
#ifndef THIRDPERSONCAM_HPP
#define THIRDPERSONCAM_HPP
#include "../Camera.hpp"
#include "../Meshes/MovingMesh.hpp"
#include "../Chrono.hpp"

#define mouseTurnButton SDL_BUTTON_RIGHT
#define turnPlayerWithCam false //a faire ?

/// \brief Classe de camera à la troisième personne.
///
/// Classe permettant de créer et d'utiliser une caméra à la troisième personne sans collisions.
///
struct ThirdPersonCam : public Camera{
    double sens; ///< Indique la vitesse de rotation.
    char zoom; ///< Indique le niveau de zoom ( 0 = default, negatif vers l'arriere, positif vers l'avant ).
    bool turning; ///< Indique si la souris est enfoncée.

    /// \param position Position de la caméra.
    /// \param target Le point que la caméra fixe ( Exemple: position du joueur ).
    /// \param up Vecteur représentant le haut de la caméra.
    /// \param speed Vitesse de déplacement de la caméra.
    /// \param sens Sensitivité de la rotation de la caméra.
    ThirdPersonCam(Vector3 position, Vector3 target, Vector3 up, double speed, double sens) : Camera(position, target, up) {
        turning = false;
        this->sens = sens;
        zoom = 0;
    }

    /// \brief Update la position de la caméra.
    /// \param mouvement Mouvement de la cible de la caméra.
    void Move(Vector3 mouvement){
        SetPosition(position + mouvement);
        SetTarget(target + mouvement);
    }

    /// \brief Définie le vecteur du haut de la caméra.
    /// \param v Vecteur du haut de la caméra.
    void SetUp(Vector3 v){
        up = v;
    }
	
	/// \brief Update la caméra en 3eme personne.
    void Update(){
        if(turning)
            SDL_SetRelativeMouseMode(SDL_TRUE);
        else
            SDL_SetRelativeMouseMode(SDL_FALSE);
    }

    /// \brief Gère les évenements.
    void Notification(){
        double deltaTime = DeltaTime::time;

        //tourner la caméra, zoom, etc
        if(Event::GetType() == SDL_MOUSEWHEEL){
            Vector3 front = target - position;
            front /= front.GetNorm();
            if(Event::GetScrollY() > 0 && zoom < 3){
                SetPosition(position + front);
                zoom++;
            }
            if(Event::GetScrollY() < 0 && zoom > -8){
                SetPosition(position - front);
                zoom--;
            }
        }
        if(Event::GetType() == SDL_MOUSEMOTION && turning){
            Vector3 front = target - position;
            front /= front.GetNorm();
            Vector3 right = front%up;
            right /= right.GetNorm();
            SDL_Point relativePosition = Event::GetMouseRelativePosition();
            up = {0.0,1.0,0.0};
            if(relativePosition.x != 0){
                SetPosition({position.x + (right.x*(double)relativePosition.x*sens*-deltaTime), position.y + (right.y*(double)relativePosition.x*sens*-deltaTime), position.z + (right.z*(double)relativePosition.x*sens*-deltaTime)});//a changer pour delta temps.
                ApplyView();
            }
            if(relativePosition.y != 0 && (front.y > -0.9 || relativePosition.y < 0) && (front.y < 0.9 || relativePosition.y > 0)){
                SetPosition({position.x + (up.x*(double)relativePosition.y*sens*deltaTime), position.y + (up.y*(double)relativePosition.y*sens*deltaTime), position.z + (up.z*(double)relativePosition.y*sens*deltaTime)});//a changer pour delta temps.
            }
        }
        if(Event::GetType() == SDL_MOUSEBUTTONDOWN){
            if(Event::GetMouseButton() == mouseTurnButton)
                turning = true;
        }
        if(Event::GetType() == SDL_MOUSEBUTTONUP){
            if(Event::GetMouseButton() == mouseTurnButton)
                turning = false;
        }
    }
};

#endif // THIRDPERSONCAM_HPP
