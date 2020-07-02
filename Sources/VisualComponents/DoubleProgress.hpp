/// \file DoubleProgress.hpp
#ifndef PI2020_DOUBLEPROGRESS_HPP
#define PI2020_DOUBLEPROGRESS_HPP

#include "Progress.hpp"

/// \brief Classe pour la création et l'affichage de barre d'élément de l'interface utilisateur
class DoubleProgress : public Progress{
private:
	Image* frontBar; ///< Objet qui représente l'avant plan de la barre.
	ushort p1; ///< Point de référence en x qui est le plus en haut à droite pour la barre qui progresse.
	ushort p2; ///< Point de référence en x qui est le plus en bas à droite pour la barre qui progresse.
	ushort progressStart; ///< Centre de la barre

public:
	/// \param position Position de la barre (à partir du coin supérieur gauche de la barre).
	/// \param size Grosseur de la barre (en pixels).
	/// \param texture Texture de l'avant plan de la barre.
	/// \param color Couleur de la barre.
	/// \param total Nombre total de la barre.
	/// \param count Nombre de progrès de la barre.
	/// \param ProgressStart Position de départ de la barre qui progresse.
	/// \param sizeTexture Grosseur de la texture.
	DoubleProgress(Vector2 position, Vector2 size, Texture* texture, ushort total, ushort count = 0, uint progressStart = 0, Vector2 sizeTexture = {0,0}) : Progress(position, size, total){
		if((!sizeTexture.x) && (!sizeTexture.y)){
			sizeTexture.x = size.x;
			sizeTexture.y = size.y;
		}
		progressStart = ((progressStart) ? progressStart : (size.x / 2))  + position.x;
		this->progressStart = progressStart;
		this->p1 = size.x;
		this->p2 = size.x;
		this->total = total;
		this->count = ((count >= total) ? total : count);
		frontBar = new Image(position, sizeTexture, texture->GetId());
		((ColoredMesh2*)mesh)->LoadRect({26, 223, 72, 255}, {position.x + 1, position.y + 1}, {(double)this->p1, size.y - 5});
		SetCount(count);
	}
	
	~DoubleProgress(){
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
				{(((double)-this->count / total) * (p1 - progressStart)) + progressStart, sdlRect.y + 1},
				{(((double)this->count / total) * (p1 - progressStart)) + progressStart, sdlRect.y + 1},
				{(((double)this->count / total) * (p2 - progressStart)) + progressStart, sdlRect.y + sdlRect.h - 2},
				{(((double)-this->count / (double)total) * (p2 - progressStart)) + progressStart, sdlRect.y + sdlRect.h - 2}
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
	
	
	/// \brief Applique les points de références pour la position de la texture
	/// \param position coordonnée en haut a gauche
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

#endif //PI2020_DOUBLEPROGRESS_HPP
