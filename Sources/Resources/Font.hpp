/// \file Font.hpp
#ifndef FONT_HPP
#define FONT_HPP

#include "../ResourceManager.hpp"

/// \brief Gère les polices d'écriture.
class Font : public Resource{
	TTF_Font* fontPtr; ///< Pointeur de la police d'écriture.
	
public:
	/// \param filename Nom du fichier (.ttf) de police d'écriture.
	/// \param size Taille de la police d'écriture.
	Font(const char * filename, uint size) : Resource() {
		fontPtr = TTF_OpenFont(filename, size);
	}
	
	~Font() {
		TTF_CloseFont(fontPtr);
	}

	/// \brief Retourne le pointeur TTF_Font*.
	/// \return Pointeur du font.
	inline void* GetInstance() {
		return fontPtr;
	}
};

#endif 

