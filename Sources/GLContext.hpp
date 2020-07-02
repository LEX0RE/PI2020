/// \file GLContext.hpp
#ifndef GLCONTEXT_HPP
#define GLCONTEXT_HPP

#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>

#include "Typedef.hpp"
#include "Matrix44.hpp"

/// \brief Classe représentant un contexte OpenGL.
///
/// Classe embalant une fenêtre SDL2 et un contexte OpenGL.
///
class GLContext {
    static SDL_Window* sdlWindow;   ///< Pointeur vers une fenêtre SDL2.
    static SDL_GLContext glContext; ///< Contexte OpenGL.
	static Matrix44d mOrthogonal;     ///< Matrice orthogonale
	static Matrix44d mPerspective;    ///< Matrice perspective

public:
    static double width;            ///< Largeur de la fenêtre.
    static double height;           ///< Hauteur de la fenêtre.

    /// \param title Titre de la fenêtre.
    /// \param x Position de la fenêtre sur l'axe des x.
    /// \param y Position de la fenêtre sur l'axe des y.
    /// \param width Largeur de la fenêtre.
    /// \param height Hauteur de la fenêtre.
    /// \param flags Options de la fenêtre.
    GLContext(const char* title = "", const int& x = SDL_WINDOWPOS_CENTERED, const int& y = SDL_WINDOWPOS_CENTERED, const int& width = 1024, const int& height = 768, const uint& flags = 0){
        GLContext::height = height;
        GLContext::width = width;

        sdlWindow =SDL_CreateWindow(title, x, y, width, height, flags | SDL_WINDOW_OPENGL);
        glContext = SDL_GL_CreateContext(sdlWindow);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_POINT_SMOOTH);
    }

    ~GLContext() {
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(sdlWindow);
    }

    /// \brief Défini la matrice de projection.
    static void SetOrthogonal(){
      mOrthogonal.LoadOrthogonal(width, height);
    }

	  /// \brief Applique la matrice orthogonale.
	  static void ApplyOrthogonal(){
		    glMatrixMode(GL_PROJECTION);
		    glLoadIdentity();
		    glMultMatrixd(mOrthogonal.matrix);
		    glMatrixMode(GL_MODELVIEW);
		    glLoadIdentity();
	  }
	
    /// \brief Défini la matrice de projection.
    /// \param fov Angle du champ de vision.
    /// \param nearPlane Distance du plan rapproché.
    /// \param farPlane Distance du plan éloigné.
    static void SetPerspective(double fov, double nearPlane, double farPlane) {
      const double right = std::tan((fov / 2.0) * 3.14159 / 180.0) * nearPlane;
      const double top = right * (height / width);

      mPerspective.LoadPerspective(right, top, nearPlane, farPlane);
    }

	  /// \brief Applique la matrice perspective.
	  static void ApplyPerspective(){
		    glMatrixMode(GL_PROJECTION);
		    glLoadIdentity();
		    glMultMatrixd(mPerspective.matrix);
		    glMatrixMode(GL_MODELVIEW);
		    glLoadIdentity();
	  }

    /// \brief Chargement d'une texture
    /// \param fileName Nom du fichier.
    /// \return Identifiant de la texture.
    static uint LoadTexture(const char* fileName){
        uint id;
        glGenTextures(1, &id);

        glBindTexture(GL_TEXTURE_2D, id);
        SDL_Surface* sdlSurface = IMG_Load(fileName);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sdlSurface->w, sdlSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, sdlSurface->pixels);
        SDL_FreeSurface(sdlSurface);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        return id;
    }

    /// \brief Vider le contenu de la fenêtre.
    static void Clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    /// \brief Afficher le contenu de la fenêtre.
    static void Refresh() {
        SDL_GL_SwapWindow(sdlWindow);
    }
};

double GLContext::width = 0;
double GLContext::height = 0;
SDL_Window* GLContext::sdlWindow = nullptr;

SDL_GLContext GLContext::glContext;
Matrix44d GLContext::mOrthogonal;
Matrix44d GLContext::mPerspective;

#endif