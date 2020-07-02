/// \file HintBox.hpp
#ifndef PI2020_HINTBOX_HPP
#define PI2020_HINTBOX_HPP

#include "Label.hpp"

/// \brief Classe représentant une liste de lignes de texte.
class HintBox : public VisualComp {
    list<Label*> labelList; ///< Liste des lignes à afficher dans la boîte d'informations.

public:
    /// \param position Position de la boîte d'informations.
    HintBox() : VisualComp({0.0, 0.0}, {0.0, 0.0}) {
        visible = false;
    }

    ~HintBox() {
        for (auto it : labelList)
            delete it;
    }

    /// \brief Ajoute une ligne de texte à la boîte d'informations.
    /// \param text Texte à ajouter à la boîte d'informations.
    inline void AddText(const char* text, const SDL_Color& color, const bool& dropShadow = false){
        labelList.push_back(new Label({sdlRect.x, 20.0 * (double)labelList.size() + sdlRect.y}, GetResource("fontHintBox", Font), text, color, dropShadow));
    }

    /// \brief Affichage de la boîte d'informations.
    void Draw(){
        if (visible)
            for (auto it : labelList)
                it->Draw();
    }

    /// \brief Vide la liste de lignes de texte.
    inline void ResetText(){
        labelList.clear();
    }

	/// \brief Détermine la position du composant visuel.
	/// \param position Position.
	inline void SetPosition(const Vector2& position) {
        SetPosition(position.x, position.y);
	}

	/// \brief Détermine la position de la boîte d'informations.
	/// \param x Position sur l'axe des x.
	/// \param y Position sur l'axe des y.
    void SetPosition(const double& x, const double& y) {
        sdlRect.x = x;
        sdlRect.y = y;

        list<Label*>::iterator it = labelList.begin();
        for (size_t i = 0; i < labelList.size(); i++) {
            (*it)->SetPosition(x, 20.0 * (double)i + y);
            it++;
        }
    }
};

#endif