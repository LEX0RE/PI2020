/// \file Dungeon.hpp
#ifndef PI2020_DUNGEON_HPP
#define PI2020_DUNGEON_HPP
#include "../VisualComponents/DebugPanel.hpp"
#include "../Items/Item.hpp"
#include "../Event.hpp"
#include "../GLContext.hpp"
#include "../Entities/Platform.hpp"
#include "../Models/Chest.hpp"
//#include "../Scene.hpp"

/// \brief Classe pour la scene de Dongon.
///
/// Classe les elements du dongon.
///
class Dungeon: public Scene3{
    
public:
    /// \brief Execute le constructeur de la scène lorsqu'elle est chargée.
    /// \param param Paramètre de transit.
    /// \return Un pointeur vers la scène du dongon.
    static inline Scene* LoadDungeon(void* param) {
        return new Dungeon();
    }

    Dungeon(){
        
    }

    ~Dungeon(){

    }

    void Draw(){}



};

#endif //PI2020_DUNGEON_HPP
