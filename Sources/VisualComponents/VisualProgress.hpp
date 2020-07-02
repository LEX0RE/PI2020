/// \file VisualProgress.hpp
#ifndef PI2020_UIBAR_HPP
#define PI2020_UIBAR_HPP

#include "Progress.hpp"
#include "Image.hpp"

/// \brief Classe pour la création et l'affichage de barre d'élément de l'interface utilisateur
class VisualProgress : public Progress{
private:
	Image* frontBar; ///< Objet qui représente l'avant plan de la barre.
	ushort p1; ///< Point de référence en x qui est le plus en haut à droite pour la barre qui progresse.
	ushort p2; ///< Point de référence en x qui est le plus en bas à droite pour la barre qui progresse.
	
public:
	/// \param position Position de la barre (à partir du coin supérieur gauche de la barre).
	/// \param size Grosseur de la barre (en pixels).
	/// \param texture Texture de l'avant plan de la barre.
	/// \param color Couleur de la barre.
	/// \param total Nombre total de la barre.
	/// \param count Nombre de progrès de la barre.
	/// \param ProgressStart Position de départ de la barre qui progresse.
	/// \param sizeTexture Grosseur de la texture.
	VisualProgress(Vector2 position, Vector2 size, Texture* texture, ushort total, ushort count = 0, uint progressStart = 0, Vector2 sizeTexture = {0,0}) : Progress(position, size, total){
		if((!sizeTexture.x) && (!sizeTexture.y)){
			sizeTexture.x = size.x;
			sizeTexture.y = size.y;
		}
		progressStart = (progressStart) ? progressStart : position.x;
		this->p1 = (((double) count / total) * (size.x - progressStart));
		this->p2 = (((double) count / total) * (size.x - progressStart));
		this->total = total;
		this->count = ((count >= total) ? total : count);
		frontBar = new Image(position, sizeTexture, texture->GetId());
		((ColoredMesh2*)mesh)->LoadRect({26, 223, 72, 255}, {(double)progressStart, position.y + 1}, {(double)this->p1, size.y - 5});
		SetCount(count);
	}
	
	~VisualProgress(){
		delete frontBar;
	}
	
	/// \brief Modifie le nombre de progrès de la barre.
	/// \param count Nombre à donner de progrès
	void SetCount(short count){
		if(count < 0)
			this->count = 0;
		else if(count >= total)
			this->count = total;
		else
			this->count = count;
		mesh->LoadQuad(
			{sdlRect.x + 1, sdlRect.y + 1},
			{(((double)this->count / total) * (sdlRect.w - 5 - p1)) + p1, sdlRect.y + 1},
			{(((double)this->count / total) * (sdlRect.w - 5 - p1)) + p2, sdlRect.y + sdlRect.h - 2},
			{sdlRect.x + 1, sdlRect.y + sdlRect.h - 2}
		);
	}
	
	/// \brief Applique les points de références pour la barre de chargement
	/// \param p1 Point ayant une arrête avec la point p3 et p1 qui est le plus en haut à droite
	/// \param p2 Point ayant une arrête avec la point p4 et p2 qui est le plus en bas à droite
	void SetRefPoint(ushort p1, ushort p2){
		this->p1 = p1;
		this->p2 = p2;
		SetCount(count);
	}

    /// \brief Retourne le point de référence en x qui est le plus en haut à droite pour la barre qui progresse.
	inline ushort GetP1(){
	    return p1;
	}

    /// \brief Retourne le point de référence en x qui est le plus en bas à droite pour la barre qui progresse.
    inline ushort GetP2(){
        return p2;
    }
	
	/// \brief Applique les points de références pour la position de la texture
	/// \param coordonnée haut gauche
	void SetPositionTexture(const int& x, const int& y){
		frontBar->SetPosition(x, y);
	}
	
	/// \brief Affiche la barre.
	void Draw(){
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_COLOR);
		mesh->Draw();
		glDisable(GL_COLOR);
		glEnable(GL_TEXTURE_2D);
		frontBar->Draw();
	}
	
	/// \brief Notification sur la barre.
	void Notification(){}
	
};

#endif //PI2020_UIBAR_HPP
