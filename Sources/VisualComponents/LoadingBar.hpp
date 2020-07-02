/// \file LoadingBar.hpp
#ifndef LOADINGBAR_HPP
#define LOADINGBAR_HPP

/// \brief Classe représentant une barre de chargement.
class LoadingBar : public VisualComp {
    ushort value; ///< Valeur actuelle
    ushort count; ///< Valeur total
    Label* text;
    
public:
    /// \param position Position de la barre de chargement
    /// \param size Taille de la barre de chargement
    /// \param count Nombre d'éléments à charger
    LoadingBar(const Vector2& position, const Vector2& size, const ushort count) : VisualComp(position, size) {
        value = 0;
        this->count = count;
        text = new Label(Vector2(position.x, position.y + 3), new Font("resources/Fonts/HintBox.ttf", 20), " ", { 200, 200, 200, 255 }, true);
    }

    ~LoadingBar() {
        delete text;
    }

    /// \brief Affiche la barre de chargement.
    void Draw() {
        const ushort progress = (double)value / (double)count * (double)sdlRect.w - 6.0;
		glBegin(GL_QUADS);
        	glColor3d(0.45, 0.32, 0.14);
            glVertex3d(sdlRect.x, sdlRect.y, 0.0);
			glVertex3d(sdlRect.x + sdlRect.w, sdlRect.y, 0.0);
			glVertex3d(sdlRect.x + sdlRect.w, sdlRect.y + sdlRect.h, 0.0);
			glVertex3d(sdlRect.x, sdlRect.y + sdlRect.h, 0.0);

    		glColor3d(0.10, 0.67, 0.13);
            glVertex3d(sdlRect.x + 3, sdlRect.y + 3, 0.0);
			glVertex3d(sdlRect.x + progress, sdlRect.y + 3, 0.0);
			glVertex3d(sdlRect.x + progress, sdlRect.y + sdlRect.h / 2, 0.0);
			glVertex3d(sdlRect.x + 3, sdlRect.y + sdlRect.h / 2, 0.0);

		    glColor3d(0.10, 0.57, 0.13);
            glVertex3d(sdlRect.x + 3, sdlRect.y + sdlRect.h / 2, 0.0);
			glVertex3d(sdlRect.x + progress, sdlRect.y + sdlRect.h / 2, 0.0);
			glVertex3d(sdlRect.x + progress, sdlRect.y + sdlRect.h - 3, 0.0);
			glVertex3d(sdlRect.x + 3, sdlRect.y + sdlRect.h - 3, 0.0);
		glEnd();

  	    glColor3d(1.0, 1.0, 1.0);
        text->Draw();
    }

	/// \brief Validation du chargement.
    /// \return Chargement est terminé.
    inline bool IsLoaded() { return  (value == count); }
    
    /// \brief Passe au prochain chargement.
    void Load() {
        if (value <= count)
            value++;
    }

    void SetText(const string& text) {
        this->text->SetText(text.c_str());
        this->text->SetPosition(sdlRect.w / 2 - this->text->GetWidth() / 2, this->text->GetPosition().y);
    }
};

#endif