#ifndef ARROW_HPP
#define ARROW_HPP
#include "../Meshes/CollisionableMesh.hpp"
#include "../Resources/Texture.hpp"
#include "../Chrono.hpp"

#define showHitbox true
#define showGround false
//On pourrais mettre ca en variable plutot que define si on doit les changer, exemple pour faire de la glace qui dérape ou un puzzle avec de la gravité.
#define gravityForce 0.05 //0.0 = sans gravité, plus tu montes plus ca descends vite.
#define maxFallingSpeed -5.0

/// \brief Classe représentant une flèche.
///
/// Classe permettant de gerer le déplacement et les collisions d'une flèche.
///
class Arrow : public CMTexturedMesh3{
    CTexturedMesh3* ground; ///< Représente la map de collision.
    Vector2 speed; ///< Vitesse de la flèche. ( x = tout droit, y = haut/bas )
public:
    Vector3 prevPos; ///< Position de la flèche lors du dernier Update.
	uint damage; ///< Dommage de la flèche.

    /// \param front Vecteur de la flèche.
    /// \param up Vecteur vers de la flèche.
    /// \param currentSpeed Vitesse de déplacement initial de la flèche.
    /// \param ground Pointeur vers la map de collision de sol.
    Arrow(Vector3 front, Vector3 up, Vector2 currentSpeed, CTexturedMesh3* ground, uint damage = 1) : CMTexturedMesh3(front, up){
        this->speed = currentSpeed;
        this->ground = ground;
		this->damage = damage;
        Reset(front);
    }

    /// \brief Détermine la vitesse de la flèche
    /// \param s Vitesse
    void SetSpeed(Vector2 s){
        speed = s;
    }

    /// \brief Affichage de la flèche
    void DrawArrow(){
		if(damage) {
			if(showHitbox)
				hitbox->Draw();
			Draw();
		}
    }

    /// \brief Bouge la flèche et sa hitbox s'il n'y a pas de collisions.
    /// \param CMeshes liste des meshs pouvant entrer en collision avec la flèche.
    /// \param mouvement Vecteur de mouvement de la flèche.
    void Move(list<CTexturedMesh3*> CMeshes, Vector3 mouvement){
        bool collision = false;
        for(auto it : CMeshes){
            if(CheckCollisionSphere(it)){
                if(CheckCollisionBox(it, mouvement) || it->CheckCollisionBox(this, mouvement*-1)){
                    collision = true;
                    damage = 0;
                    it->CollisionArrow();
                }
            }
        }
        if(!collision){
            MoveBy(mouvement);
            hitbox->MoveBy(mouvement);
        }
    }

    /// \brief Rénitialise la flèche.
    /// \param direction Direction ou la flèche va pointer.
    void Reset(Vector3 direction){
        Texture* tex = GetResource("ShieldTexture", Texture);
        this->CopyMesh(GetResource("Arrow", TexturedMesh3));
        this->LoadMeshInfo(GetResource("Arrow", TexturedMesh3)->info);
        this->SetTextureID(tex->GetId());
        prevPos = this->position;
        if(direction.z > 0){
            Matrix44d rotation;
            rotation.LoadXRotation(3.141592);
            this->ApplyLocalRotation(rotation);
        }

        front = direction;
        up = {0.0, 1.0, 0.0};
        Vector3 directionX = {direction.x, 0.0, direction.z};
        directionX /= directionX.GetNorm();
        Vector3 straight;
        if(direction.z > 0)
            straight = {0.0,0.0,1.0};
        else
            straight = {0.0,0.0,-1.0};
        double angleX = directionX.GetAngle(straight, AngleUnit::RADIAN);
        Matrix44d rotationX;
        rotationX.LoadYRotation(angleX);
        this->ApplyLocalRotation(rotationX);
        this->hitbox->ApplyLocalRotation(rotationX);

        double angleY = directionX.GetAngle(direction, AngleUnit::RADIAN);
        if(direction.y < 0)
            angleY *= -1;
        Vector3 right = directionX%up;
        right.y = 0.0;
        Matrix44d rotationY;
        rotationY.LoadRotation(angleY*-1, right);
        this->ApplyLocalRotation(rotationY);
        this->hitbox->ApplyLocalRotation(rotationY);
    }

    /// \brief Fait bouger la flèche sans collision.
    void MoveNoCollision(Vector3 mouvement){
        MoveBy(mouvement);
        hitbox->MoveBy(mouvement);
    }

    /// \brief Fait descendre la flèche.
    void ApplyGravity(Vector3 mouvement){
        bool collision = false;
        if(speed.y <= 0 && CheckCollisionFace(ground, position, {0.0,-1.0,0.0}))
            collision = true;
        if(collision){
            speed.y = 0.0;
            speed.x = 0.0;
        }
        else{
            if(speed.y > maxFallingSpeed)
                speed.y -= gravityForce;
            MoveNoCollision(mouvement);
        }
    }

    /// \brief Update la position de la flèche.
    /// \param CMeshes liste des meshs pouvant entrer en collision avec la flèche.
    void Update(list<CTexturedMesh3*> CMeshes){
        double deltaTime = DeltaTime::time;

        Vector3 right = front%up;
        right /= right.GetNorm();
        up = right%front;
        up /= up.GetNorm();
        Vector3 mouvement = up * speed.y * deltaTime;
        this->ApplyGravity(mouvement);
        mouvement = front * speed.x * deltaTime;
        this->Move(CMeshes, mouvement);

        mouvement = position - prevPos;
        Vector3 straight = mouvement;
        straight.y = 0.0;
        double angle = (straight*mouvement)/4;
        Matrix44d rotationX;
        rotationX.LoadRotation(angle, right);
        if(angle > 0 && angle < 1){
            hitbox->ApplyLocalRotation(rotationX);
            ApplyLocalRotation(rotationX);
        }
        prevPos = position;
    }

    /// \brief Gère les évènements.
    void Notification(){}
};

#endif // ARROW_HPP
