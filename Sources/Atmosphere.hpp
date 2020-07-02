/// \file Atmosphere.hpp
#ifndef PI2020_ATMOSPHERE_HPP
#define PI2020_ATMOSPHERE_HPP

#include "Meshes/TexturedMesh.hpp"
#include "Resources/Texture.hpp"
#include "Matrix44.hpp"

/// \brief Classe représentant l'atmosphere.
///
/// Classe comprenant un rayon et une vitesse afin de générer l'atmosphere.
///
class Atmosphere: public TexturedMesh3{
private:
     double radius; ///< Double représentant le rayon.
     double speed; ///< Double représentant la vitesse.

     Matrix44d matrix;///< Matrice pour les transformations.

public:
    Atmosphere(): TexturedMesh3(){
       radius = 1.0;
       speed = 1.0;

      texId = GetResource("dayStarSky", Texture)->GetId();
      LoadMesh(texId, "resources/skySphere.obj");

    }

    
    /// \brief Règle la valeur du rayon de l'atmosphere.
    /// \param radius valeur du rayon de l'astomsphere.
    void setRadius(uint radius){
        matrix.LoadScale(radius / this->radius);
        Transform(matrix);
    }

    /// \brief Règle la valeur de la vitesse de l'atmosphere.
    /// \param speed Valeur de la vitesse de l'atmosphere
    void setSpeed(uint speed){
        this->speed = speed;
    }

    /// \brief Met à jour l'atmosphere.
    /// \param deltaTime Temps écoulé depuis la dernière mise à jour.
    void update(){
       matrix.LoadYRotation( speed * 0.001 );
       Transform(matrix);
    }

};

#endif //PI2020_ATMOSPHERE_HPP
