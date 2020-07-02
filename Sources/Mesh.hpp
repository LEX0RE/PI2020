/// \file Mesh.hpp
#ifndef MESH_HPP
#define MESH_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Matrix44.hpp"
#include "ResourceManager.hpp"
#include "Typedef.hpp"

#define MESH_RECT_X vertices[frame][0]
#define MESH_RECT_Y vertices[frame][1]
#define MESH_RECT_W vertices[frame][3]
#define MESH_RECT_H vertices[frame][7]

using namespace std;

/// \brief Classe gérant un tableau de sommets d'objets en 2 et 3 dimensions.
class _Mesh : public Resource {
protected:
	double ** vertices; ///< Tableau de vertices.
	uint frame; ///< Mesh actuelle de l'animation.
	uint maxFrame; ///< Nombre de mesh total de l'animation.
	bool animated; ///< Indique si le mesh est animé.
	size_t vertexCount; ///< Nombre de vertex.

public:
	/// \param maxFrame Nombre de frame total de l'animation
	_Mesh(uint maxFrame){
		this->maxFrame = maxFrame;
		frame = animated = 0;
		vertices = new double*[maxFrame];
	}
	
	virtual ~_Mesh(){
		for(uint i = 0; i < maxFrame; i++){
			if(vertices[i])
				delete[] vertices[i];
		}
		delete[] vertices;
	}
	
	/// \brief Affiche la mesh à l'écran.
	virtual void Draw() = 0;

	/// \brief Obtient le tableau de sommets sous forme de void*.
	/// \return Adresse du tableau de sommets.
	inline void * GetInstance() {
		return (void*) vertices[frame];
	}
	
	/// \brief Inverse l'activation de l'animation (True/False)
	inline void SwitchAnimated(){
		animated = (!animated);
	}
	
	/// \brief Change de frame.
	/// \param frame Frame que l'on veut être.
	inline void SetFrame(uint frame){
		this->frame = frame;
	}
	
	/// \brief Obtient le nombre de frame total.
	/// \return Retourne le nombre de frame total de ce mesh.
	inline uint GetMaxFrame(){
		return maxFrame;
	}
	
	/// \brief Change le frame au suivant.
	/// \return frame qui est présentement utilisé dans le mesh.
	inline void LoopFrame(){
		frame = (frame + 1) % maxFrame;
	}
};

/// \brief Classe contenant des outils afin de gérer des objets en 2 dimensions.
struct _Mesh2 : public _Mesh {
	
	/// \param maxFrame Nombre de frame total de l'animation
	_Mesh2(uint maxFrame = 1) : _Mesh (maxFrame){
		vertices[frame] = nullptr;
		vertexCount = 0;
	}

	virtual ~_Mesh2() {}

	/// \brief Vecteur 2D représentant la position du mesh.
    	/// \return Position du mesh.
    	inline Vector2 GetPosition(){
		if(vertices[frame])
        	return {vertices[frame][0], vertices[frame][1]};
		return Vector2(-1.0, -1.0);
   	 }

	/// \brief Charge un rectangle dans la Mesh
	/// \param position Position sur l'écran en pixel à partir de (0, 0).
	/// \param size Taille de la mesh en pixels.
	void LoadRect(Vector2 position, Vector2 size) {	
		double width = position.x + size.x;
		double height = position.y + size.y;
		
		if(vertices[frame]) { //les indices sont toujours constantes. Pas besoin d'itérer.
			vertices[frame][0] = vertices[frame][6] = vertices[frame][12] = position.x;
			vertices[frame][1] = vertices[frame][4] = vertices[frame][16] = position.y;
			vertices[frame][3] = vertices[frame][9] = vertices[frame][15] = width;
			vertices[frame][7] = vertices[frame][10] = vertices[frame][13] = height;
		}
		
		else {
			vertexCount = 6;
			vertices[frame] = new double[vertexCount * 3] {
				position.x, position.y,   0,
				width,      position.y,   0,
				position.x, height,       0,
				
				width,      height,       0,
				position.x, height,       0, 
				width,      position.y,   0
			};
		}
	}

