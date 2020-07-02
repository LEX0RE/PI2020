/// \file Vector2.hpp
#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <cmath> 

/// \brief Énumération autorisant 2 valeurs pour spécifier l'unité de l'angle.
enum AngleUnit {
	RADIAN = 0,
	DEGREE = 1
};

/// \brief Structure contenant une composante x et y se comportant comme un vecteur.
struct Vector2 {
	double x; ///< Composante de l'axe des "x".
	double y; ///< Composante de l'axe des "y".
	
	/// \param x Coordonnée en X.
	/// \param y Coordonne en Y.
	Vector2(double x, double y) {
		this->x = x;
		this->y = y;
	}
	
	
	Vector2() {
		this->x = 0.0;
		this->y = 0.0;
	}
	
	
	/// \brief Calcule la norme du vecteur.
	/// \return Le type de retour est un double.
	 inline double GetNorm() {
		return sqrt(pow(x, 2) + pow(y, 2));
	}

     /// \brief Calcule le déterminant de 2 vecteurs.
     /// \param v Autre vecteur
     /// \return Le déterminant des 2 vecteurs.
     inline double GetDet(Vector2 v) {
         return (x*v.y - v.x*y);
     }
	
	/// \brief Calcule l'angle entre 2 vecteurs.
	/// \param au "au" indique si l'angle est calculé en degré ou en radian.
	/// \param v "v" est un vecteur en 2 dimensions.s.
	/// \return Le type de retour est un double.
	 double GetAngle(Vector2 v, AngleUnit au) {
		double norm = GetNorm();
		if(au) 
			return (acos((x * v.x) + (y * v.y) / (norm * v.GetNorm())) * 180) / 3.14159265359;
		return acos((x * v.x) + (y * v.y) / (norm * v.GetNorm()));
	}
	
	/// \brief Additionne 2 vecteur
	/// \param v "v" est un vecteur en 2 dimensions.
	/// \return L'opérateur retourne le vecteur modifié.
	Vector2& operator+=(Vector2 v) {
		x += v.x;
		y += v.y;
		return *this;
	}
	
	/// \brief Additionne 2 vecteur
	/// \param v "v" est un vecteur en 2 dimensions.
	/// \return L'opérateur retourne le vecteur modifié.
	Vector2& operator-=(Vector2 v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}
	
	/// \brief Multiplie les composantes du vecteur pas un nombre.
	/// \param s "s" est un chiffre à virgule.
	/// \return L'opérateur retourne le vecteur modifié.
	Vector2& operator*=(double s) {
		x *= s;
		y *= s;
		return *this;
	}
	
	/// \brief Divise les composantes du vecteur par un nombre.
	/// \param s "s" est un chiffre à virgule.
	/// \return L'opérateur retourne le vecteur modifié.
	Vector2& operator/=(double s) {
		if(s) {
			x /= s;
			y /= s;
		}
		return *this;
	}
	
	/// \brief Additionne 2 vecteur
	/// \param v "v" est un vecteur en 2 dimensions.
	/// \return L'opérateur retourne le vecteur modifié.
	inline Vector2 operator+(Vector2 v) {
		return {v.x + x, v.y + y};
	}
	
	/// \brief Soustrait 2 vecteur
	/// \param v "v" est un vecteur en 2 dimensions.
	/// \return L'opérateur retourne le vecteur modifié.
	inline Vector2 operator-(Vector2 v) {
        return {x - v.x, y - v.y};
	}
	
	/// \brief fait le pro 2 vecteur
	/// \param v "v" est un vecteur en 2 dimensions.
	/// \return L'opérateur retourne un "double".
	inline double operator*(Vector2 v) {
		return (x * v.x) + (y * v.y);
	}
	
	/// \brief Multiplie les composantes du vecteur par un nombre.
	/// \param s "s" est un chiffre à virgule.
	/// \return L'opérateur retourne le vecteur modifié.
	inline Vector2 operator* (double s) {
		return Vector2(x * s, y * s);
	}
	
	/// \brief Compare deux vecteurs.
	/// \param v Vecteur 2D.
	/// \return Renvoie vrai si les deux vecteurs sont identiques, sinon faux.
	inline bool operator==(Vector2 v) {
		return ((x == v.x) && (y == v.y));
	}
};

#endif //VECTOR2_HPP
