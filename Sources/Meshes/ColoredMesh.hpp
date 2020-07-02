/// \file ColoredMesh.hpp
#ifndef COLOREDMESH_HPP
#define COLOREDMESH_HPP

#include "../Mesh.hpp"
#include "../Vector2.hpp"
#include "../Typedef.hpp"

/// \brief Classe contenant des infos nécéssaire à la gestion d'une ColoredMesh2 et ColoredMesh3
class ColoredMeshInfo {
protected:
	Uint8* color; ///< tableau où 

public:
	/// \brief retourne l'information de couleur.
	/// \return SDL_Color.
	inline SDL_Color GetColor() {
		return {color[0], color[1], color[2], color[3]};
	}
};

struct ColoredMesh2 : public _Mesh2, public ColoredMeshInfo {
	
	/// \param maxFrame Nombre de frame total de l'animation.
	ColoredMesh2(uint maxFrame = 1) : _Mesh2(maxFrame){
		color = nullptr;
	} 

	virtual ~ColoredMesh2() {
		if(color)
			delete[] color;
	}

	/// \brief Règle la couleur 
	/// \param c Structure de couleur de SDL.
	void SetColor(SDL_Color c) {
		for(uint i = 0, m = 4 * vertexCount; i < m; i += 4) {
			color[  i  ] = c.r;
			color[i + 1] = c.g;
			color[i + 2] = c.b;
			color[i + 3] = c.a;
		}
	}

	/// \brief Charge un rectangle dans la Mesh avec une Couleur spécifique.
	/// \param c Structure de couleur {R, G, B, A}.
	/// \param position Position sur l'écran en pixel à partir de (0, 0).
	/// \param size Taille de la mesh en pixels.
	void LoadRect(SDL_Color c, Vector2 position, Vector2 size) {
		if(!color) {
			color = new Uint8[24] {
				c.r, c.g, c.b, c.a,
				c.r, c.g, c.b, c.a,
				c.r, c.g, c.b, c.a,

				c.r, c.g, c.b, c.a,
				c.r, c.g, c.b, c.a,
				c.r, c.g, c.b, c.a,
			};
		}
		else 
			SetColor(c);

		_Mesh2::LoadRect(position, size);
	}

	/// \brief Affiche à l'écran la mesh.
	void Draw() {
		glEnableClientState(GL_VERTEX_ARRAY);
    	glEnableClientState(GL_COLOR_ARRAY);
	
    	glVertexPointer(3, GL_DOUBLE, 0, vertices[frame]);
    	glColorPointer(4, GL_UNSIGNED_BYTE, 0, color);
    	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	
    	glDisableClientState(GL_VERTEX_ARRAY);
   		glDisableClientState(GL_COLOR_ARRAY);
	}
};

struct ColoredMesh3 : public _Mesh3, public ColoredMeshInfo {
	
	/// \param maxFrame Nombre de frame total de l'animation.
	ColoredMesh3(uint maxFrame = 1) : _Mesh3(maxFrame) {
		color = nullptr;
	}

	virtual ~ColoredMesh3() {
		if(color && toDelete)
			delete[] color;
	}

	/// \brief Règle la couleur 
	/// \param c Structure de couleur de SDL.
	void SetColor(SDL_Color c) {
		for(uint i = 0, m = 4 * vertexCount; i < m; i += 4) {
			color[  i  ] = c.r;
			color[i + 1] = c.g;
			color[i + 2] = c.b;
			color[i + 3] = c.a;
		}
	}

	/// \brief Assigne les données membre d'une texturedMesh depuis une resource chargée avec le resourceManager.
	/// \param constPtr Mesh obtenue avec GetResource(..., TexturedMesh3)
	void AssignMeshFromResource(const ColoredMesh3* constPtr) {
		this->toDelete = false;
		this->normals[frame] = ((ColoredMesh3*)constPtr)->normals[frame];
		this->position = ((ColoredMesh3*)constPtr)->position;
		this->color = ((ColoredMesh3*)constPtr)->color;
		this->vertexCount = ((ColoredMesh3*)constPtr)->vertexCount;
		this->vertices[frame] = ((ColoredMesh3*)constPtr)->vertices[frame];
	}

