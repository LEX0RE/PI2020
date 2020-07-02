/// \file DebugPanel.hpp
#ifndef PI2020_DEBUGPANEL_HPP
#define PI2020_DEBUGPANEL_HPP

#define DEBUG_OPTION_COUNT 4

#define CHARACTER_BOUNDING_BOX 0
#define ENEMIES_BOUNDING_BOX 1
#define NPCS_BOUNDING_BOX 2
#define GROUND_GRID 3

#include "../Observer.hpp"
#include "HintBox.hpp"

/// \brief Panneau de débogage.
class DebugPanel : public Observer {
private:
  bool visible; ///< Visibilité du panneau
  SDL_Rect optionsRect[DEBUG_OPTION_COUNT]; ///< Rectangle des boutons
  unsigned int texturesId[2]; ///< Textures des boutons
  HintBox text; ///< Titre des options

public:
  static bool options[DEBUG_OPTION_COUNT]; ///< Options de débogage

  DebugPanel() : visible(false) {
    for (int i = 0; i < DEBUG_OPTION_COUNT; i++) optionsRect[i] = { 350, 250 + 40 * i, 31, 14 };
    texturesId[0] = GLContext::LoadTexture("resources/off.png");
    texturesId[1] = GLContext::LoadTexture("resources/on.png");
    text.AddText("Character Bounding Box", { 200, 200, 200, 255 });
    text.AddText(" ", { 0, 0, 0, 0 });
    text.AddText("Ennemies Bounding Box", { 200, 200, 200, 255 });
    text.AddText(" ", { 0, 0, 0, 0 });
    text.AddText("NPCs Bounding Box", { 200, 200, 200, 255 });
    text.AddText(" ", { 0, 0, 0, 0 });
    text.AddText("Ground Grid", { 200, 200, 200, 255 });
    text.SetPosition(400.0, 243.0);
    text.visible = true;
  }

  ~DebugPanel() {
    glDeleteTextures(2, texturesId);
  }

  void Draw() {
    if (visible) {
      glDisable(GL_TEXTURE_2D);
      glEnable(GL_COLOR);
      glColor4d(0.0, 0.0, 0.0, 0.5);
      glBegin(GL_QUADS);
          glVertex3d(300.0, 200.0, 0.0);
          glVertex3d(GLContext::width - 300.0, 200.0, 0.0);
          glVertex3d(GLContext::width - 300.0, GLContext::height - 200.0, 0.0);
          glVertex3d(300.0, GLContext::height - 200.0, 0.0);
      glEnd();
      glColor4d(1.0, 1.0, 1.0, 1.0);
      glDisable(GL_COLOR);
      glEnable(GL_TEXTURE_2D);

      for (int i = 0; i < DEBUG_OPTION_COUNT; i++) {
          glBindTexture(GL_TEXTURE_2D, (DebugPanel::options[i]) ? texturesId[1] : texturesId[0]);
          glBegin(GL_QUADS);
              glTexCoord2d(0.0, 0.0); glVertex3d(optionsRect[i].x, optionsRect[i].y, 0.0);
              glTexCoord2d(1.0, 0.0); glVertex3d(optionsRect[i].x + optionsRect[i].w, optionsRect[i].y, 0.0);
              glTexCoord2d(1.0, 1.0); glVertex3d(optionsRect[i].x + optionsRect[i].w, optionsRect[i].y + optionsRect[i].h, 0.0);
              glTexCoord2d(0.0, 1.0); glVertex3d(optionsRect[i].x, optionsRect[i].y + optionsRect[i].h, 0.0);
          glEnd();
      }

      text.Draw();
    }
  }

  void Notification() {
    switch(Event::GetType()) {
      case SDL_KEYUP:
        if ((Event::GetKey() == 'p') && (SDL_GetModState() == KMOD_LCTRL))
          visible = !visible;
        break;

      case SDL_MOUSEBUTTONUP:
        SDL_Point cursor = Event::GetMousePosition();
        for (int i = 0; i < DEBUG_OPTION_COUNT; i++)
          if (SDL_PointInRect(&cursor, &optionsRect[i]))
            options[i] = !options[i];
        break;
    }
  }
};

bool DebugPanel::options[4] = { false, false, false, true };

#endif