	/// \brief Charge un quadrilatère dans les vertices
	/// \param p1 Point ayant une arrête avec la point p2 et p4 qui est le plus en haut à gauche
	/// \param p2 Point ayant une arrête avec la point p3 et p1 qui est le plus en haut à droite
	/// \param p3 Point ayant une arrête avec la point p4 et p2 qui est le plus en bas à droite
	/// \param p4 Point ayant une arrête avec la point p1 et p3 qui est le plus en bas à gauche
	void LoadQuad(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4) {
		if(vertices[frame])
			delete[] vertices[frame];
		vertexCount = 6;
		vertices[frame] = new double[vertexCount * 3] {
			p1.x, p1.y, 0,
			p2.x, p2.y, 0,
			p4.x, p4.y, 0,
				
			p3.x, p3.y, 0,
			p4.x, p4.y, 0,
			p2.x, p2.y, 0
		};
	}

	/// \brief Déplace à la coordonnée 2d, avec comme point de référence le premier sommet, la présente Mesh.
	/// \param v Vecteur de position.
	void MoveTo(Vector2 v) {
		Vector2 p = GetPosition();
		v -= p;
		MoveBy(v);
	}

	/// \brief Déplace la Mesh selon un vecteur qui est additionné à la matrice de sommets.
	/// \param v Vecteur déplacement.
	/// \param frame Nombre de frame à appliquer la transfromation
	void MoveBy(Vector2 v) {
		for(uint f = 0; f < maxFrame; f++) {
			for(uint i = 0, m = vertexCount * 3; i < m; i += 3) {
				vertices[f][i] += v.x;
				vertices[f][i + 1] += v.y;
			}
		}
	}

	/// \brief Vérifie si le point est à l'intérieur.
	/// \param v Vecteur 2D de position
	/// \return true si le point est à l'intérieur, sinon false.
	inline bool IsInside(Vector2 v) {
		if(vertices[frame])
			return ((v.x >= MESH_RECT_X) && (v.y >= MESH_RECT_Y) && (v.x < MESH_RECT_W) && (v.y < MESH_RECT_H));
		return false;
	}
};


/// \brief Classe contenant des informations utiles sur un mesh
/// \brief Classe contenant des informations utiles sur un mesh
struct MeshInfo{
    double radius; ///< Rayon de la Mesh
    Vector3 minsize, size; ///< taille

    /// \param r Rayon de la mesh.
    /// \param s Taille de la mesh a partir du centre.
    MeshInfo(double r, Vector3 minSize, Vector3 maxSize) {
        radius = r;
        this->minsize = minSize;
        size = maxSize;
    }
};

/// \brief Classe contenant des outils afin de gérer des objets en 3 dimensions.
struct _Mesh3 : public _Mesh {
protected: 
	double** normals; ///< tableau dynamique de normales.
	Vector3 position; ///< le centre, la position de la mesh.
	bool toDelete; ///< si la mesh est assingée depuis une mesh contenue dans le resourcemanager, alors elle ne doit pas être supprimée.
public:
    MeshInfo* info; ///< Information de la mesh.
	
	/// \param maxFrame Nombre de frame total de l'animation.
	_Mesh3(uint maxFrame) : _Mesh (maxFrame){
		normals = new double*[maxFrame];
		normals[frame] = vertices[frame] = nullptr;
		position = Vector3(0, 0, 0);
		vertexCount = 0;
		toDelete = true;
	}

    	virtual ~_Mesh3() {
		/*
		if(vertices && toDelete) {
			for(unsigned int i = 0; i < maxFrame; i++) {
				delete[] vertices[i];
			}
			delete[] vertices;
		}
		 */
		if(normals && toDelete) {
			for(uint i = 0; i < maxFrame; i++) {
				delete[] normals[i];
			}
			delete[] normals;
		}
	}

