/// \file texturedMesh.hpp
#ifndef TEXTUREDMESH_H
#define TEXTUREDMESH_H

#include "../Mesh.hpp"
#include "../Vector2.hpp"
#include "../Typedef.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdarg>

/// \brief Classe contenant des infos nécéssaire à la gestion d'une TexturedMesh2 et TexturedMesh3
class TexturedMeshInfo {
protected:
	double* texCoord;
	uint texCoordCount;
	uint texId;

public:
	/// \brief Donne l'identifiant de texture.
	/// \return Identifiant de la texture.
	inline uint GetTextureID() {
		return texId;
	}
	
	/// \brief Remplace la texture. ATTENTION: La taille de la nouvelle texture ne sera pas prise en compte pour 
	/// la mesh qui utilise la texture.
	/// \param id Numéro d'identification de la texture souhaitée.
	inline void SetTextureID(uint id) {
		texId = id;
	}
};

/// \brief Classe gérant des Mesh en 2 dimensions auxquelles une texture est appliquée
struct TexturedMesh2 : public _Mesh2, public TexturedMeshInfo {
	
	/// \param maxFrame Nombre de frame total de l'animation
	TexturedMesh2(uint maxFrame = 1) : _Mesh2 (maxFrame){
		texCoord = nullptr;
		texId = 0;
	} 

	virtual ~TexturedMesh2() {
		if(texCoord)
			delete[] texCoord;
	}

	/// \brief Charge un rectangle dans la Mesh avec une texture spécifique.
	/// \param textureId identifiant de la texture.
	/// \param position Position sur l'écran en pixel à partir de (0, 0).
	/// \param size Taille de la mesh en pixels.
	void LoadRect(uint textureId, Vector2 position, Vector2 size = {0, 0}) {

		//Vérifie si texCoord existe.
		if(!texCoord) {
			texCoord = new double[12] {
				0.0, 0.0,
				1.0, 0.0,
				0.0, 1.0, 
				
				1.0, 1.0,
				0.0, 1.0,
				1.0, 0.0
			};
		}	
		
		//vérifie si size est à {0, 0} (qui signifie "prends la taille logique de la texture").
		if(((int)size.x == 0) && ((int)size.y == 0)) {
			int w, h;
			glGetTexLevelParameteriv(GL_TEXTURE_2D, textureId, GL_TEXTURE_WIDTH, &w);
			glGetTexLevelParameteriv(GL_TEXTURE_2D, textureId, GL_TEXTURE_HEIGHT, &h);
			size.x = w;
			size.y = h;
		}
		
		this->texId = textureId;
		_Mesh2::LoadRect(position, size);
		vertexCount = 6;
	}

	/// \brief Règle les coordonnées de texture d'une mesh rectangulaire
	/// \param p Vecteur 2D de coordonnées de texture (Coin supérieur gauche)
	/// \param s Vecteur 2D de coordonnées de texture (Coin inférieur droite)
	inline void SetTextureCoord(Vector2 p1, Vector2 p2) {
		texCoord[8] = texCoord[4] = texCoord[0] = p1.x;           
		texCoord[11] = texCoord[3] = texCoord[1] = p1.y;
		texCoord[10] = texCoord[6] = texCoord[2] = p2.x; 
		texCoord[9] = texCoord[7] = texCoord[5] = p2.y;
	}
	
	/// \brief Déplace les Coordonées de textures d'une mesh rectangulaire 
	/// \param v Vecteur 2D
	inline void MoveTextureCoord(Vector2 v) {
		for(uint i = 0; i < 12; i += 2) {
			texCoord[i] += v.x;
			texCoord[i + 1] += v.y;
		}
	}
	
	/// \brief Obtient la position de coordonnées de texture.
	/// \return Vecteur 2D
	inline Vector2 GetTextureCoordPosition() {
		return {texCoord[0], texCoord[1]};
	}

	/// \brief Affichage de l'objet.
	void Draw() {
		glEnableClientState(GL_VERTEX_ARRAY);
    		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		
    		glVertexPointer(3, GL_DOUBLE, 0, vertices[frame]);
    		glTexCoordPointer(2, GL_DOUBLE, 0, texCoord);
    		glBindTexture(GL_TEXTURE_2D, texId);
    		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		
    		glDisableClientState(GL_VERTEX_ARRAY);
    		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

};

/// \brief Classe gérant des Mesh en 3 dimensions auxquelles une texture est appliquée
struct TexturedMesh3 : public _Mesh3, public TexturedMeshInfo {
	
	/// \param maxFrame Nombre de frame total de l'animation
	TexturedMesh3(uint maxFrame = 1) : _Mesh3(maxFrame) {
		texCoord = nullptr;
		texId = 0;
	}

	virtual ~TexturedMesh3() {
		if(texCoord && toDelete) 
			delete[] texCoord;
	}

