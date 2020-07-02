#ifndef COLLISIONABLEMESH_HPP
#define COLLISIONABLEMESH_HPP
#include "../Vector3.hpp"
#include "../Meshes/TexturedMesh.hpp"
#include "../Observer.hpp"
#include <cmath>

/// \brief Classe de mesh qui comporte des collisions.
///
/// Classe permettant de gerer les collisions.
///
typedef class CollisionableTexturedMesh : public TexturedMesh3{
    /// \brief Calcule le déterminant de 2 vecteurs 2D partant du même point
    /// \param p1x Point de départ des 2 vecteurs ( x )
    /// \param p1y Point de départ des 2 vecteurs ( y )
    /// \param p2x Point de fin du premier vecteur ( x )
    /// \param p2y Point de fin du premier vecteur ( y )
    /// \param p3x Point de fin du deuxième vecteur ( x )
    /// \param p3y Point de fin du deuxième vecteur ( y )
    /// \return Déterminant
    double Det (double p1x, double p1y, double p2x, double p2y, double p3x, double p3y) {
        return ((p1x - p3x) * (p2y - p3y)) - ((p2x - p3x) * (p1y - p3y));
    }

    /// \brief Calcule si un point est dans un triangle.
    /// \param px Point ( x )
    /// \param py Point ( y )
    /// \param t1x Premier point du triangle ( x )
    /// \param t1y Premier point du triangle ( y )
    /// \param t2x Deuxième point du triangle ( x )
    /// \param t2y Deuxième point du triangle ( y )
    /// \param t3x Troisième point du triangle ( x )
    /// \param t3y Troisième point du triangle ( y )
    /// \return Si le point est dans le triangle.
    bool IsInTriangle(double px, double py, double t1x, double t1y, double t2x, double t2y, double t3x, double t3y){
        //return (Det(t1x, t1y, t2x, t2y, px, py) && Det(t2x, t2y, t3x, t3y, px, py) && Det(t3x, t3y, t1x, t1y, px, py));
        double det1 = Det(t1x, t1y, t2x, t2y, px, py);
        double det2 = Det(t2x, t2y, t3x, t3y, px, py);
        double det3 = Det(t3x, t3y, t1x, t1y, px, py);
        return ((det1 >= 0 && det2 >= 0 && det3 >= 0) || (det1 <= 0 && det2 <= 0 && det3 <= 0));
    }

public:
    double radius; ///< Rayon du modèle.
    InvisibleMesh3* hitbox; ///< Boite de collision du modèle.

	/// \param maxFrame Nombre de frame total de l'animation.
	CollisionableTexturedMesh(uint maxFrame = 1) : TexturedMesh3(maxFrame){}
	
    virtual ~CollisionableTexturedMesh(){}

    /// \brief Charge l'information d'un mesh.
    /// \param info Information du mesh.
    void LoadMeshInfo(MeshInfo* info){
        radius = info->radius;
        hitbox = new InvisibleMesh3(position, info->minsize, info->size, maxFrame);
    }

    /// \brief Vérifie pour une collision avec les spheres de meshs.
    /// \param m Autre mesh avec laquelle on doit vérifier les collisions.
    /// \return S'il y a collision.
    bool CheckCollisionSphere(CollisionableTexturedMesh* m){
        Vector3 distance = m->GetPosition() - position;
        if(distance.x < 0)
            distance *= -1;
        return(distance.GetNorm()-m->radius-radius <= 0);
    }

    /// \brief Vérifie pour une collision avec les boites de meshs.
    /// \param m Autre mesh avec laquelle on doit vérifier les collisions.
    /// \param mouvement Vecteur du mouvement
    /// \return S'il y a collision.
    bool CheckCollisionBox(CollisionableTexturedMesh* m, Vector3 mouvement){
        //regarde si le mouvement, partant des 8 coins de la hitbox et du milieu, entre en collision avec un des triangles du mesh.
        Vector3 sourcePoint = {position.x, position.y + hitbox->minsize.y, position.z};
        Vector3 segment = mouvement;
        for(short i = 0; i < 8; i++){
            //fait en sorte que seulement les coins de droit de la boite seront pris pour les mouvements a droite, gauche vers la gauche, etc.
            //fix un bug qui fesait en sorte que, par exemple, en se déplacant vers la droite, le côté gauche de la boite entre en collision avec l'autre mesh.
            if((segment.x < segment.y && segment.x < segment.z && sourcePoint.x <= position.x) ||
               (segment.x > segment.y && segment.x > segment.z && sourcePoint.x >= position.x) ||
               (segment.z < segment.y && segment.z < segment.x && sourcePoint.z <= position.z) ||
               (segment.z > segment.y && segment.z > segment.x && sourcePoint.z >= position.z) ||
               (segment.y < segment.z && segment.y < segment.x && sourcePoint.y <= position.y) ||
               (segment.y > segment.z && segment.y > segment.x && sourcePoint.y >= position.y)){
                for(short j = 0; j < 11; j++){
                    Vector3 collisionPoint = m->hitbox->GetBoxTrianglePoint(j);
                    Vector3 normal = m->hitbox->GetBoxTriangleNormal(j);
                    //si le rayon partant du segment est pas parallèle au plan
                    if(normal*(segment) != 0.0){
                        //ratio  < 0 = mauvais sens, 0-1 = collision si le mouvement est exécuté, > 1 = collision bientot
                        double ratio = (normal*(collisionPoint-sourcePoint)/(normal*(segment)));
                        if(ratio >= 0 && ratio <= 1){
                            //collision vers la gauche/droite
                            if(abs(normal.x) > abs(normal.y) && abs(normal.x) > abs(normal.z)){
                                bool triangle = IsInTriangle((sourcePoint+(segment*ratio)).y, (sourcePoint+(segment*ratio)).z,
                                                            m->hitbox->GetBoxTrianglePoint(j,0).y, m->hitbox->GetBoxTrianglePoint(j,0).z,
                                                            m->hitbox->GetBoxTrianglePoint(j,1).y, m->hitbox->GetBoxTrianglePoint(j,1).z,
                                                            m->hitbox->GetBoxTrianglePoint(j,2).y, m->hitbox->GetBoxTrianglePoint(j,2).z);
                                if(triangle)
                                    return true;
                            }
                            //collision vers le haut/bas
                            else if(abs(normal.y) > abs(normal.x) && abs(normal.y) > abs(normal.z)){
                                bool triangle = IsInTriangle((sourcePoint+(segment*ratio)).x, (sourcePoint+(segment*ratio)).z,
                                                            m->hitbox->GetBoxTrianglePoint(j,0).x, m->hitbox->GetBoxTrianglePoint(j,0).z,
                                                            m->hitbox->GetBoxTrianglePoint(j,1).x, m->hitbox->GetBoxTrianglePoint(j,1).z,
                                                            m->hitbox->GetBoxTrianglePoint(j,2).x, m->hitbox->GetBoxTrianglePoint(j,2).z);
                                if(triangle)
                                    return true;
                            }
                            //collision vers l'avant/l'arriere
                            else if(abs(normal.z) > abs(normal.x) && abs(normal.z) > abs(normal.y)){
                                bool triangle = IsInTriangle((sourcePoint+(segment*ratio)).y, (sourcePoint+(segment*ratio)).x,
                                                            m->hitbox->GetBoxTrianglePoint(j,0).y, m->hitbox->GetBoxTrianglePoint(j,0).x,
                                                            m->hitbox->GetBoxTrianglePoint(j,1).y, m->hitbox->GetBoxTrianglePoint(j,1).x,
                                                            m->hitbox->GetBoxTrianglePoint(j,2).y, m->hitbox->GetBoxTrianglePoint(j,2).x);
                                if(triangle)
                                    return true;
                            }
                        }
                    }
                }
            }
            sourcePoint = hitbox->GetBoxPoint(i);
        }
        return false;
    }

    /// \brief Vérifie pour une collision avec les faces de meshs. ( Vertical )
    /// \param m Autre mesh avec laquelle on doit vérifier les collisions.
    /// \param mouvement Vecteur du mouvement
    /// \return S'il y a collision.
    bool CheckCollisionFace(CollisionableTexturedMesh* m, Vector3 sourcePoint, Vector3 mouvement){
        Vector3 segment = mouvement;
        double* mVertices = m->GetFrameVertices();
        double* mNormals = m->GetFrameNormals();
        for(uint j = 0; j < m->GetFrameVerticeCount() * 3; j+=9){
            Vector3 collisionPoint = {mVertices[j], mVertices[j + 1], mVertices[j + 2]};
            Vector3 normal = {mNormals[j], mNormals[j + 1], mNormals[j + 2]};
            //si le rayon partant du segment est pas parallèle au plan
            if(normal*(segment) != 0.0){
                //ratio  < 0 = mauvais sens, 0-1 = collision si le mouvement est exécuté, > 1 = collision bientot
                double ratio = (normal*(collisionPoint-sourcePoint)/(normal*(segment)));
                if(ratio >= 0 && ratio <= 1){
                    //collision vers le haut/bas
                    if(abs(normal.y) > abs(normal.x) && abs(normal.y) > abs(normal.z)){
                        Vector3 side1 = {mVertices[j], mVertices[j + 1], mVertices[j + 2]};
                        Vector3 side2 = {mVertices[j + 3], mVertices[j + 4], mVertices[j + 5]};
                        Vector3 side3 = {mVertices[j + 6], mVertices[j + 7], mVertices[j + 8]};
                        bool triangle = IsInTriangle((sourcePoint+(segment*ratio)).x, (sourcePoint+(segment*ratio)).z,
                                                    side1.x, side1.z,
                                                    side2.x, side2.z,
                                                    side3.x, side3.z);
                        if(triangle)
                            return true;
                    }
                }
            }
        }
        return false;
    }

    /// \brief Vérifie pour une collision avec les faces de meshs. ( Horitzontal )
    /// \param m Autre mesh avec laquelle on doit vérifier les collisions.
    /// \param mouvement Vecteur du mouvement
    /// \return S'il y a collision.
    Vector3 CheckCollisionFaceH(CollisionableTexturedMesh* m, Vector3 sourcePoint, Vector3 mouvement){
        Vector3 segment = mouvement;
        segment.y += 0.5;
        double* mVertices = m->GetFrameVertices();
        double* mNormals = m->GetFrameNormals();
        double closestRatio = 10000;
        for(uint j = 0; j < m->GetFrameVerticeCount() * 3; j+=9){
            Vector3 collisionPoint = {mVertices[j], mVertices[j + 1], mVertices[j + 2]};
            Vector3 normal = {mNormals[j], mNormals[j + 1], mNormals[j + 2]};
            //si le rayon partant du segment est pas parallèle au plan
            if(normal*(segment) != 0.0){
                double ratio = (normal*(collisionPoint-sourcePoint)/(normal*(segment)));
                if(ratio > 0 && ratio < closestRatio){
                    if(abs(normal.x) > abs(normal.y) && abs(normal.x) > abs(normal.z)){
                        Vector3 side1 = {mVertices[j], mVertices[j + 1], mVertices[j + 2]};
                        Vector3 side2 = {mVertices[j + 3], mVertices[j + 4], mVertices[j + 5]};
                        Vector3 side3 = {mVertices[j + 6], mVertices[j + 7], mVertices[j + 8]};
                        bool triangle = IsInTriangle((sourcePoint+(segment*ratio)).y, (sourcePoint+(segment*ratio)).z,
                                                    side1.y, side1.z,
                                                    side2.y, side2.z,
                                                    side3.y, side3.z);
                        if(triangle)
                            closestRatio = ratio;
                    }
                    if(abs(normal.z) > abs(normal.y) && abs(normal.z) > abs(normal.x)){
                        Vector3 side1 = {mVertices[j], mVertices[j + 1], mVertices[j + 2]};
                        Vector3 side2 = {mVertices[j + 3], mVertices[j + 4], mVertices[j + 5]};
                        Vector3 side3 = {mVertices[j + 6], mVertices[j + 7], mVertices[j + 8]};
                        bool triangle = IsInTriangle((sourcePoint+(segment*ratio)).x, (sourcePoint+(segment*ratio)).y,
                                                    side1.x, side1.y,
                                                    side2.x, side2.y,
                                                    side3.x, side3.y);
                        if(triangle)
                            closestRatio = ratio;
                    }
                    if(abs(normal.y) > abs(normal.x) && abs(normal.y) > abs(normal.z)){
                        Vector3 side1 = {mVertices[j], mVertices[j + 1], mVertices[j + 2]};
                        Vector3 side2 = {mVertices[j + 3], mVertices[j + 4], mVertices[j + 5]};
                        Vector3 side3 = {mVertices[j + 6], mVertices[j + 7], mVertices[j + 8]};
                        bool triangle = IsInTriangle((sourcePoint+(segment*ratio)).x, (sourcePoint+(segment*ratio)).z,
                                                    side1.x, side1.z,
                                                    side2.x, side2.z,
                                                    side3.x, side3.z);
                        if(triangle)
                            closestRatio = ratio;
                    }
                    //closestRatio = ratio;
                }
            }
        }
        if(closestRatio > 0 && closestRatio < 10000){
            Vector3 cp = sourcePoint+(segment*closestRatio);
            cp.y += 0.1;
            Vector3 distance = cp - sourcePoint;
            distance /= distance.GetNorm();
            distance.y = cp.y - sourcePoint.y;
            if(distance.y > 0)
                return distance;
        }
        return {1.0, 0.0, 1.0};
    }

    /// \brief Sert a update les meshs qui ont besoin d'être updaté comme les plateformes.
    virtual void Update(){}

    /// \brief Sert a informer un mesh qui a été en contact avec une flèche.
    virtual void CollisionArrow(){}

    /// \brief Sert a informer un mesh que le joueur a marcher dessus.
    virtual void CollisionPlayerTop(){}
} CTexturedMesh3;