	/// \brief Vector 3D représentant la position du mesh.
    	/// \return Position du mesh.
	inline Vector3 GetPosition() {
		return position;
	}
	
	/// \brief Pointeur de Vector 3D représentant la position du mesh.
	/// \return Pointeur sur la position du mesh.
	inline Vector3* GetPointPosition() {
		return &position;
	}

	/// \brief Déplace à la coordonnée 3d, avec comme point de référence le premier sommet, la présente Mesh.
	/// \param v Vecteur de position.
	void MoveTo(Vector3 v) {
		v -= position;
        	MoveBy(v,false);
		v += position;
		position = v;
	}

	/// \brief Déplace la Mesh selon un vecteur qui est additionné à la matrice de sommets.
	/// \param v Vecteur déplacement.
	void MoveBy(Vector3 v, bool setPos = true) {
		for(uint f = 0; f < maxFrame; f++) {
			for(uint i = 0; i < vertexCount * 3; i += 3) {
				vertices[f][i] += v.x;
				vertices[f][i + 1] += v.y;
				vertices[f][i + 2] += v.z;
			}
			if(setPos)
				position += v;
		}
	}
	
	/// \brief Copie les données d'une mesh.
	/// \param m3 Mesh 3D.
	void CopyMesh(_Mesh3* m3) {
		for(uint i = 0; i < maxFrame; i++) {
			this->normals[i] = new double[m3->vertexCount * 3];
			this->vertices[i] = new double[m3->vertexCount * 3];
			this->position = m3->position;
			this->vertexCount = m3->vertexCount;
			
			for(uint j = 0, max = vertexCount * 3; j < max; j++) {
				this->vertices[i][j] = m3->vertices[i][j];
				this->normals[i][j] = m3->normals[i][j];
			}
		}
	}

	/// \brief Transforme la matrice de sommet.
	/// \param m Matrice modifiant la matrice de sommet.
    /// \param normal Indique si l'on doit transformer les normales.
    void Transform(Matrix44d m, bool normal = true){
        double x, y, z;
		for(uint f = 0; f < maxFrame; f++) {
			for(uint i = 0; i < vertexCount * 3; i += 3) {
				x = vertices[f][i] + m.m41;
				y = vertices[f][i + 1] + m.m42;
				z = vertices[f][i + 2] + m.m43;
				
				vertices[f][i] = x * m.m11 + m.m41 + y * m.m21 + m.m41 + z * m.m31 + m.m41;
				vertices[f][i + 1] = x * m.m12 + m.m42 + y * m.m22 + m.m42 + z * m.m32 + m.m42;
				vertices[f][i + 2] = x * m.m13 + m.m43 + y * m.m23 + m.m43 + z * m.m33 + m.m43;
				
				if(normal) {
					x = normals[f][i] + m.m41;
					y = normals[f][i + 1] + m.m42;
					z = normals[f][i + 2] + m.m43;
					
					normals[f][i] = x * m.m11 + m.m41 + y * m.m21 + m.m41 + z * m.m31 + m.m41;
					normals[f][i + 1] = x * m.m12 + m.m42 + y * m.m22 + m.m42 + z * m.m32 + m.m42;
					normals[f][i + 2] = x * m.m13 + m.m43 + y * m.m23 + m.m43 + z * m.m33 + m.m43;
				}
			}
		}
    }

    /// \brief Applique une rotation local
    /// \param m Matrice de rotation
    void ApplyLocalRotation(Matrix44d m){
			Vector3 prevposition = position;
			MoveTo({0.0, 0.0, 0.0});
			Transform(m);
			MoveTo(prevposition);
    }

    inline uint GetFrameVerticeCount(){
        return vertexCount;
    }

    inline double* GetFrameVertices(){
        return vertices[frame];
    }

    inline double* GetFrameNormals(){
        return normals[frame];
    }
};

