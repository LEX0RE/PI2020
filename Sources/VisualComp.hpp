/// \file VisualComp.hpp
#ifndef VISUALCOMP_H
#define VISUALCOMP_H

#include "Observer.hpp"
#include "Vector2.hpp"
#include "Mesh.hpp"
#include "GLContext.hpp"
#include <map>


/// \brief Classe dérivant de Observer, servant de base aux éléments graphique.
class VisualComp : public Observer {
protected:
	uint textureId;
	SDL_Rect sdlRect;
	_Mesh2* mesh; ///< Mesh servant de base.
	std::map<uint, void(*)(VisualComp*)> eventRouter; ///< Assigne un numéro d'évenement à un pointeur de fonction.

public:
	bool visible; ///< Détermine si le composant sera visible ou non.

	/// \param pos Position sur l'écran partant de (0, 0).
	/// \param size Taille du composant.
	VisualComp(const Vector2& position, const Vector2& size) : visible(true) {
		this->mesh = nullptr;
		sdlRect = { position.x, position.y, size.x, size.y };
	}

	virtual ~VisualComp() {
		delete mesh;
	}

	/// \brief Ajoute un pointeur de fonction associé à un numéro d'évenement.
	/// \param eventNumber Numéro d'évenement.
	/// \param function Pointeur de fonction prenant un VisualComp* en paramètre.
	inline void Add(uint eventNumber, void(*function)(VisualComp*)){
		eventRouter[eventNumber] = function;
	}

	/// \brief Affiche le composant visuel. 
	virtual void Draw() = 0;

	inline const int& GetWidth() {
		return sdlRect.w;
	}

	/// \brief Notifie le composant visuel.
	virtual void Notification() {
		uint event = Event::GetType();
		if(eventRouter[event])
			eventRouter[event](this);
	}

	/// \brief Vérifie si un point en 2 dimensions est à l'intérieur du composant.
	/// \param v Point en 2 dimension.
	/// \return Retourne un booléen où "true" indique que le point est à l'intérieur du composant et vice-versa.
	bool IsInside(Vector2 v) {
		const SDL_Point position = { v.x, v.y };
		return SDL_PointInRect(&position, &sdlRect);
	}

	inline SDL_Point GetPosition() {
		return { sdlRect.x, sdlRect.y };
	}

	/// \brief Détermine la position du composant visuel.
	/// \param position Position.
	inline virtual void SetPosition(const Vector2& position) {
		SetPosition(position.x, position.y);
	}

	/// \brief Détermine la position du composant visuel.
	/// \param x Position sur l'axe des x.
	/// \param y Position sur l'axe des y.
	virtual void SetPosition(const double& x, const double& y) {
		sdlRect.x = x;
		sdlRect.y = y;

		if (mesh)
			mesh->MoveTo(Vector2(x, y));
	}
};

#endif
