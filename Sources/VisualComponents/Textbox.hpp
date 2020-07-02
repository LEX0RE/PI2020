/// \file Textbox.hpp
#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

#include "../Event.hpp"
#include "../VisualComp.hpp"
#include "../Meshes/ColoredMesh.hpp"
#include "../Resources/Font.hpp"
#include "Label.hpp"
#include <list>
#include <string>

/// \brief Classe représentant une boite de texte.
///
/// Classe représentant une boite contenant son texte.
///
class Textbox : public VisualComp {
	std::list<char> list; ///< Liste contenant les caractères affichés
	Label* label; ///< Label affichant le texte à l'écran.
	bool active; ///< Indique si la textbox est active.
	SDL_Color activeColor; ///< Couleur d'activation de la textbox.
	SDL_Color defaultColor; ///< Couleur par défault du fond de la textbox.
	
	static bool allowActive; ///< Indique si la textbox peut être active ou non. (Pour éviter que 2 textbox soient activées en même temps.)

public:
	uint limit;

	/// \param pos Position, en pixels, à l'écran
	/// \param len Longueur, en pixels, de la boîte. 
	/// \param fntNm Nom de la police d'écriture.
	/// \param bckgrd Couleur de fond de la boîte.
	/// \param atvClr Couleur de fond lorsque la boîte est active.
	/// \param fntClr Couleur de la police d'écriture.
	Textbox(Vector2 pos, uint len, Font* font, SDL_Color bckgrd, SDL_Color atvClr, SDL_Color fntClr) : VisualComp(Vector2(pos), {len, 6.0}) {
		mesh = new ColoredMesh2();
		Vector2 meshPos = pos;
		((ColoredMesh2*)mesh)->LoadRect(bckgrd, meshPos, {len, TTF_FontHeight((TTF_Font*)font->GetInstance()) + 6.0});
		label = new Label({pos.x + 3.0, pos.y + 3.0}, font, " ", fntClr);
		list = std::list<char>();
		active = false;
		limit = 10;
		activeColor = atvClr;
		defaultColor = bckgrd;
	}

	~Textbox() {
		delete label;
		allowActive = true;
	}

	/// \brief obtient le texte de la textbox.
	/// \return Texture sous format de chaine de caractère.
	std::string GetText() {
		char * arr = new char[list.size()];
		uint i = 0; 
		for(auto it : list) 
			arr[it++] = it;
		std::string str = std::string(arr);
		delete[] arr;
		return str;
	}

	/// \brief Ajoute 1 caractère à la textbox.
	/// \param c un caratère.
	void Add(char c)
	{
		uint listcount = list.size();
		if(listcount < limit)
			list.push_back(c);
		listcount = list.size();
		char * arr = new char[listcount + 2];
		uint i = 0;
		for(auto it : list)
		{
			arr[i] = it;
			i++;
		}

		arr[listcount] = '\0'; //null-ending string.
		label->SetText(arr);
		delete[] arr;
	}

	/// \brief supprime 1 caractère de la textbox.
	void Remove()
	{
		if(list.size())
		{
			list.pop_back();
			uint listcount = list.size();
			if(listcount == 0)
			{
				string arr = " ";
				label->SetText(arr.c_str());
			}
			else
			{
				char * arr = new char[listcount + 2];
				uint i = 0;
				for(auto it : list)
				{
					arr[i] = it;
					i++;
				}
				arr[listcount] = '\0'; //null-ending string.
				label->SetText(arr);
				delete[] arr;
			}
		}
	}

	/// \brief dessine le composant.
	void Draw() {
		label->Draw();
		mesh->Draw();
	}

	/// \brief Gère les évenements.
	void Notification() {
		uint event = Event::GetType();
		switch(event) {

			case SDL_MOUSEBUTTONUP: {
				int x, y;
				SDL_GetMouseState(&x, &y);
				Vector2 point = {(double)x, (double)y};
				if(active && (!IsInside(point))) {
					allowActive = true;
					active = false;
					((ColoredMesh2*)mesh)->LoadRect(defaultColor, position, size);
				}
				else if((!active) && allowActive && (IsInside(point))) {
					allowActive = false;
					active = true;
					((ColoredMesh2*)mesh)->LoadRect(activeColor, position, size);
				}
				Remove();
				break;
			}

			case SDL_KEYUP: {
				if(active) {
					char key = Event::GetKey();
					switch(key) {
						case '\b': {
							Remove();
							break;
						}
						default: {
							SDL_Keymod shift = SDL_GetModState();
							if((key > 96) && (key <123))
								if((shift == KMOD_LSHIFT) || (shift == KMOD_RSHIFT)) 
									key -= 32;
							Add(key);
							break;
						}
					}
				}
				break;
			}

			default: {
				if(eventRouter[event])
					eventRouter[event](this);
				break;
			}
		}
	}
};
bool Textbox::allowActive = true;
#endif
