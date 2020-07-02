/// \file Progress.hpp
#ifndef PI2020_PROGRESS_HPP
#define PI2020_PROGRESS_HPP


#include "../VisualComp.hpp"
#include "../Meshes/TexturedMesh.hpp"
#include "../Meshes/ColoredMesh.hpp"
#include "../VisualComponents/Image.hpp"
#include "../Typedef.hpp"

/// \brief Classe Parent d'une barre de progression.
class Progress : public VisualComp {
protected:
	ushort count;   ///< Indique le nombre Actuel.
	ushort total;    ///<  Indique le nombre Total.

public:
	/// \param position Position de la barre (à partir du coin supérieur gauche de la barre)
	/// \param size Grosseur de la barre (en pixels)
	/// \param Total Indique le nombre total de la barre (Pour la remplir complètement)
	Progress(Vector2 position, Vector2 size, ushort total) : VisualComp(position, size) {
		this->total = total;
		mesh = new ColoredMesh2();
	}
	
	/// \brief remet la barre a 0
	inline void Reset() {
		count = 0;
	}
	
	/// \brief Indique si la barre est pleine ou non
	/// \return si la barre est pleine ou non
	inline bool IfFull() { return  (count == total); }
	
	/// \brief Renvoie le progrès de la barre
	/// \return progrès de la barre en chiffre
	inline ushort GetCount(){
		return count;
	}
	
	/// \brief Renvoie le maximum de la barre
	/// \return total de la barre en chiffre
	inline ushort GetTotal(){
		return total;
	}

	double GetPercentage(){
	    double test = ((double)count / (double)total) * 100;
	    return ((double)count / (double)total) * 100;
	}
	
	/// \brief Affiche la barre.
	virtual void Draw() = 0;
	
	/// \brief Notification sur la barre.
	virtual void Notification() = 0;
	
	
};

#endif //PI2020_PROGRESS_HPP