	/// \brief Charge un fichier de model Blender (.obj) lors du chargement des modèles de base.
	/// \param filename Nom du fichier ".obj".
	/// \param textureId Identifiant de texture OpenGL.
    /// \return L'information sur la mesh
    MeshInfo* LoadMesh(uint textureId, const char* fileName) {
      this->texId = textureId;

		  double radius, topRadius = 0.0;
		  Vector3 vertex, topSize = {0, 0, 0}, minSize = {0, 0, 0};

		  for (size_t i = 0; i < maxFrame; i++) {
        string filename = fileName;
			  if(maxFrame > 1) {
				  string number = to_string(i + 1);
				  while (number.size() < 6)
					  number.insert(0, "0");
				  filename.insert(filename.size() - 4, "_" + number);
			  }

			  ifstream file(filename);
			
          string type;
          unsigned int face;
          vector<double> Vvertices;
          vector<double> VtexCoords;
          vector<double> Vnormals;
          vector<unsigned int> faces;
				
				  while(!file.eof()) {
				    type = "#"; // Prévention de ligne vide en fin de fichier
					  file >> type;
					  switch(type[0]) {
						  case 'f':
						    for(unsigned char j = 0; j < 3; j++) {
                  file >> face; faces.push_back(face);
                  file.ignore(1, 1); // Ignorer le slash
                  file >> face; faces.push_back(face);
                  file.ignore(1, 1); // Ignorer le slash
                  file >> face; faces.push_back(face);
                }
                break;

						  case 'v':
						    switch (type[1]) {
						      case 't':
                    file >> vertex.x; VtexCoords.push_back(vertex.x);
                    file >> vertex.y; VtexCoords.push_back(1 - vertex.y);
                    break;

						      case 'n':
                    file >> vertex.x; Vnormals.push_back(vertex.x);
                    file >> vertex.y; Vnormals.push_back(vertex.y);
                    file >> vertex.z; Vnormals.push_back(vertex.z);
                    break;

						      default:
                    file >> vertex.x; Vvertices.push_back(vertex.x);
                    file >> vertex.y; Vvertices.push_back(vertex.y);
                    file >> vertex.z; Vvertices.push_back(vertex.z);

                    radius = vertex.GetNorm();
                    if (radius > topRadius) topRadius = radius;
                    if (vertex.x > topSize.x) topSize.x = vertex.x;
                    if (vertex.x < minSize.x) minSize.x = vertex.x;
                    if (vertex.y > topSize.y) topSize.y = vertex.y;
                    if (vertex.y < minSize.y) minSize.y = vertex.y;
                    if (vertex.z < topSize.z) topSize.z = vertex.z;
                    if (vertex.z > minSize.z) minSize.z = vertex.z;
                    break;
                }
							  break;
						
						  default:
							  file.ignore(50, 10);
							  break;
					  }
				  }

        int size3 = faces.size();
        vertexCount = size3 / 3;
        texCoordCount = vertexCount * 2;

        vertices[frame] = new double[size3];
        normals[frame] = new double[size3];
				texCoord = new double[vertexCount * 2];

				size_t texpos = 0;
        for(int v = 0; v < size3; v += 3) {
					face = (faces[v] - 1) * 3;
					vertices[frame][v] = Vvertices[face++];
					vertices[frame][v + 1] = Vvertices[face++];
					vertices[frame][v + 2] = Vvertices[face];

					face = (faces[v + 1] - 1) * 2;
					texCoord[texpos++] = VtexCoords[face++];
					texCoord[texpos++] = VtexCoords[face];

					face = (faces[v + 2] - 1) * 3;
					normals[frame][v] = Vnormals[face++];
					normals[frame][v + 1] = Vnormals[face++];
					normals[frame][v + 2] = Vnormals[face];
				}
			file.close();
      if(maxFrame > 1) SetFrame(i + 1);
		}
		SetFrame(0);

    info = new MeshInfo(topRadius, minSize, topSize);
    return info;
	}
	
	/// \brief Copie une mesh.
	/// \param tm3 Pointeur de TexturedMesh3.
	void CopyMesh(TexturedMesh3* tm3) {
		this->texId = tm3->texId;
		this->texCoord = new double[tm3->texCoordCount];
		for(uint i = 0; i < tm3->texCoordCount; i++) 
			this->texCoord[i] = tm3->texCoord[i];
		_Mesh3::CopyMesh(tm3);
	}

	/// \brief Assigne les données membre d'une texturedMesh depuis une resource chargée avec le resourceManager.
	/// \param constPtr Mesh obtenue avec GetResource(..., TexturedMesh3)
	void AssignMeshFromResource(const TexturedMesh3* constPtr) {
		this->toDelete = false;
		this->normals[frame] = ((TexturedMesh3*)constPtr)->normals[frame];
		this->position = ((TexturedMesh3*)constPtr)->position;
		this->texCoord = ((TexturedMesh3*)constPtr)->texCoord;
		this->texCoordCount = ((TexturedMesh3*)constPtr)->texCoordCount;
		this->texId = ((TexturedMesh3*)constPtr)->texId;
		this->vertexCount = ((TexturedMesh3*)constPtr)->vertexCount;
		this->vertices[frame] = ((TexturedMesh3*)constPtr)->vertices[frame];
	}

	/// \brief Affichage de l'objet.
	void Draw() {
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

};

#endif