/// \brief Classe représentant une Mesh3 sans affichage.
struct InvisibleMesh3 : public _Mesh3 {
    Vector3 minsize; ///< Taille de la mesh a partir de 0,0,0 vers le négatif.
    Vector3 size; ///< Taille de la mesh a partir du 0,0,0 vers le positif.
    bool lines; ///< Si la boite doit être désinnée avec des lignes ou avec des faces pleines.

    /// \param position Position du milieu de la mesh.
    /// \param minsize Taille de la mesh a partir de 0,0,0 vers le négatif.
    /// \param size Taille de la mesh a partir du 0,0,0 vers le positif.
	/// \param maxFrame Nombre de frame total de l'animation
    InvisibleMesh3(Vector3 position, Vector3 minsize, Vector3 size, uint maxFrame) : _Mesh3 (maxFrame){
        LoadBox(position,minsize,size);
        this->minsize = minsize;
        this->size = size;
        this->position = position;
        lines = true;
    }

    /// \brief Déplace la Mesh selon un vecteur qui est additionné à la matrice de sommets.
    /// \param v Vecteur déplacement.
    void MoveBy(Vector3 v) {
        for(uint i = 0; i < vertexCount * 3; i += 3) {
            vertices[frame][  i  ] += v.x;
            vertices[frame][i + 1] += v.y;
            vertices[frame][i + 2] += v.z;
        }
        position += v;
    }

    /// \brief Charge un rectangle 3D dans la Mesh
    /// \param position Position du milieu de la mesh.
    /// \param minsize Taille de la mesh a partir du centre vers le négatif.
    /// \param size Taille de la mesh a partir du centre vers le positif.
    void LoadBox(Vector3 position, Vector3 minsize, Vector3 size) {
        if(vertices[frame]) {
            //a faire ? si nécessaire...
        }
        else {
            vertexCount = 36;
            vertices[frame] = new double[vertexCount * 3] {
            //face
            position.x+minsize.x, position.y+minsize.y,   position.z+minsize.z,
            position.x+size.x, position.y+minsize.y,   position.z+minsize.z,
            position.x+size.x, position.y+size.y,   position.z+minsize.z,

            position.x+size.x, position.y+size.y,   position.z+minsize.z,
            position.x+minsize.x, position.y+size.y,   position.z+minsize.z,
            position.x+minsize.x, position.y+minsize.y,   position.z+minsize.z,
            //derriere
            position.x+minsize.x, position.y+minsize.y,   position.z+size.z,
            position.x+size.x, position.y+minsize.y,   position.z+size.z,
            position.x+size.x, position.y+size.y,   position.z+size.z,

            position.x+size.x, position.y+size.y,   position.z+size.z,
            position.x+minsize.x, position.y+size.y,   position.z+size.z,
            position.x+minsize.x, position.y+minsize.y,   position.z+size.z,
            //gauche
            position.x+minsize.x, position.y+minsize.y,   position.z+minsize.z,
            position.x+minsize.x, position.y+minsize.y,   position.z+size.z,
            position.x+minsize.x, position.y+size.y,   position.z+size.z,

            position.x+minsize.x, position.y+size.y,   position.z+size.z,
            position.x+minsize.x, position.y+size.y,   position.z+minsize.z,
            position.x+minsize.x, position.y+minsize.y,   position.z+minsize.z,
            //droite
            position.x+size.x, position.y+minsize.y,   position.z+minsize.z,
            position.x+size.x, position.y+minsize.y,   position.z+size.z,
            position.x+size.x, position.y+size.y,   position.z+size.z,

            position.x+size.x, position.y+size.y,   position.z+size.z,
            position.x+size.x, position.y+size.y,   position.z+minsize.z,
            position.x+size.x, position.y+minsize.y,   position.z+minsize.z,
            //dessus
            position.x+minsize.x, position.y+size.y,   position.z+minsize.z,
            position.x+size.x, position.y+size.y,   position.z+size.z,
            position.x+minsize.x, position.y+size.y,   position.z+size.z,

            position.x+size.x, position.y+size.y,   position.z+size.z,
            position.x+minsize.x, position.y+size.y,   position.z+minsize.z,
            position.x+size.x, position.y+size.y,   position.z+minsize.z,
            //dessous
            position.x+minsize.x, position.y+minsize.y,   position.z+minsize.z,
            position.x+size.x, position.y+minsize.y,   position.z+size.z,
            position.x+minsize.x, position.y+minsize.y,   position.z+size.z,

            position.x+size.x, position.y+minsize.y,   position.z+size.z,
            position.x+minsize.x, position.y+minsize.y,   position.z+minsize.z,
            position.x+size.x, position.y+minsize.y,   position.z+minsize.z,
            };
            normals[frame] = new double[vertexCount * 3] {
            //face
            0.0, 0.0, -1.0,
            0.0, 0.0, -1.0,
            0.0, 0.0, -1.0,

            0.0, 0.0, -1.0,
            0.0, 0.0, -1.0,
            0.0, 0.0, -1.0,
            //derriere
            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0,

            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0,
            //gauche
            -1.0, 0.0, 0.0,
            -1.0, 0.0, 0.0,
            -1.0, 0.0, 0.0,

            -1.0, 0.0, 0.0,
            -1.0, 0.0, 0.0,
            -1.0, 0.0, 0.0,
            //droite
            1.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 0.0, 0.0,

            1.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            //dessus
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,

            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            //dessous
            0.0, -1.0, 0.0,
            0.0, -1.0, 0.0,
            0.0, -1.0, 0.0,

            0.0, -1.0, 0.0,
            0.0, -1.0, 0.0,
            0.0, -1.0, 0.0,
            };
        }
    }

