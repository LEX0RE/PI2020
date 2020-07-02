/// \file Label.hpp
#ifndef LABEL_HPP
#define LABEL_HPP

#include "../VisualComp.hpp"
#include "../Resources/Font.hpp"

/// \brief Classe représentant une ligne de texte.
class Label : public VisualComp {
    SDL_Color color; ///< Couleur du texte.
    TTF_Font* font; ///< Font à utiliser pour changer le texte.

public:
    bool dropShadow;

    /// \param pos Vecteur de position.
    /// \param font Resource de police d'écriture.
    /// \param text Texte à afficher.
    /// \param color Couleur du texte.
	/// \param maxFrame Nombre de frame total de l'animation.
    Label(const Vector2& position, Font* font, const char* text, const SDL_Color& color, const bool& dropShadow = false) : VisualComp(position, Vector2(0.0, 0.0)), color(color), dropShadow(dropShadow) {
        this->color = color;
        this->font = (TTF_Font*)font->GetInstance();
        SetText(text);
    }

    ~Label(){
        glDeleteTextures(1, &textureId);
    }

    /// \brief Modifie le texte.
    /// \param text Nouveau texte.
    void SetText(const char* text) {
        SDL_Surface* sdlSurface = TTF_RenderUTF8_Blended(font, text, color);
        sdlRect.w = sdlSurface->w;
        sdlRect.h = sdlSurface->h;

        glDeleteTextures(1, &textureId);
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sdlSurface->w, sdlSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, sdlSurface->pixels);        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        SDL_FreeSurface(sdlSurface);
    }
    
    /// \brief Affichage de la ligne de texte.
    void Draw() {
		glBindTexture(GL_TEXTURE_2D, textureId);
		glBegin(GL_QUADS);
            if (dropShadow) {
                glColor4d(0.0, 0.0, 0.0, 0.5);
                glTexCoord2d(0.0, 0.0); glVertex3d(sdlRect.x + 2, sdlRect.y + 2, 0.0);
                glTexCoord2d(1.0, 0.0); glVertex3d(sdlRect.x + sdlRect.w + 2, sdlRect.y + 2, 0.0);
                glTexCoord2d(1.0, 1.0); glVertex3d(sdlRect.x + sdlRect.w + 2, sdlRect.y + sdlRect.h + 2, 0.0);
                glTexCoord2d(0.0, 1.0); glVertex3d(sdlRect.x + 2, sdlRect.y + sdlRect.h + 2, 0.0);
            }

            glColor4d(1.0, 1.0, 1.0, 1.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(sdlRect.x, sdlRect.y, 0.0);
			glTexCoord2d(1.0, 0.0); glVertex3d(sdlRect.x + sdlRect.w, sdlRect.y, 0.0);
			glTexCoord2d(1.0, 1.0); glVertex3d(sdlRect.x + sdlRect.w, sdlRect.y + sdlRect.h, 0.0);
			glTexCoord2d(0.0, 1.0); glVertex3d(sdlRect.x, sdlRect.y + sdlRect.h, 0.0);
		glEnd();        
    }
};

#endif