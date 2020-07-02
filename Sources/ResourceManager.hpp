#ifndef BASERESOURCE_HPP
#define BASERESOURCE_HPP

#include "Typedef.hpp"
#include "Vector2.hpp"
#include <string>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define GetResource(name, type) ResourceManager::Get<type>(name)

struct Resource {
	/// \brief Retourne l'instance de la resource.
	virtual void* GetInstance() = 0;
};

/// \brief Gère les resources contenues.
class ResourceManager {
	static std::map<std::string, Resource*> resources; ///< Associe une resource à un std::string.
	
public:
	/// \brief Ajoute une ressource au gestionnaire.
	/// \param name Identifiant / nom de la ressource.
	/// \param resource Pointeur de la resource.
	static void Add(const char * name, Resource* resource) {
		resources[name] = resource;
	}
	
	/// \brief Vérifie si un nom donné existe dans le gestionnaire.
	/// \param name Identifiant.
	static bool Exists(const char * name) {
		return (bool)resources[name];
	}
	
	/// \brief Supprime la ressource spécifiée.
	/// \param name Identifiant.
	static void Remove(const char * name) {
		if (resources[name])
			delete resources[name];
		resources.erase(name);
	}
	
	/// \brief Obtiens le pointeur d'une ressource.
	/// \param name Identifiant.
	/// \return Type spécifié avec le template.
	template <class T> static T* Get(const char * name) {
		return (T*)resources[name];
	}
};	
std::map<std::string, Resource*> ResourceManager::resources = std::map<std::string, Resource*>();

#endif