/// \brief Classe de mesh qui bouge et comporte des collisions.
///
/// Classe permettant de bouger un mesh avec des touches et de gerer les collisions.
///
typedef class CollisionableMovingTexturedMesh : public Observer, public CollisionableTexturedMesh{
protected:
    Vector3 front; ///< Vecteur pointant vers l'avant du mesh.
    Vector3 up; ///< Vecteur pointant vers le haut du mesh.
    Vector3 speed; ///< Vitesse actuel du mesh sur les 3 axes.
public:
    /// \param front Vecteur pointant vers l'avant du mesh.
    /// \param up Vecteur pointant vers le haut du mesh.
	/// \param maxFrame Nombre de frame total de l'animation.
    CollisionableMovingTexturedMesh(Vector3 front, Vector3 up, uint maxFrame = 1) : CollisionableTexturedMesh(maxFrame){
        this->front = front;
        this->up = up;
        speed = {0.0,0.0,0.0};
    }

    virtual ~CollisionableMovingTexturedMesh(){}

    /// \brief Application de la matrice de rotation sur le modèle.
    /// \param angle Angle de rotation.
    void ApplyModelRotation(double angle){
        //rotation des vecteurs
        up = {0,1,0};
        Matrix44d rotation;
        rotation.LoadRotation(angle, up);
        rotation.ApplyRotation(front);

        //rotation du modèle et de la hitbox
        Matrix44d rotationY;
        rotationY.LoadYRotation(-angle);
        ApplyLocalRotation(rotationY);
        hitbox->ApplyLocalRotation(rotationY);
    }

    /// \brief Gère les évenements.
    virtual void Notification() = 0;
} CMTexturedMesh3;

#endif // COLLISIONABLEMESH_HPP
