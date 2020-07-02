/// \file Ground.hpp
#ifndef PI2020_GROUND_HPP
#define PI2020_GROUND_HPP

#include "Meshes/TexturedMesh.hpp"
#include "Resources/Texture.hpp"

/// \brief Classe représentant le terrain.
///
/// Classe faisant afficher le terrain.
///
class Ground: public TexturedMesh3{
private:



public:
    Ground(): TexturedMesh3(){

        texId = GetResource("grass.png", Texture)->GetId();
        LoadMesh("resources/GroundMapV1.obj", texId);

    }


};

#endif //PI2020_GROUND_HPP
