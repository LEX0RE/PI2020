/// \file Image.hpp
#ifndef PI2020_IMAGE_HPP
#define PI2020_IMAGE_HPP

#include "../VisualComp.hpp"

/// \brief Classe représentant une Image.
class Image : public VisualComp {
public:
	/// \param texture Pointeur de resource de texture.
	/// \param position Position du coin haut gauche de l'image.
	/// \param size Grandeur de l'Image.
	/// \param maxFrame Nombre de frame total de l'animation.
	Image(const Vector2& position, const Vector2& size, const uint& textureId) : VisualComp(position, size) {
		this->textureId = textureId;
		sdlRect = { position.x, position.y, size.x, size.y };
	}

	/// \brief Affichage de l'image.
	void Draw() {
		glBindTexture(GL_TEXTURE_2D, textureId);
		glBegin(GL_QUADS);
			glTexCoord2d(0.0, 0.0); glVertex3d(sdlRect.x, sdlRect.y, 0.0);
			glTexCoord2d(1.0, 0.0); glVertex3d(sdlRect.x + sdlRect.w, sdlRect.y, 0.0);
			glTexCoord2d(1.0, 1.0); glVertex3d(sdlRect.x + sdlRect.w, sdlRect.y + sdlRect.h, 0.0);
			glTexCoord2d(0.0, 1.0); glVertex3d(sdlRect.x, sdlRect.y + sdlRect.h, 0.0);
		glEnd();
	}

	/// \brief Positionnement de l'image.
	/// \param x Position sur l'axe des x.
	/// \param y Position sur l'axe des y.
	void SetPosition(const int& x, const int& y) {
		sdlRect.x = x;
		sdlRect.y = y;
	}
};

#endif