	/// \brief Charge un fichier de model Blender (.obj) lors du chargement des modèles de base.
	/// \param filename Nom du fichier ".obj".
	void LoadMesh(const char * filename) {

		// Chargeur de model.
	}

	/// \brief Affiche à l'écran la mesh.
	void Draw() {
		glEnableClientState(GL_VERTEX_ARRAY);
    	glEnableClientState(GL_NORMAL_ARRAY);
    	glEnableClientState(GL_COLOR_ARRAY);
	
    	glVertexPointer(3, GL_DOUBLE, 0, vertices[frame]);
    	glNormalPointer(GL_DOUBLE, 0, normals[frame]);
    	glColorPointer(4, GL_UNSIGNED_BYTE, 0, color);
	
    	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	
    	glDisableClientState(GL_VERTEX_ARRAY);
    	glDisableClientState(GL_NORMAL_ARRAY);
   		glDisableClientState(GL_COLOR_ARRAY);
	}
};


/*
/// \brief Classe dérivant de Mesh où le mesh aura une couleur fixe. 
class ColoredMesh : public Mesh {
	Uint8 * color; ///< tableau de couleur RGBA où chaque séquence RGBA égale un sommet.

public:
	ColoredMesh() : Mesh() {
		color = nullptr;
	}
	
	~ColoredMesh() {
		if(color)
			delete[] color;
	}

	
	/// \brief Charge un rectangle dans la Mesh avec une Couleur spécifique.
	/// \param color Structure de couleur {R, G, B, A}.
	/// \param position Position sur l'écran en pixel à partir de (0, 0).
	/// \param size Taille de la mesh en pixels.
	void LoadRect(SDL_Color color, Vector2 position, Vector2 size) {
		Mesh* thisptr = this;
		thisptr->LoadRect(position, size);
		Uint8* colorBuffer = new Uint8[24] {
			color.r, color.g, color.b, color.a,
			color.r, color.g, color.b, color.a,
			color.r, color.g, color.b, color.a,

			color.r, color.g, color.b, color.a,
			color.r, color.g, color.b, color.a,
			color.r, color.g, color.b, color.a,
		};

		if(this->color) {
			for(Uint8 i = 0; i < 24; i++) 
				this->color[i] = colorBuffer[i];
			delete[] colorBuffer;
		}
		else 
			this->color = colorBuffer;
	}

	/// \brief Charge un fichier de model Blender (.obj) lors du chargement des modèles de base.
	/// \param filename Nom du fichier ".obj".
	void LoadMesh(const char * filename) {

		// Chargeur de model.
	}

	/// \brief Copie une ColoredMesh dans l'instance active. 
	/// \param mesh Mesh de base provenant du ResourceManager. 
	void CopyMesh(ColoredMesh* mesh) {
		vertexCount = mesh->vertexCount; //nombre de sommets
		uint normalCount = vertexCount / 9; //nombre de faces
		uint colorCount = normalCount * 12; //nombre de valeurs RGBA par sommets

		vertices = new double[vertexCount]; 
		normals = new double[normalCount];
		color = new Uint8[colorCount];

		for(uint i = 0; i < vertexCount; i++) 
			vertices[i] = mesh->vertices[i];
		for(uint i = 0; i < normalCount; i++)
			normals[i] = mesh->normals[i];
		for(uint i = 0; i < colorCount; i++)
			color[i] = mesh->color[i];
	}

	/// \brief Affiche à l'écran la mesh.
	void Draw() {
		glEnableClientState(GL_VERTEX_ARRAY);
    	glEnableClientState(GL_NORMAL_ARRAY);
    	glEnableClientState(GL_COLOR_ARRAY);
	
    	glVertexPointer(3, GL_DOUBLE, 0, vertices);
    	glNormalPointer(GL_DOUBLE, 0, normals);
    	glColorPointer(4, GL_UNSIGNED_BYTE, 0, color);
	
    	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	
    	glDisableClientState(GL_VERTEX_ARRAY);
    	glDisableClientState(GL_NORMAL_ARRAY);
   		glDisableClientState(GL_COLOR_ARRAY);
	}
};
*/
#endif
