/// \file Platform.hpp
#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include "../Meshes/CollisionableMesh.hpp"
#include "../Resources/Texture.hpp"
#include "../Chrono.hpp"

/// \brief Classe représentant une plateforme.
///
/// Classe permettant la création facile de ce modèle et la gestion de ses charactéristiques.
///
struct Platform : public CMTexturedMesh3{
    bool visible; ///< Indique si la plateforme est visible, ne désactive pas les collisions. ( Peut être utiliser pour faire un parcours avec les passages invisibles )
    uint repeat; ///< Après combien de mouvement la plateforme va tourner et recommencer.
    uint mouvements; ///< Mouvements dans la direction actuelle fait.

    /// \param position Position de la plateforme.
    /// \param speed Vitesse de la plateforme.
    /// \param repeat Après combien de mouvement la plateforme va tourner et recommencer.
    Platform(Vector3 position, Vector3 speed = {0.0, 0.0, 0.0}, uint repeat = 0) : CMTexturedMesh3({0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}){
        Texture* tex = GetResource("crateimg", Texture);
        this->CopyMesh(GetResource("crateobj", TexturedMesh3));
        this->LoadMeshInfo(GetResource("crateobj", TexturedMesh3)->info);
        this->SetTextureID(tex->GetId());
        MoveNoCollision(position);
        this->repeat = repeat;
        this->speed = speed;
        visible = true;
        mouvements = 0;
    }

    /// \brief Fait bouger la plateforme sans collision.
    void MoveNoCollision(Vector3 mouvement){
        MoveBy(mouvement);
        hitbox->MoveBy(mouvement);
    }

    /// \brief Étire la plateforme.
    /// \param v Vecteur d'étirement, 1 = pas d'étirement, < 1 = plus petit, > 1 = plus grand.
    void Stretch(Vector3 v){
        Matrix44d stretch;
        stretch.LoadStretch(v);
        ApplyLocalRotation(stretch);
        hitbox->ApplyLocalRotation(stretch);
        hitbox->size.x *= v.x; hitbox->size.y *= v.y; hitbox->size.z *= v.z;
        hitbox->minsize.x *= v.x; hitbox->minsize.y *= v.y; hitbox->minsize.z *= v.z;
        if(v.x >= v.y && v.x >= v.z)
            radius *= v.x;
        if(v.y >= v.x && v.y >= v.z)
            radius *= v.y;
        if(v.z >= v.y && v.z >= v.x)
            radius *= v.z;
    }

    /// \brief Change la vitesse.
    /// \param s Vitesse.
    void SetSpeed(Vector3 s){
        speed = s;
    }

    /// \brief Update la position de la plateforme.
    void Update(){
        double deltaTime = DeltaTime::time;

        if(++mouvements <= repeat){
            Vector3 right = front%up;
            right /= right.GetNorm();
            Vector3 mouvement = up * speed.y * deltaTime;
            MoveNoCollision(mouvement);
            mouvement = right * speed.x * deltaTime;
            MoveNoCollision(mouvement);
            mouvement = front * speed.z * deltaTime;
            MoveNoCollision(mouvement);
        }
        else{
            speed *= -1;
            mouvements = 0;
        }
    }

    /// \brief Affichage de l'objet.
    void Draw() {
        if(visible){
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_NORMAL_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);

            glVertexPointer(3, GL_DOUBLE, 0, vertices[frame]);
            glNormalPointer(GL_DOUBLE, 0, normals[frame]);
            glTexCoordPointer(2, GL_DOUBLE, 0, texCoord);
            glBindTexture(GL_TEXTURE_2D, texId);
            glDrawArrays(GL_TRIANGLES, 0, vertexCount);

            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_NORMAL_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
    }

    /// \brief Gère les évènements.
    void Notification(){}
};

#endif // PLATFORM_HPP