    /// \brief Obtien un sommet de la boite
    /// \param point Sommet de la boite ( 0-7 )
    /// \return Le sommet.
    Vector3 GetBoxPoint(short point){
        /*
        Dessous        Dessus
        0,4             2,6
        1,5             3,7
        */
        switch(point){
        case 0:
            return {position.x+minsize.x,position.y+minsize.y,position.z+minsize.z};
        case 1:
            return {position.x+minsize.x,position.y+minsize.y,position.z+size.z};
        case 2:
            return {position.x+minsize.x,position.y+size.y,position.z+minsize.z};
        case 3:
            return {position.x+minsize.x,position.y+size.y,position.z+size.z};
        case 4:
            return {position.x+size.x,position.y+minsize.y,position.z+minsize.z};
        case 5:
            return {position.x+size.x,position.y+minsize.y,position.z+size.z};
        case 6:
            return {position.x+size.x,position.y+size.y,position.z+minsize.z};
        case 7:
            return {position.x+size.x,position.y+size.y,position.z+size.z};
        default:
            return {0.0, 0.0, 0.0};
        }
    }
	
    /// \brief Obtien un point d'un triangle de la boite
    /// \param triangle Triangle de la boite ( 0-11 )
    /// \param point Point du triangle ( 0-2 )
    /// \return Position du premier point du triangle
    Vector3 GetBoxTrianglePoint(short triangle, short point = 0){
        return {vertices[frame][(triangle * 9) + (point * 3)], vertices[frame][(triangle * 9) + (point * 3) + 1], vertices[frame][(triangle * 9) + (point * 3) + 2]};
    }

    /// \brief Obtien une normale d'un triangle de la boite
    /// \param triangle Triangle de la boite ( 0-11 )
    /// \return Normale du triangle
    Vector3 GetBoxTriangleNormal(short triangle){
        return {normals[frame][triangle * 9], normals[frame][triangle * 9 + 1], normals[frame][triangle * 9 + 2]};
    }

    /// \brief Affichage de la boite. Pour tester seulement, ne devrait pas être appelé normalement.
    virtual void Draw(){
        glEnableClientState(GL_VERTEX_ARRAY);

        glVertexPointer(3, GL_DOUBLE, 0, vertices[frame]);

        if(lines)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glDisableClientState(GL_VERTEX_ARRAY);
    }
};

#endif
