/// \file Texture.hpp
#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "../ResourceManager.hpp"


///\brief Classe gérant les textures.
class Texture : public Resource {
	uint textureId; ///< identifiant de texture OpenGL.

	public: 
	///\param filename Nom du fichier de texture à charger.
	Texture(const char * filename) {
		textureId = GLContext::LoadTexture(filename);
	}
	
	~Texture() {
		glDeleteTextures(1, &textureId);
	}
	
	/// \brief Retourne l'identifiant de texture.
	/// \return Identifiant de la texture
	inline uint GetId() {
		return textureId;
	}
	
	/// \brief Retourne un vecteur contenant les dimensions de l'images.
	/// \return Vecteur 2 dimensions.
	inline Vector2 GetSize() {
		int x, y;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, (int*)&x);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, (int*)&y);
		return {(double)x, (double)y};
	}

	/// \brief Obtient le pointeur de la resource.
	/// \return Pointeur de la ressource.
	void* GetInstance() {
		return &textureId;
	}
};
#endif
