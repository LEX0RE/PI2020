/// \file Event.hpp
#ifndef EVENT_H
#define EVENT_H

#include <SDL2/SDL.h>
#include "Typedef.hpp"

/// \brief Classe statique pour les SDL_Event.
class Event {
private:
    static SDL_Event sdlEvent;  ///< Pointeur statique pour représenter l'event.

public:
    /// \brief Retourne valeur int de combien d'évènements il y a à traiter.
    /// \return Nombre d'évènement dans la liste d'évènement.
    inline static int Poll() {
        return SDL_PollEvent(&sdlEvent);
    }

    /// \brief Retourne le type d'event (à utiliser avec une switch).
    /// /return Le type d'évènement.
    inline static uint GetType() {
        return sdlEvent.type;
    }

    /// \brief Se débarasse du dernier évènement.
    inline static void Flush() {
        SDL_FlushEvent(SDL_LASTEVENT);
    }

    /// \brief Obtention de la position du curseur de la souris.
    /// \return Position du curseur de souris.
    inline static SDL_Point GetMousePosition() {
        return { sdlEvent.motion.x, sdlEvent.motion.y };
    }

    /// \brief Obtention de la position relative du curseur de la souris.
    /// \return Position relative du curseur de souris.
    inline static SDL_Point GetMouseRelativePosition() {
        return { sdlEvent.motion.xrel, sdlEvent.motion.yrel };
    }

    /// \brief Retourne la touche appuyée.
    /// \return Le code en int de la touche appuyer.
    inline static int GetKey() {
        return sdlEvent.key.keysym.sym;
    }

    /// \brief Retourne le roullement de la molette.
    /// \return Le roullement de la molette.
    inline static int GetScrollY(){
        return sdlEvent.wheel.y;
    }

    /// \brief Retourne le bouton de la souris.
    /// \return Le bouton de la souris.
    inline static int GetMouseButton(){
        return sdlEvent.button.button;
    }
};
/// \brief Déclaration de l'évènement statique qu'on peut accéder de partout.
SDL_Event Event::sdlEvent = SDL_Event();

#endif
